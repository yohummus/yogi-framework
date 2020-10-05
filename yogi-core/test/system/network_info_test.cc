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

TEST(SystemTest, GetFilteredNetworkInterfaces) {
  std::vector<IpVersion> ip_versions = {IpVersion::kAny, IpVersion::k4, IpVersion::k6};
  for (auto ip_version : ip_versions) {
    auto ifs = get_filtered_network_interfaces({"localhost"}, ip_version);
    ASSERT_EQ(ifs.size(), 1);
    EXPECT_TRUE(ifs[0].is_loopback);

    ifs = get_filtered_network_interfaces({"all"}, ip_version);
    ASSERT_GT(ifs.size(), 1) << "Make sure you have an active LAN or Wi-Fi connection, otherwise "
                                "the test fails because it cannot find any network interfaces other "
                                "than the loopback interface.";

    if (ip_version != IpVersion::kAny) {
      for (auto& info : ifs) {
        for (auto& addr : info.addresses) {
          EXPECT_TRUE(addr.is_v4() == (ip_version == IpVersion::k4));
        }
      }
    }

    auto if_it = find_if(ifs, [](auto& info) { return !info.is_loopback && !info.mac.empty(); });
    ASSERT_NE(if_it, ifs.end()) << "No network interface found that has a MAC "
                                   "and that is not a loopback interface.";
    auto ifc = *if_it;

    ifs = get_filtered_network_interfaces({ifc.name}, ip_version);
    ASSERT_EQ(ifs.size(), 1);
    EXPECT_EQ(ifs[0].name, ifc.name);

    ifs = get_filtered_network_interfaces({ifc.mac}, ip_version);
    ASSERT_EQ(ifs.size(), 1);
    EXPECT_EQ(ifs[0].mac, ifc.mac);
  }
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
