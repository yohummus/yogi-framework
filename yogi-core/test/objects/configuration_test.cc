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

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class ConfigurationTest : public TestFixture {
 protected:
  virtual void SetUp() override {
    cfg_ = make_configuration(YOGI_CFG_NONE);
  }

  void* make_configuration(int flags, const char* json = R"({"person": {"name": "Joe", "age": 42}})") {
    void* cfg = nullptr;
    int res   = YOGI_ConfigurationCreate(&cfg, flags);
    EXPECT_OK(res);
    EXPECT_NE(cfg, nullptr);

    res = YOGI_ConfigurationUpdateFromJson(cfg, json);
    EXPECT_OK(res);

    return cfg;
  }

  TemporaryWorkdirGuard write_temp_file(const char* filename, const char* content = R"({"person": {"age": 10}})") {
    TemporaryWorkdirGuard workdir;

    fs::ofstream file(filename);
    file << content;

    return workdir;
  }

  nlohmann::json dump_configuration(void* cfg) {
    const char* json;
    int res = YOGI_ConfigurationDump(cfg, &json, nullptr, YOGI_TRUE, -1);
    if (res == YOGI_ERR_NO_VARIABLE_SUPPORT) {
      res = YOGI_ConfigurationDump(cfg, &json, nullptr, YOGI_FALSE, -1);
    }
    EXPECT_OK(res);

    return nlohmann::json::parse(json);
  }

  void check_configuration_is_original() {
    auto expected = nlohmann::json::parse(R"({"person": {"name": "Joe", "age": 42}})");
    auto actual   = dump_configuration(cfg_);
    EXPECT_TRUE(actual == expected) << "Expected:" << std::endl
                                    << expected.dump(2) << std::endl
                                    << "Actual:" << std::endl
                                    << actual.dump(2) << std::endl;
  }

  void* cfg_;
};

TEST_F(ConfigurationTest, UpdateFromJson) {
  int res = YOGI_ConfigurationUpdateFromJson(cfg_, R"({"person": {"age": 10}})");
  EXPECT_OK(res);

  auto json = dump_configuration(cfg_);
  ASSERT_TRUE(json.count("person")) << json;
  EXPECT_EQ(json["person"].value("name", "NOT FOUND"), "Joe");
  EXPECT_EQ(json["person"].value("age", -1), 10);
}

TEST_F(ConfigurationTest, UpdateFromCorruptJson) {
  int res = YOGI_ConfigurationUpdateFromJson(cfg_, R"({"person": {"age": 10})");
  EXPECT_ERR(res, YOGI_ERR_PARSING_JSON_FAILED);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "unexpected end of input");

  check_configuration_is_original();
}

TEST_F(ConfigurationTest, UpdateFromCommandLine) {
  auto workdir = write_temp_file("a.json");

  // clang-format off
  CommandLine cmdline{
    "--name", "My Branch",
    "a.json",
  };
  // clang-format on

  int res = YOGI_ConfigurationUpdateFromCommandLine(cfg_, cmdline.argc, cmdline.argv, YOGI_CLO_ALL);
  ASSERT_OK(res);

  auto json = dump_configuration(cfg_);
  ASSERT_TRUE(json.count("person")) << json;
  EXPECT_EQ(json["person"].value("name", "NOT FOUND"), "Joe");
  EXPECT_EQ(json["person"].value("age", -1), 10);
  ASSERT_TRUE(json.count("branch")) << json;
  EXPECT_EQ(json["branch"].value("name", "NOT FOUND"), "My Branch");
}

TEST_F(ConfigurationTest, UpdateFromCommandLineCorruptFile) {
  auto workdir = write_temp_file("a.json", R"({"person": {"age": 10})");

  // clang-format off
  CommandLine cmdline{
    "--name", "My Branch",
    "a.json",
  };
  // clang-format on

  int res = YOGI_ConfigurationUpdateFromCommandLine(cfg_, cmdline.argc, cmdline.argv, YOGI_CLO_ALL);
  ASSERT_ERR(res, YOGI_ERR_PARSING_FILE_FAILED);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "unexpected end of input");

  check_configuration_is_original();
}

TEST_F(ConfigurationTest, UpdateFromFile) {
  auto workdir = write_temp_file("a.json");

  int res = YOGI_ConfigurationUpdateFromFile(cfg_, "a.json");
  ASSERT_OK(res);

  auto json = dump_configuration(cfg_);
  ASSERT_TRUE(json.count("person")) << json;
  EXPECT_EQ(json["person"].value("name", "NOT FOUND"), "Joe");
  EXPECT_EQ(json["person"].value("age", -1), 10);
}

TEST_F(ConfigurationTest, UpdateFromCorruptFile) {
  auto workdir = write_temp_file("a.json", R"({"person": {"age": 10})");

  int res = YOGI_ConfigurationUpdateFromFile(cfg_, "a.json");
  ASSERT_ERR(res, YOGI_ERR_PARSING_FILE_FAILED);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "unexpected end of input");

  check_configuration_is_original();
}

TEST_F(ConfigurationTest, Dump) {
  void* cfg1 = make_configuration(YOGI_CFG_NONE);
  void* cfg2 = make_configuration(YOGI_CFG_DISABLE_VARIABLES);

  const char* json;
  int jsonsize;

  int res = YOGI_ConfigurationDump(cfg1, &json, &jsonsize, YOGI_TRUE, -1);
  EXPECT_OK(res);
  EXPECT_STRNE(json, "");
  EXPECT_EQ(jsonsize, strlen(json) + 1);
  EXPECT_EQ(std::string{json}.find_first_of(" \n"), std::string::npos);

  res = YOGI_ConfigurationDump(cfg1, &json, &jsonsize, YOGI_FALSE, -1);
  EXPECT_OK(res);
  EXPECT_STRNE(json, "");
  EXPECT_EQ(std::string{json}.find_first_of(" \n"), std::string::npos);

  res = YOGI_ConfigurationDump(cfg1, &json, &jsonsize, YOGI_TRUE, 2);
  EXPECT_OK(res);
  EXPECT_STRNE(json, "");
  EXPECT_NE(std::string{json}.find('\n'), std::string::npos);
  EXPECT_NE(std::string{json}.find("  "), std::string::npos);

  res = YOGI_ConfigurationDump(cfg2, &json, &jsonsize, YOGI_FALSE, -1);
  EXPECT_OK(res);
  EXPECT_STRNE(json, "");
  EXPECT_EQ(std::string{json}.find_first_of(" \n"), std::string::npos);

  res = YOGI_ConfigurationDump(cfg2, &json, &jsonsize, YOGI_TRUE, -1);
  EXPECT_ERR(res, YOGI_ERR_NO_VARIABLE_SUPPORT);
}

TEST_F(ConfigurationTest, WriteToFile) {
  TemporaryWorkdirGuard workdir;
  void* cfg1 = make_configuration(YOGI_CFG_NONE);
  void* cfg2 = make_configuration(YOGI_CFG_DISABLE_VARIABLES);

  int res = YOGI_ConfigurationWriteToFile(cfg1, "a.json", YOGI_TRUE, -1);
  EXPECT_OK(res);
  auto content = read_file("a.json");
  EXPECT_FALSE(content.empty());
  EXPECT_EQ(std::string{content}.find_first_of(" \n"), std::string::npos);

  res = YOGI_ConfigurationWriteToFile(cfg1, "b.json", YOGI_FALSE, -1);
  EXPECT_OK(res);
  content = read_file("b.json");
  EXPECT_FALSE(content.empty());
  EXPECT_EQ(std::string{content}.find_first_of(" \n"), std::string::npos);

  res = YOGI_ConfigurationWriteToFile(cfg1, "c.json", YOGI_TRUE, 2);
  EXPECT_OK(res);
  content = read_file("c.json");
  EXPECT_FALSE(content.empty());
  EXPECT_NE(std::string{content}.find('\n'), std::string::npos);
  EXPECT_NE(std::string{content}.find("  "), std::string::npos);

  res = YOGI_ConfigurationWriteToFile(cfg2, "d.json", YOGI_FALSE, -1);
  EXPECT_OK(res);
  content = read_file("d.json");
  EXPECT_FALSE(content.empty());
  EXPECT_EQ(std::string{content}.find_first_of(" \n"), std::string::npos);

  res = YOGI_ConfigurationWriteToFile(cfg2, "e.json", YOGI_TRUE, -1);
  EXPECT_ERR(res, YOGI_ERR_NO_VARIABLE_SUPPORT);
}

TEST_F(ConfigurationTest, ImmutableCommandLine) {
  auto workdir = write_temp_file("a.json");

  // clang-format off
  CommandLine cmdline{
    "--name", "My Branch",
    "--override", "{\"person\":{\"age\":55}}",
    "a.json",
  };

  nlohmann::json json_update = {
    {"person", {
      {"age", 88}
    }},
    {"branch", {
      {"name", "Edgar"}
    }}
  };
  // clang-format on

  // Immutable command line
  void* cfg1 = make_configuration(YOGI_CFG_NONE);
  int res    = YOGI_ConfigurationUpdateFromCommandLine(cfg1, cmdline.argc, cmdline.argv, YOGI_CLO_ALL);
  EXPECT_OK(res);
  auto json = dump_configuration(cfg1);
  ASSERT_TRUE(json.count("person")) << json;
  EXPECT_EQ(json["person"].value("age", -1), 55);
  ASSERT_TRUE(json.count("branch")) << json;
  EXPECT_EQ(json["branch"].value("name", "NOT FOUND"), "My Branch");

  res = YOGI_ConfigurationUpdateFromJson(cfg1, json_update.dump().c_str());
  EXPECT_OK(res);
  json = dump_configuration(cfg1);
  ASSERT_TRUE(json.count("person")) << json;
  EXPECT_EQ(json["person"].value("age", -1), 55);
  ASSERT_TRUE(json.count("branch")) << json;
  EXPECT_EQ(json["branch"].value("name", "NOT FOUND"), "My Branch");

  // Mutable command line
  void* cfg2 = make_configuration(YOGI_CFG_MUTABLE_CMD_LINE);
  res        = YOGI_ConfigurationUpdateFromCommandLine(cfg2, cmdline.argc, cmdline.argv, YOGI_CLO_ALL);
  EXPECT_OK(res);
  json = dump_configuration(cfg2);
  ASSERT_TRUE(json.count("person")) << json;
  EXPECT_EQ(json["person"].value("age", -1), 55);
  ASSERT_TRUE(json.count("branch")) << json;
  EXPECT_EQ(json["branch"].value("name", "NOT FOUND"), "My Branch");

  res = YOGI_ConfigurationUpdateFromJson(cfg2, json_update.dump().c_str());
  EXPECT_OK(res);
  json = dump_configuration(cfg2);
  ASSERT_TRUE(json.count("person")) << json;
  EXPECT_EQ(json["person"].value("age", -1), 88);
  ASSERT_TRUE(json.count("branch")) << json;
  EXPECT_EQ(json["branch"].value("name", "NOT FOUND"), "Edgar");
}

TEST_F(ConfigurationTest, Variables) {
  // clang-format off
  nlohmann::json json_update = {
    {"person", {
      {"age", "${AGE}"},
      {"age-string", "${AGE} years"},
      {"name", "${NAME} Wayne"}
    }},
    {"variables", {
      {"AGE", 33},
      {"NAME", "Joe ${MIDDLENAME}"}
    }}
  };
  // clang-format on

  // clang-format off
  CommandLine cmdline{
    "--var", "MIDDLENAME=Rob",
  };
  // clang-format on

  int res = YOGI_ConfigurationUpdateFromCommandLine(cfg_, cmdline.argc, cmdline.argv, YOGI_CLO_ALL);
  ASSERT_OK(res);

  res = YOGI_ConfigurationUpdateFromJson(cfg_, json_update.dump().c_str());
  ASSERT_OK(res);

  auto json = dump_configuration(cfg_);
  ASSERT_TRUE(json.count("person")) << json;
  EXPECT_EQ(json["person"].value("age", -1), 33);
  EXPECT_EQ(json["person"].value("age-string", "NOT FOUND"), "33 years");
  EXPECT_EQ(json["person"].value("name", "NOT FOUND"), "Joe Rob Wayne");
  ASSERT_TRUE(json.count("variables")) << json;
  EXPECT_EQ(json["variables"].value("AGE", -1), 33);
  EXPECT_EQ(json["variables"].value("NAME", "NOT FOUND"), "Joe Rob");
  EXPECT_EQ(json["variables"].value("MIDDLENAME", "NOT FOUND"), "Rob");
}

TEST_F(ConfigurationTest, BadVariables) {
  // clang-format off
  nlohmann::json json = {
    {"name", "My ${NAME} please"}
  };
  // clang-format on

  int res = YOGI_ConfigurationUpdateFromJson(cfg_, json.dump().c_str());
  ASSERT_ERR(res, YOGI_ERR_UNDEFINED_VARIABLES);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "NAME");

  // clang-format off
  json = {
    {"name", "Var ${NAME is unterminated"}
  };
  // clang-format on

  res = YOGI_ConfigurationUpdateFromJson(cfg_, json.dump().c_str());
  ASSERT_ERR(res, YOGI_ERR_UNDEFINED_VARIABLES);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "NAME is unterminated");

  // clang-format off
  json = {
    {"variables", {
      {"ABX", "${ABO}"},
      {"ABO", "${ABI}"},
      {"ABI", "${ABX}"},
    }}
  };
  // clang-format on

  res = YOGI_ConfigurationUpdateFromJson(cfg_, json.dump().c_str());
  ASSERT_ERR(res, YOGI_ERR_UNDEFINED_VARIABLES);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "AB");
}

TEST_F(ConfigurationTest, Validate) {
  void* scm = make_configuration(YOGI_CFG_NONE, R"(
    {
      "$schema": "http://json-schema.org/draft-07/schema#",
      "title": "Test schema",
      "properties": {
        "name": {
          "description": "Name",
          "type": "string"
        },
        "age": {
          "description": "Age of the person",
          "type": "number",
          "minimum": 2,
          "maximum": 200
        }
      },
      "required": ["name", "age"],
      "type": "object"
    }
  )");

  int res = YOGI_ConfigurationValidate(cfg_, nullptr, scm);
  EXPECT_ERR(res, YOGI_ERR_CONFIGURATION_VALIDATION_FAILED);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "not found");

  res = YOGI_ConfigurationValidate(cfg_, "/person", scm);
  EXPECT_OK(res);

  res = YOGI_ConfigurationValidate(cfg_, "/someone", scm);
  EXPECT_ERR(res, YOGI_ERR_CONFIGURATION_SECTION_NOT_FOUND);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "someone");

  res = YOGI_ConfigurationUpdateFromJson(cfg_, R"({"person": {"age": 500}})");
  EXPECT_OK(res);

  res = YOGI_ConfigurationValidate(cfg_, "/person", scm);
  EXPECT_ERR(res, YOGI_ERR_CONFIGURATION_VALIDATION_FAILED);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "/age");
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "200");
}
