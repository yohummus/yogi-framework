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

#include <src/objects/branch/branch_info.h>
#include <src/objects/context.h>
#include <src/objects/logger/log_user.h>

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <memory>

class AdvertisingSender : public std::enable_shared_from_this<AdvertisingSender>, public LogUser {
 public:
  AdvertisingSender(ContextPtr context, const boost::asio::ip::udp::endpoint& adv_ep);
  void start(LocalBranchInfoPtr info);

  const boost::asio::ip::udp::endpoint& get_endpoint() const {
    return adv_ep_;
  }

 private:
  struct SocketEntry {
    std::string interface_name;
    boost::asio::ip::address address;
    boost::asio::ip::udp::socket socket;

    SocketEntry(boost::asio::io_context& ioc) : socket(ioc) {
    }
  };

  void setup_sockets();
  bool configure_socket(std::shared_ptr<SocketEntry> entry);
  void send_advertisements();
  void on_advertisement_sent(const boost::system::error_code& ec, const std::shared_ptr<SocketEntry>& socket);
  void start_timer();
  void on_timer_expired(const boost::system::error_code& ec);

  const ContextPtr context_;
  boost::asio::ip::udp::endpoint adv_ep_;
  LocalBranchInfoPtr info_;
  boost::asio::steady_timer timer_;
  std::vector<std::shared_ptr<SocketEntry>> sockets_;
  int active_send_ops_;
};

typedef std::shared_ptr<AdvertisingSender> AdvertisingSenderPtr;
