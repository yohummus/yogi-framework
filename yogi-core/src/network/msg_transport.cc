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

namespace network {
namespace internal {

std::size_t CalculateMsgSizeFieldLength(std::size_t msg_size) {
  return 1u + (msg_size >= (1 << 7)) + (msg_size >= (1 << 14)) + (msg_size >= (1 << 21)) + (msg_size >= (1 << 28));
}

std::size_t SerializeMsgSizeField(std::size_t msg_size, std::array<Byte, 5>* buffer) {
  auto length = CalculateMsgSizeFieldLength(msg_size);

  auto it = buffer->begin();
  for (auto i = length; i > 0; --i) {
    auto byte = static_cast<Byte>((msg_size >> ((i - 1) * 7)) & 0x7F);
    byte |= static_cast<Byte>((i > 1 ? (1 << 7) : 0));
    *it++ = byte;
  }

  return length;
}

bool DeserializeMsgSizeField(const std::array<Byte, 5>& buffer, std::size_t size, std::size_t* msg_size) {
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

}  // namespace internal

MessageTransport::MessageTransport(TransportPtr transport, std::size_t tx_queue_size, std::size_t rx_queue_size)
    : context_(transport->GetContext()),
      transport_(transport),
      tx_rb_(tx_queue_size),
      rx_rb_(rx_queue_size),
      last_tx_error_(YOGI_OK),
      send_to_transport_running_(false),
      receive_from_transport_running_(false),
      last_rx_error_(YOGI_OK) {
  ResetReceivedSizeField();
}

void MessageTransport::Start() {
  set_logging_prefix("[peer " + transport_->GetPeerDescription() + ']');
  ReceiveSomeBytesFromTransport();
}

bool MessageTransport::TrySend(const OutgoingMessage& msg) {
  std::lock_guard<std::mutex> lock(tx_mutex_);
  if (last_tx_error_.is_error()) {
    throw last_tx_error_.to_error();
  }

  if (pending_sends_.empty()) {
    return TrySendImpl(msg.Serialize());
  } else {
    return false;
  }
}

void MessageTransport::SendAsync(OutgoingMessage* msg, OperationTag tag, SendHandler handler) {
  YOGI_ASSERT(tag != 0);
  SendAsyncImpl(msg, tag, handler);
}

void MessageTransport::SendAsync(OutgoingMessage* msg, SendHandler handler) {
  SendAsyncImpl(msg, 0, handler);
}

bool MessageTransport::CancelSend(OperationTag tag) {
  YOGI_ASSERT(tag != 0);

  std::lock_guard<std::mutex> lock(tx_mutex_);
  auto it = find_if(pending_sends_, [&](auto& ps) { return ps.tag == tag; });

  if (it == pending_sends_.end()) return false;

  auto handler = std::move(it->handler);
  pending_sends_.erase(it);

  transport_->GetContext()->post([=] { handler(Error(YOGI_ERR_CANCELED)); });

  return true;
}

void MessageTransport::ReceiveAsync(boost::asio::mutable_buffer msg, ReceiveHandler handler) {
  YOGI_ASSERT(!pending_receive_handler_);
  YOGI_ASSERT(!size_field_valid_);

  if (last_rx_error_.is_error()) {
    transport_->GetContext()->post([=] { handler(last_rx_error_, 0); });
    return;
  }

  pending_receive_buffer_  = msg;
  pending_receive_handler_ = handler;
  TryDeliveringPendingReceive();

  ReceiveSomeBytesFromTransport();
}

void MessageTransport::CancelReceive() {
  if (!pending_receive_handler_) return;

  ReceiveHandler handler;
  std::swap(handler, pending_receive_handler_);

  transport_->GetContext()->post([=] { handler(Error(YOGI_ERR_CANCELED), 0); });
}

bool MessageTransport::TrySendImpl(const SmallBuffer& msg_bytes) {
  if (!CanSend(msg_bytes.size())) return false;

  SizeFieldBuffer size_field_buf;
  auto n             = internal::SerializeMsgSizeField(msg_bytes.size(), &size_field_buf);
  auto bytes_written = tx_rb_.write(size_field_buf.data(), n);
  YOGI_UNUSED(bytes_written);
  YOGI_ASSERT(bytes_written == n);

  bytes_written = tx_rb_.write(static_cast<const Byte*>(msg_bytes.data()), msg_bytes.size());
  YOGI_ASSERT(bytes_written == msg_bytes.size());

  SendSomeBytesToTransport();

  return true;
}

bool MessageTransport::CanSend(std::size_t msg_size) const {
  YOGI_ASSERT(msg_size + internal::CalculateMsgSizeFieldLength(msg_size) <= tx_rb_.Capacity());

  auto n = tx_rb_.available_for_write();
  if (n >= msg_size + 5) return true;  // optimisation since this is very likely
  return n >= msg_size + internal::CalculateMsgSizeFieldLength(msg_size);
}

void MessageTransport::SendAsyncImpl(OutgoingMessage* msg, OperationTag tag, SendHandler handler) {
  std::lock_guard<std::mutex> lock(tx_mutex_);

  if (tag != 0) {
    CheckOperationTagIsNotUsed(tag);
  }

  if (last_tx_error_.is_error()) {
    transport_->GetContext()->post([=] { handler(last_tx_error_); });
    return;
  }

  if (pending_sends_.empty() && TrySendImpl(msg->Serialize())) {
    transport_->GetContext()->post([=] { handler(Success()); });
  } else {
    PendingSend ps = {tag, msg->SerializeShared(), handler};
    pending_sends_.push_back(ps);

    YOGI_ASSERT(send_to_transport_running_);
  }
}

void MessageTransport::SendSomeBytesToTransport() {
  YOGI_ASSERT(!tx_rb_.Empty());

  if (send_to_transport_running_) return;
  send_to_transport_running_ = true;

  auto weak_self = MakeWeakPtr();
  transport_->SendSomeAsync(tx_rb_.first_read_array(), [=](auto& res, auto n) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      self->HandleSendError(res.to_error());
      return;
    }

    std::lock_guard<std::mutex> lock(tx_mutex_);
    self->tx_rb_.commit_first_read_array(n);
    send_to_transport_running_ = false;

    if (!self->tx_rb_.empty()) {
      self->SendSomeBytesToTransport();
    }

    self->RetrySendingPendingSends();
  });
}

void MessageTransport::RetrySendingPendingSends() {
  auto it = pending_sends_.begin();
  while (it != pending_sends_.end() && TrySendImpl(*it->msg_bytes)) {
    auto handler = std::move(it->handler);
    transport_->GetContext()->post([=] { handler(Success()); });
    ++it;
  }

  pending_sends_.erase(pending_sends_.begin(), it);
}

bool MessageTransport::TryGetReceivedSizeField(std::size_t* msg_size) {
  if (size_field_valid_) {
    *msg_size = size_field_;
    return true;
  }

  rx_rb_.pop_until([&](Byte byte) {
    size_field_buffer_[size_field_buffer_size_++] = byte;
    bool ok = internal::DeserializeMsgSizeField(size_field_buffer_, size_field_buffer_size_, msg_size);

    if (ok) {
      size_field_       = *msg_size;
      size_field_valid_ = true;

      if (size_field_ > rx_rb_.capacity()) {
        HandleReceiveError(Error(YOGI_ERR_DESERIALIZE_MSG_FAILED));
      }

      return true;
    } else if (size_field_buffer_size_ >= size_field_buffer_.size()) {
      HandleReceiveError(Error(YOGI_ERR_DESERIALIZE_MSG_FAILED));
      return true;
    }

    return false;
  });

  return size_field_valid_;
}

void MessageTransport::ResetReceivedSizeField() {
  size_field_buffer_size_ = 0;
  size_field_valid_       = false;
}

void MessageTransport::ReceiveSomeBytesFromTransport() {
  if (receive_from_transport_running_) return;
  receive_from_transport_running_ = true;

  auto weak_self = MakeWeakPtr();
  transport_->ReceiveSomeAsync(rx_rb_.first_write_array(), [=](auto& res, auto n) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      self->HandleReceiveError(res.to_error());
      return;
    }

    self->rx_rb_.commit_first_write_array(n);
    receive_from_transport_running_ = false;

    self->TryDeliveringPendingReceive();

    if (!self->rx_rb_.full()) {
      self->ReceiveSomeBytesFromTransport();
    }
  });
}

void MessageTransport::TryDeliveringPendingReceive() {
  if (!pending_receive_handler_) return;

  std::size_t size;
  if (!TryGetReceivedSizeField(&size) || rx_rb_.available_for_read() < size) {
    return;
  }

  ReceiveHandler handler;
  std::swap(handler, pending_receive_handler_);
  ResetReceivedSizeField();

  auto n = std::min(size, pending_receive_buffer_.size());
  rx_rb_.read(static_cast<Byte*>(pending_receive_buffer_.data()), n);

  if (n < size) {
    rx_rb_.discard(size - n);
    transport_->GetContext()->post([=] { handler(Error(YOGI_ERR_BUFFER_TOO_SMALL), size); });
  } else {
    transport_->GetContext()->post([=] { handler(Success(), size); });
  }
}

void MessageTransport::HandleSendError(const Error& err) {
  LOG_ERR("Sending message failed: " << err);

  Close();

  std::lock_guard<std::mutex> lock(tx_mutex_);
  last_tx_error_ = err;

  for (auto& ps : pending_sends_) {
    ps.handler(err);
  }

  pending_sends_.clear();
}

void MessageTransport::HandleReceiveError(const Error& err) {
  LOG_ERR("Receiving message failed: " << err);

  Close();

  last_rx_error_ = err;
  if (pending_receive_handler_) {
    ReceiveHandler handler;
    std::swap(handler, pending_receive_handler_);

    transport_->GetContext()->post([=] { handler(Error(err), 0); });
  }
}

void MessageTransport::CheckOperationTagIsNotUsed(OperationTag tag) {
  YOGI_ASSERT(tag != 0);
  YOGI_ASSERT(!contains_if(pending_sends_, [&](auto& ps) { return ps.tag == tag; }));
}

}  // namespace network
