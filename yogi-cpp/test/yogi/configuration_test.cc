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

class ConfigurationTest : public Test {};

TEST_F(ConfigurationTest, Create) {
  MOCK_ConfigurationCreate([](void** config, int flags) {
    EXPECT_NE(config, nullptr);
    EXPECT_EQ(flags, YOGI_CFG_NONE);
    return YOGI_OK;
  });

  EXPECT_TRUE(!!Configuration::create());

  MOCK_ConfigurationCreate([](void**, int flags) {
    EXPECT_EQ(flags, YOGI_CFG_MUTABLE_CMD_LINE);
    return YOGI_OK;
  });

  EXPECT_TRUE(!!Configuration::create(yogi::ConfigurationFlags::kMutableCmdLine));
}

TEST_F(ConfigurationTest, CreateError) {
  MOCK_ConfigurationCreate([](void**, int flags) {
    EXPECT_EQ(flags, YOGI_CFG_MUTABLE_CMD_LINE);
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(Configuration::create(ConfigurationFlags::kMutableCmdLine), FailureException);
}

TEST_F(ConfigurationTest, CreateFromJson) {
  MOCK_ConfigurationCreate([](void** config, int flags) {
    EXPECT_NE(config, nullptr);
    EXPECT_EQ(flags, YOGI_CFG_NONE);
    *config = kPointer;
    return YOGI_OK;
  });

  MOCK_ConfigurationUpdateFromJson([](void* config, const char* json) {
    EXPECT_EQ(config, kPointer);
    EXPECT_STREQ(json, "foo");
    return YOGI_OK;
  });

  EXPECT_TRUE(!!yogi::Configuration::create("foo"));
}

TEST_F(ConfigurationTest, Flags) {
  auto cfg = create_configuration(ConfigurationFlags::kMutableCmdLine);
  EXPECT_EQ(cfg->flags(), ConfigurationFlags::kMutableCmdLine);
}

TEST_F(ConfigurationTest, UpdateFromCommandLine) {
  auto cfg = create_configuration();

  MOCK_ConfigurationUpdateFromCommandLine([](void* config, int argc, const char* const* argv, int options) {
    EXPECT_EQ(config, kPointer);
    EXPECT_EQ(argc, 5);
    EXPECT_NE(argv, nullptr);
    EXPECT_EQ(options, YOGI_CLO_BRANCH_GHOST_MODE);
    return YOGI_OK;
  });

  const char* const argv[] = {"foo"};
  cfg->update_from_command_line(5, argv, CommandLineOptions::kBranchGhostMode);
}

TEST_F(ConfigurationTest, UpdateFromCommandLineError) {
  auto cfg = create_configuration();
  MOCK_ConfigurationUpdateFromCommandLine([](void*, int, const char* const*, int) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(cfg->update_from_command_line(5, nullptr), FailureException);
}

TEST_F(ConfigurationTest, UpdateFromJson) {
  auto cfg = create_configuration();

  MOCK_ConfigurationUpdateFromJson([](void* config, const char* json) {
    EXPECT_EQ(config, kPointer);
    EXPECT_STREQ(json, "foo");
    return YOGI_OK;
  });

  cfg->update_from_json("foo");
}

TEST_F(ConfigurationTest, UpdateFromJsonError) {
  auto cfg = create_configuration();
  MOCK_ConfigurationUpdateFromJson([](void*, const char*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(cfg->update_from_json("foo"), FailureException);
}

TEST_F(ConfigurationTest, UpdateFromFile) {
  auto cfg = create_configuration();

  MOCK_ConfigurationUpdateFromFile([](void* config, const char* filename) {
    EXPECT_EQ(config, kPointer);
    EXPECT_STREQ(filename, "foo");
    return YOGI_OK;
  });

  cfg->update_from_file("foo");
}

TEST_F(ConfigurationTest, UpdateFromFileError) {
  auto cfg = create_configuration();
  MOCK_ConfigurationUpdateFromFile([](void*, const char*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(cfg->update_from_file("foo"), FailureException);
}

TEST_F(ConfigurationTest, Dump) {
  auto cfg = create_configuration();

  MOCK_ConfigurationDump([](void* config, const char** json, int* jsonsize, int resvars, int indent) {
    EXPECT_EQ(config, kPointer);
    EXPECT_NE(nullptr, json);
    EXPECT_NE(nullptr, jsonsize);
    EXPECT_EQ(resvars, YOGI_TRUE);
    EXPECT_EQ(indent, -1);
    *json     = "foo";
    *jsonsize = static_cast<int>(strlen(*json) + 1);
    return YOGI_OK;
  });
  EXPECT_EQ(cfg->dump(), "foo");

  MOCK_ConfigurationDump([](void*, const char** json, int* jsonsize, int resvars, int indent) {
    EXPECT_EQ(resvars, YOGI_FALSE);
    EXPECT_EQ(indent, 5);
    *json     = "foo";
    *jsonsize = static_cast<int>(strlen(*json) + 1);
    return YOGI_OK;
  });
  EXPECT_EQ(cfg->dump(false, 5), "foo");

  MOCK_ConfigurationDump([](void*, const char** json, int* jsonsize, int resvars, int indent) {
    EXPECT_EQ(resvars, YOGI_FALSE);
    EXPECT_EQ(indent, -1);
    *json     = "foo";
    *jsonsize = static_cast<int>(strlen(*json) + 1);
    return YOGI_OK;
  });
  EXPECT_EQ(cfg->dump(false), "foo");

  MOCK_ConfigurationDump([](void*, const char** json, int* jsonsize, int resvars, int indent) {
    EXPECT_EQ(resvars, YOGI_TRUE);
    EXPECT_EQ(indent, 5);
    *json     = "foo";
    *jsonsize = static_cast<int>(strlen(*json) + 1);
    return YOGI_OK;
  });
  EXPECT_EQ(cfg->dump(5), "foo");

  cfg = create_configuration(ConfigurationFlags::kDisableVariables);
  MOCK_ConfigurationDump([](void*, const char** json, int* jsonsize, int resvars, int) {
    EXPECT_EQ(resvars, YOGI_FALSE);
    *json     = "foo";
    *jsonsize = static_cast<int>(strlen(*json) + 1);
    return YOGI_OK;
  });
  EXPECT_EQ(cfg->dump(), "foo");
}

TEST_F(ConfigurationTest, DumpError) {
  auto cfg = create_configuration();

  MOCK_ConfigurationDump([](void*, const char**, int*, int, int) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(cfg->dump(), FailureException);

  MOCK_ConfigurationDump([](void*, const char**, int*, int, int) { return YOGI_OK; });
  EXPECT_THROW(cfg->dump(-1), FailureException);
}

TEST_F(ConfigurationTest, WriteToFile) {
  auto cfg = create_configuration();

  MOCK_ConfigurationWriteToFile([](void* config, const char* filename, int resvars, int indent) {
    EXPECT_EQ(config, kPointer);
    EXPECT_STREQ(filename, "foo");
    EXPECT_EQ(resvars, YOGI_TRUE);
    EXPECT_EQ(indent, -1);
    return YOGI_OK;
  });
  cfg->write_to_file("foo");

  MOCK_ConfigurationWriteToFile([](void*, const char*, int resvars, int indent) {
    EXPECT_EQ(resvars, YOGI_FALSE);
    EXPECT_EQ(indent, 5);
    return YOGI_OK;
  });
  cfg->write_to_file("foo", false, 5);

  MOCK_ConfigurationWriteToFile([](void*, const char*, int resvars, int indent) {
    EXPECT_EQ(resvars, YOGI_FALSE);
    EXPECT_EQ(indent, -1);
    return YOGI_OK;
  });
  cfg->write_to_file("foo", false);

  MOCK_ConfigurationWriteToFile([](void*, const char*, int resvars, int indent) {
    EXPECT_EQ(resvars, YOGI_TRUE);
    EXPECT_EQ(indent, 5);
    return YOGI_OK;
  });
  cfg->write_to_file("foo", 5);

  cfg = create_configuration(ConfigurationFlags::kDisableVariables);
  MOCK_ConfigurationWriteToFile([](void*, const char*, int resvars, int) {
    EXPECT_EQ(resvars, YOGI_FALSE);
    return YOGI_OK;
  });
  cfg->write_to_file("foo");
}

TEST_F(ConfigurationTest, WriteToFileError) {
  auto cfg = create_configuration();

  MOCK_ConfigurationWriteToFile([](void*, const char*, int, int) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(cfg->write_to_file("foo"), FailureException);

  MOCK_ConfigurationWriteToFile([](void*, const char*, int, int) { return YOGI_OK; });
  EXPECT_THROW(cfg->write_to_file("foo", -1), FailureException);
}

TEST_F(ConfigurationTest, Validate) {
  MOCK_ConfigurationCreate([](void** cfg, int) {
    *cfg = reinterpret_cast<void*>(0x1234);
    return YOGI_OK;
  });
  auto cfg = Configuration::create();

  MOCK_ConfigurationCreate([](void** cfg, int) {
    *cfg = reinterpret_cast<void*>(0x5678);
    return YOGI_OK;
  });
  auto scm = Configuration::create();

  MOCK_ConfigurationValidate([](void* config, const char* section, void* schema) {
    EXPECT_EQ(config, reinterpret_cast<void*>(0x1234));
    EXPECT_STREQ(section, "foo");
    EXPECT_EQ(schema, reinterpret_cast<void*>(0x5678));
    return YOGI_OK;
  });
  cfg->validate("foo", *scm);

  MOCK_ConfigurationValidate([](void*, const char* section, void*) {
    EXPECT_EQ(section, nullptr);
    return YOGI_OK;
  });
  cfg->validate(*scm);
}

TEST_F(ConfigurationTest, ValidateError) {
  auto cfg = create_configuration();
  auto scm = create_configuration();

  MOCK_ConfigurationValidate([](void*, const char*, void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(cfg->validate(*scm), FailureException);
}
