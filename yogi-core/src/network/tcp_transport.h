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

#include <src/network/transport.h>
#include <src/objects/context.h>
#include <src/objects/logger/log_user.h>

#include <boost/asio.hpp>

class TcpTransport;
typedef std::shared_ptr<TcpTransport> TcpTransportPtr;
typedef std::weak_ptr<TcpTransport> TcpTransportWeakPtr;

class TcpTransport : public Transport {
 public:
  template <typename T>
  class AsioGuard {
    friend class TcpTransport;

   public:
    AsioGuard(T* obj) : obj_(obj) {
    }
    ~AsioGuard() {
      if (obj_) obj_->cancel();
    }

   private:
    void disable() {
      obj_ = nullptr;
    }

    T* obj_;
  };

  typedef AsioGuard<boost::asio::ip::tcp::acceptor> AcceptGuard;
  typedef std::shared_ptr<AcceptGuard> AcceptGuardPtr;
  typedef std::weak_ptr<AcceptGuard> AcceptGuardWeakPtr;
  typedef AsioGuard<boost::asio::ip::tcp::socket> ConnectGuard;
  typedef std::shared_ptr<ConnectGuard> ConnectGuardPtr;
  typedef std::weak_ptr<ConnectGuard> ConnectGuardWeakPtr;
  typedef std::function<void(const Result&, TcpTransportPtr, AcceptGuardPtr)> AcceptHandler;
  typedef std::function<void(const Result&, TcpTransportPtr, ConnectGuardPtr)> ConnectHandler;

  static AcceptGuardPtr accept_async(ContextPtr context, boost::asio::ip::tcp::acceptor* acceptor,
                                     std::chrono::nanoseconds timeout, std::size_t transceive_byte_limit,
                                     AcceptHandler handler);

  static ConnectGuardPtr connect_async(ContextPtr context, const boost::asio::ip::tcp::endpoint& ep,
                                       std::chrono::nanoseconds timeout, std::size_t transceive_byte_limit,
                                       ConnectHandler handler);

  TcpTransport(ContextPtr context, boost::asio::ip::tcp::socket&& socket, std::chrono::nanoseconds timeout,
               std::size_t transceive_byte_limit, bool created_via_accept);

  boost::asio::ip::tcp::endpoint get_peer_endpoint() const {
    return socket_.remote_endpoint();
  }

 protected:
  virtual void write_some_async(boost::asio::const_buffer data, TransferSomeHandler handler) override;
  virtual void read_some_async(boost::asio::mutable_buffer data, TransferSomeHandler handler) override;
  virtual void shutdown() override;

 private:
  static std::string make_peer_description(const boost::asio::ip::tcp::socket& socket);
  static void close_socket(boost::asio::ip::tcp::socket* s);

  void set_no_delay_option();

  boost::asio::ip::tcp::socket socket_;
};
