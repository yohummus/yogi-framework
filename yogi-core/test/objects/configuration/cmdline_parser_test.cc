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

#include <src/objects/configuration/cmdline_parser.h>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class CmdlineParserTest : public TestFixture {
 protected:
  nlohmann::json check_parsing_succeeds(const CommandLine& cmdline, int clo_flags, const char* section_name) {
    CmdlineParser parser(cmdline.argc, cmdline.argv, clo_flags);
    EXPECT_NO_THROW(parser.parse());

    auto section = parser.get_direct_configuration()[section_name];
    EXPECT_FALSE(section.empty());

    return section;
  }

  void check_parsing_fails_with_no_options(const CommandLine& cmdline) {
    CmdlineParser parser(cmdline.argc, cmdline.argv, YOGI_CLO_NONE);
    EXPECT_THROW_DESCRIPTIVE_ERROR(parser.parse(), YOGI_ERR_PARSING_CMDLINE_FAILED);
  }

  template <typename T>
  void check_parsing_single_value(const char* cmdline_option, T val, int clo_flags, const char* section_name,
                                  const char* key) {
    CommandLine cmdline{
        cmdline_option,
        std::to_string(val),
    };

    auto section = check_parsing_succeeds(cmdline, clo_flags, section_name);
    ASSERT_TRUE(!!section.count(key));
    EXPECT_FLOAT_EQ(static_cast<float>(section[key].get<T>()), static_cast<float>(val));

    check_parsing_fails_with_no_options(cmdline);
  }

  void check_parsing_single_value(const char* cmdline_option, const char* val, int clo_flags, const char* section_name,
                                  const char* key) {
    CommandLine cmdline{
        cmdline_option,
        val,
    };

    auto section = check_parsing_succeeds(cmdline, clo_flags, section_name);
    ASSERT_TRUE(!!section.count(key));
    EXPECT_EQ(section[key].get<std::string>(), val);

    check_parsing_fails_with_no_options(cmdline);
  }
};

TEST_F(CmdlineParserTest, HelpOption) {
  // clang-format off
  CommandLine cmdline{
    "--help",
  };
  // clang-format on

  CmdlineParser parser(cmdline.argc, cmdline.argv, YOGI_CLO_NONE);

  EXPECT_THROW_DESCRIPTIVE_ERROR(parser.parse(), YOGI_ERR_HELP_REQUESTED);
}

TEST_F(CmdlineParserTest, HelpLoggingOption) {
  // clang-format off
  CommandLine cmdline{
    "--help-logging",
  };
  // clang-format on

  CmdlineParser parser(cmdline.argc, cmdline.argv, YOGI_CLO_LOGGING);

  EXPECT_THROW_DESCRIPTIVE_ERROR(parser.parse(), YOGI_ERR_HELP_REQUESTED);

  check_parsing_fails_with_no_options(cmdline);
}

TEST_F(CmdlineParserTest, LoggingOptions) {
  // clang-format off
  CommandLine cmdline{
    "--log-file", "/tmp/logfile.txt",
    "--log-console=STDOUT",
    "--log-color",
    "--log-fmt", "some entry format",
    "--log-time-fmt", "some time format",
    "--log-verbosity", "Yogi.*=WARNING",
    "--log-verbosity=App=DEBUG",
  };
  // clang-format on

  auto section = check_parsing_succeeds(cmdline, YOGI_CLO_LOGGING, "logging");

  EXPECT_EQ(section.value("file", "NOT FOUND"), "/tmp/logfile.txt");
  EXPECT_EQ(section.value("console", "NOT FOUND"), "STDOUT");
  EXPECT_EQ(section.value("color", false), true);
  EXPECT_EQ(section.value("entry-format", "NOT FOUND"), "some entry format");
  EXPECT_EQ(section.value("time-format", "NOT FOUND"), "some time format");

  auto verbosities = section["verbosity"];
  EXPECT_FALSE(verbosities.empty());

  EXPECT_EQ(verbosities.value("Yogi.*", "NOT FOUND"), "WARNING");
  EXPECT_EQ(verbosities.value("App", "NOT FOUND"), "DEBUG");

  check_parsing_fails_with_no_options(cmdline);
}

TEST_F(CmdlineParserTest, BranchNameOption) {
  check_parsing_single_value("--name", "My Branch", YOGI_CLO_BRANCH_NAME, "branch", "name");
}

TEST_F(CmdlineParserTest, BranchDescriptionOption) {
  check_parsing_single_value("--description", "Some text...", YOGI_CLO_BRANCH_DESCRIPTION, "branch", "description");
}

TEST_F(CmdlineParserTest, BranchNetworkOption) {
  check_parsing_single_value("--network", "General", YOGI_CLO_BRANCH_NETWORK, "branch", "network");
}

TEST_F(CmdlineParserTest, BranchPasswordOption) {
  check_parsing_single_value("--password", "Secret", YOGI_CLO_BRANCH_PASSWORD, "branch", "password");
}

TEST_F(CmdlineParserTest, BranchPathOption) {
  check_parsing_single_value("--path", "/some/path", YOGI_CLO_BRANCH_PATH, "branch", "path");
}

TEST_F(CmdlineParserTest, BranchAdvertisingInterfacesOption) {
  // clang-format off
  CommandLine cmdline{
    "--adv-ifs", "localhost",
    "--adv-ifs", "wlan0",
  };
  // clang-format on

  auto section = check_parsing_succeeds(cmdline, YOGI_CLO_BRANCH_ADV_IFS, "branch")["advertising_interfaces"];

  EXPECT_EQ(section[0].get<std::string>(), "localhost");
  EXPECT_EQ(section[1].get<std::string>(), "wlan0");
}

TEST_F(CmdlineParserTest, BranchAdvertisingAddressOption) {
  check_parsing_single_value("--adv-addr", "0::0", YOGI_CLO_BRANCH_ADV_ADDR, "branch", "advertising_address");
}

TEST_F(CmdlineParserTest, BranchAdvertisingPortOption) {
  check_parsing_single_value("--adv-port", 12345, YOGI_CLO_BRANCH_ADV_PORT, "branch", "advertising_port");
}

TEST_F(CmdlineParserTest, BranchAdvertisingIntervalOption) {
  check_parsing_single_value("--adv-int", 3.5, YOGI_CLO_BRANCH_ADV_INT, "branch", "advertising_interval");
}

TEST_F(CmdlineParserTest, BranchTimeoutOption) {
  check_parsing_single_value("--timeout", 6.6, YOGI_CLO_BRANCH_TIMEOUT, "branch", "timeout");
}

TEST_F(CmdlineParserTest, BranchGhostModeOption) {
  check_parsing_single_value("--ghost", true, YOGI_CLO_BRANCH_GHOST_MODE, "branch", "ghost_mode");
  check_parsing_single_value("--ghost", false, YOGI_CLO_BRANCH_GHOST_MODE, "branch", "ghost_mode");
}

TEST_F(CmdlineParserTest, FileOption) {
  TemporaryWorkdirGuard workdir;
  {
    fs::ofstream file("a.json");
    file << "{\"person\": {\"name\": \"Joe\", \"age\": 42}}";
  }
  {
    fs::ofstream file("bcde.json");
    file << "{\"person\": {\"age\": 88}}";
  }

  // clang-format off
  CommandLine cmdline{
    "a.json",
    "bc*.json",
  };
  // clang-format on

  CmdlineParser parser(cmdline.argc, cmdline.argv, YOGI_CLO_FILES);
  EXPECT_NO_THROW(parser.parse());

  auto section = parser.get_files_configuration()["person"];
  EXPECT_FALSE(section.empty());

  EXPECT_EQ(section.value("name", "NOT FOUND"), "Joe");
  EXPECT_EQ(section.value("age", -1), 88);

  check_parsing_fails_with_no_options(cmdline);
}

TEST_F(CmdlineParserTest, FileOptionCorruptFile) {
  TemporaryWorkdirGuard workdir;
  {
    fs::ofstream file("bad.json");
    file << "{\"person\": {\"age\": 44}";
  }

  // clang-format off
  CommandLine cmdline{
    "bad.json",
  };
  // clang-format on

  CmdlineParser parser(cmdline.argc, cmdline.argv, YOGI_CLO_FILES);
  EXPECT_THROW_DESCRIPTIVE_ERROR(parser.parse(), YOGI_ERR_PARSING_FILE_FAILED);
}

TEST_F(CmdlineParserTest, OverrideOption) {
  // clang-format off
  CommandLine cmdline{
    "--override", "{\"person\":{\"age\":42}}",
    "--o={\"person\":{\"name\":\"Joe\"}}",
    "-o", "/person/name=Marc",
  };
  // clang-format on

  auto section = check_parsing_succeeds(cmdline, YOGI_CLO_OVERRIDES, "person");

  EXPECT_EQ(section.value("age", -1), 42);
  EXPECT_EQ(section.value("name", "NOT FOUND"), "Marc");

  check_parsing_fails_with_no_options(cmdline);
}

TEST_F(CmdlineParserTest, VariableOptions) {
  // clang-format off
  CommandLine cmdline{
    "--var", "DIR=\"/usr/local\"",
    "--var=NAME=Yohummus",
    "--v=NUM=55",
    "--v", "PI=3.14",
  };
  // clang-format on

  auto section = check_parsing_succeeds(cmdline, YOGI_CLO_VARIABLES, "variables");

  EXPECT_EQ(section.value("DIR", "NOT FOUND"), "/usr/local");
  EXPECT_EQ(section.value("NAME", "NOT FOUND"), "Yohummus");
  EXPECT_EQ(section.value("NUM", -1), 55);
  EXPECT_FLOAT_EQ(section.value("PI", -1.0f), 3.14f);

  check_parsing_fails_with_no_options(cmdline);
}
