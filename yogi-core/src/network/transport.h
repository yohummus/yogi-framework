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

#pragma once

#include <src/config.h>

#include <src/data/buffer.h>
#include <src/objects/context.h>
#include <src/objects/logger/log_user.h>

#include <boost/asio.hpp>

#include <chrono>
#include <functional>
#include <memory>
#include <sstream>
#include <string>

namespace network {

class Transport;
typedef std::shared_ptr<Transport> TransportPtr;
typedef std::weak_ptr<Transport> TransportWeakPtr;

class Transport : public std::enable_shared_from_this<Transport>, public LogUser {
 public:
  typedef std::function<void(const Result&, const std::size_t bytes_transferred)> TransferSomeHandler;
  typedef std::function<void(const Result&)> TransferAllHandler;

  Transport(ContextPtr context, std::chrono::nanoseconds timeout, bool created_from_incoming_conn_req,
            std::string peer_description, std::size_t transceive_byte_limit);
  virtual ~Transport();

  ContextPtr GetContext() const {
    return context_;
  }
  const std::string& GetPeerDescription() const {
    return peer_description_;
  }

  bool CreatedFromIncomingConnectionRequest() const {
    return created_from_incoming_conn_req_;
  };

  void SendSomeAsync(boost::asio::const_buffer data, TransferSomeHandler handler);
  void SendAllAsync(boost::asio::const_buffer data, TransferAllHandler handler);
  void SendAllAsync(SharedBuffer data, TransferAllHandler handler);
  void SendAllAsync(SharedSmallBuffer data, TransferAllHandler handler);
  void ReceiveSomeAsync(boost::asio::mutable_buffer data, TransferSomeHandler handler);
  void ReceiveAllAsync(boost::asio::mutable_buffer data, TransferAllHandler handler);
  void ReceiveAllAsync(SharedBuffer data, TransferAllHandler handler);
  void Close();

 protected:
  virtual void WriteSomeAsync(boost::asio::const_buffer data, TransferSomeHandler handler)  = 0;
  virtual void ReadSomeAsync(boost::asio::mutable_buffer data, TransferSomeHandler handler) = 0;
  virtual void Shutdown()                                                                   = 0;

 private:
  TransportWeakPtr MakeWeakPtr() {
    return shared_from_this();
  }
  void SendAllAsyncImpl(boost::asio::const_buffer data, const Result& res, std::size_t bytes_written,
                        TransferAllHandler handler);
  void ReceiveAllAsyncImpl(boost::asio::mutable_buffer data, const Result& res, std::size_t bytes_read,
                           TransferAllHandler handler);
  void StartTimeout(boost::asio::steady_timer* timer);
  void OnTimeout(boost::system::error_code ec);

  const ContextPtr context_;
  const std::chrono::nanoseconds timeout_;
  const bool created_from_incoming_conn_req_;
  const std::string peer_description_;
  const std::size_t transceive_byte_limit_;
  boost::asio::steady_timer tx_timer_;
  boost::asio::steady_timer rx_timer_;
  bool timed_out_;
  YOGI_DEBUG_ONLY(bool close_called_ = false;)
};

}  // namespace network

std::ostream& operator<<(std::ostream& os, const network::Transport& transport);
