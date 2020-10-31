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

TEST_F(BranchTest, CreateFromConfiguration) {
  auto context = create_context();
  auto config  = create_configuration();

  MOCK_BranchCreate([](void** branch, void* context, void* config, const char* section) {
    EXPECT_NE(branch, nullptr);
    EXPECT_EQ(context, kPointer);
    EXPECT_EQ(config, kPointer);
    EXPECT_STREQ(section, "bar");
    *branch = kPointer;
    return YOGI_OK;
  });

  MOCK_BranchGetInfo([](void* branch, void* uuid, char* json, int jsonsize) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_NE(uuid, nullptr);
    EXPECT_NE(json, nullptr);
    EXPECT_GT(jsonsize, 100);
    strcpy(json, "{}");
    return YOGI_OK;
  });

  auto branch = yogi::Branch::create(context, config, "bar");
}

TEST_F(BranchTest, CreateFromJson) {
  auto context = create_context();

  MOCK_ConfigurationCreate([](void** config, int) {
    *config = kPointer;
    return YOGI_OK;
  });

  MOCK_ConfigurationUpdateFromJson([](void* config, const char* json) {
    EXPECT_EQ(config, kPointer);
    EXPECT_STREQ(json, "foo");
    return YOGI_OK;
  });

  MOCK_BranchCreate([](void** branch, void* context, void* config, const char* section) {
    EXPECT_NE(branch, nullptr);
    EXPECT_EQ(context, kPointer);
    EXPECT_EQ(config, kPointer);
    EXPECT_STREQ(section, "bar");
    *branch = kPointer;
    return YOGI_OK;
  });

  MOCK_BranchGetInfo([](void* branch, void* uuid, char* json, int jsonsize) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_NE(uuid, nullptr);
    EXPECT_NE(json, nullptr);
    EXPECT_GT(jsonsize, 100);
    strcpy(json, "{}");
    return YOGI_OK;
  });

  auto branch = yogi::Branch::create(context, "foo", "bar");
}

TEST_F(BranchTest, Info) {
  auto context = create_context();
  auto config  = create_configuration();

  MOCK_BranchCreate([](void** branch, void* context, void* config, const char* section) {
    EXPECT_NE(branch, nullptr);
    EXPECT_EQ(context, kPointer);
    EXPECT_EQ(config, kPointer);
    EXPECT_STREQ(section, "bar");
    *branch = kPointer;
    return YOGI_OK;
  });

  MOCK_BranchGetInfo([](void* branch, void* uuid, char* json, int jsonsize) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_NE(uuid, nullptr);
    EXPECT_NE(json, nullptr);

    auto json_str = R"({
      "name":                 "foo",
      "description":          "bar",
      "network_name":         "local",
      "path":                 "/test",
      "hostname":             "localhost",
      "pid":                  12345,
      "advertising_interval": 5.5,
      "advertising_address":  "1.2.3.4",
      "advertising_port":     5555,
      "tcp_server_address":   "1.2.3.4",
      "tcp_server_port":      10000,
      "tx_queue_size":        6666,
      "rx_queue_size":        7777,
      "start_time":           "foobar",
      "timeout":              3.5,
      "ghost_mode":           true
    })";

    if (jsonsize < static_cast<int>(strlen(json_str))) return YOGI_ERR_BUFFER_TOO_SMALL;
    strcpy(json, json_str);

    return YOGI_OK;
  });

  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    EXPECT_NE(timestamp, nullptr);
    EXPECT_STREQ(str, "foobar");
    EXPECT_EQ(timefmt, nullptr);
    *timestamp = 1234356789123000000ll;
    return YOGI_OK;
  });

  auto branch = yogi::Branch::create(context, config, "bar");
  EXPECT_EQ(branch->info().name(), "foo");

  EXPECT_NE(branch->uuid(), yogi::Uuid{});
  EXPECT_EQ(branch->name(), "foo");
  EXPECT_EQ(branch->description(), "bar");
  EXPECT_EQ(branch->network_name(), "local");
  EXPECT_EQ(branch->path(), "/test");
  EXPECT_EQ(branch->hostname(), "localhost");
  EXPECT_EQ(branch->pid(), 12345);
  EXPECT_FLOAT_EQ(branch->advertising_interval().total_seconds(), 5.5);
  EXPECT_EQ(branch->advertising_address(), "1.2.3.4");
  EXPECT_EQ(branch->advertising_port(), 5555);
  EXPECT_EQ(branch->tcp_server_port(), 10000);
  EXPECT_EQ(branch->tx_queue_size(), 6666);
  EXPECT_EQ(branch->rx_queue_size(), 7777);
  EXPECT_EQ(branch->start_time().duration_since_epoch(), 1234356789123000000ns);
  EXPECT_FLOAT_EQ(branch->timeout().total_seconds(), 3.5);
  EXPECT_EQ(branch->ghost_mode(), true);
}

TEST_F(BranchTest, GetConnectedBranches) {
  MOCK_BranchGetConnectedBranches(
      [](void* branch, void* uuid, char* json, int jsonsize, void (*fn)(int res, void* userarg), void* userarg) {
        EXPECT_EQ(branch, kPointer);
        EXPECT_NE(uuid, nullptr);
        EXPECT_NE(json, nullptr);
        EXPECT_GT(jsonsize, 100);
        EXPECT_NE(fn, nullptr);

        auto uuid_data = static_cast<char*>(uuid);
        memset(uuid_data, 0, 16);

        uuid_data[0] = 33;
        strcpy(json, R"({"name": "foo"})");
        fn(YOGI_OK, userarg);

        uuid_data[0] = 66;
        strcpy(json, R"({"name": "bar"})");
        fn(YOGI_OK, userarg);

        return YOGI_OK;
      });

  auto branch = create_branch();
  auto infos  = branch->get_connected_branches();

  ASSERT_EQ(infos.size(), 2);
  yogi::Uuid uuid;
  memset(uuid.data(), 0, 16);

  uuid.data()[0] = 33;
  ASSERT_EQ(infos.count(uuid), 1);
  EXPECT_EQ(infos.at(uuid).name(), "foo");

  uuid.data()[0] = 66;
  ASSERT_EQ(infos.count(uuid), 1);
  EXPECT_EQ(infos.at(uuid).name(), "bar");
}

TEST_F(BranchTest, AwaitEventAsync) {
  static void (*callback_fn)(int res, int ev, int evres, void* userarg);
  static void* callback_userarg;

  MOCK_BranchAwaitEventAsync([](void* branch, int events, void* uuid, char* json, int jsonsize,
                                void (*fn)(int res, int ev, int evres, void* userarg), void* userarg) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_EQ(events, YOGI_BEV_BRANCH_QUERIED);
    EXPECT_NE(nullptr, uuid);
    EXPECT_NE(json, nullptr);
    EXPECT_GT(jsonsize, 100);
    EXPECT_NE(fn, nullptr);

    memset(uuid, 123, 16);
    strcpy(json, R"({"name": "foo"})");

    callback_fn      = fn;
    callback_userarg = userarg;

    return YOGI_OK;
  });

  auto branch = create_branch();

  int calls = 0;
  branch->await_event_async(yogi::BranchEvents::kBranchQueried, [&](auto& res, auto event, auto& ev_res, auto& info) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(event, yogi::BranchEvents::kConnectFinished);
    EXPECT_EQ(ev_res.error_code(), yogi::ErrorCode::kBusy);
    EXPECT_NE(dynamic_cast<const yogi::ConnectFinishedEventInfo*>(&info), nullptr);
    calls++;
  });

  EXPECT_EQ(calls, 0);
  callback_fn(YOGI_OK, YOGI_BEV_CONNECT_FINISHED, YOGI_ERR_BUSY, callback_userarg);
  EXPECT_EQ(calls, 1);
}

/*

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
