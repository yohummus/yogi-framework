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

class StringViewTest : public Test {};

TEST_F(StringViewTest, Default) {
  EXPECT_EQ(static_cast<const char*>(StringView()), nullptr);
}

TEST_F(StringViewTest, ConstCharString) {
  const char* s = "Hello";
  EXPECT_EQ(static_cast<const char*>(StringView(s)), s);
}

TEST_F(StringViewTest, StdString) {
  std::string s = "Hello";
  EXPECT_EQ(static_cast<const char*>(StringView(s)), s.c_str());
}

TEST_F(StringViewTest, ComparisonOperators) {
  const char* a = "Hello";
  std::string b = a;
  std::string c = b + " you";
  std::string d = "olleH";

  EXPECT_TRUE(StringView(a) == StringView(a));
  EXPECT_TRUE(StringView(a) == StringView(b));
  EXPECT_FALSE(StringView(a) == StringView(c));
  EXPECT_FALSE(StringView(a) == StringView(d));

  EXPECT_FALSE(StringView(a) != StringView(a));
  EXPECT_FALSE(StringView(a) != StringView(b));
  EXPECT_TRUE(StringView(a) != StringView(c));
  EXPECT_TRUE(StringView(a) != StringView(d));
}
