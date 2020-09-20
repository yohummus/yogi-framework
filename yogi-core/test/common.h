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

#pragma once

#define YOGI_API  // To remove __declspec(dllimport) from API functions since we are linking them statically

#include <include/yogi_core.h>
#include <src/api/errors.h>

#include <gtest/gtest.h>
#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>

#include <initializer_list>
#include <string>
#include <string_view>

#define EXPECT_OK(res) EXPECT_ERR(res, YOGI_OK) << YOGI_GetLastErrorDetails()
#define ASSERT_OK(res) ASSERT_ERR(res, YOGI_OK) << YOGI_GetLastErrorDetails()
#define EXPECT_ERR(res, err) EXPECT_EQ(res, err) << YOGI_GetLastErrorDetails()
#define ASSERT_ERR(res, err) EXPECT_EQ(res, err) << YOGI_GetLastErrorDetails()
#define EXPECT_CONTAINS(str, substr) EXPECT_NE(std::string_view{str}.find(substr), std::string::npos) << str;

#define _EXPECT_THROW_ERROR(statement, ec, catch_statement)                                     \
  try {                                                                                         \
    statement;                                                                                  \
    FAIL() << "No exception thrown";                                                            \
  }                                                                                             \
  catch_statement catch (const std::exception& e) {                                             \
    FAIL() << "Wrong exception type thrown: " << typeid(e).name() << " with msg: " << e.what(); \
  }

#define EXPECT_THROW_ERROR(statement, ec) \
  _EXPECT_THROW_ERROR(statement, ec, catch (const Error& err) { EXPECT_EQ(err.value(), ec); })

#define EXPECT_THROW_DESCRIPTIVE_ERROR(statement, ec)                      \
  _EXPECT_THROW_ERROR(statement, ec, catch (const DescriptiveError& err) { \
    EXPECT_EQ(err.value(), ec);                                            \
    EXPECT_FALSE(err.details().empty());                                   \
  })

class TestFixture : public testing::Test {
 public:
  TestFixture();
  virtual ~TestFixture();
};

class TemporaryWorkdirGuard final {
 public:
  TemporaryWorkdirGuard();
  TemporaryWorkdirGuard(TemporaryWorkdirGuard&& other);
  ~TemporaryWorkdirGuard();

  TemporaryWorkdirGuard(const TemporaryWorkdirGuard&) = delete;
  TemporaryWorkdirGuard& operator=(const TemporaryWorkdirGuard&) = delete;

 private:
  boost::filesystem::path old_working_dir_;
  boost::filesystem::path temp_path_;
};

struct CommandLine final {
  CommandLine(std::initializer_list<std::string> args);
  ~CommandLine();

  CommandLine(const CommandLine&) = delete;
  CommandLine& operator=(const CommandLine&) = delete;

  int argc;
  char** argv;
};

std::string make_version_string(int major, int minor, int patch, const std::string& suffix = {});
void* create_context();
std::string read_file(const std::string& filename);
