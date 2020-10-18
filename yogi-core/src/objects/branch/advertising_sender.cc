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

#include <src/objects/branch/advertising_sender.h>
#include <src/util/algorithm.h>
#include <src/util/bind.h>

#include <boost/asio/ip/multicast.hpp>

YOGI_DEFINE_INTERNAL_LOGGER("Branch.AdvertisingSender")

AdvertisingSender::AdvertisingSender(ContextPtr context, const boost::asio::ip::udp::endpoint& adv_ep)
    : context_(context), adv_ep_(adv_ep), timer_(context->io_context()), active_send_ops_(0) {
}

void AdvertisingSender::start(LocalBranchInfoPtr info) {
  YOGI_ASSERT(!info_);

  info_ = info;
  set_logging_prefix(info->logging_prefix());

  setup_sockets();
  for (auto& socket : sockets_) {
    LOG_IFO("Using interface " << socket->address << " for sending advertising messages.");
  }

  send_advertisements();
}

void AdvertisingSender::setup_sockets() {
  for (auto& ifc : info_->get_advertising_interfaces()) {
    for (auto& addr : ifc.addresses) {
      auto entry            = std::make_shared<SocketEntry>(context_->io_context());
      entry->interface_name = ifc.name;
      entry->address        = addr;
      if (configure_socket(entry)) {
        sockets_.push_back(entry);
      }
    }
  }
}

bool AdvertisingSender::configure_socket(std::shared_ptr<SocketEntry> entry) {
  boost::system::error_code ec;
  entry->socket.open(adv_ep_.protocol(), ec);
  if (ec) {
    throw Error(YOGI_ERR_OPEN_SOCKET_FAILED);
  }

  auto opt =
      entry->address.is_v6()
          ? boost::asio::ip::multicast::outbound_interface(static_cast<unsigned int>(entry->address.to_v6().scope_id()))
          : boost::asio::ip::multicast::outbound_interface(entry->address.to_v4());
  entry->socket.set_option(opt, ec);

  if (ec) {
    LOG_ERR("Could not set outbound interface for socket using address " << entry->address << ": " << ec.message()
                                                                         << ". This interface will be ignored.");
    return false;
  }

  return true;
}

void AdvertisingSender::send_advertisements() {
  YOGI_ASSERT(active_send_ops_ == 0);

  if (sockets_.empty()) {
    LOG_ERR("No network interfaces available for sending advertising messages.");
    return;
  }

  auto msg = info_->make_advertising_message();

  auto weak_self = std::weak_ptr<AdvertisingSender>{shared_from_this()};
  for (const auto& socket : sockets_) {
    socket->socket.async_send_to(boost::asio::buffer(*msg), adv_ep_, [weak_self, msg, socket](auto ec, auto) {
      auto self = weak_self.lock();
      if (!self) return;

      self->on_advertisement_sent(ec, socket);
    });
    ++active_send_ops_;
  }
}

void AdvertisingSender::on_advertisement_sent(const boost::system::error_code& ec,
                                              const std::shared_ptr<SocketEntry>& socket) {
  if (ec) {
    LOG_ERR("Sending advertisement over " << socket->address << " failed: " << ec.message()
                                          << ". No more advertising messages will be sent over this interface.");

    sockets_.erase(find(sockets_, socket));
  }

  --active_send_ops_;
  if (active_send_ops_ == 0) {
    start_timer();
  }
}

void AdvertisingSender::start_timer() {
  timer_.expires_after(info_->get_advertising_interval());
  timer_.async_wait(bind_weak(&AdvertisingSender::on_timer_expired, this));
}

void AdvertisingSender::on_timer_expired(const boost::system::error_code& ec) {
  if (!ec) {
    send_advertisements();
  } else {
    LOG_ERR("Awaiting advertising timer expiry failed: " << ec.message()
                                                         << ". No more advertising messages will be sent.");
  }
}
