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
#include <src/system/network_info.h>
#include <src/system/process.h>
#include <src/util/algorithm.h>

#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>
#include <regex>

class BranchTest : public TestFixture {
 protected:
  virtual void SetUp() override {
    context_ = create_context();
    branch_  = create_branch(context_);
  }

  void* context_;
  void* branch_;
};

TEST_F(BranchTest, CreateWithDefaults) {
  void* branch;
  int res = YOGI_BranchCreate(&branch, context_, nullptr, nullptr);
  EXPECT_OK(res);
}

TEST_F(BranchTest, CreateWithJsonPointer) {
  nlohmann::json props;
  props["arr"]            = {"some string", kBranchProps, 123};
  props["arr"][1]["name"] = "Samosa";

  void* branch;
  int res = YOGI_BranchCreate(&branch, context_, create_configuration(props), "/blabla");
  EXPECT_ERR(res, YOGI_ERR_CONFIGURATION_SECTION_NOT_FOUND);

  res = YOGI_BranchCreate(&branch, context_, create_configuration(props), "/arr/1");
  EXPECT_OK(res);
  auto info = get_branch_info(branch);
  EXPECT_EQ(info.value("name", "NOT FOUND"), "Samosa");
}

TEST_F(BranchTest, DefaultQueueSizes) {
  void* branch;
  int res = YOGI_BranchCreate(&branch, context_, nullptr, nullptr);
  ASSERT_OK(res);
  auto info = get_branch_info(branch);
  EXPECT_EQ(info.value("tx_queue_size", -1), constants::kDefaultTxQueueSize);
  EXPECT_EQ(info.value("rx_queue_size", -1), constants::kDefaultRxQueueSize);
}

TEST_F(BranchTest, CustomQueueSizes) {
  nlohmann::json props;
  props["tx_queue_size"] = constants::kMaxTxQueueSize;
  props["rx_queue_size"] = constants::kMaxRxQueueSize;

  void* branch;
  int res = YOGI_BranchCreate(&branch, context_, create_configuration(props), nullptr);
  ASSERT_OK(res);
  auto info = get_branch_info(branch);
  EXPECT_EQ(info.value("tx_queue_size", -1), constants::kMaxTxQueueSize);
  EXPECT_EQ(info.value("rx_queue_size", -1), constants::kMaxRxQueueSize);
}

TEST_F(BranchTest, InvalidQueueSizes) {
  std::vector<std::pair<const char*, int>> entries = {
      {"tx_queue_size", constants::kMinTxQueueSize - 1},
      {"tx_queue_size", constants::kMaxTxQueueSize + 1},
      {"rx_queue_size", constants::kMinTxQueueSize - 1},
      {"rx_queue_size", constants::kMaxTxQueueSize + 1},
  };

  for (auto entry : entries) {
    nlohmann::json props;
    props[entry.first] = entry.second;

    void* branch;
    int res = YOGI_BranchCreate(&branch, context_, create_configuration(props), nullptr);
    EXPECT_ERR(res, YOGI_ERR_CONFIGURATION_VALIDATION_FAILED);
    EXPECT_NE(std::string(YOGI_GetLastErrorDetails()).find(entry.first), std::string::npos);
  }
}

TEST_F(BranchTest, GetInfoUuid) {
  get_branch_uuid(branch_);
}

TEST_F(BranchTest, GetInfoJson) {
  boost::uuids::uuid uuid;
  const char* json = nullptr;
  int jsonsize     = 0;
  int res          = YOGI_BranchGetInfo(branch_, &uuid, &json, &jsonsize);
  EXPECT_OK(res);
  EXPECT_NE(json, nullptr);
  EXPECT_GT(jsonsize, 0);
  EXPECT_EQ(json[jsonsize - 1], '\0');
  EXPECT_STRNE(json, "");
  auto info = nlohmann::json::parse(json);

  auto time_regex   = std::regex("^20\\d\\d-\\d\\d-\\d\\dT[0-2]\\d:[0-5]\\d:[0-5]\\d\\.\\d\\d\\dZ$");
  auto default_name = std::to_string(get_process_id()) + '@' + get_hostname();

  EXPECT_EQ(info.value("uuid", "NOT FOUND"), boost::uuids::to_string(uuid));
  EXPECT_EQ(info.value("name", "NOT FOUND"), default_name);
  EXPECT_FALSE(info.value("description", "").empty());
  EXPECT_EQ(info.value("network_name", "NOT FOUND"), get_hostname());
  EXPECT_EQ(info.value("path", "NOT FOUND"), std::string("/") + default_name);
  EXPECT_EQ(info.value("hostname", "NOT FOUND"), get_hostname());
  EXPECT_EQ(info.value("pid", -1), get_process_id());
  EXPECT_GT(info.value("tcp_server_port", 0), 1024);
  EXPECT_TRUE(std::regex_match(info.value("start_time", "NOT FOUND"), time_regex));
  EXPECT_EQ(info.value("timeout", -1.0f), kBranchProps["timeout"]);
  EXPECT_EQ(info.value("advertising_address", "NOT FOUND"), kAdvAddress);
  EXPECT_EQ(info.value("advertising_port", -1), static_cast<int>(kAdvPort));
  EXPECT_EQ(info.value("advertising_interval", -1.0f), static_cast<float>(kBranchProps["advertising_interval"]));
  EXPECT_EQ(info.value("ghost_mode", true), false);
  EXPECT_EQ(info.value("tx_queue_size", -1), constants::kDefaultTxQueueSize);
  EXPECT_EQ(info.value("rx_queue_size", -1), constants::kDefaultRxQueueSize);

  auto ifs = info["advertising_interfaces"];
  ASSERT_TRUE(ifs.is_array());
  ASSERT_FALSE(ifs.empty());
  EXPECT_TRUE(ifs[0]["name"].is_string());
  EXPECT_TRUE(ifs[0]["identifier"].is_string());
  EXPECT_TRUE(ifs[0]["mac"].is_string());
  EXPECT_TRUE(ifs[0]["is_loopback"].is_boolean());
  EXPECT_TRUE(!ifs[0].value("mac", "").empty() || ifs[0].value("is_loopback", false));

  auto addrs = ifs[0]["addresses"];
  ASSERT_TRUE(addrs.is_array());
  ASSERT_FALSE(addrs.empty());
  EXPECT_TRUE(addrs[0].is_string());

  bool loopback_found = contains_if(ifs, [](auto& info) { return info["is_loopback"].template get<bool>(); });
  EXPECT_TRUE(loopback_found);
}
