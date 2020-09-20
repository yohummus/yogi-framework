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

#include <src/api/version.h>

#include <sstream>
#include <type_traits>

TEST(YogiCoreTest, HeaderVersion) {
  static_assert(std::is_same<decltype(YOGI_HDR_VERSION_MAJOR), int>::value);
  EXPECT_GE(YOGI_HDR_VERSION_MAJOR, 0);

  static_assert(std::is_same<decltype(YOGI_HDR_VERSION_MINOR), int>::value);
  EXPECT_GE(YOGI_HDR_VERSION_MINOR, 0);

  static_assert(std::is_same<decltype(YOGI_HDR_VERSION_PATCH), int>::value);
  EXPECT_GE(YOGI_HDR_VERSION_PATCH, 0);

  static_assert(std::is_convertible<decltype(YOGI_HDR_VERSION), const char*>::value);
  static_assert(std::is_convertible<decltype(YOGI_HDR_VERSION_SUFFIX), const char*>::value);

  std::stringstream ss;
  ss << YOGI_HDR_VERSION_MAJOR << '.' << YOGI_HDR_VERSION_MINOR << '.' << YOGI_HDR_VERSION_PATCH
     << YOGI_HDR_VERSION_SUFFIX;
  auto version = ss.str();
  EXPECT_EQ(version, YOGI_HDR_VERSION);
}

TEST(VersionTest, GetVersion) { EXPECT_STREQ(YOGI_HDR_VERSION, YOGI_GetVersion()); }

TEST(VersionTest, CheckBindingsCompatibility) {
  EXPECT_OK(YOGI_CheckBindingsCompatibility(YOGI_HDR_VERSION));

  EXPECT_EQ(YOGI_CheckBindingsCompatibility(YOGI_HDR_VERSION " "), YOGI_ERR_INVALID_PARAM);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "not have a valid format");

  auto version = make_version_string(YOGI_HDR_VERSION_MAJOR, YOGI_HDR_VERSION_MINOR + 1, YOGI_HDR_VERSION_PATCH + 1);
  EXPECT_OK(YOGI_CheckBindingsCompatibility(version.c_str()));

  version = make_version_string(YOGI_HDR_VERSION_MAJOR + 1, YOGI_HDR_VERSION_MINOR, YOGI_HDR_VERSION_PATCH);
  EXPECT_EQ(YOGI_CheckBindingsCompatibility(version.c_str()), YOGI_ERR_INCOMPATIBLE_VERSION);
  EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "is incompatible with");

  if (YOGI_HDR_VERSION_MINOR > 0) {
    version = make_version_string(YOGI_HDR_VERSION_MAJOR, YOGI_HDR_VERSION_MINOR - 1, YOGI_HDR_VERSION_PATCH);
    EXPECT_EQ(YOGI_CheckBindingsCompatibility(version.c_str()), YOGI_ERR_INCOMPATIBLE_VERSION);
    EXPECT_CONTAINS(YOGI_GetLastErrorDetails(), "is incompatible with");
  }
}
