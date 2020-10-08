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

#include <src/network/transport.h>

#include <gmock/gmock.h>
using namespace testing;
using namespace std::chrono_literals;

MATCHER_P(BufferEq, other, std::string(negation ? "isn't" : "is")) {
  *result_listener << "";
  return arg.data() == other.data() && arg.size() == other.size();
}

class MockTransport : public Transport {
 public:
  MockTransport(ContextPtr context, std::chrono::nanoseconds timeout, bool created_from_incoming_conn_req)
      : Transport(context, timeout, created_from_incoming_conn_req, "Broccoli",
                  std::numeric_limits<std::size_t>::max()) {
  }

  MOCK_METHOD2(write_some_async, void(boost::asio::const_buffer data, TransferSomeHandler handler));
  MOCK_METHOD2(read_some_async, void(boost::asio::mutable_buffer data, TransferSomeHandler handler));
  MOCK_METHOD0(shutdown, void());
};

class TransportTest : public TestFixture {
 protected:
  ContextPtr context_                       = Context::create();
  std::shared_ptr<MockTransport> transport_ = std::make_shared<MockTransport>(context_, 10s, false);
  Buffer data_                              = {1, 2, 3, 4, 5, 6};
};

TEST_F(TransportTest, get_context) {
  EXPECT_EQ(transport_->get_context(), context_);
}

TEST_F(TransportTest, CreatedFromIncomingConnectionRequest) {
  auto a = std::make_shared<MockTransport>(context_, 1s, true);
  EXPECT_TRUE(a->created_from_incoming_connection_request());

  auto b = std::make_shared<MockTransport>(context_, 1s, false);
  EXPECT_FALSE(b->created_from_incoming_connection_request());
}

TEST_F(TransportTest, GetPeerDescription) {
  EXPECT_EQ(transport_->get_peer_description(), "Broccoli");
}

TEST_F(TransportTest, SendSomeSuccess) {
  // clang-format off
  EXPECT_CALL(*transport_, shutdown())
    .Times(0);

  EXPECT_CALL(*transport_, write_some_async(BufferEq(boost::asio::buffer(data_)), _))
      .WillOnce(InvokeArgument<1>(Success(), 2));
  // clang-format on

  bool called = false;
  transport_->send_some_async(boost::asio::buffer(data_), [&](auto& res, auto bytes_sent) {
    EXPECT_EQ(res, Success());
    EXPECT_EQ(bytes_sent, 2);
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, SendSomeFailure) {
  // clang-format off
  EXPECT_CALL(*transport_, shutdown());

  EXPECT_CALL(*transport_, write_some_async(_, _))
    .WillOnce(InvokeArgument<1>(Error(YOGI_ERR_RW_SOCKET_FAILED), 2));
  // clang-format on

  bool called = false;
  transport_->send_some_async(boost::asio::buffer(data_), [&](auto& res, auto bytes_sent) {
    EXPECT_EQ(res, Error(YOGI_ERR_RW_SOCKET_FAILED));
    EXPECT_EQ(bytes_sent, 2);
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, SendSomeTimeout) {
  // clang-format off
  transport_ = std::make_shared<MockTransport>(context_, 1ms, false);

  Transport::TransferSomeHandler handler;
  EXPECT_CALL(*transport_, write_some_async(_, _))
    .WillOnce(SaveArg<1>(&handler));

  EXPECT_CALL(*transport_, shutdown())
    .WillOnce(Invoke([&] { handler(Error(YOGI_ERR_CANCELED), 2); }));
  // clang-format on

  auto start_time = std::chrono::steady_clock::now();

  bool called = false;
  transport_->send_some_async(boost::asio::buffer(data_), [&](auto& res, auto bytes_sent) {
    EXPECT_EQ(res, Error(YOGI_ERR_TIMEOUT));
    EXPECT_EQ(bytes_sent, 2);
    called = true;
  });

  while (!called) {
    context_->run_one(100us);
  }

  EXPECT_GT(std::chrono::steady_clock::now(), start_time + 1ms);
  EXPECT_LT(std::chrono::steady_clock::now(), start_time + 1ms + kTimingMargin);
}

TEST_F(TransportTest, SendAllSuccess) {
  // clang-format off
  EXPECT_CALL(*transport_, shutdown())
    .Times(0);

  InSequence dummy;
  EXPECT_CALL(*transport_, write_some_async(BufferEq(boost::asio::buffer(data_)), _))
    .WillOnce(InvokeArgument<1>(Success(), 2));

  EXPECT_CALL(*transport_, write_some_async(BufferEq(boost::asio::buffer(data_) + 2), _))
    .WillOnce(InvokeArgument<1>(Success(), 1));

  EXPECT_CALL(*transport_, write_some_async(BufferEq(boost::asio::buffer(data_) + 3), _))
    .WillOnce(InvokeArgument<1>(Success(), 3));
  // clang-format on

  bool called = false;
  transport_->send_all_async(boost::asio::buffer(data_), [&](auto& res) {
    EXPECT_EQ(res, Success());
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, SendAllFailure) {
  // clang-format off
  EXPECT_CALL(*transport_, shutdown());

  InSequence dummy;
  EXPECT_CALL(*transport_, write_some_async(BufferEq(boost::asio::buffer(data_)), _))
    .WillOnce(InvokeArgument<1>(Success(), 2));

  EXPECT_CALL(*transport_, write_some_async(BufferEq(boost::asio::buffer(data_) + 2), _))
    .WillOnce(InvokeArgument<1>(Error(YOGI_ERR_RW_SOCKET_FAILED), 1));
  // clang-format on

  bool called = false;
  transport_->send_all_async(boost::asio::buffer(data_), [&](auto& res) {
    EXPECT_EQ(res, Error(YOGI_ERR_RW_SOCKET_FAILED));
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, SendAllTimeout) {
  // clang-format off
  transport_ = std::make_shared<MockTransport>(context_, 1ms, false);

  Transport::TransferSomeHandler handler;
  EXPECT_CALL(*transport_, write_some_async(_, _))
    .WillOnce(InvokeArgument<1>(Success(), 1))
    .WillOnce(SaveArg<1>(&handler));

  EXPECT_CALL(*transport_, shutdown())
    .WillOnce(Invoke([&] { handler(Error(YOGI_ERR_CANCELED), 2); }));
  // clang-format on

  auto start_time = std::chrono::steady_clock::now();

  bool called = false;
  transport_->send_all_async(boost::asio::buffer(data_), [&](auto& res) {
    EXPECT_EQ(res, Error(YOGI_ERR_TIMEOUT));
    called = true;
  });

  while (!called) {
    context_->run_one(100us);
  }

  EXPECT_GT(std::chrono::steady_clock::now(), start_time + 1ms);
  EXPECT_LT(std::chrono::steady_clock::now(), start_time + 1ms + kTimingMargin);
}

TEST_F(TransportTest, SendAllSharedBuffer) {
  // clang-format off
  EXPECT_CALL(*transport_, write_some_async(_, _))
    .WillOnce(InvokeArgument<1>(Success(), data_.size()));
  // clang-format on

  bool called = false;
  transport_->send_all_async(make_shared_buffer(data_), [&](auto& res) {
    EXPECT_EQ(res, Success());
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, SendAllSharedSmallBuffer) {
  // clang-format off
  EXPECT_CALL(*transport_, write_some_async(_, _))
    .WillOnce(InvokeArgument<1>(Success(), data_.size()));
  // clang-format on

  bool called = false;
  transport_->send_all_async(make_shared_small_buffer(data_.begin(), data_.end()), [&](auto& res) {
    EXPECT_EQ(res, Success());
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, ReceiveSomeSuccess) {
  // clang-format off
  EXPECT_CALL(*transport_, shutdown())
    .Times(0);

  EXPECT_CALL(*transport_, read_some_async(BufferEq(boost::asio::buffer(data_)), _))
    .WillOnce(InvokeArgument<1>(Success(), 2));
  // clang-format on

  bool called = false;
  transport_->receive_some_async(boost::asio::buffer(data_), [&](auto& res, auto bytes_received) {
    EXPECT_EQ(res, Success());
    EXPECT_EQ(bytes_received, 2);
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, ReceiveSomeFailure) {
  // clang-format off
  EXPECT_CALL(*transport_, shutdown());

  EXPECT_CALL(*transport_, read_some_async(_, _))
    .WillOnce(InvokeArgument<1>(Error(YOGI_ERR_RW_SOCKET_FAILED), 2));
  // clang-format on

  bool called = false;
  transport_->receive_some_async(boost::asio::buffer(data_), [&](auto& res, auto bytes_received) {
    EXPECT_EQ(res, Error(YOGI_ERR_RW_SOCKET_FAILED));
    EXPECT_EQ(bytes_received, 2);
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, ReceiveSomeTimeout) {
  // clang-format off
  transport_ = std::make_shared<MockTransport>(context_, 1ms, false);

  Transport::TransferSomeHandler handler;
  EXPECT_CALL(*transport_, read_some_async(_, _))
    .WillOnce(SaveArg<1>(&handler));

  EXPECT_CALL(*transport_, shutdown())
    .WillOnce(Invoke([&] { handler(Error(YOGI_ERR_CANCELED), 2); }));
  // clang-format on

  auto start_time = std::chrono::steady_clock::now();

  bool called = false;
  transport_->receive_some_async(boost::asio::buffer(data_), [&](auto& res, auto bytes_sent) {
    EXPECT_EQ(res, Error(YOGI_ERR_TIMEOUT));
    EXPECT_EQ(bytes_sent, 2);
    called = true;
  });

  while (!called) {
    context_->run_one(100us);
  }

  EXPECT_GT(std::chrono::steady_clock::now(), start_time + 1ms);
  EXPECT_LT(std::chrono::steady_clock::now(), start_time + 1ms + kTimingMargin);
}

TEST_F(TransportTest, ReceiveAllSuccess) {
  // clang-format off
  EXPECT_CALL(*transport_, shutdown())
    .Times(0);

  InSequence dummy;
  EXPECT_CALL(*transport_, read_some_async(BufferEq(boost::asio::buffer(data_)), _))
    .WillOnce(InvokeArgument<1>(Success(), 2));

  EXPECT_CALL(*transport_, read_some_async(BufferEq(boost::asio::buffer(data_) + 2), _))
    .WillOnce(InvokeArgument<1>(Success(), 1));

  EXPECT_CALL(*transport_, read_some_async(BufferEq(boost::asio::buffer(data_) + 3), _))
    .WillOnce(InvokeArgument<1>(Success(), 3));
  // clang-format on

  bool called = false;
  transport_->receive_all_async(boost::asio::buffer(data_), [&](auto& res) {
    EXPECT_EQ(res, Success());
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, ReceiveAllFailure) {
  // clang-format off
  EXPECT_CALL(*transport_, shutdown());

  InSequence dummy;
  EXPECT_CALL(*transport_, read_some_async(BufferEq(boost::asio::buffer(data_)), _))
    .WillOnce(InvokeArgument<1>(Success(), 2));

  EXPECT_CALL(*transport_, read_some_async(BufferEq(boost::asio::buffer(data_) + 2), _))
    .WillOnce(InvokeArgument<1>(Error(YOGI_ERR_RW_SOCKET_FAILED), 1));
  // clang-format on

  bool called = false;
  transport_->receive_all_async(boost::asio::buffer(data_), [&](auto& res) {
    EXPECT_EQ(res, Error(YOGI_ERR_RW_SOCKET_FAILED));
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, ReceiveAllTimeout) {
  // clang-format off
  transport_ = std::make_shared<MockTransport>(context_, 1ms, false);

  Transport::TransferSomeHandler handler;
  EXPECT_CALL(*transport_, read_some_async(_, _))
    .WillOnce(InvokeArgument<1>(Success(), 1))
    .WillOnce(SaveArg<1>(&handler));

  EXPECT_CALL(*transport_, shutdown())
    .WillOnce(Invoke([&] { handler(Error(YOGI_ERR_CANCELED), 2); }));
  // clang-format on

  auto start_time = std::chrono::steady_clock::now();

  bool called = false;
  transport_->receive_all_async(boost::asio::buffer(data_), [&](auto& res) {
    EXPECT_EQ(res, Error(YOGI_ERR_TIMEOUT));
    called = true;
  });

  while (!called) {
    context_->run_one(100us);
  }

  EXPECT_GT(std::chrono::steady_clock::now(), start_time + 1ms);
  EXPECT_LT(std::chrono::steady_clock::now(), start_time + 1ms + kTimingMargin);
}

TEST_F(TransportTest, ReceiveAllSharedBuffer) {
  // clang-format off
  EXPECT_CALL(*transport_, read_some_async(_, _))
    .WillOnce(InvokeArgument<1>(Success(), data_.size()));
  // clang-format on

  bool called = false;
  transport_->receive_all_async(make_shared_buffer(data_), [&](auto& res) {
    EXPECT_EQ(res, Success());
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, CallingSendHandlerOnDestruction) {
  // clang-format off
  Transport::TransferSomeHandler handler;
  EXPECT_CALL(*transport_, write_some_async(_, _))
    .WillOnce(SaveArg<1>(&handler));
  // clang-format on

  bool called = false;
  transport_->send_some_async(boost::asio::buffer(data_), [&](auto& res, auto) {
    EXPECT_EQ(res, Error(YOGI_ERR_CANCELED));
    called = true;
  });

  transport_.reset();
  EXPECT_FALSE(called);

  handler(Error(YOGI_ERR_CANCELED), 0);
  EXPECT_TRUE(called);
}

TEST_F(TransportTest, CallingReceiveHandlerOnDestruction) {
  // clang-format off
  Transport::TransferSomeHandler handler;
  EXPECT_CALL(*transport_, read_some_async(_, _))
    .WillOnce(SaveArg<1>(&handler));
  // clang-format on

  bool called = false;
  transport_->receive_some_async(boost::asio::buffer(data_), [&](auto& res, auto) {
    EXPECT_EQ(res, Error(YOGI_ERR_CANCELED));
    called = true;
  });

  transport_.reset();
  EXPECT_FALSE(called);

  handler(Error(YOGI_ERR_CANCELED), 0);
  EXPECT_TRUE(called);
}
