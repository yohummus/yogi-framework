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
#include <src/schemas/schemas.h>

#include <nlohmann/json.hpp>

TEST(SchemasTest, GetSchema) {
  auto schema = YOGI_GetSchema(YOGI_SCM_BRANCH_EVENT);
  ASSERT_NE(schema, nullptr);
  ASSERT_NE(schema[0], 0);
  EXPECT_NE(std::string(schema).find(".schema.json"), std::string::npos);

  schema = YOGI_GetSchema(-123);
  ASSERT_NE(schema, nullptr);
  ASSERT_EQ(schema[0], 0);
}

TEST(SchemasTest, BranchConfigDefaults) {
  auto schema = nlohmann::json::parse(YOGI_GetSchema(YOGI_SCM_BRANCH_PROPERTIES));

  EXPECT_EQ(schema["properties"]["advertising_interfaces"]["default"],
            nlohmann::json::parse(constants::kDefaultAdvInterfaces));
  EXPECT_EQ(schema["properties"]["advertising_address"]["default"], constants::kDefaultAdvAddress);
  EXPECT_EQ(schema["properties"]["advertising_port"]["default"], constants::kDefaultAdvPort);
  EXPECT_EQ(schema["properties"]["advertising_interval"]["default"], constants::kDefaultAdvInterval / 1e9);
  EXPECT_EQ(schema["properties"]["timeout"]["default"], constants::kDefaultConnectionTimeout / 1e9);
  EXPECT_EQ(schema["properties"]["ghost_mode"]["default"], false);
  EXPECT_EQ(schema["properties"]["tx_queue_size"]["default"], constants::kDefaultTxQueueSize);
  EXPECT_EQ(schema["properties"]["rx_queue_size"]["default"], constants::kDefaultRxQueueSize);
}

TEST(SchemasTest, ValidateJson) {
  auto json_good = nlohmann::json::parse(R"( {"uuid": "6ba7b810-9dad-11d1-80b4-00c04fd430c8"} )");
  auto json_bad  = nlohmann::json::parse(R"( {"uuid": 123} )");

  ASSERT_NO_THROW(validate_json(json_good, YOGI_SCM_BRANCH_EVENT));
  ASSERT_THROW(validate_json(json_bad, YOGI_SCM_BRANCH_EVENT), DescriptiveError);

  try {
    validate_json(json_bad, YOGI_SCM_BRANCH_EVENT, "foobar");
    FAIL() << "Should have thrown an exception";
  } catch (const DescriptiveError& err) {
    ASSERT_NE(err.details().find("foobar"), std::string::npos);
  }
}
