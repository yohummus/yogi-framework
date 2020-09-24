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

#include <nlohmann/json.hpp>

#include <test/common.h>

TEST(JsonViewTest, Default) {
  auto view = yogi::JsonView();
  EXPECT_EQ(view.data(), nullptr);
  EXPECT_EQ(view.size(), 0);
}

TEST(JsonViewTest, ConstCharString) {
  const char* s = "Hello";
  auto view     = yogi::JsonView(s);
  EXPECT_EQ(view.data(), s);
  EXPECT_EQ(view.size(), static_cast<int>(strlen(s)) + 1);
}

TEST(JsonViewTest, Vector) {
  std::vector<char> data = {'h', 'i'};
  int size               = static_cast<int>(data.size());
  auto view              = yogi::JsonView(data);
  EXPECT_EQ(view.data(), data.data());
  EXPECT_EQ(view.size(), size);
}

TEST(JsonViewTest, StdString) {
  std::string s = "Hello";
  auto view     = yogi::JsonView(s);
  EXPECT_EQ(std::string(view.data()), s);
  EXPECT_EQ(view.size(), static_cast<int>(s.size()) + 1);
}

TEST(JsonViewTest, JsonObject) {
  nlohmann::json json = {12345};
  auto view           = yogi::JsonView(json);
  EXPECT_EQ(json.dump(), view.data());
  EXPECT_EQ(view.size(), static_cast<int>(json.dump().size()) + 1);
}

TEST(JsonViewTest, ConversionOperator) {
  const char* s = "Hello";
  auto view     = yogi::JsonView(s);
  EXPECT_EQ(static_cast<const char*>(view), view.data());

  nlohmann::json json = {12345};
  auto view2          = yogi::JsonView(json);
  EXPECT_EQ(static_cast<const char*>(view), view.data());
}

TEST(JsonViewTest, ComparisonOperators) {
  const char* a = "Hello";
  std::string b = a;
  std::string c = b + " you";
  std::string d = "olleH";

  EXPECT_TRUE(yogi::JsonView(a) == yogi::JsonView(a));
  EXPECT_TRUE(yogi::JsonView(a) == yogi::JsonView(b));
  EXPECT_FALSE(yogi::JsonView(a) == yogi::JsonView(c));
  EXPECT_FALSE(yogi::JsonView(a) == yogi::JsonView(d));

  EXPECT_FALSE(yogi::JsonView(a) != yogi::JsonView(a));
  EXPECT_FALSE(yogi::JsonView(a) != yogi::JsonView(b));
  EXPECT_TRUE(yogi::JsonView(a) != yogi::JsonView(c));
  EXPECT_TRUE(yogi::JsonView(a) != yogi::JsonView(d));
}
