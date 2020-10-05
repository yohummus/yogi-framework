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

#include <test/common.h>

#include <src/system/network_info.h>
#include <src/util/algorithm.h>

#include <regex>

TEST(SystemTest, GetHostname) {
  EXPECT_FALSE(get_hostname().empty());
}

TEST(SystemTest, GetNetworkInterfaces) {
  auto ifs = get_network_interfaces();
  ASSERT_FALSE(ifs.empty());

  auto localhost_found = false;

  for (auto& info : ifs) {
    EXPECT_FALSE(info.name.empty());
    EXPECT_FALSE(info.identifier.empty());
    EXPECT_FALSE(info.mac.empty() && info.addresses.empty());

    if (!info.mac.empty()) {
      std::regex re("^([0-9a-f][0-9a-f]:){5}[0-9a-f][0-9a-f]$");
      std::smatch m;
      EXPECT_TRUE(std::regex_match(info.mac, m, re)) << "String: " << info.mac;
    }

    bool is_loopback = contains_if(info.addresses, [](auto& addr) { return addr.is_loopback(); });
    EXPECT_EQ(is_loopback, info.is_loopback);
    localhost_found |= is_loopback;
  }

  EXPECT_TRUE(localhost_found);
}

TEST(SystemTest, MakeIpAddressString) {
  auto addr = boost::asio::ip::address::from_string("192.168.1.2");
  EXPECT_EQ(make_ip_address_string(addr), "192.168.1.2");

  auto tcp_ep = boost::asio::ip::tcp::endpoint(addr, 12345);
  EXPECT_EQ(make_ip_address_string(tcp_ep), "192.168.1.2");

  auto udp_ep = boost::asio::ip::udp::endpoint(addr, 12345);
  EXPECT_EQ(make_ip_address_string(udp_ep), "192.168.1.2");

  addr = boost::asio::ip::address::from_string("0:0:0:0:0:0:0:1");
  EXPECT_EQ(make_ip_address_string(addr), "::1");
}
