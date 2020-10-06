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

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>

#include <string>
#include <vector>

enum class IpVersion { kAny, k4, k6 };

struct NetworkInterfaceInfo {
  std::string name;
  std::string identifier;  // The part after the % sign (e.g. ::1%en5 => "en5")
  std::string mac;
  std::vector<boost::asio::ip::address> addresses;
  bool is_loopback = false;
};

typedef std::vector<NetworkInterfaceInfo> NetworkInterfaceInfosVector;

NetworkInterfaceInfosVector get_network_interfaces();
NetworkInterfaceInfosVector get_filtered_network_interfaces(const std::vector<std::string>& if_strings,
                                                            IpVersion ip_version = IpVersion::kAny);
NetworkInterfaceInfosVector get_filtered_network_interfaces(const std::vector<std::string>& if_strings,
                                                            const boost::asio::ip::udp& protocol);

std::string get_hostname();

std::string make_ip_address_string(const boost::asio::ip::address& addr);
std::string make_ip_address_string(const boost::asio::ip::tcp::endpoint& ep);
std::string make_ip_address_string(const boost::asio::ip::udp::endpoint& ep);
