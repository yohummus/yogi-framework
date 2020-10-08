/*
 * This file is part of the Yogi Framework
 * https://github.com/yohummus/yogi-framework.
 *
 * Copyright (c) 2020 Johannes Bergmann.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <src/api/errors.h>
#include <src/network/msg_transport.h>
#include <src/util/algorithm.h>

using namespace std::string_literals;

YOGI_DEFINE_INTERNAL_LOGGER("MessageTransport")

std::size_t calculate_msg_size_field_length(std::size_t msg_size) {
  return 1u + (msg_size >= (1 << 7)) + (msg_size >= (1 << 14)) + (msg_size >= (1 << 21)) + (msg_size >= (1 << 28));
}

std::size_t serialize_msg_size_field(std::size_t msg_size, std::array<Byte, 5>* buffer) {
  auto length = calculate_msg_size_field_length(msg_size);

  auto it = buffer->begin();
  for (auto i = length; i > 0; --i) {
    auto byte = static_cast<Byte>((msg_size >> ((i - 1) * 7)) & 0x7F);
    byte |= static_cast<Byte>((i > 1 ? (1 << 7) : 0));
    *it++ = byte;
  }

  return length;
}

bool deserialize_msg_size_field(const std::array<Byte, 5>& buffer, std::size_t size, std::size_t* msg_size) {
  YOGI_ASSERT(size <= buffer.size());

  std::size_t tmp = 0;

  for (std::size_t i = 0; i < size; ++i) {
    auto byte = buffer[i];

    tmp |= static_cast<std::size_t>(byte & ~(1 << 7));
    if (!(byte & (1 << 7))) {
      *msg_size = tmp;
      return true;
    }

    tmp <<= 7;
  }

  return false;
}

MessageTransport::MessageTransport(TransportPtr transport, std::size_t tx_queue_size, std::size_t rx_queue_size)
    : context_(transport->get_context()),
      transport_(transport),
      tx_rb_(tx_queue_size),
      rx_rb_(rx_queue_size),
      last_tx_error_(YOGI_OK),
      send_to_transport_running_(false),
      receive_from_transport_running_(false),
      last_rx_error_(YOGI_OK) {
  reset_received_size_field();
}

void MessageTransport::start() {
  set_logging_prefix("[peer " + transport_->get_peer_description() + ']');
  receive_some_bytes_from_transport();
}

bool MessageTransport::try_send(const OutgoingMessage& msg) {
  std::lock_guard<std::mutex> lock(tx_mutex_);
  if (last_tx_error_.is_error()) {
    throw last_tx_error_.to_error();
  }

  if (pending_sends_.empty()) {
    return try_send_impl(msg.serialize());
  } else {
    return false;
  }
}

void MessageTransport::send_async(OutgoingMessage* msg, OperationTag tag, SendHandler handler) {
  YOGI_ASSERT(tag != 0);
  send_async_impl(msg, tag, handler);
}

void MessageTransport::send_async(OutgoingMessage* msg, SendHandler handler) {
  send_async_impl(msg, 0, handler);
}

bool MessageTransport::cancel_send(OperationTag tag) {
  YOGI_ASSERT(tag != 0);

  std::lock_guard<std::mutex> lock(tx_mutex_);
  auto it = find_if(pending_sends_, [&](auto& ps) { return ps.tag == tag; });

  if (it == pending_sends_.end()) return false;

  auto handler = std::move(it->handler);
  pending_sends_.erase(it);

  transport_->get_context()->post([=] { handler(Error(YOGI_ERR_CANCELED)); });

  return true;
}

void MessageTransport::receive_async(boost::asio::mutable_buffer msg, ReceiveHandler handler) {
  YOGI_ASSERT(!pending_receive_handler_);
  YOGI_ASSERT(!size_field_valid_);

  if (last_rx_error_.is_error()) {
    transport_->get_context()->post([=] { handler(last_rx_error_, 0); });
    return;
  }

  pending_receive_buffer_  = msg;
  pending_receive_handler_ = handler;
  try_deliver_pending_receive();

  receive_some_bytes_from_transport();
}

void MessageTransport::cancel_receive() {
  if (!pending_receive_handler_) return;

  ReceiveHandler handler;
  std::swap(handler, pending_receive_handler_);

  transport_->get_context()->post([=] { handler(Error(YOGI_ERR_CANCELED), 0); });
}

bool MessageTransport::try_send_impl(const SmallBuffer& msg_bytes) {
  if (!can_send(msg_bytes.size())) return false;

  SizeFieldBuffer size_field_buf;
  auto n             = serialize_msg_size_field(msg_bytes.size(), &size_field_buf);
  auto bytes_written = tx_rb_.write(size_field_buf.data(), n);
  YOGI_UNUSED(bytes_written);
  YOGI_ASSERT(bytes_written == n);

  bytes_written = tx_rb_.write(static_cast<const Byte*>(msg_bytes.data()), msg_bytes.size());
  YOGI_ASSERT(bytes_written == msg_bytes.size());

  send_some_bytes_to_transport();

  return true;
}

bool MessageTransport::can_send(std::size_t msg_size) const {
  YOGI_ASSERT(msg_size + calculate_msg_size_field_length(msg_size) <= tx_rb_.capacity());

  auto n = tx_rb_.available_for_write();
  if (n >= msg_size + 5) return true;  // optimisation since this is very likely
  return n >= msg_size + calculate_msg_size_field_length(msg_size);
}

void MessageTransport::send_async_impl(OutgoingMessage* msg, OperationTag tag, SendHandler handler) {
  std::lock_guard<std::mutex> lock(tx_mutex_);

  if (tag != 0) {
    check_operation_tag_not_used(tag);
  }

  if (last_tx_error_.is_error()) {
    transport_->get_context()->post([=] { handler(last_tx_error_); });
    return;
  }

  if (pending_sends_.empty() && try_send_impl(msg->serialize())) {
    transport_->get_context()->post([=] { handler(Success()); });
  } else {
    PendingSend ps = {tag, msg->serialize_shared(), handler};
    pending_sends_.push_back(ps);

    YOGI_ASSERT(send_to_transport_running_);
  }
}

void MessageTransport::send_some_bytes_to_transport() {
  YOGI_ASSERT(!tx_rb_.empty());

  if (send_to_transport_running_) return;
  send_to_transport_running_ = true;

  auto weak_self = make_weak_ptr();
  transport_->send_some_async(tx_rb_.first_read_array(), [=](auto& res, auto n) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      self->handle_send_error(res.to_error());
      return;
    }

    std::lock_guard<std::mutex> lock(tx_mutex_);
    self->tx_rb_.commit_first_read_array(n);
    send_to_transport_running_ = false;

    if (!self->tx_rb_.empty()) {
      self->send_some_bytes_to_transport();
    }

    self->retry_sending_pending_sends();
  });
}

void MessageTransport::retry_sending_pending_sends() {
  auto it = pending_sends_.begin();
  while (it != pending_sends_.end() && try_send_impl(*it->msg_bytes)) {
    auto handler = std::move(it->handler);
    transport_->get_context()->post([=] { handler(Success()); });
    ++it;
  }

  pending_sends_.erase(pending_sends_.begin(), it);
}

bool MessageTransport::try_get_received_size_field(std::size_t* msg_size) {
  if (size_field_valid_) {
    *msg_size = size_field_;
    return true;
  }

  rx_rb_.pop_until([&](Byte byte) {
    size_field_buffer_[size_field_buffer_size_++] = byte;
    bool ok = deserialize_msg_size_field(size_field_buffer_, size_field_buffer_size_, msg_size);

    if (ok) {
      size_field_       = *msg_size;
      size_field_valid_ = true;

      if (size_field_ > rx_rb_.capacity()) {
        handle_receive_error(Error(YOGI_ERR_DESERIALIZE_MSG_FAILED));
      }

      return true;
    } else if (size_field_buffer_size_ >= size_field_buffer_.size()) {
      handle_receive_error(Error(YOGI_ERR_DESERIALIZE_MSG_FAILED));
      return true;
    }

    return false;
  });

  return size_field_valid_;
}

void MessageTransport::reset_received_size_field() {
  size_field_buffer_size_ = 0;
  size_field_valid_       = false;
}

void MessageTransport::receive_some_bytes_from_transport() {
  if (receive_from_transport_running_) return;
  receive_from_transport_running_ = true;

  auto weak_self = make_weak_ptr();
  transport_->receive_some_async(rx_rb_.first_write_array(), [=](auto& res, auto n) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      self->handle_receive_error(res.to_error());
      return;
    }

    self->rx_rb_.commit_first_write_array(n);
    receive_from_transport_running_ = false;

    self->try_deliver_pending_receive();

    if (!self->rx_rb_.full()) {
      self->receive_some_bytes_from_transport();
    }
  });
}

void MessageTransport::try_deliver_pending_receive() {
  if (!pending_receive_handler_) return;

  std::size_t size;
  if (!try_get_received_size_field(&size) || rx_rb_.available_for_read() < size) {
    return;
  }

  ReceiveHandler handler;
  std::swap(handler, pending_receive_handler_);
  reset_received_size_field();

  auto n = std::min(size, pending_receive_buffer_.size());
  rx_rb_.read(static_cast<Byte*>(pending_receive_buffer_.data()), n);

  if (n < size) {
    rx_rb_.discard(size - n);
    transport_->get_context()->post([=] { handler(Error(YOGI_ERR_BUFFER_TOO_SMALL), size); });
  } else {
    transport_->get_context()->post([=] { handler(Success(), size); });
  }
}

void MessageTransport::handle_send_error(const Error& err) {
  LOG_ERR("Sending message failed: " << err);

  close();

  std::lock_guard<std::mutex> lock(tx_mutex_);
  last_tx_error_ = err;

  for (auto& ps : pending_sends_) {
    ps.handler(err);
  }

  pending_sends_.clear();
}

void MessageTransport::handle_receive_error(const Error& err) {
  LOG_ERR("Receiving message failed: " << err);

  close();

  last_rx_error_ = err;
  if (pending_receive_handler_) {
    ReceiveHandler handler;
    std::swap(handler, pending_receive_handler_);

    transport_->get_context()->post([=] { handler(Error(err), 0); });
  }
}

void MessageTransport::check_operation_tag_not_used(OperationTag tag) {
  YOGI_ASSERT(tag != 0);
  YOGI_ASSERT(!contains_if(pending_sends_, [&](auto& ps) { return ps.tag == tag; }));
}
