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

#include <src/api/constants.h>
#include <src/network/tcp_listener.h>
#include <src/util/algorithm.h>

#include <boost/asio/ip/tcp.hpp>
using namespace boost::asio;

#include <chrono>
#include <vector>
using namespace std::chrono_literals;

#define EXPECT_CONNECT(if_idx, ip_version, should_connect)               \
  EXPECT_EQ(can_connect(ifs_[if_idx].addr_##ip_version), should_connect) \
      << "Trying to connect to " << ifs_[if_idx].addr_##ip_version;

class TcpListenerTest : public TestFixture {
 protected:
  enum InterfacesCount { kOne, kTwo, kAll };

  struct TestInterface {
    std::string name;
    ip::address_v4 addr_v4;
    ip::address_v6 addr_v6;
  };

  virtual void SetUp() override {
    // Find network interfaces that support both IPv4 and IPv6
    for (auto info : get_network_interfaces()) {
      auto v4_it = find_if(info.addresses, [](auto& addr) { return addr.is_v4(); });
      auto v6_it = find_if(info.addresses, [](auto& addr) { return addr.is_v6(); });
      if (v4_it != info.addresses.end() && v6_it != info.addresses.end()) {
        ifs_.push_back({info.name, v4_it->to_v4(), v6_it->to_v6()});
      }
    }

    ASSERT_GE(ifs_.size(), 2) << "Not enough configured network interfaces "
                                 "available that support both IPv4 and IPv6. Usually this can be "
                                 "fixed by making sure that you have an active LAN or Wi-Fi "
                                 "connection.";
  }

  ContextPtr context_ = std::make_shared<Context>();
  TcpListenerPtr listener_;
  std::vector<TestInterface> ifs_;
  int fixed_port_   = find_unused_port();
  int accepted_cnt_ = 0;

  void create_listener(InterfacesCount if_cnt, IpVersion ip_version, int port = 0) {
    std::vector<std::string> interfaces;

    switch (if_cnt) {
      case kOne: interfaces.push_back(ifs_[0].name); break;

      case kTwo:
        interfaces.push_back(ifs_[0].name);
        interfaces.push_back(ifs_[1].name);
        break;

      case kAll: interfaces.push_back("all"); break;
    }

    listener_ = std::make_shared<TcpListener>(context_, interfaces, ip_version, port, "");

    listener_->start([this](auto socket) { ++this->accepted_cnt_; });
  }

  bool can_connect(const ip::address& addr) {
    ip::tcp::socket s(context_->io_context());

    int port = listener_ ? listener_->get_port() : fixed_port_;
    ip::tcp::endpoint ep(addr, static_cast<unsigned short>(port));

    int prev_accepted_cnt = accepted_cnt_;

    bool called    = false;
    bool connected = false;
    s.async_connect(ep, [&](auto ec) {
      connected = !ec;
      called    = true;
    });

    while (!called && context_->run_one(5s))
      ;

    EXPECT_TRUE(called);

    if (connected) {
      while (accepted_cnt_ == prev_accepted_cnt && context_->run_one(1s))
        ;
    }

    EXPECT_EQ(accepted_cnt_, prev_accepted_cnt + (connected ? 1 : 0));

    return connected;
  }
};

TEST_F(TcpListenerTest, GetPortForAllInterfacesWithAnyPort) {
  create_listener(kAll, IpVersion::kAny);
  EXPECT_GT(listener_->get_port(), 0);
}

TEST_F(TcpListenerTest, GetPortForOneInterfaceWithAnyPort) {
  create_listener(kTwo, IpVersion::kAny);
  EXPECT_GT(listener_->get_port(), 0);
}

TEST_F(TcpListenerTest, GetPortForAllInterfacesWithFixedPort) {
  create_listener(kAll, IpVersion::kAny, fixed_port_);
  EXPECT_EQ(listener_->get_port(), fixed_port_);
}

TEST_F(TcpListenerTest, GetPortForOneInterfaceWithFixedPort) {
  create_listener(kTwo, IpVersion::kAny, fixed_port_);
  EXPECT_EQ(listener_->get_port(), fixed_port_);
}

TEST_F(TcpListenerTest, AllInterfacesAnyPort) {
  create_listener(kAll, IpVersion::kAny);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v4, true);
  EXPECT_CONNECT(1, v6, true);
}

TEST_F(TcpListenerTest, AllInterfacesIpv4AnyPort) {
  create_listener(kAll, IpVersion::k4);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, false);
  EXPECT_CONNECT(1, v4, true);
  EXPECT_CONNECT(1, v6, false);
}

TEST_F(TcpListenerTest, AllInterfacesIpv6AnyPort) {
  create_listener(kAll, IpVersion::k6);

  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v6, true);

  // Connecting using IPv4 may still work
}

TEST_F(TcpListenerTest, AllInterfacesFixedPort) {
  EXPECT_CONNECT(0, v4, false);
  EXPECT_CONNECT(0, v6, false);
  EXPECT_CONNECT(1, v4, false);
  EXPECT_CONNECT(1, v6, false);

  create_listener(kAll, IpVersion::kAny, fixed_port_);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v4, true);
  EXPECT_CONNECT(1, v6, true);
}

TEST_F(TcpListenerTest, AllInterfacesIpv4FixedPort) {
  create_listener(kAll, IpVersion::k4, fixed_port_);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, false);
  EXPECT_CONNECT(1, v4, true);
  EXPECT_CONNECT(1, v6, false);
}

TEST_F(TcpListenerTest, AllInterfacesIpv6FixedPort) {
  create_listener(kAll, IpVersion::k6, fixed_port_);

  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v6, true);

  // Connecting using IPv4 may still work
}

TEST_F(TcpListenerTest, OneInterfaceAnyPort) {
  create_listener(kOne, IpVersion::kAny);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v4, false);
  EXPECT_CONNECT(1, v6, false);
}

TEST_F(TcpListenerTest, OneInterfaceIpv4AnyPort) {
  create_listener(kOne, IpVersion::k4);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, false);
  EXPECT_CONNECT(1, v4, false);
  EXPECT_CONNECT(1, v6, false);
}

TEST_F(TcpListenerTest, OneInterfaceIpv6AnyPort) {
  create_listener(kOne, IpVersion::k6);

  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v6, false);

  // Connecting using IPv4 may still work
}

TEST_F(TcpListenerTest, OneInterfaceFixedPort) {
  create_listener(kOne, IpVersion::kAny, fixed_port_);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v4, false);
  EXPECT_CONNECT(1, v6, false);
}

TEST_F(TcpListenerTest, OneInterfaceIpv4FixedPort) {
  create_listener(kOne, IpVersion::k4, fixed_port_);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, false);
  EXPECT_CONNECT(1, v4, false);
  EXPECT_CONNECT(1, v6, false);
}

TEST_F(TcpListenerTest, OneInterfaceIpv6FixedPort) {
  create_listener(kOne, IpVersion::k6, fixed_port_);

  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v6, false);

  // Connecting using IPv4 may still work
}

TEST_F(TcpListenerTest, TwoInterfacesAnyPort) {
  create_listener(kTwo, IpVersion::kAny);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v4, true);
  EXPECT_CONNECT(1, v6, true);
}

TEST_F(TcpListenerTest, TwoInterfacesFixedPort) {
  create_listener(kTwo, IpVersion::kAny, fixed_port_);

  EXPECT_CONNECT(0, v4, true);
  EXPECT_CONNECT(0, v6, true);
  EXPECT_CONNECT(1, v4, true);
  EXPECT_CONNECT(1, v6, true);
}
