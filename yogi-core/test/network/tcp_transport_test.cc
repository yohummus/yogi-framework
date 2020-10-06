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

#include <src/network/tcp_transport.h>

using namespace std::chrono_literals;

class TcpTransportTest : public TestFixture {
 protected:
  virtual void SetUp() override {
    acceptor_.open(kTcpProtocol);
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(boost::asio::ip::tcp::endpoint(kTcpProtocol, 0));
    acceptor_.listen();

    acceptor_ep_ = boost::asio::ip::tcp::endpoint(kLoopbackAddress, acceptor_.local_endpoint().port());
  }

  TcpTransportPtr connect() {
    TcpTransportPtr transport;
    auto guard = TcpTransport::connect_async(context_, acceptor_ep_, 10s, std::numeric_limits<std::size_t>::max(),
                                             [&](auto& res, auto tp, auto) {
                                               ASSERT_EQ(res, Success());
                                               transport = tp;
                                             });

    bool accepted = false;
    acceptor_.async_accept(socket_, [&](auto& ec) {
      EXPECT_TRUE(!ec) << ec.message();
      accepted = true;
    });

    while (!transport || !accepted) {
      context_->run_one(100us);
    }

    return transport;
  };

  ContextPtr context_ = Context::create();
  boost::asio::ip::tcp::acceptor acceptor_{context_->io_context()};
  boost::asio::ip::tcp::endpoint acceptor_ep_;
  boost::asio::ip::tcp::socket socket_{context_->io_context()};
  std::vector<char> data_ = {1, 2, 3, 4, 5, 6};
};

TEST_F(TcpTransportTest, Accept) {
  bool called = false;
  auto guard  = TcpTransport::accept_async(context_, &acceptor_, 10s, std::numeric_limits<std::size_t>::max(),
                                          [&](auto& res, auto transport, auto) {
                                            EXPECT_EQ(res, Success());
                                            ASSERT_TRUE(!!transport);
                                            EXPECT_TRUE(transport->created_from_incoming_connection_request());
                                            called = true;
                                          });

  socket_.async_connect(acceptor_ep_, [](auto& ec) { EXPECT_TRUE(!ec) << ec.message(); });

  while (!called) {
    context_->run_one(100us);
  }
}

TEST_F(TcpTransportTest, CancelAccept) {
  bool called = false;
  auto guard  = TcpTransport::accept_async(context_, &acceptor_, 10s, std::numeric_limits<std::size_t>::max(),
                                          [&](auto& res, auto transport, auto guard) {
                                            EXPECT_EQ(res, Error(YOGI_ERR_CANCELED));
                                            ASSERT_FALSE(!!transport);
                                            ASSERT_FALSE(!!guard);
                                            called = true;
                                          });

  guard.reset();

  while (!called) {
    context_->run_one(100us);
  }
}

TEST_F(TcpTransportTest, Connect) {
  auto transport = connect();
  EXPECT_FALSE(transport->created_from_incoming_connection_request());
}

TEST_F(TcpTransportTest, CancelConnect) {
  acceptor_.close();

  bool called = false;
  auto guard  = TcpTransport::connect_async(context_, acceptor_ep_, 10s, std::numeric_limits<std::size_t>::max(),
                                           [&](auto& res, auto transport, auto guard) {
                                             EXPECT_EQ(res, Error(YOGI_ERR_CANCELED));
                                             ASSERT_FALSE(!!transport);
                                             ASSERT_FALSE(!!guard);
                                             called = true;
                                           });

  guard.reset();

  while (!called) {
    context_->run_one(100us);
  }
}

TEST_F(TcpTransportTest, ConnectTimeout) {
  acceptor_.close();

  auto start_time = std::chrono::steady_clock::now();

  bool called = false;
  auto guard  = TcpTransport::connect_async(
      context_, acceptor_ep_, 1ms, std::numeric_limits<std::size_t>::max(), [&](auto& res, auto transport, auto) {
        EXPECT_TRUE(res == Error(YOGI_ERR_TIMEOUT) || res == Error(YOGI_ERR_CONNECT_SOCKET_FAILED));
        EXPECT_FALSE(!!transport);
        called = true;
      });

  while (!called) {
    context_->run_one(100us);
  }

  EXPECT_LT(std::chrono::steady_clock::now(), start_time + 1ms + 1s);
}

TEST_F(TcpTransportTest, PeerDescription) {
  auto transport = connect();
  EXPECT_GT(transport->get_peer_description().size(), 3);
  EXPECT_NE(transport->get_peer_description().find(':'), std::string::npos);
}

TEST_F(TcpTransportTest, Send) {
  auto transport = connect();

  bool received = false;
  std::vector<char> buffer(data_.size());
  boost::asio::async_read(socket_, boost::asio::buffer(buffer), [&](auto& ec, auto bytes_read) {
    EXPECT_TRUE(!ec) << ec.message();
    EXPECT_EQ(bytes_read, buffer.size());
    received = true;
  });

  bool sent = false;
  transport->send_all_async(boost::asio::buffer(data_), [&](auto& res) {
    EXPECT_EQ(res, Success());
    sent = true;
  });

  while (!sent || !received) {
    context_->run_one(100us);
  }

  EXPECT_EQ(data_, buffer);
}

TEST_F(TcpTransportTest, SendFailure) {
  auto transport = connect();
  transport->close();

  bool called = false;
  transport->send_some_async(boost::asio::buffer(data_), [&](auto& res, auto) {
    EXPECT_TRUE(res.is_error());
    called = true;
  });

  while (!called) {
    context_->run_one(100us);
  }
}

TEST_F(TcpTransportTest, Receive) {
  auto transport = connect();

  bool received = false;
  std::vector<char> buffer(data_.size());
  transport->receive_all_async(boost::asio::buffer(buffer), [&](auto& res) {
    EXPECT_EQ(res, Success());
    received = true;
  });

  bool sent = false;
  boost::asio::async_write(socket_, boost::asio::buffer(data_), [&](auto& ec, auto bytes_written) {
    EXPECT_TRUE(!ec) << ec.message();
    EXPECT_EQ(bytes_written, this->data_.size());
    sent = true;
  });

  while (!sent || !received) {
    context_->run_one(100us);
  }

  EXPECT_EQ(data_, buffer);
}

TEST_F(TcpTransportTest, ReceiveFailure) {
  auto transport = connect();

  bool called = false;
  transport->receive_some_async(boost::asio::buffer(data_), [&](auto& res, auto) {
    EXPECT_TRUE(res.is_error());
    called = true;
  });

  transport->close();

  while (!called) {
    context_->run_one(100us);
  }
}
