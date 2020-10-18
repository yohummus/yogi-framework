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
#include <boost/uuid/uuid.hpp>
#include <functional>
#include <memory>
#include <vector>

class AdvertisingReceiver : public std::enable_shared_from_this<AdvertisingReceiver>, public LogUser {
 public:
  typedef std::function<void(const boost::uuids::uuid& uuid, const boost::asio::ip::tcp::endpoint& ep)> ObserverFn;

  AdvertisingReceiver(ContextPtr context, const boost::asio::ip::udp::endpoint& adv_ep);

  void start(LocalBranchInfoPtr info, ObserverFn observer_fn);

 private:
  void setup_socket();
  bool join_multicast_groups();
  void start_receive_advertisement();
  void on_received_advertisement_finished(const boost::system::error_code& ec, std::size_t bytes_received);

  const ContextPtr context_;
  boost::asio::ip::udp::endpoint adv_ep_;
  ObserverFn observer_fn_;
  SharedBuffer buffer_;
  LocalBranchInfoPtr info_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_ep_;
};

typedef std::shared_ptr<AdvertisingReceiver> AdvertisingReceiverPtr;
