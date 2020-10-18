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

#include <boost/asio.hpp>

class ConnectionManagerTest : public TestFixture {
 protected:
  virtual void SetUp() override {
    context_ = create_context();
    branch_  = create_branch(context_);
  }

  void re_create_branch(const char* adv_addr) {
    auto res = YOGI_Destroy(branch_);
    ASSERT_OK(res);
    branch_ = create_branch(context_, nullptr, nullptr, nullptr, nullptr, adv_addr);
  }

  void* context_;
  void* branch_;

  void test_advertising(const char* adv_addr) {
    MulticastSocket multicast(boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(adv_addr), kAdvPort));

    re_create_branch(adv_addr);
    run_context_in_background(context_);

    bool uuid_ok = false;
    for (int i = 0; !uuid_ok && i < 10; ++i) {
      auto msg = multicast.receive().second;
      ASSERT_EQ(msg.size(), 25) << "Unexpected advertising message size";

      boost::uuids::uuid uuid;
      YOGI_BranchGetInfo(branch_, &uuid, nullptr, 0);

      EXPECT_EQ(msg[0], 'Y');
      EXPECT_EQ(msg[1], 'O');
      EXPECT_EQ(msg[2], 'G');
      EXPECT_EQ(msg[3], 'I');
      EXPECT_EQ(msg[4], '\0');
      EXPECT_EQ(msg[5], constants::kVersionMajor);
      EXPECT_EQ(msg[6], constants::kVersionMinor);
      EXPECT_TRUE(msg[23] != 0 || msg[24] != 0);

      uuid_ok = std::memcmp(&uuid, msg.data() + 7, sizeof(uuid)) == 0;
    }

    EXPECT_TRUE(uuid_ok);
  }

  void test_connect_normally(const char* adv_addr) {
    re_create_branch(adv_addr);

    void* branch_a = create_branch(context_, "a", nullptr, nullptr, nullptr, adv_addr);
    void* branch_b = create_branch(context_, "b", nullptr, nullptr, nullptr, adv_addr);

    BranchEventRecorder rec(context_, branch_);
    rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_a, YOGI_OK);
    rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_b, YOGI_OK);
  }
};

TEST_F(ConnectionManagerTest, AdvertisingIPv4) {
  test_advertising("239.255.0.1");
}

TEST_F(ConnectionManagerTest, AdvertisingIPv6) {
  test_advertising("ff02::8000:2439");
}

TEST_F(ConnectionManagerTest, ConnectNormallyIPv4) {
  test_connect_normally("239.255.0.1");
}

TEST_F(ConnectionManagerTest, ConnectNormallyIPv6) {
  test_connect_normally("ff02::8000:2439");
}

TEST_F(ConnectionManagerTest, DuplicateBranchName) {
  void* branch_a = create_branch(context_);  // Same name as branch_
  BranchEventRecorder rec(context_, branch_);
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_a, YOGI_ERR_DUPLICATE_BRANCH_NAME);
  YOGI_Destroy(branch_a);

  void* branch_b = create_branch(context_, "Same name");
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_b, YOGI_OK);

  void* branch_c = create_branch(context_, "Same name");
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_c, YOGI_ERR_DUPLICATE_BRANCH_NAME);
}

TEST_F(ConnectionManagerTest, DuplicateBranchPath) {
  auto branch_path = get_branch_info(branch_)["path"].get<std::string>();
  void* branch_a   = create_branch(context_, "a", nullptr, nullptr, branch_path.c_str());
  BranchEventRecorder rec(context_, branch_);
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_a, YOGI_ERR_DUPLICATE_BRANCH_PATH);
  YOGI_Destroy(branch_a);

  void* branch_b = create_branch(context_, "b", nullptr, nullptr, "/tmp");
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_b, YOGI_OK);

  void* branch_c = create_branch(context_, "c", nullptr, nullptr, "/tmp");
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_c, YOGI_ERR_DUPLICATE_BRANCH_PATH);
}

TEST_F(ConnectionManagerTest, NetNameMismatch) {
  void* branch_a = create_branch(context_, "a", "other net");
  BranchEventRecorder rec(context_, branch_);
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_a, YOGI_ERR_NET_NAME_MISMATCH);
}

TEST_F(ConnectionManagerTest, PasswordMismatch) {
  void* branch_a = create_branch(context_, "a", nullptr, "diferent password");
  BranchEventRecorder rec(context_, branch_);
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_a, YOGI_ERR_PASSWORD_MISMATCH);
}

TEST_F(ConnectionManagerTest, AdvAndInfoMessageUuidMismatch) {
  run_context_in_background(context_);
  FakeBranch fake;

  auto fn = [](auto msg) { ++msg->at(8); };
  fake.advertise(fn);

  EXPECT_THROW(fake.accept(), boost::system::system_error);
}

TEST_F(ConnectionManagerTest, Reconnect) {
  run_context_in_background(context_);
  FakeBranch fake;

  fake.connect(branch_);
  while (!fake.is_connected_to(branch_))
    ;

  fake.disconnect();
  while (fake.is_connected_to(branch_))
    ;

  fake.advertise();
  fake.accept();
  while (!fake.is_connected_to(branch_))
    ;
}

TEST_F(ConnectionManagerTest, InvalidMagicPrefix) {
  run_context_in_background(context_);
  FakeBranch fake;

  auto fn = [](auto msg) { msg->at(1) = 'X'; };

  EXPECT_THROW(fake.connect(branch_, fn), boost::system::system_error);
  fake.advertise();
  EXPECT_THROW(fake.accept(fn), boost::system::system_error);
}

TEST_F(ConnectionManagerTest, IncompatibleVersion) {
  run_context_in_background(context_);
  FakeBranch fake;

  auto fn = [](auto msg) { ++msg->at(5); };

  EXPECT_THROW(fake.connect(branch_, fn), boost::system::system_error);
  fake.advertise();
  EXPECT_THROW(fake.accept(fn), boost::system::system_error);
}

TEST_F(ConnectionManagerTest, LoopbackConnection) {
  run_context_in_background(context_);
  FakeBranch fake;

  auto fn = [&](auto msg) {
    auto uuid = get_branch_uuid(branch_);
    std::copy(uuid.begin(), uuid.end(), msg->begin() + 7);
  };

  EXPECT_THROW(fake.connect(branch_, fn), boost::system::system_error);
  fake.advertise();
  EXPECT_THROW(fake.accept(fn), boost::system::system_error);
}

TEST_F(ConnectionManagerTest, BrokenAdvertisementMessage) {
  run_context_in_background(context_);
  FakeBranch fake;

  fake.advertise([](auto msg) { msg->push_back('x'); });
  std::this_thread::sleep_for(1ms);
  // just checking that nothing crashes
}

TEST_F(ConnectionManagerTest, BrokenInfoMessage) {
  run_context_in_background(context_);
  FakeBranch fake;

  auto fn = [](auto msg) { ++msg->at(BranchInfo::kAdvertisingMessageSize) = 0xFF; };

  EXPECT_THROW(fake.connect(branch_, fn), boost::system::system_error);
  fake.advertise();
  EXPECT_THROW(fake.accept(fn), boost::system::system_error);
}

TEST_F(ConnectionManagerTest, BranchEvents) {
  void* branch_a = create_branch(context_, "a");
  auto uuid      = get_branch_uuid(branch_a);
  auto info      = get_branch_info(branch_a);

  BranchEventRecorder rec(context_, branch_);
  auto json = rec.run_context_until(YOGI_BEV_BRANCH_DISCOVERED, uuid, YOGI_OK);
  check_json_elements_are_equal(json, info, "uuid");
  EXPECT_FALSE(json.value("tcp_server_address", "").empty());
  check_json_elements_are_equal(json, info, "tcp_server_port");

  json = rec.run_context_until(YOGI_BEV_BRANCH_QUERIED, uuid, YOGI_OK);
  check_json_elements_are_equal(json, info, "uuid");
  check_json_elements_are_equal(json, info, "name");
  check_json_elements_are_equal(json, info, "description");
  check_json_elements_are_equal(json, info, "network_name");
  check_json_elements_are_equal(json, info, "path");
  check_json_elements_are_equal(json, info, "hostname");
  check_json_elements_are_equal(json, info, "pid");
  check_json_elements_are_equal(json, info, "tcp_server_port");
  check_json_elements_are_equal(json, info, "start_time");
  check_json_elements_are_equal(json, info, "timeout");
  check_json_elements_are_equal(json, info, "advertising_interval");
  check_json_elements_are_equal(json, info, "ghost_mode");

  json = rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, uuid, YOGI_OK);
  check_json_elements_are_equal(json, info, "uuid");

  YOGI_Destroy(branch_a);
  json = rec.run_context_until(YOGI_BEV_CONNECTION_LOST, uuid, YOGI_ERR_RW_SOCKET_FAILED);
  check_json_elements_are_equal(json, info, "uuid");
}

TEST_F(ConnectionManagerTest, CancelAwaitBranchEvent) {
  int res = YOGI_BranchCancelAwaitEvent(branch_);
  ASSERT_ERR(res, YOGI_ERR_OPERATION_NOT_RUNNING);

  bool called = false;
  res         = YOGI_BranchAwaitEventAsync(
      branch_, 0, nullptr, nullptr, 0,
      [](int res, int event, int ev_res, void* userarg) {
        EXPECT_ERR(res, YOGI_ERR_CANCELED);
        EXPECT_EQ(event, YOGI_BEV_NONE);
        EXPECT_EQ(ev_res, YOGI_OK);
        *static_cast<bool*>(userarg) = true;
      },
      &called);
  ASSERT_OK(res);

  res = YOGI_BranchCancelAwaitEvent(branch_);
  ASSERT_OK(res);

  while (!called) {
    res = YOGI_ContextRunOne(context_, nullptr, -1);
    EXPECT_OK(res);
  }
}

TEST_F(ConnectionManagerTest, AwaitBranchEventOnDestruction) {
  bool called = false;
  int res     = YOGI_BranchAwaitEventAsync(
      branch_, 0, nullptr, nullptr, 0,
      [](int res, int event, int ev_res, void* userarg) {
        EXPECT_ERR(res, YOGI_ERR_CANCELED);
        EXPECT_EQ(event, YOGI_BEV_NONE);
        EXPECT_EQ(ev_res, YOGI_OK);
        *static_cast<bool*>(userarg) = true;
      },
      &called);
  ASSERT_OK(res);

  YOGI_Destroy(branch_);

  while (!called) {
    res = YOGI_ContextRunOne(context_, nullptr, -1);
    EXPECT_OK(res);
  }
}

TEST_F(ConnectionManagerTest, GetConnectedBranches) {
  void* branch_a = create_branch(context_, "a");
  void* branch_b = create_branch(context_, "b");

  BranchEventRecorder rec(context_, branch_);
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_a, YOGI_OK);
  rec.run_context_until(YOGI_BEV_CONNECT_FINISHED, branch_b, YOGI_OK);

  auto branches = get_connected_branches(branch_);
  EXPECT_EQ(branches.size(), 2);

  auto fn = [&](void* branch) {
    auto info = get_branch_info(branch);
    auto uuid = get_branch_uuid(branch);
    ASSERT_TRUE(branches.count(uuid));

    auto json = branches[uuid];
    check_json_elements_are_equal(json, info, "uuid");
    check_json_elements_are_equal(json, info, "name");
    check_json_elements_are_equal(json, info, "description");
    check_json_elements_are_equal(json, info, "network_name");
    check_json_elements_are_equal(json, info, "path");
    check_json_elements_are_equal(json, info, "hostname");
    check_json_elements_are_equal(json, info, "pid");
    check_json_elements_are_equal(json, info, "tcp_server_port");
    check_json_elements_are_equal(json, info, "start_time");
    check_json_elements_are_equal(json, info, "timeout");
    check_json_elements_are_equal(json, info, "advertising_interval");
    check_json_elements_are_equal(json, info, "ghost_mode");
  };

  fn(branch_a);
  fn(branch_b);
}

TEST_F(ConnectionManagerTest, GhostMode) {
  auto props          = kBranchProps;
  props["name"]       = "B";
  props["ghost_mode"] = true;

  void* ghost_branch;
  int res = YOGI_BranchCreate(&ghost_branch, context_, create_configuration(props), nullptr);
  EXPECT_OK(res);

  BranchEventRecorder rec(context_, ghost_branch);
  rec.run_context_until(YOGI_BEV_BRANCH_QUERIED, branch_, YOGI_OK);
  YOGI_ContextRun(context_, nullptr, 10'000'000);

  auto branches = get_connected_branches(ghost_branch);
  EXPECT_TRUE(branches.empty());
}
