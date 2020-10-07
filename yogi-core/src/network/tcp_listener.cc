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

#include <src/network/tcp_listener.h>
#include <src/util/algorithm.h>

#include <boost/asio/ip/v6_only.hpp>
#include <boost/asio/strand.hpp>
namespace asio = boost::asio;
namespace ip   = asio::ip;
using tcp      = ip::tcp;

#include <sstream>
using namespace std::string_literals;

YOGI_DEFINE_INTERNAL_LOGGER("TcpListener")

TcpListener::TcpListener(ContextPtr context, const std::vector<std::string>& interfaces, IpVersion ip_version, int port,
                         const std::string& owner_type)
    : context_(context), ip_version_(ip_version), owner_type_(owner_type), port_(port) {
  ifs_         = get_filtered_network_interfaces(interfaces, ip_version_);
  use_all_ifs_ = contains(interfaces, "all");

  if (ifs_.empty() && !use_all_ifs_) {
    throw DescriptiveError(YOGI_ERR_CONFIG_NOT_VALID) << "No matching network interfaces found";
  }

  update_logging_prefix();
  setup_acceptors();
}

TcpListener::~TcpListener() {
  for (auto& acc : acceptors_) {
    boost::system::error_code ec;
    acc.close(ec);
  }
}

void TcpListener::start(AcceptFn accept_fn) {
  accept_fn_ = accept_fn;
  for (auto& acc : acceptors_) {
    start_accept(&acc);
  }
}

void TcpListener::update_logging_prefix() {
  std::stringstream ss;
  ss << "For " << owner_type_;
  if (port_) ss << " on port " << port_;
  set_logging_prefix(ss.str());
}

void TcpListener::setup_acceptors() {
  if (use_all_ifs_) {
    create_acceptor_for_all();
  } else {
    create_acceptors_for_specific();
  }

  update_logging_prefix();
  set_option_reuse_addr(true);
  set_option_v6_only(ip_version_ != IpVersion::k4);
  listen_on_all_acceptors();

  YOGI_ASSERT(!acceptors_.empty());
}

void TcpListener::create_acceptor_for_all() {
  add_acceptor(tcp::endpoint(ip_version_ == IpVersion::k4 ? tcp::v4() : tcp::v6(), static_cast<unsigned short>(port_)));
}

void TcpListener::create_acceptors_for_specific() {
  for (auto& info : ifs_) {
    for (auto& addr : info.addresses) {
      add_acceptor(tcp::endpoint(addr, static_cast<unsigned short>(port_)));
    }
  }
}

void TcpListener::add_acceptor(tcp::endpoint ep) {
  boost::system::error_code ec;
  tcp::acceptor acc(asio::make_strand(context_->io_context()));

  acc.open(ep.protocol(), ec);
  throw_if_error(ec, YOGI_ERR_OPEN_SOCKET_FAILED, ep.address());

  acc.bind(ep, ec);
  throw_if_error(ec, YOGI_ERR_BIND_SOCKET_FAILED, ep.address());

  port_ = static_cast<int>(acc.local_endpoint().port());
  acceptors_.push_back(std::move(acc));
}

void TcpListener::throw_if_error(const boost::system::error_code& ec, int error_code, const ip::address& addr) {
  if (!ec) return;

  DescriptiveError e(error_code);
  if (error_code != YOGI_ERR_OPEN_SOCKET_FAILED) {
    e << addr;
    if (port_) e << ":" << port_;
    e << ": ";
  }
  e << ec.message();
  throw e;
}

void TcpListener::set_option_reuse_addr(bool on) {
  for (auto& acc : acceptors_) {
    boost::system::error_code ec;
    acc.set_option(tcp::acceptor::reuse_address(on), ec);
    if (ec) {
      LOG_ERR("Could not set socket option SO_REUSEADDR: " << ec.message());
    }
  }
}

void TcpListener::set_option_v6_only(bool on) {
  for (auto& acc : acceptors_) {
    boost::system::error_code ec;
    acc.set_option(tcp::acceptor::reuse_address(on), ec);
    if (ec) {
      LOG_ERR("Could not clear socket option IPV6_V6ONLY: " << ec.message());
    }
  }
}

void TcpListener::listen_on_all_acceptors() {
  for (auto& acc : acceptors_) {
    boost::system::error_code ec;
    acc.listen(acc.max_listen_connections, ec);
    throw_if_error(ec, YOGI_ERR_LISTEN_SOCKET_FAILED, acc.local_endpoint().address());
    LOG_IFO("Listening for connections on " << acc.local_endpoint().address());
  }
}

void TcpListener::start_accept(tcp::acceptor* acc) {
  auto weak_self = make_weak_ptr();
  acc->async_accept(asio::make_strand(context_->io_context()), [=](auto ec, auto socket) {
    auto self = weak_self.lock();
    if (!self) return;

    self->on_accept_finished(ec, std::move(socket), acc);
  });
}

void TcpListener::on_accept_finished(boost::system::error_code ec, tcp::socket socket, tcp::acceptor* acc) {
  if (!ec) {
    LOG_DBG("Accepted connection from " << socket.remote_endpoint());
    accept_fn_(std::move(socket));
  } else if (ec != asio::error::operation_aborted) {
    LOG_ERR("Accepting connection on " << acc->local_endpoint().address() << " failed: " << ec.message());
  }

  start_accept(acc);
}
