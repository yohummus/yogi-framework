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

#include <yogi_core.h>

#include <chrono>
using namespace std::chrono_literals;

#define MAKE_CTORS_PUBLIC(cls, new_cls)                          \
  class new_cls : public cls {                                   \
   public:                                                       \
    template <typename... Args>                                  \
    new_cls(Args&&... args) : cls(std::forward<Args>(args)...) { \
    }                                                            \
  }

MAKE_CTORS_PUBLIC(yogi::BranchInfo, TestBranchInfo);
MAKE_CTORS_PUBLIC(yogi::RemoteBranchInfo, TestRemoteBranchInfo);
MAKE_CTORS_PUBLIC(yogi::LocalBranchInfo, TestLocalBranchInfo);
MAKE_CTORS_PUBLIC(yogi::BranchEventInfo, TestBranchEventInfo);
MAKE_CTORS_PUBLIC(yogi::BranchDiscoveredEventInfo, TestBranchDiscoveredEventInfo);
MAKE_CTORS_PUBLIC(yogi::BranchQueriedEventInfo, TestBranchQueriedEventInfo);
MAKE_CTORS_PUBLIC(yogi::ConnectFinishedEventInfo, TestConnectFinishedEventInfo);
MAKE_CTORS_PUBLIC(yogi::ConnectionLostEventInfo, TestConnectionLostEventInfo);

class BranchTest : public Test {};

TEST_F(BranchTest, BranchInfo) {
  yogi::Uuid uuid = {};
  uuid.data()[0]  = 123;

  auto json = R"({
    "name":                 "foo",
    "description":          "bar",
    "network_name":         "local",
    "path":                 "/test",
    "hostname":             "localhost",
    "pid":                  12345,
    "advertising_interval": 5.5,
    "tcp_server_port":      10000,
    "start_time":           "foobar",
    "timeout":              3.5,
    "ghost_mode":           true
  })";

  TestBranchInfo info(uuid, json);

  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    EXPECT_NE(timestamp, nullptr);
    EXPECT_STREQ(str, "foobar");
    EXPECT_EQ(timefmt, nullptr);
    *timestamp = 1234356789123000000ll;
    return YOGI_OK;
  });

  EXPECT_EQ(info.uuid(), uuid);
  EXPECT_EQ(info.to_string(), json);
  EXPECT_EQ(info.to_json(), yogi::Json::parse(json));
  EXPECT_EQ(info.name(), "foo");
  EXPECT_EQ(info.description(), "bar");
  EXPECT_EQ(info.network_name(), "local");
  EXPECT_EQ(info.path(), "/test");
  EXPECT_EQ(info.hostname(), "localhost");
  EXPECT_EQ(info.pid(), 12345);
  EXPECT_FLOAT_EQ(info.advertising_interval().total_seconds(), 5.5);
  EXPECT_EQ(info.tcp_server_port(), 10000);
  EXPECT_EQ(info.start_time().duration_since_epoch(), 1234356789123000000ns);
  EXPECT_FLOAT_EQ(info.timeout().total_seconds(), 3.5);
  EXPECT_EQ(info.ghost_mode(), true);
}

TEST_F(BranchTest, RemoteBranchInfo) {
  yogi::Uuid uuid = {};
  uuid.data()[0]  = 123;

  TestRemoteBranchInfo info(uuid, R"({
    "tcp_server_address": "1.2.3.4"
  })");

  EXPECT_EQ(info.uuid(), uuid);
  EXPECT_EQ(info.tcp_server_address(), "1.2.3.4");
}

TEST_F(BranchTest, LocalBranchInfo) {
  yogi::Uuid uuid = {};
  uuid.data()[0]  = 123;

  TestLocalBranchInfo info(uuid, R"({
    "advertising_address": "1.2.3.4",
    "advertising_port":    5555,
    "tx_queue_size":       6666,
    "rx_queue_size":       7777
  })");

  EXPECT_EQ(info.uuid(), uuid);
  EXPECT_EQ(info.advertising_address(), "1.2.3.4");
  EXPECT_EQ(info.advertising_port(), 5555);
  EXPECT_EQ(info.tx_queue_size(), 6666);
  EXPECT_EQ(info.rx_queue_size(), 7777);
}

TEST_F(BranchTest, BranchEventInfo) {
  yogi::Uuid uuid = {};
  uuid.data()[0]  = 123;

  auto json = R"({
  })";

  TestBranchEventInfo info(uuid, json);

  EXPECT_EQ(info.uuid(), uuid);
  EXPECT_EQ(info.to_string(), json);
  EXPECT_EQ(info.to_json(), yogi::Json::parse(json));
}

TEST_F(BranchTest, BranchDiscoveredEventInfo) {
  yogi::Uuid uuid = {};
  uuid.data()[0]  = 123;

  TestBranchDiscoveredEventInfo info(uuid, R"({
    "tcp_server_address": "1.2.3.4",
    "tcp_server_port":    10000
  })");

  EXPECT_EQ(info.uuid(), uuid);
  EXPECT_EQ(info.tcp_server_address(), "1.2.3.4");
  EXPECT_EQ(info.tcp_server_port(), 10000);
}

TEST_F(BranchTest, BranchQueriedEventInfo) {
  yogi::Uuid uuid = {};
  uuid.data()[0]  = 123;

  TestBranchQueriedEventInfo info(uuid, R"({
    "name":                 "foo",
    "description":          "bar",
    "network_name":         "local",
    "path":                 "/test",
    "hostname":             "localhost",
    "pid":                  12345,
    "advertising_interval": 5.5,
    "tcp_server_address":   "1.2.3.4",
    "tcp_server_port":      10000,
    "start_time":           "foobar",
    "timeout":              3.5,
    "ghost_mode":           true
  })");

  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    EXPECT_NE(timestamp, nullptr);
    EXPECT_STREQ(str, "foobar");
    EXPECT_EQ(timefmt, nullptr);
    *timestamp = 1234356789123000000ll;
    return YOGI_OK;
  });

  EXPECT_EQ(info.uuid(), uuid);
  EXPECT_EQ(info.name(), "foo");
  EXPECT_EQ(info.description(), "bar");
  EXPECT_EQ(info.network_name(), "local");
  EXPECT_EQ(info.path(), "/test");
  EXPECT_EQ(info.hostname(), "localhost");
  EXPECT_EQ(info.pid(), 12345);
  EXPECT_FLOAT_EQ(info.advertising_interval().total_seconds(), 5.5);
  EXPECT_EQ(info.tcp_server_address(), "1.2.3.4");
  EXPECT_EQ(info.tcp_server_port(), 10000);
  EXPECT_EQ(info.start_time().duration_since_epoch(), 1234356789123000000ns);
  EXPECT_FLOAT_EQ(info.timeout().total_seconds(), 3.5);
  EXPECT_EQ(info.ghost_mode(), true);
}

TEST_F(BranchTest, ConnectFinishedEventInfo) {
  yogi::Uuid uuid = {};
  uuid.data()[0]  = 123;

  TestConnectFinishedEventInfo info(uuid, R"({
  })");

  EXPECT_EQ(info.uuid(), uuid);
}

TEST_F(BranchTest, ConnectionLostEventInfo) {
  yogi::Uuid uuid = {};
  uuid.data()[0]  = 123;

  TestConnectionLostEventInfo info(uuid, R"({
  })");

  EXPECT_EQ(info.uuid(), uuid);
}

/*
TEST_F(BranchTest, CreateWithSubSection) {
  auto branch = yogi::Branch::create(context_, "{\"branch\":{\"name\":\"Cow\"}}", "/branch");
  EXPECT_EQ(branch->name(), "Cow");
}

TEST_F(BranchTest, Info) {
  auto branch = yogi::Branch::create(context_, R"raw({
    "name": "My Branch",
    "description": "Stuff",
    "network_name": "My Network",
    "network_password": "Password",
    "path": "/some/path",
    "advertising_address": "239.255.0.1",
    "advertising_port": 12345,
    "advertising_interval": 7,
    "timeout": -1
    })raw");

  auto info = branch->info();
  EXPECT_EQ(info.to_json(), yogi::Json::parse(info.to_string()));

  EXPECT_NE(info.uuid(), yogi::Uuid{});
  EXPECT_EQ(info.name(), "My Branch");
  EXPECT_EQ(info.description(), "Stuff");
  EXPECT_EQ(info.network_name(), "My Network");
  EXPECT_EQ(info.path(), "/some/path");
  EXPECT_FALSE(info.hostname().empty());
  EXPECT_GT(info.pid(), 0);
  EXPECT_EQ(info.advertising_address(), "239.255.0.1");
  EXPECT_EQ(info.advertising_port(), 12345);
  EXPECT_EQ(info.advertising_interval(), yogi::Duration::from_seconds(7));
  EXPECT_GT(info.tcp_server_port(), 0);
  EXPECT_LT(info.start_time(), yogi::Timestamp::now());
  EXPECT_EQ(info.timeout(), yogi::Duration::kInf);
  EXPECT_FALSE(info.ghost_mode());
  EXPECT_GT(info.tx_queue_size(), 1000);
  EXPECT_GT(info.rx_queue_size(), 1000);

  EXPECT_EQ(branch->uuid(), info.uuid());
  EXPECT_EQ(branch->name(), info.name());
  EXPECT_EQ(branch->description(), info.description());
  EXPECT_EQ(branch->network_name(), info.network_name());
  EXPECT_EQ(branch->path(), info.path());
  EXPECT_EQ(branch->hostname(), info.hostname());
  EXPECT_EQ(branch->pid(), info.pid());
  EXPECT_EQ(branch->advertising_address(), info.advertising_address());
  EXPECT_EQ(branch->advertising_port(), info.advertising_port());
  EXPECT_EQ(branch->advertising_interval(), info.advertising_interval());
  EXPECT_EQ(branch->tcp_server_port(), info.tcp_server_port());
  EXPECT_EQ(branch->start_time(), info.start_time());
  EXPECT_EQ(branch->timeout(), info.timeout());
  EXPECT_EQ(branch->ghost_mode(), info.ghost_mode());
  EXPECT_EQ(branch->tx_queue_size(), info.tx_queue_size());
  EXPECT_EQ(branch->rx_queue_size(), info.rx_queue_size());
}

TEST_F(BranchTest, GetConnectedBranches) {
  auto branch   = yogi::Branch::create(context_, "{\"name\":\"My Branch\"}");
  auto branch_a = yogi::Branch::create(context_, "{\"name\":\"A\"}");
  auto branch_b = yogi::Branch::create(context_, "{\"name\":\"B\"}");

  while (!branch->connected_branches().count(branch_a->uuid()) ||
         !branch->connected_branches().count(branch_b->uuid())) {
    context_->run_one();
  }

  auto branches = branch->connected_branches();

  ASSERT_EQ(branches.count(branch_a->uuid()), 1u);
  EXPECT_EQ(branches.at(branch_a->uuid()).name(), branch_a->name());
  EXPECT_FALSE(branches.at(branch_a->uuid()).tcp_server_address().empty());

  ASSERT_EQ(branches.count(branch_b->uuid()), 1u);
  EXPECT_EQ(branches.at(branch_b->uuid()).name(), branch_b->name());
  EXPECT_FALSE(branches.at(branch_b->uuid()).tcp_server_address().empty());
}

TEST_F(BranchTest, AwaitEventAsync) {
  auto branch   = yogi::Branch::create(context_, "{\"name\":\"My Branch\"}");
  auto branch_a = yogi::Branch::create(context_, "{\"name\":\"A\"}");

  auto events = yogi::BranchEvents::kBranchQueried | yogi::BranchEvents::kConnectionLost;
  bool called = false;
  branch->await_event_async(events, [&](auto& res, auto event, auto& evres, auto& info) {
    EXPECT_NE(dynamic_cast<const yogi::Success*>(&res), nullptr);
    EXPECT_EQ(res.error_code(), yogi::ErrorCode::kOk);
    EXPECT_EQ(event, yogi::BranchEvents::kBranchQueried);
    EXPECT_NE(dynamic_cast<const yogi::Success*>(&evres), nullptr);
    EXPECT_EQ(evres.error_code(), yogi::ErrorCode::kOk);
    EXPECT_NE(dynamic_cast<const yogi::BranchQueriedEventInfo*>(&info), nullptr);
    EXPECT_EQ(static_cast<const yogi::BranchQueriedEventInfo&>(info).start_time(), branch_a->start_time());
    EXPECT_EQ(static_cast<const yogi::BranchQueriedEventInfo&>(info).timeout(), branch_a->timeout());
    EXPECT_EQ(info.to_json(), yogi::Json::parse(info.to_string()));
    called = true;
  });

  while (!called) context_->run_one();

  EXPECT_TRUE(called);
}

TEST_F(BranchTest, CancelAwaitEvent) {
  auto branch = yogi::Branch::create(context_, "{\"name\":\"My Branch\"}");

  EXPECT_FALSE(branch->cancel_await_event());

  bool called = false;
  branch->await_event_async(yogi::BranchEvents::kAll, [&](auto& res, auto event, auto& evres, auto&) {
    EXPECT_NE(dynamic_cast<const yogi::Failure*>(&res), nullptr);
    EXPECT_EQ(res.error_code(), yogi::ErrorCode::kCanceled);
    EXPECT_EQ(event, yogi::BranchEvents::kNone);
    EXPECT_NE(dynamic_cast<const yogi::Success*>(&evres), nullptr);
    EXPECT_EQ(evres.error_code(), yogi::ErrorCode::kOk);
    called = true;
  });

  EXPECT_TRUE(branch->cancel_await_event());
  context_->poll();
  EXPECT_TRUE(called);
}

TEST_F(BranchTest, SendBroadcast) {
  auto branch_a = yogi::Branch::create(context_, "{\"name\":\"a\", \"_transceive_byte_limit\": 5}");
  auto branch_b = yogi::Branch::create(context_, "{\"name\":\"b\"}");
  run_context_until_branches_are_connected(context_, {branch_a, branch_b});
  context_->run_in_background();

  // Receive a broadcast to verify that it has actually been sent
  bool broadcast_received = false;
  branch_b->receive_broadcast_async(yogi::Encoding::kJson, [&](auto& res, auto&, auto& payload) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, big_json_view_);
    broadcast_received = true;
  });

  // Blocking
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(branch_a->send_broadcast(big_json_view_, true));
  }

  // Non-blocking
  while (branch_a->send_broadcast(big_json_view_, false))
    ;

  context_->stop();
  context_->wait_for_stopped();

  // Verify that a broadcast has actually been sent
  while (!broadcast_received) context_->run_one();
}

TEST_F(BranchTest, SendBroadcastAsync) {
  auto branch_a = yogi::Branch::create(context_, "{\"name\":\"a\", \"_transceive_byte_limit\": 5}");
  auto branch_b = yogi::Branch::create(context_, "{\"name\":\"b\"}");
  run_context_until_branches_are_connected(context_, {branch_a, branch_b});

  // Receive a broadcast to verify that it has actually been sent
  bool broadcast_received = false;
  branch_b->receive_broadcast_async(yogi::Encoding::kJson, [&](auto& res, auto&, auto& payload) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, big_json_view_);
    broadcast_received = true;
  });

  // Send with retry = true
  const int n = 3;
  std::vector<yogi::Result> results;
  for (int i = 0; i < n; ++i) {
    auto oid = branch_a->send_broadcast_async(big_json_view_, true, [&](auto& res, auto oid) {
      EXPECT_EQ(res, yogi::Success());
      EXPECT_TRUE(oid.is_valid());
      results.push_back(res);
    });
    EXPECT_TRUE(oid.is_valid());
  }

  while (results.size() != n) context_->poll();

  // Send with retry = false
  do {
    branch_a->send_broadcast_async(big_json_view_, false, [&](auto& res, auto) { results.push_back(res); });

    context_->poll_one();
  } while (results.back() == yogi::Success());

  EXPECT_EQ(results.back(), yogi::Failure(yogi::ErrorCode::kTxQueueFull));

  // Verify that a broadcast has actually been sent
  while (!broadcast_received) context_->run_one();
}

TEST_F(BranchTest, CancelSendBroadcast) {
  auto branch_a = yogi::Branch::create(context_, "{\"name\":\"a\", \"_transceive_byte_limit\": 5}");
  auto branch_b = yogi::Branch::create(context_, "{\"name\":\"b\"}");
  run_context_until_branches_are_connected(context_, {branch_a, branch_b});

  std::map<yogi::OperationId, yogi::ErrorCode> oid_to_ec;
  while (true) {
    auto oid =
        branch_a->send_broadcast_async(big_json_view_, [&](auto& res, auto oid) { oid_to_ec[oid] = res.error_code(); });
    EXPECT_TRUE(oid.is_valid());

    oid_to_ec[oid] = yogi::ErrorCode::kUnknown;

    if (branch_a->cancel_send_broadcast(oid)) {
      context_->poll();
      EXPECT_EQ(oid_to_ec[oid], yogi::ErrorCode::kCanceled);
      break;
    }
  }
}

TEST_F(BranchTest, ReceiveBroadcast) {
  auto branch_a = yogi::Branch::create(context_, "{\"name\":\"a\"}");
  auto branch_b = yogi::Branch::create(context_, "{\"name\":\"b\"}");
  run_context_until_branches_are_connected(context_, {branch_a, branch_b});

  // Simplest form
  auto uuid_b = branch_b->uuid();
  bool called = false;
  branch_a->receive_broadcast_async([&](auto& res, auto& source, auto& payload) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(source, uuid_b);
    EXPECT_EQ(payload, msgpack_view_);
    called = true;
  });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();

  // With buffer in handler function
  called = false;
  branch_a->receive_broadcast_async([&](auto& res, auto&, auto& payload, auto&& buffer) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, msgpack_view_);
    EXPECT_GE(buffer->size(), static_cast<std::size_t>(yogi::constants::kMaxMessagePayloadSize));
    called = true;
  });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();

  // With encoding
  called = false;
  branch_a->receive_broadcast_async(yogi::Encoding::kJson, [&](auto& res, auto&, auto& payload) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, json_view_);
    called = true;
  });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();

  // With encoding and with buffer in handler function
  called = false;
  branch_a->receive_broadcast_async(yogi::Encoding::kJson, [&](auto& res, auto&, auto& payload, auto&& buffer) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, json_view_);
    EXPECT_GE(buffer->size(), static_cast<std::size_t>(yogi::constants::kMaxMessagePayloadSize));
    called = true;
  });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();

  // With custom buffer
  auto buffer = std::make_unique<yogi::Buffer>(123);
  called      = false;
  branch_a->receive_broadcast_async(std::move(buffer), [&](auto& res, auto&, auto& payload) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, msgpack_view_);
    called = true;
  });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();

  // With custom buffer and with buffer in handler function
  buffer = std::make_unique<yogi::Buffer>(123);
  called = false;
  branch_a->receive_broadcast_async(std::move(buffer), [&](auto& res, auto&, auto& payload, auto&& buffer) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, msgpack_view_);
    EXPECT_EQ(buffer->size(), 123u);
    called = true;
  });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();

  // With custom buffer and encoding
  buffer = std::make_unique<yogi::Buffer>(123);
  called = false;
  branch_a->receive_broadcast_async(yogi::Encoding::kJson, std::move(buffer), [&](auto& res, auto&, auto& payload) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, json_view_);
    called = true;
  });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();

  // With custom buffer and encoding and buffer in handler function
  buffer = std::make_unique<yogi::Buffer>(123);
  called = false;
  branch_a->receive_broadcast_async(yogi::Encoding::kJson, std::move(buffer),
                                    [&](auto& res, auto&, auto& payload, auto&& buffer) {
                                      EXPECT_EQ(res, yogi::Success());
                                      EXPECT_EQ(payload, json_view_);
                                      EXPECT_GE(buffer->size(), 123u);
                                      called = true;
                                    });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();
}

TEST_F(BranchTest, CancelReceiveBroadcast) {
  auto branch_a = yogi::Branch::create(context_, "{\"name\":\"a\"}");

  EXPECT_FALSE(branch_a->cancel_receive_broadcast());

  bool called = false;
  branch_a->receive_broadcast_async([&](auto& res, auto&, auto&, auto&&) {
    EXPECT_EQ(res, yogi::Failure(yogi::ErrorCode::kCanceled));
    called = true;
  });

  EXPECT_TRUE(branch_a->cancel_receive_broadcast());
  context_->poll();
  EXPECT_TRUE(called);
}
*/
