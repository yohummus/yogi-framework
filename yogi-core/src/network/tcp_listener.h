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

#include <src/objects/context.h>
#include <src/objects/logger/log_user.h>
#include <src/system/network_info.h>

#include <boost/asio/ip/tcp.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

class TcpListener;
typedef std::shared_ptr<TcpListener> TcpListenerPtr;
typedef std::weak_ptr<TcpListener> TcpListenerWeakPtr;

class TcpListener : public std::enable_shared_from_this<TcpListener>, public LogUser {
 public:
  typedef std::function<void(boost::asio::ip::tcp::socket socket)> AcceptFn;

  TcpListener(ContextPtr context, const std::vector<std::string>& interfaces, IpVersion ip_version, int port,
              const std::string& owner_type);

  TcpListener(ContextPtr context, const std::vector<std::string>& interfaces, IpVersion ip_version,
              const std::string& owner_type)
      : TcpListener(context, interfaces, ip_version, 0, owner_type) {
  }

  virtual ~TcpListener();

  int get_port() const {
    return port_;
  }

  const NetworkInterfaceInfosVector& get_interfaces() {
    return ifs_;
  }

  void start(AcceptFn accept_fn);

 private:
  TcpListenerWeakPtr make_weak_ptr() {
    return {shared_from_this()};
  }
  void update_logging_prefix();
  void setup_acceptors();
  void create_acceptor_for_all();
  void create_acceptors_for_specific();
  void add_acceptor(boost::asio::ip::tcp::endpoint ep);
  void throw_if_error(const boost::system::error_code& ec, int error_code, const boost::asio::ip::address& addr);
  void set_option_reuse_addr(bool on);
  void set_option_v6_only(bool on);
  void listen_on_all_acceptors();
  void start_accept(boost::asio::ip::tcp::acceptor* acc);
  void on_accept_finished(boost::system::error_code ec, boost::asio::ip::tcp::socket socket,
                          boost::asio::ip::tcp::acceptor* acc);

  const ContextPtr context_;
  const IpVersion ip_version_;
  const std::string owner_type_;
  AcceptFn accept_fn_;
  NetworkInterfaceInfosVector ifs_;
  bool use_all_ifs_;
  int port_;
  std::vector<boost::asio::ip::tcp::acceptor> acceptors_;
};
