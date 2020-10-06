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

#include <src/network/transport.h>
#include <src/util/bind.h>

#include <limits>

Transport::Transport(ContextPtr context, std::chrono::nanoseconds timeout, bool created_from_incoming_conn_req,
                     std::string peer_description, std::size_t transceive_byte_limit)
    : context_(context),
      timeout_(timeout),
      created_from_incoming_conn_req_(created_from_incoming_conn_req),
      peer_description_(peer_description),
      transceive_byte_limit_(transceive_byte_limit),
      tx_timer_(context->io_context()),
      rx_timer_(context->io_context()),
      timed_out_(false) {
}

Transport::~Transport() {
}

void Transport::send_some_async(boost::asio::const_buffer data, TransferSomeHandler handler) {
  YOGI_ASSERT(data.size() > 0);

  if (data.size() > transceive_byte_limit_) {
    data = boost::asio::buffer(data.data(), transceive_byte_limit_);
  }

  start_timeout(&tx_timer_);

  auto weak_self = make_weak_ptr();
  write_some_async(data, [=](auto& res, auto bytes_written) {
    auto self = weak_self.lock();
    if (!self) {
      handler(Error(YOGI_ERR_CANCELED), bytes_written);
      return;
    }

    self->tx_timer_.cancel();

    if (self->timed_out_) {
      handler(Error(YOGI_ERR_TIMEOUT), bytes_written);
    } else {
      if (res.is_error()) {
        self->close();
      }

      handler(res, bytes_written);
    }
  });
}

void Transport::send_all_async(boost::asio::const_buffer data, TransferAllHandler handler) {
  send_all_async_impl(data, Success(), 0, handler);
}

void Transport::send_all_async(SharedBuffer data, TransferAllHandler handler) {
  send_all_async(boost::asio::buffer(*data), [=, _ = data](auto& res) { handler(res); });
}

void Transport::send_all_async(SharedSmallBuffer data, TransferAllHandler handler) {
  send_all_async(boost::asio::buffer(data->data(), data->size()), [=, _ = data](auto& res) { handler(res); });
}

void Transport::receive_some_async(boost::asio::mutable_buffer data, TransferSomeHandler handler) {
  YOGI_ASSERT(data.size() > 0);

  if (data.size() > transceive_byte_limit_) {
    data = boost::asio::buffer(data.data(), transceive_byte_limit_);
  }

  start_timeout(&rx_timer_);

  auto weak_self = make_weak_ptr();
  read_some_async(data, [=](auto& res, auto bytes_read) {
    auto self = weak_self.lock();
    if (!self) {
      handler(Error(YOGI_ERR_CANCELED), bytes_read);
      return;
    }

    self->rx_timer_.cancel();

    if (self->timed_out_) {
      handler(Error(YOGI_ERR_TIMEOUT), bytes_read);
    } else {
      if (res.is_error()) {
        self->close();
      }

      handler(res, bytes_read);
    }
  });
}

void Transport::receive_all_async(boost::asio::mutable_buffer data, TransferAllHandler handler) {
  receive_all_async_impl(data, Success(), 0, handler);
}

void Transport::receive_all_async(SharedBuffer data, TransferAllHandler handler) {
  receive_all_async(boost::asio::buffer(*data), [=, _ = data](auto& res) { handler(res); });
}

void Transport::send_all_async_impl(boost::asio::const_buffer data, const Result& res, std::size_t bytes_written,
                                    TransferAllHandler handler) {
  YOGI_ASSERT(data.size() > 0);

  if (res.is_success() && bytes_written < data.size()) {
    data += bytes_written;
    this->send_some_async(
        data, [=](auto& res, auto bytes_written) { this->send_all_async_impl(data, res, bytes_written, handler); });
  } else {
    handler(res);
  }
}

void Transport::close() {
  shutdown();
  YOGI_DEBUG_ONLY(close_called_ = true;)
}

void Transport::receive_all_async_impl(boost::asio::mutable_buffer data, const Result& res, std::size_t bytes_read,
                                       TransferAllHandler handler) {
  YOGI_ASSERT(data.size() > 0);

  if (res.is_success() && bytes_read < data.size()) {
    data += bytes_read;
    this->receive_some_async(
        data, [=](auto& res, auto bytes_read) { this->receive_all_async_impl(data, res, bytes_read, handler); });
  } else {
    handler(res);
  }
}

void Transport::start_timeout(boost::asio::steady_timer* timer) {
  timer->expires_from_now(timeout_);
  timer->async_wait(bind_weak(&Transport::on_timeout, this));
}

void Transport::on_timeout(boost::system::error_code ec) {
  if (ec) return;

  timed_out_ = true;

  close();
}

std::ostream& operator<<(std::ostream& os, const Transport& transport) {
  os << transport.get_peer_description();
  return os;
}
