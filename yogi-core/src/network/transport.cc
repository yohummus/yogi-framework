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

namespace network {

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

void Transport::SendSomeAsync(boost::asio::const_buffer data, TransferSomeHandler handler) {
  YOGI_ASSERT(data.size() > 0);

  if (data.size() > transceive_byte_limit_) {
    data = boost::asio::buffer(data.data(), transceive_byte_limit_);
  }

  StartTimeout(&tx_timer_);

  auto weak_self = MakeWeakPtr();
  WriteSomeAsync(data, [=](auto& res, auto bytes_written) {
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
        self->Close();
      }

      handler(res, bytes_written);
    }
  });
}

void Transport::SendAllAsync(boost::asio::const_buffer data, TransferAllHandler handler) {
  SendAllAsyncImpl(data, YOGI_OK, 0, handler);
}

void Transport::SendAllAsync(SharedBuffer data, TransferAllHandler handler) {
  SendAllAsync(boost::asio::buffer(*data), [=, _ = data](auto& res) { handler(res); });
}

void Transport::SendAllAsync(SharedSmallBuffer data, TransferAllHandler handler) {
  SendAllAsync(boost::asio::buffer(data->data(), data->size()), [=, _ = data](auto& res) { handler(res); });
}

void Transport::ReceiveSomeAsync(boost::asio::mutable_buffer data, TransferSomeHandler handler) {
  YOGI_ASSERT(data.size() > 0);

  if (data.size() > transceive_byte_limit_) {
    data = boost::asio::buffer(data.data(), transceive_byte_limit_);
  }

  StartTimeout(&rx_timer_);

  auto weak_self = MakeWeakPtr();
  ReadSomeAsync(data, [=](auto& res, auto bytes_read) {
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
        self->Close();
      }

      handler(res, bytes_read);
    }
  });
}

void Transport::ReceiveAllAsync(boost::asio::mutable_buffer data, TransferAllHandler handler) {
  ReceiveAllAsyncImpl(data, YOGI_OK, 0, handler);
}

void Transport::ReceiveAllAsync(SharedBuffer data, TransferAllHandler handler) {
  ReceiveAllAsync(boost::asio::buffer(*data), [=, _ = data](auto& res) { handler(res); });
}

void Transport::SendAllAsyncImpl(boost::asio::const_buffer data, const Result& res, std::size_t bytes_written,
                                 TransferAllHandler handler) {
  YOGI_ASSERT(data.size() > 0);

  if (res.IsSuccess() && bytes_written < data.size()) {
    data += bytes_written;
    this->SendSomeAsync(
        data, [=](auto& res, auto bytes_written) { this->SendAllAsyncImpl(data, res, bytes_written, handler); });
  } else {
    handler(res);
  }
}

void Transport::Close() {
  Shutdown();
  YOGI_DEBUG_ONLY(close_called_ = true;)
}

void Transport::ReceiveAllAsyncImpl(boost::asio::mutable_buffer data, const Result& res, std::size_t bytes_read,
                                    TransferAllHandler handler) {
  YOGI_ASSERT(data.size() > 0);

  if (res.IsSuccess() && bytes_read < data.size()) {
    data += bytes_read;
    this->ReceiveSomeAsync(
        data, [=](auto& res, auto bytes_read) { this->ReceiveAllAsyncImpl(data, res, bytes_read, handler); });
  } else {
    handler(res);
  }
}

void Transport::StartTimeout(boost::asio::steady_timer* timer) {
  timer->expires_from_now(timeout_);
  timer->async_wait(BindWeak(&Transport::OnTimeout, this));
}

void Transport::OnTimeout(boost::system::error_code ec) {
  if (ec) return;

  timed_out_ = true;

  Close();
}

}  // namespace network

std::ostream& operator<<(std::ostream& os, const network::Transport& transport) {
  os << transport.GetPeerDescription();
  return os;
}
