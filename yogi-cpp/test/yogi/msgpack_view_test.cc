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

TEST(MsgpackViewTest, Default) {
  auto view = yogi::MsgpackView();
  EXPECT_EQ(view.data(), nullptr);
  EXPECT_EQ(view.size(), 0);
}

TEST(MsgpackViewTest, Buffer) {
  const char* data = "Hello";
  int size         = static_cast<int>(strlen(data));
  auto view        = yogi::MsgpackView(data, size);
  EXPECT_EQ(view.data(), data);
  EXPECT_EQ(view.size(), size);

  auto view2 = yogi::MsgpackView(data, static_cast<std::size_t>(size));
  EXPECT_EQ(view2.size(), size);
}

TEST(MsgpackViewTest, Vector) {
  std::vector<char> data = {'h', 'i'};
  int size               = static_cast<int>(data.size());
  auto view              = yogi::MsgpackView(data);
  EXPECT_EQ(view.data(), data.data());
  EXPECT_EQ(view.size(), size);
}

TEST(MsgpackViewTest, StdString) {
  std::string s = "Hello";
  auto view     = yogi::MsgpackView(s);
  EXPECT_EQ(std::string(view.data()), s);
  EXPECT_EQ(view.size(), static_cast<int>(s.size()));
}

TEST(MsgpackViewTest, StringStream) {
  std::stringstream ss;
  ss << "Hello";
  auto view = yogi::MsgpackView(ss);
  EXPECT_EQ(ss.str(), view.data());
  EXPECT_EQ(view.size(), static_cast<int>(ss.str().size()));
}

TEST(MsgpackViewTest, ComparisonOperators) {
  const char* a = "Hello";
  std::string b = a;
  std::string c = b + " you";
  std::string d = "olleH";

  EXPECT_TRUE(yogi::MsgpackView(a) == yogi::MsgpackView(a));
  EXPECT_TRUE(yogi::MsgpackView(a) == yogi::MsgpackView(b));
  EXPECT_FALSE(yogi::MsgpackView(a) == yogi::MsgpackView(c));
  EXPECT_FALSE(yogi::MsgpackView(a) == yogi::MsgpackView(d));

  EXPECT_FALSE(yogi::MsgpackView(a) != yogi::MsgpackView(a));
  EXPECT_FALSE(yogi::MsgpackView(a) != yogi::MsgpackView(b));
  EXPECT_TRUE(yogi::MsgpackView(a) != yogi::MsgpackView(c));
  EXPECT_TRUE(yogi::MsgpackView(a) != yogi::MsgpackView(d));
}
