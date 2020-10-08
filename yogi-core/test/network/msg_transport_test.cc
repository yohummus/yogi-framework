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

#include <src/network/msg_transport.h>

#include <algorithm>
#include <atomic>
#include <random>

class FakeOutgoingMessage : public OutgoingMessage, public MessageT<MessageType::kBroadcast> {
 public:
  using MessageT::make_msg_bytes;

  FakeOutgoingMessage(SmallBuffer serialized_msg) : OutgoingMessage(serialized_msg) {
  }

  virtual std::string to_string() const override {
    return {};
  }
};

class FakeTransport : public Transport {
 public:
  FakeTransport(ContextPtr context)
      : Transport(context, std::chrono::nanoseconds::max(), true, "", std::numeric_limits<std::size_t>::max()) {
  }

  Buffer tx_data;
  Buffer rx_data;
  std::size_t tx_send_limit = std::numeric_limits<std::size_t>::max();
  bool dead                 = false;

 protected:
  virtual void write_some_async(boost::asio::const_buffer data, TransferSomeHandler handler) override {
    static std::default_random_engine gen;
    std::uniform_int_distribution<std::size_t> dist(1, data.size());
    auto n = std::min(dist(gen), tx_send_limit);
    auto p = static_cast<const char*>(data.data());

    tx_data.insert(tx_data.end(), p, p + n);

    post_handler(handler, n);
  }

  virtual void read_some_async(boost::asio::mutable_buffer data, TransferSomeHandler handler) override {
    if (rx_data.empty()) return;

    static std::default_random_engine gen;
    std::uniform_int_distribution<std::size_t> dist(1, std::min(data.size(), rx_data.size()));
    auto n = dist(gen);
    std::copy_n(rx_data.begin(), n, static_cast<char*>(data.data()));
    rx_data.erase(rx_data.begin(), rx_data.begin() + static_cast<Buffer::difference_type>(n));

    post_handler(handler, n);
  }

  virtual void shutdown() override {
    dead = true;
  }

 private:
  void post_handler(TransferSomeHandler handler, std::size_t bytes_transferred) {
    if (dead) {
      get_context()->post([=] { handler(Error(YOGI_ERR_RW_SOCKET_FAILED), bytes_transferred); });
    } else {
      get_context()->post([=] { handler(Success(), bytes_transferred); });
    }
  }
};

class MessageTransportTest : public TestFixture {
 protected:
  virtual void SetUp() override {
    context_   = std::make_shared<Context>();
    transport_ = std::make_shared<FakeTransport>(context_);
    uut_       = std::make_shared<MessageTransport>(transport_, 8, 8);
  }

  static FakeOutgoingMessage make_message(std::size_t msg_size) {
    if (msg_size == 0) {
      return FakeOutgoingMessage({});
    }

    static std::default_random_engine gen;
    std::uniform_int_distribution<int> dist(1, 100);

    SmallBuffer data(msg_size - 1);
    for (auto& byte : data) {
      byte = static_cast<Byte>(dist(gen));
    }
    data.insert(data.begin(), FakeOutgoingMessage::kMessageType);

    auto msg = FakeOutgoingMessage(data);
    msg.serialize_shared();  // To trigger copying the data vector
    return msg;
  }

  template <typename... Parts>
  static Buffer make_transport_bytes(Parts... parts) {
    Buffer v;
    make_transport_bytes_impl(&v, parts...);
    return v;
  }

  template <typename Next, typename... Rest>
  static void make_transport_bytes_impl(Buffer* v, Next next, Rest&&... rest) {
    append_to_byte_vector(v, next);
    make_transport_bytes_impl(v, rest...);
  }

  template <typename Last>
  static void make_transport_bytes_impl(Buffer* v, Last last) {
    append_to_byte_vector(v, last);
  }

  static void append_to_byte_vector(Buffer* v, int byte) {
    v->push_back(static_cast<Byte>(byte));
  }

  static void append_to_byte_vector(Buffer* v, const SmallBuffer& bytes) {
    v->insert(v->end(), bytes.begin(), bytes.end());
  }

  static void append_to_byte_vector(Buffer* v, const OutgoingMessage& msg) {
    append_to_byte_vector(v, msg.serialize());
  }

  ContextPtr context_;
  std::shared_ptr<FakeTransport> transport_;
  MessageTransportPtr uut_;
};

TEST_F(MessageTransportTest, MsgSizeFieldSerialization) {
  struct Entry {
    std::size_t ser_length;
    std::size_t value;
  };

  // those are all the corner cases
  const Entry entries[] = {
      // clang-format off
      {1, 0ul},
      {1, 127ul},
      {2, 128ul},
      {2, 16383ul},
      {3, 16384ul},
      {3, 2097151ul},
      {4, 2097152ul},
      {4, 268435455ul},
      {5, 268435456ul},
      {5, 4294967295ul},
      // clang-format on
  };

  for (auto entry : entries) {
    std::array<Byte, 5> buffer = {0};

    auto n = serialize_msg_size_field(entry.value, &buffer);
    EXPECT_EQ(n, entry.ser_length);

    std::size_t des_value;
    for (std::size_t i = 1; i < entry.ser_length; ++i) {
      EXPECT_FALSE(deserialize_msg_size_field(buffer, i, &des_value));
    }

    EXPECT_TRUE(deserialize_msg_size_field(buffer, entry.ser_length, &des_value));
    EXPECT_EQ(des_value, entry.value);
  }
}

TEST_F(MessageTransportTest, try_send) {
  uut_->start();

  auto msg = make_message(5);
  EXPECT_TRUE(uut_->try_send(msg));
  EXPECT_FALSE(uut_->try_send(msg));
  context_->poll();
  EXPECT_TRUE(uut_->try_send(msg));
  context_->poll();

  EXPECT_EQ(transport_->tx_data, make_transport_bytes(5, msg, 5, msg));
}

TEST_F(MessageTransportTest, TrySendTransportFailure) {
  uut_->start();

  transport_->close();
  uut_->try_send(make_message(5));
  context_->poll();

  EXPECT_THROW_ERROR(uut_->try_send(make_message(5)), YOGI_ERR_RW_SOCKET_FAILED);
}

TEST_F(MessageTransportTest, SendAsync) {
  transport_->tx_send_limit = 1;
  uut_->start();

  auto msg    = make_message(6);
  bool called = false;
  uut_->send_async(&msg, [&](auto& res) {
    EXPECT_EQ(res, Success());
    called = true;
  });
  context_->poll_one();
  context_->poll_one();
  EXPECT_TRUE(called);

  called = false;
  uut_->send_async(&msg, [&](auto& res) {
    EXPECT_EQ(res, Success());
    called = true;
  });
  context_->poll_one();
  context_->poll_one();
  EXPECT_FALSE(called);
  context_->poll();
  EXPECT_TRUE(called);

  EXPECT_EQ(transport_->tx_data, make_transport_bytes(6, msg, 6, msg));
}

TEST_F(MessageTransportTest, AsyncSendTransportFailure) {
  uut_->start();
  transport_->close();

  auto msg = make_message(6);
  uut_->try_send(msg);

  bool called = false;
  uut_->send_async(&msg, [&](auto& res) {
    EXPECT_EQ(res, Error(YOGI_ERR_RW_SOCKET_FAILED));
    called = true;
  });
  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(MessageTransportTest, cancel_send) {
  uut_->start();

  auto msg = make_message(6);
  EXPECT_TRUE(uut_->try_send(msg));

  bool called                        = false;
  MessageTransport::OperationTag tag = 123;
  uut_->send_async(&msg, tag, [&](auto& res) {
    EXPECT_EQ(res, Error(YOGI_ERR_CANCELED));
    called = true;
  });
  EXPECT_TRUE(uut_->cancel_send(tag));
  EXPECT_FALSE(uut_->cancel_send(tag));

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(MessageTransportTest, receive_async) {
  transport_->rx_data = Buffer{5, 1, 2, 3, 4, 5, 4, 1, 2, 3, 4};
  uut_->start();
  context_->poll();

  Buffer data(5);
  bool called = false;
  uut_->receive_async(boost::asio::buffer(data), [&](auto& res, auto size) {
    EXPECT_EQ(res, Success());
    EXPECT_EQ(size, 5);
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
  EXPECT_EQ(data, (Buffer{1, 2, 3, 4, 5}));

  data   = Buffer(4);
  called = false;
  uut_->receive_async(boost::asio::buffer(data), [&](auto& res, auto size) {
    EXPECT_EQ(res, Success());
    EXPECT_EQ(size, 4);
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
  EXPECT_EQ(data, (Buffer{1, 2, 3, 4}));
}

TEST_F(MessageTransportTest, ReceiveAsyncTransportFailure) {
  transport_->rx_data = Buffer{5, 1, 2, 3, 4, 5, 4, 1, 2, 3, 4};
  transport_->close();
  uut_->start();

  Buffer data(5);
  bool called = false;
  uut_->receive_async(boost::asio::buffer(data), [&](auto& res, auto) {
    EXPECT_EQ(res, Error(YOGI_ERR_RW_SOCKET_FAILED));
    called = true;
  });

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(MessageTransportTest, CancelReceive) {
  uut_->start();

  Buffer data(10);
  bool called = false;
  uut_->receive_async(boost::asio::buffer(data), [&](auto& res, auto) {
    EXPECT_EQ(res, Error(YOGI_ERR_CANCELED));
    called = true;
  });
  uut_->cancel_receive();

  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(MessageTransportTest, Close) {
  uut_->start();
  EXPECT_FALSE(transport_->dead);
  uut_->close();
  EXPECT_TRUE(transport_->dead);
}

TEST_F(MessageTransportTest, MessageOrderPreservation) {
  transport_->tx_send_limit = 0;  // Make sure buffer is not emptied
  uut_->start();

  auto msg1 = make_message(4);
  EXPECT_TRUE(uut_->try_send(msg1));

  // The msg2 and msg3 operations are to check that async writes are in order
  auto msg2 = make_message(3);
  uut_->send_async(&msg2, [&](auto& res) { EXPECT_EQ(res, Success()); });

  auto msg3 = make_message(1);
  uut_->send_async(&msg3, [&](auto& res) { EXPECT_EQ(res, Success()); });

  // This is to check that sync writes do not come before delayed async writes
  auto msg4 = make_message(1);
  EXPECT_FALSE(uut_->try_send(msg4));

  transport_->tx_send_limit = 100;  // Allow emptying the buffer

  auto msg5 = make_message(1);
  uut_->send_async(&msg5, [&](auto& res) { EXPECT_EQ(res, Success()); });

  context_->poll();

  EXPECT_EQ(transport_->tx_data, make_transport_bytes(4, msg1, 3, msg2, 1, msg3, 1, msg5));
}

TEST_F(MessageTransportTest, Stress) {
  // Bigger queue size so that the message size field can go beyond one byte
  const std::size_t kQueueSize = 300;
  uut_                         = std::make_shared<MessageTransport>(transport_, kQueueSize, kQueueSize);
  uut_->start();

  // Create randomly sized messages
  const std::size_t kInputSize = 20'000;

  static std::default_random_engine gen;
  std::uniform_int_distribution<std::size_t> dist(1, kQueueSize - 5);

  std::vector<FakeOutgoingMessage> msgs;
  while (transport_->tx_data.size() < kInputSize) {
    auto msg = make_message(dist(gen));
    msgs.push_back(msg);
    EXPECT_TRUE(uut_->try_send(msg));
    context_->poll();
  }

  auto old_tx_data = transport_->tx_data;

  // Re-create the transport and load the messages
  transport_          = std::make_shared<FakeTransport>(context_);
  uut_                = std::make_shared<MessageTransport>(transport_, kQueueSize, kQueueSize);
  transport_->rx_data = old_tx_data;
  uut_->start();
  context_->poll();

  // Create send and receive threads
  std::mutex mutex;
  std::thread tx_thread;
  std::thread rx_thread;

  std::vector<Buffer> sent_msgs_bytes;
  std::vector<Buffer> received_msgs_bytes;
  std::atomic<bool> send_done{false};

  // Start threads simultaneously'ish (that's what the mutex is for)
  {
    std::lock_guard<std::mutex> lock(mutex);

    // Send thread
    tx_thread = std::thread([&] {
      { std::lock_guard<std::mutex> lock(mutex); }

      for (auto& msg : msgs) {
        std::atomic<bool> called(false);
        uut_->send_async(&msg, [&](auto& res) {
          EXPECT_EQ(res, Success());
          auto bytes = msg.serialize();
          sent_msgs_bytes.push_back(Buffer(bytes.begin(), bytes.end()));
          called = true;
        });

        while (!called) {
          std::this_thread::yield();
        }
      }

      send_done = true;
    });

    // Receive thread
    rx_thread = std::thread([&] {
      { std::lock_guard<std::mutex> lock(mutex); }

      Buffer msg_bytes(kQueueSize);

      while (received_msgs_bytes.size() < msgs.size()) {
        std::generate(msg_bytes.begin(), msg_bytes.end(), [] { return static_cast<Byte>(0); });

        std::atomic<bool> called(false);
        uut_->receive_async(boost::asio::buffer(msg_bytes), [&](auto& res, auto msg_size) {
          EXPECT_EQ(res, Success());
          received_msgs_bytes.push_back(
              Buffer(msg_bytes.begin(), msg_bytes.begin() + static_cast<Buffer::difference_type>(msg_size)));
          called = true;
        });

        while (!called) {
          context_->poll();
        }
      }

      while (!send_done) {
        context_->poll();
      }
    });
  }

  // Wait for threads to finish
  tx_thread.join();
  rx_thread.join();

  EXPECT_EQ(received_msgs_bytes.size(), sent_msgs_bytes.size());
  EXPECT_EQ(received_msgs_bytes, sent_msgs_bytes);
}
