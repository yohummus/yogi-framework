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
#include <src/objects/branch/advertising_receiver.h>

#include <boost/asio/ip/multicast.hpp>
using namespace std::string_literals;

YOGI_DEFINE_INTERNAL_LOGGER("Branch.AdvertisingReceiver")

AdvertisingReceiver::AdvertisingReceiver(ContextPtr context, const boost::asio::ip::udp::endpoint& adv_ep)
    : context_(context), adv_ep_(adv_ep), socket_(context->io_context()) {
  buffer_ = make_shared_buffer(BranchInfo::kAdvertisingMessageSize + 1);

  setup_socket();
}

void AdvertisingReceiver::start(LocalBranchInfoPtr info, ObserverFn observer_fn) {
  YOGI_ASSERT(!info_);
  observer_fn_ = observer_fn;

  info_ = info;
  set_logging_prefix(info->logging_prefix());

  if (join_multicast_groups()) {
    start_receive_advertisement();
  }
}

void AdvertisingReceiver::setup_socket() {
  using namespace boost::asio::ip;

  boost::system::error_code ec;
  socket_.open(adv_ep_.protocol(), ec);
  if (ec) throw Error(YOGI_ERR_OPEN_SOCKET_FAILED);

  socket_.set_option(udp::socket::reuse_address(true), ec);
  if (ec) throw Error(YOGI_ERR_SET_SOCKET_OPTION_FAILED);

  socket_.bind(udp::endpoint(adv_ep_.protocol(), adv_ep_.port()), ec);
  if (ec) throw Error(YOGI_ERR_BIND_SOCKET_FAILED);
}

bool AdvertisingReceiver::join_multicast_groups() {
  using namespace boost::asio::ip;

  bool joined_at_least_once = false;
  for (auto& ifc : info_->get_advertising_interfaces()) {
    for (auto& addr : ifc.addresses) {
      boost::system::error_code ec;
      auto opt = addr.is_v6() ? multicast::join_group(adv_ep_.address().to_v6(), addr.to_v6().scope_id())
                              : multicast::join_group(adv_ep_.address().to_v4(), addr.to_v4());
      socket_.set_option(opt, ec);

      if (ec) {
        LOG_ERR("Could not join advertising multicast group " << adv_ep_ << " for interface " << addr << ": "
                                                              << ec.message() << ". This interface will be ignored.");
        continue;
      }

      LOG_IFO("Using interface " << addr << " for receiving advertising messages.");
      joined_at_least_once = true;
    }

    if (!joined_at_least_once) {
      LOG_ERR(
          "No network interfaces available for receiving advertising "
          "messages.");
    }
  }

  return joined_at_least_once;
}

void AdvertisingReceiver::start_receive_advertisement() {
  auto buffer    = buffer_;
  auto weak_self = std::weak_ptr<AdvertisingReceiver>{shared_from_this()};
  socket_.async_receive_from(boost::asio::buffer(*buffer_), sender_ep_,
                             [weak_self, buffer](auto ec, auto bytes_received) {
                               auto self = weak_self.lock();
                               if (!self) return;

                               self->on_received_advertisement_finished(ec, bytes_received);
                             });
}

void AdvertisingReceiver::on_received_advertisement_finished(const boost::system::error_code& ec,
                                                             std::size_t bytes_received) {
  if (ec) {
    LOG_ERR("Failed to receive advertising message: " << ec.message()
                                                      << ". No more advertising messages will be received.");
    return;
  }

  if (bytes_received != BranchInfo::kAdvertisingMessageSize) {
    LOG_WRN("Unexpected advertising message size received");
    start_receive_advertisement();
    return;
  }

  boost::uuids::uuid uuid;
  unsigned short tcp_port;
  auto res = RemoteBranchInfo::deserialize_advertising_message(&uuid, &tcp_port, *buffer_);
  if (res.is_error()) {
    LOG_WRN("Invalid advertising message received from " << sender_ep_.address() << ": " << res);
    start_receive_advertisement();
    return;
  }

  // Ignore advertising messages that we sent ourself
  if (uuid != info_->get_uuid()) {
    auto tcp_ep = boost::asio::ip::tcp::endpoint(sender_ep_.address(), tcp_port);
    observer_fn_(uuid, tcp_ep);
  }

  start_receive_advertisement();
}
