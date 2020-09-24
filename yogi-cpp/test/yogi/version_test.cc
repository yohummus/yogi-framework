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

#include <yogi_core.h>

#include <test/common.h>

class VersionTest : public Test {};

TEST_F(VersionTest, Macros) {
  EXPECT_STREQ(YOGI_BINDINGS_VERSION, YOGI_HDR_VERSION);
  EXPECT_EQ(YOGI_BINDINGS_VERSION_MAJOR, YOGI_HDR_VERSION_MAJOR);
  EXPECT_EQ(YOGI_BINDINGS_VERSION_MINOR, YOGI_HDR_VERSION_MINOR);
  EXPECT_EQ(YOGI_BINDINGS_VERSION_PATCH, YOGI_HDR_VERSION_PATCH);
  EXPECT_STREQ(YOGI_BINDINGS_VERSION_SUFFIX, YOGI_HDR_VERSION_SUFFIX);
}

TEST_F(VersionTest, BindingsInfo) {
  EXPECT_EQ(bindings_info::kVersion, YOGI_BINDINGS_VERSION);
  EXPECT_EQ(bindings_info::kVersionMajor, YOGI_BINDINGS_VERSION_MAJOR);
  EXPECT_EQ(bindings_info::kVersionMinor, YOGI_BINDINGS_VERSION_MINOR);
  EXPECT_EQ(bindings_info::kVersionPatch, YOGI_BINDINGS_VERSION_PATCH);
  EXPECT_EQ(bindings_info::kVersionSuffix, YOGI_BINDINGS_VERSION_SUFFIX);
}

TEST_F(VersionTest, CoreInfo) {
  EXPECT_EQ(core_info::kVersion, YOGI_HDR_VERSION);
  EXPECT_EQ(core_info::kVersionMajor, YOGI_HDR_VERSION_MAJOR);
  EXPECT_EQ(core_info::kVersionMinor, YOGI_HDR_VERSION_MINOR);
  EXPECT_EQ(core_info::kVersionPatch, YOGI_HDR_VERSION_PATCH);
  EXPECT_EQ(core_info::kVersionSuffix, YOGI_HDR_VERSION_SUFFIX);
}
