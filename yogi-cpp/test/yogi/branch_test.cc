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

#include "../common.h"

#include <yogi_core.h>

class BranchTest : public testing::Test {
 protected:
  BranchTest()
      : context_(yogi::Context::create()),
        json_view_(json_data_),
        big_json_data_(make_big_json_data()),
        big_json_view_(big_json_data_),
        msgpack_view_(msgpack_data_, sizeof(msgpack_data_)) {
  }

  static std::vector<char> make_big_json_data(std::size_t size = 10000) {
    std::vector<char> data{'[', '"', '"', ']', '\0'};
    data.insert(data.begin() + 2, size - data.size(), '.');
    return data;
  }

  yogi::ContextPtr context_;
  const char json_data_[8] = "[1,2,3]";
  const yogi::JsonView json_view_;
  const std::vector<char> big_json_data_;
  const yogi::JsonView big_json_view_;
  const char msgpack_data_[4] = {-109, 1, 2, 3};
  const yogi::MsgpackView msgpack_view_;
};

TEST_F(BranchTest, CreateWithSubSection) {
  auto branch = yogi::Branch::create(context_, "{\"branch\":{\"name\":\"Cow\"}}", "/branch");
  EXPECT_EQ(branch->name(), "Cow");
}

TEST_F(BranchTest, BranchEventsEnum) {
  // clang-format off
  CHECK_ENUM_ELEMENT(BranchEvents, kNone,             YOGI_BEV_NONE);
  CHECK_ENUM_ELEMENT(BranchEvents, kBranchDiscovered, YOGI_BEV_BRANCH_DISCOVERED);
  CHECK_ENUM_ELEMENT(BranchEvents, kBranchQueried,    YOGI_BEV_BRANCH_QUERIED);
  CHECK_ENUM_ELEMENT(BranchEvents, kConnectFinished,  YOGI_BEV_CONNECT_FINISHED);
  CHECK_ENUM_ELEMENT(BranchEvents, kConnectionLost,   YOGI_BEV_CONNECTION_LOST);
  CHECK_ENUM_ELEMENT(BranchEvents, kAll,              YOGI_BEV_ALL);
  // clang-format on

  auto events = yogi::BranchEvents::kNone;
  EXPECT_EQ(yogi::to_string(events), "kNone");
  events = events | yogi::BranchEvents::kBranchDiscovered;
  EXPECT_EQ(yogi::to_string(events), "kBranchDiscovered");
  events |= yogi::BranchEvents::kConnectFinished;
  EXPECT_EQ(yogi::to_string(events), "kBranchDiscovered | kConnectFinished");
  events |= yogi::BranchEvents::kAll;
  EXPECT_EQ(yogi::to_string(events), "kAll");
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
  EXPECT_EQ(info.get_description(), "Stuff");
  EXPECT_EQ(info.get_network_name(), "My Network");
  EXPECT_EQ(info.path(), "/some/path");
  EXPECT_FALSE(info.hostname().empty());
  EXPECT_GT(info.pid(), 0);
  EXPECT_EQ(info.advertising_address(), "239.255.0.1");
  EXPECT_EQ(info.advertising_port(), 12345);
  EXPECT_EQ(info.advertising_interval(), yogi::Duration::from_seconds(7));
  EXPECT_GT(info.get_tcp_server_port(), 0);
  EXPECT_LT(info.start_time(), yogi::current_time());
  EXPECT_EQ(info.timeout(), yogi::Duration::kInfinity);
  EXPECT_FALSE(info.ghost_mode());
  EXPECT_GT(info.tx_queue_size(), 1000);
  EXPECT_GT(info.rx_queue_size(), 1000);

  EXPECT_EQ(branch->uuid(), info.uuid());
  EXPECT_EQ(branch->name(), info.name());
  EXPECT_EQ(branch->get_description(), info.get_description());
  EXPECT_EQ(branch->get_network_name(), info.get_network_name());
  EXPECT_EQ(branch->path(), info.path());
  EXPECT_EQ(branch->hostname(), info.hostname());
  EXPECT_EQ(branch->pid(), info.pid());
  EXPECT_EQ(branch->advertising_address(), info.advertising_address());
  EXPECT_EQ(branch->advertising_port(), info.advertising_port());
  EXPECT_EQ(branch->advertising_interval(), info.advertising_interval());
  EXPECT_EQ(branch->get_tcp_server_port(), info.get_tcp_server_port());
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
    EXPECT_NO_THROW(dynamic_cast<const yogi::Success&>(res));
    EXPECT_EQ(res.error_code(), yogi::ErrorCode::kOk);
    EXPECT_EQ(event, yogi::BranchEvents::kBranchQueried);
    EXPECT_NO_THROW(dynamic_cast<const yogi::Success&>(evres));
    EXPECT_EQ(evres.error_code(), yogi::ErrorCode::kOk);
    EXPECT_NO_THROW(dynamic_cast<const yogi::BranchQueriedEventInfo&>(info));
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
    EXPECT_NO_THROW(dynamic_cast<const yogi::Failure&>(res));
    EXPECT_EQ(res.error_code(), yogi::ErrorCode::kCanceled);
    EXPECT_EQ(event, yogi::BranchEvents::kNone);
    EXPECT_NO_THROW(dynamic_cast<const yogi::Success&>(evres));
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
  branch_b->receive_broadcast_async(yogi::EncodingType::kJson, [&](auto& res, auto&, auto& payload) {
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
  branch_b->receive_broadcast_async(yogi::EncodingType::kJson, [&](auto& res, auto&, auto& payload) {
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
  branch_a->receive_broadcast_async(yogi::EncodingType::kJson, [&](auto& res, auto&, auto& payload) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, json_view_);
    called = true;
  });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();

  // With encoding and with buffer in handler function
  called = false;
  branch_a->receive_broadcast_async(yogi::EncodingType::kJson, [&](auto& res, auto&, auto& payload, auto&& buffer) {
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
  branch_a->receive_broadcast_async(yogi::EncodingType::kJson, std::move(buffer), [&](auto& res, auto&, auto& payload) {
    EXPECT_EQ(res, yogi::Success());
    EXPECT_EQ(payload, json_view_);
    called = true;
  });

  branch_b->send_broadcast_async(msgpack_view_, {});
  while (!called) context_->run_one();

  // With custom buffer and encoding and buffer in handler function
  buffer = std::make_unique<yogi::Buffer>(123);
  called = false;
  branch_a->receive_broadcast_async(yogi::EncodingType::kJson, std::move(buffer),
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
