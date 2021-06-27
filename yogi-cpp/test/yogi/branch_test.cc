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
  auto json = R"({
    "uuid":                 "123e4567-e89b-12d3-a456-426655440000",
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

  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    EXPECT_NE(timestamp, nullptr);
    EXPECT_STREQ(str, "foobar");
    EXPECT_EQ(timefmt, nullptr);
    *timestamp = 1234356789123000000ll;
    return YOGI_OK;
  });

  TestBranchInfo info{json};

  EXPECT_EQ(info.uuid().to_string(), "123e4567-e89b-12d3-a456-426655440000");
  EXPECT_EQ(info.to_string(), json);
  EXPECT_EQ(info.to_json(), yogi::Json::parse(json));
  EXPECT_EQ(info.name(), "foo");
  EXPECT_EQ(info.description(), "bar");
  EXPECT_EQ(info.network_name(), "local");
  EXPECT_EQ(info.path(), "/test");
  EXPECT_EQ(info.hostname(), "localhost");
  EXPECT_EQ(info.pid(), 12345);
  EXPECT_NEAR(info.advertising_interval().total_seconds(), 5.5, 1e-6);
  EXPECT_EQ(info.tcp_server_port(), 10000);
  EXPECT_EQ(info.start_time().duration_since_epoch(), 1234356789123000000ns);
  EXPECT_NEAR(info.timeout().total_seconds(), 3.5, 1e-6);
  EXPECT_EQ(info.ghost_mode(), true);
}

TEST_F(BranchTest, RemoteBranchInfo) {
  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    *timestamp = 0;
    return YOGI_OK;
  });

  TestRemoteBranchInfo info(R"({
    "uuid":                 "123e4567-e89b-12d3-a456-426655440000",
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
    "ghost_mode":           true,

    "tcp_server_address":   "1.2.3.4"
  })");

  EXPECT_EQ(info.name(), "foo");  // Check that BranchInfo::init() gets called
  EXPECT_EQ(info.tcp_server_address(), "1.2.3.4");
}

TEST_F(BranchTest, LocalBranchInfo) {
  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    *timestamp = 0;
    return YOGI_OK;
  });

  TestLocalBranchInfo info(R"({
    "uuid":                 "123e4567-e89b-12d3-a456-426655440000",
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
    "ghost_mode":           true,

    "advertising_address":  "1.2.3.4",
    "advertising_port":     5555,
    "tx_queue_size":        6666,
    "rx_queue_size":        7777
  })");

  EXPECT_EQ(info.name(), "foo");  // Check that BranchInfo::init() gets called
  EXPECT_EQ(info.advertising_address(), "1.2.3.4");
  EXPECT_EQ(info.advertising_port(), 5555);
  EXPECT_EQ(info.tx_queue_size(), 6666);
  EXPECT_EQ(info.rx_queue_size(), 7777);
}

TEST_F(BranchTest, BranchEventInfo) {
  auto json = R"({
    "uuid": "123e4567-e89b-12d3-a456-426655440000"
  })";

  TestBranchEventInfo info(json);

  EXPECT_EQ(info.uuid().to_string(), "123e4567-e89b-12d3-a456-426655440000");
  EXPECT_EQ(info.to_string(), json);
  EXPECT_EQ(info.to_json(), yogi::Json::parse(json));
}

TEST_F(BranchTest, BranchDiscoveredEventInfo) {
  TestBranchDiscoveredEventInfo info(R"({
    "uuid": "123e4567-e89b-12d3-a456-426655440000",

    "tcp_server_address": "1.2.3.4",
    "tcp_server_port":    10000
  })");

  EXPECT_EQ(info.uuid().to_string(), "123e4567-e89b-12d3-a456-426655440000");  // Check BranchEventInfo.init() is called
  EXPECT_EQ(info.tcp_server_address(), "1.2.3.4");
  EXPECT_EQ(info.tcp_server_port(), 10000);
}

TEST_F(BranchTest, BranchQueriedEventInfo) {
  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    EXPECT_NE(timestamp, nullptr);
    EXPECT_STREQ(str, "foobar");
    EXPECT_EQ(timefmt, nullptr);
    *timestamp = 1234356789123000000ll;
    return YOGI_OK;
  });

  TestBranchQueriedEventInfo info(R"({
    "uuid": "123e4567-e89b-12d3-a456-426655440000",

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

  EXPECT_EQ(info.uuid().to_string(), "123e4567-e89b-12d3-a456-426655440000");  // Check BranchEventInfo.init() is called
  EXPECT_EQ(info.name(), "foo");
  EXPECT_EQ(info.description(), "bar");
  EXPECT_EQ(info.network_name(), "local");
  EXPECT_EQ(info.path(), "/test");
  EXPECT_EQ(info.hostname(), "localhost");
  EXPECT_EQ(info.pid(), 12345);
  EXPECT_NEAR(info.advertising_interval().total_seconds(), 5.5, 1e-6);
  EXPECT_EQ(info.tcp_server_address(), "1.2.3.4");
  EXPECT_EQ(info.tcp_server_port(), 10000);
  EXPECT_EQ(info.start_time().duration_since_epoch(), 1234356789123000000ns);
  EXPECT_NEAR(info.timeout().total_seconds(), 3.5, 1e-6);
  EXPECT_EQ(info.ghost_mode(), true);
}

TEST_F(BranchTest, ConnectFinishedEventInfo) {
  TestConnectFinishedEventInfo info(R"({
    "uuid": "123e4567-e89b-12d3-a456-426655440000"
  })");

  EXPECT_EQ(info.uuid().to_string(), "123e4567-e89b-12d3-a456-426655440000");  // Check BranchEventInfo.init() is called
}

TEST_F(BranchTest, ConnectionLostEventInfo) {
  TestConnectionLostEventInfo info(R"({
    "uuid": "123e4567-e89b-12d3-a456-426655440000"
  })");

  EXPECT_EQ(info.uuid().to_string(), "123e4567-e89b-12d3-a456-426655440000");  // Check BranchEventInfo.init() is called
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

  MOCK_BranchGetInfo([](void* branch, const void** uuid, const char** json, int* jsonsize) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_NE(json, nullptr);
    EXPECT_NE(jsonsize, nullptr);
    *json     = kBranchInfo;
    *jsonsize = strlen(kBranchInfo) + 1;
    EXPECT_EQ(uuid, nullptr);  // Not needed for now
    return YOGI_OK;
  });

  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    EXPECT_NE(timestamp, nullptr);
    *timestamp = 1234356789123000000ll;
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

  MOCK_BranchGetInfo([](void* branch, const void** uuid, const char** json, int* jsonsize) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_NE(json, nullptr);
    EXPECT_NE(jsonsize, nullptr);
    *json     = kBranchInfo;
    *jsonsize = strlen(kBranchInfo) + 1;
    EXPECT_EQ(uuid, nullptr);  // Not needed for now
    return YOGI_OK;
  });

  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    EXPECT_NE(timestamp, nullptr);
    *timestamp = 1234356789123000000ll;
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

  MOCK_BranchGetInfo([](void* branch, const void** uuid, const char** json, int* jsonsize) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_NE(json, nullptr);
    EXPECT_NE(jsonsize, nullptr);

    *json = R"({
      "uuid":                 "123e4567-e89b-12d3-a456-426655440000",
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

    *jsonsize = std::strlen(*json) + 1;

    EXPECT_EQ(uuid, nullptr);  // Not needed for now

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

  EXPECT_EQ(branch->uuid().to_string(), "123e4567-e89b-12d3-a456-426655440000");
  EXPECT_EQ(branch->name(), "foo");
  EXPECT_EQ(branch->description(), "bar");
  EXPECT_EQ(branch->network_name(), "local");
  EXPECT_EQ(branch->path(), "/test");
  EXPECT_EQ(branch->hostname(), "localhost");
  EXPECT_EQ(branch->pid(), 12345);
  EXPECT_NEAR(branch->advertising_interval().total_seconds(), 5.5, 1e-6);
  EXPECT_EQ(branch->advertising_address(), "1.2.3.4");
  EXPECT_EQ(branch->advertising_port(), 5555);
  EXPECT_EQ(branch->tcp_server_port(), 10000);
  EXPECT_EQ(branch->tx_queue_size(), 6666);
  EXPECT_EQ(branch->rx_queue_size(), 7777);
  EXPECT_EQ(branch->start_time().duration_since_epoch(), 1234356789123000000ns);
  EXPECT_NEAR(branch->timeout().total_seconds(), 3.5, 1e-6);
  EXPECT_EQ(branch->ghost_mode(), true);
}

TEST_F(BranchTest, GetConnectedBranches) {
  MOCK_BranchGetConnectedBranches(
      [](void* branch, const void** uuids, int* numuuids, const char** json, int* jsonsize) {
        EXPECT_EQ(branch, kPointer);
        EXPECT_NE(json, nullptr);

        // For now, yogi-cpp does not use the following parameters, so we set them to NULL
        EXPECT_EQ(uuids, nullptr);
        EXPECT_EQ(numuuids, nullptr);
        EXPECT_EQ(jsonsize, nullptr);

        *json = R"([{
          "uuid":                 "123e4567-e89b-12d3-a456-555555555555",
          "name":                 "first",
          "description":          "bar",
          "network_name":         "local",
          "path":                 "/test",
          "hostname":             "localhost",
          "pid":                  12345,
          "advertising_interval": 5.5,
          "tcp_server_port":      10000,
          "start_time":           "foobar",
          "timeout":              3.5,
          "ghost_mode":           true,
          "tcp_server_address":   "1.2.3.4"
        }, {
          "uuid":                 "123e4567-e89b-12d3-a456-888888888888",
          "name":                 "second",
          "description":          "bar",
          "network_name":         "local",
          "path":                 "/test",
          "hostname":             "localhost",
          "pid":                  12345,
          "advertising_interval": 5.5,
          "tcp_server_port":      10000,
          "start_time":           "foobar",
          "timeout":              3.5,
          "ghost_mode":           true,
          "tcp_server_address":   "1.2.3.4"
        }])";

        return YOGI_OK;
      });

  auto branch = create_branch();
  auto infos  = branch->get_connected_branches();

  ASSERT_EQ(infos.size(), 2);
  EXPECT_EQ(infos.at(yogi::Uuid::parse("123e4567-e89b-12d3-a456-555555555555")).name(), "first");
  EXPECT_EQ(infos.at(yogi::Uuid::parse("123e4567-e89b-12d3-a456-888888888888")).name(), "second");
}

TEST_F(BranchTest, AwaitEventAsync) {
  static void (*callback_fn)(int res, int ev, int evres, const void* uuid, const char* json, int jsonsize,
                             void* userarg);
  static void* callback_userarg;

  MOCK_BranchAwaitEventAsync(
      [](void* branch, int events,
         void (*fn)(int res, int ev, int evres, const void* uuid, const char* json, int jsonsize, void* userarg),
         void* userarg) {
        EXPECT_EQ(branch, kPointer);
        EXPECT_EQ(events, YOGI_BEV_BRANCH_QUERIED);
        EXPECT_NE(fn, nullptr);

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

  const char uuid[16] = {123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123};
  const char* json    = R"({
      "uuid":                 "123e4567-e89b-12d3-a456-555555555555",
      "name":                 "first",
      "description":          "bar",
      "network_name":         "local",
      "path":                 "/test",
      "hostname":             "localhost",
      "pid":                  12345,
      "advertising_interval": 5.5,
      "tcp_server_port":      10000,
      "start_time":           "foobar",
      "timeout":              3.5,
      "ghost_mode":           true,
      "tcp_server_address":   "1.2.3.4"
    })";
  int jsonsize        = static_cast<int>(strlen(json));

  EXPECT_EQ(calls, 0);
  callback_fn(YOGI_OK, YOGI_BEV_CONNECT_FINISHED, YOGI_ERR_BUSY, uuid, json, jsonsize, callback_userarg);
  EXPECT_EQ(calls, 1);
}

TEST_F(BranchTest, CancelAwaitEvent) {
  auto branch = create_branch();

  // Successful cancellation
  MOCK_BranchCancelAwaitEvent([](void* branch) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_OK;
  });

  EXPECT_TRUE(branch->cancel_await_event());

  // No operation running
  MOCK_BranchCancelAwaitEvent([](void* branch) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_ERR_OPERATION_NOT_RUNNING;
  });

  EXPECT_FALSE(branch->cancel_await_event());

  // Error
  MOCK_BranchCancelAwaitEvent([](void* branch) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_ERR_TIMEOUT;
  });

  EXPECT_THROW(branch->cancel_await_event(), yogi::FailureException);
}

TEST_F(BranchTest, SendBroadcast) {
  auto branch = create_branch();

  // Send JSON-encoded data in blocking mode
  MOCK_BranchSendBroadcast([](void* branch, int enc, const void* data, int datasize, int block) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_EQ(enc, YOGI_ENC_JSON);
    EXPECT_NE(data, nullptr);
    EXPECT_EQ(datasize, 6);
    EXPECT_TRUE(block);
    return YOGI_OK;
  });

  const char* s = "hello";
  EXPECT_TRUE(branch->send_broadcast(yogi::JsonView(s), true));

  // Send Msgpack-encoded data in non-blocking mode
  MOCK_BranchSendBroadcast([](void* branch, int enc, const void* data, int datasize, int block) {
    EXPECT_EQ(enc, YOGI_ENC_MSGPACK);
    EXPECT_FALSE(block);
    return YOGI_ERR_TX_QUEUE_FULL;
  });

  EXPECT_FALSE(branch->send_broadcast(yogi::MsgpackView(s), false));

  // Error
  MOCK_BranchSendBroadcast([](void* branch, int enc, const void* data, int datasize, int block) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(branch->send_broadcast(yogi::MsgpackView(s)), yogi::FailureException);
}

TEST_F(BranchTest, SendBroadcastAsync) {
  auto branch = create_branch();

  // Send JSON-encoded data with retry
  bool called = false;
  auto fn     = [&](const yogi::Result& res, yogi::OperationId oid) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(oid.value(), 456);
    called = true;
  };

  MOCK_BranchSendBroadcastAsync([](void* branch, int enc, const void* data, int datasize, int retry,
                                   void (*fn)(int res, int oid, void* userarg), void* userarg) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_EQ(enc, YOGI_ENC_JSON);
    EXPECT_NE(data, nullptr);
    EXPECT_EQ(datasize, 6);
    EXPECT_EQ(retry, YOGI_TRUE);
    EXPECT_NE(userarg, nullptr);
    fn(YOGI_OK, 456, userarg);
    return 123;
  });

  const char* s = "hello";
  EXPECT_EQ(branch->send_broadcast_async(yogi::JsonView(s), true, fn).value(), 123);
  EXPECT_TRUE(called);

  // Send Msgpack-encoded data without retry
  called   = false;
  auto fn2 = [&](const yogi::Result& res, yogi::OperationId oid) {
    EXPECT_EQ(res.error_code(), yogi::ErrorCode::kBusy);
    EXPECT_EQ(oid.value(), 456);
    called = true;
  };

  MOCK_BranchSendBroadcastAsync([](void* branch, int enc, const void* data, int datasize, int retry,
                                   void (*fn)(int res, int oid, void* userarg), void* userarg) {
    EXPECT_EQ(enc, YOGI_ENC_MSGPACK);
    EXPECT_EQ(retry, YOGI_FALSE);
    EXPECT_NE(userarg, nullptr);
    fn(YOGI_ERR_BUSY, 456, userarg);
    return 123;
  });

  EXPECT_EQ(branch->send_broadcast_async(yogi::MsgpackView(s), false, fn2).value(), 123);
  EXPECT_TRUE(called);

  // Error
  MOCK_BranchSendBroadcastAsync([](void* branch, int enc, const void* data, int datasize, int retry,
                                   void (*fn)(int res, int oid, void* userarg), void* userarg) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_ERR_TIMEOUT;
  });

  EXPECT_THROW(branch->send_broadcast_async(yogi::MsgpackView(s), fn), yogi::FailureException);
}

TEST_F(BranchTest, CancelSendBroadcast) {
  auto branch = create_branch();

  // Cancel successfully
  MOCK_BranchCancelSendBroadcast([](void* branch, int oid) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_EQ(oid, 123);
    return YOGI_OK;
  });

  EXPECT_TRUE(branch->cancel_send_broadcast(yogi::detail::make_operation_id(123)));

  // Operation not running
  MOCK_BranchCancelSendBroadcast([](void* branch, int oid) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_ERR_INVALID_OPERATION_ID;
  });

  EXPECT_FALSE(branch->cancel_send_broadcast(yogi::detail::make_operation_id(123)));

  // Error
  MOCK_BranchCancelSendBroadcast([](void* branch, int oid) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_ERR_TIMEOUT;
  });

  EXPECT_THROW(branch->cancel_send_broadcast(yogi::detail::make_operation_id(123)), yogi::FailureException);
}

TEST_F(BranchTest, ReceiveBroadcastAsync) {
  auto branch = create_branch();

  // Receive msg successfully as JSON
  bool called = false;
  auto fn     = [&](const yogi::Result& res, const yogi::Uuid& source, const yogi::PayloadView& payload,
                yogi::BufferPtr&& buffer) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_NE(source, yogi::Uuid{});
    EXPECT_EQ(payload.size(), 1);
    EXPECT_EQ(buffer->size(), 100);  // kMaxMessagePayloadSize of yogi-core-mock
    called = true;
  };

  MOCK_BranchReceiveBroadcastAsync([](void* branch, void* uuid, int enc, void* data, int datasize,
                                      void (*fn)(int res, int size, void* userarg), void* userarg) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_NE(uuid, nullptr);
    *static_cast<char*>(uuid) = 111;
    EXPECT_EQ(enc, YOGI_ENC_JSON);
    EXPECT_NE(data, nullptr);
    EXPECT_EQ(datasize, 100);  // kMaxMessagePayloadSize of yogi-core-mock
    EXPECT_NE(userarg, nullptr);
    fn(YOGI_OK, 1, userarg);
    return YOGI_OK;
  });

  branch->receive_broadcast_async(yogi::Encoding::kJson, fn);
  EXPECT_TRUE(called);

  // Receive msg successfully as Msgpack with custom buffer
  auto buffer     = std::make_unique<yogi::Buffer>(123);
  auto buffer_raw = buffer.get();

  called   = false;
  auto fn2 = [&](const yogi::Result& res, const yogi::Uuid& source, const yogi::PayloadView& payload,
                 yogi::BufferPtr&& buffer) {
    EXPECT_EQ(res.error_code(), yogi::ErrorCode::kBusy);
    EXPECT_EQ(buffer.get(), buffer_raw);
    called = true;
  };

  MOCK_BranchReceiveBroadcastAsync([](void* branch, void* uuid, int enc, void* data, int datasize,
                                      void (*fn)(int res, int size, void* userarg), void* userarg) {
    EXPECT_EQ(branch, kPointer);
    EXPECT_EQ(enc, YOGI_ENC_MSGPACK);
    EXPECT_NE(data, nullptr);
    EXPECT_EQ(datasize, 123);
    EXPECT_NE(userarg, nullptr);
    fn(YOGI_ERR_BUSY, 123, userarg);
    return YOGI_OK;
  });

  branch->receive_broadcast_async(yogi::Encoding::kMsgpack, std::move(buffer), fn2);
  EXPECT_TRUE(called);

  // Error
  MOCK_BranchReceiveBroadcastAsync([](void* branch, void* uuid, int enc, void* data, int datasize,
                                      void (*fn)(int res, int size, void* userarg), void* userarg) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(branch->receive_broadcast_async(yogi::Encoding::kMsgpack, fn), yogi::FailureException);
}

TEST_F(BranchTest, CancelReceiveBroadcast) {
  auto branch = create_branch();

  // Success
  MOCK_BranchCancelReceiveBroadcast([](void* branch) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_OK;
  });

  EXPECT_TRUE(branch->cancel_receive_broadcast());

  // Operation not running
  MOCK_BranchCancelReceiveBroadcast([](void* branch) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_ERR_OPERATION_NOT_RUNNING;
  });

  EXPECT_FALSE(branch->cancel_receive_broadcast());

  // Error
  MOCK_BranchCancelReceiveBroadcast([](void* branch) {
    EXPECT_EQ(branch, kPointer);
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(branch->cancel_receive_broadcast(), yogi::FailureException);
}
