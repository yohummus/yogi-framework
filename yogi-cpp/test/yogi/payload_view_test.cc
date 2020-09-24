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

class PayloadViewTest : public Test {
 protected:
  PayloadViewTest() : data_("Hello"), view_(data_, 123, yogi::Encoding::kMsgpack) {
  }

  const char* data_;
  const yogi::PayloadView view_;
};

TEST_F(PayloadViewTest, Getters) {
  EXPECT_EQ(view_.data(), data_);
  EXPECT_EQ(view_.size(), 123);
  EXPECT_EQ(view_.encoding(), yogi::Encoding::kMsgpack);
}

TEST_F(PayloadViewTest, DefaultConstruct) {
  yogi::PayloadView view;
  EXPECT_EQ(view.data(), nullptr);
  EXPECT_EQ(view.size(), 0);
  EXPECT_EQ(view.encoding(), yogi::Encoding::kJson);
}

TEST_F(PayloadViewTest, ConstructFromBuffer) {
  const char* data = "hello";
  int size         = static_cast<int>(std::strlen(data));
  auto view        = yogi::PayloadView(data, size, yogi::Encoding::kMsgpack);
  EXPECT_EQ(view.data(), data);
  EXPECT_EQ(view.size(), size);
  EXPECT_EQ(view.encoding(), yogi::Encoding::kMsgpack);
}

TEST_F(PayloadViewTest, ConstructFromVector) {
  std::vector<char> data = {'h', 'i'};
  int size               = static_cast<int>(data.size());
  auto view              = yogi::PayloadView(data, yogi::Encoding::kMsgpack);
  EXPECT_EQ(view.data(), data.data());
  EXPECT_EQ(view.size(), size);
  EXPECT_EQ(view.encoding(), yogi::Encoding::kMsgpack);
}

TEST_F(PayloadViewTest, ConstructFromJsonView) {
  nlohmann::json json = {12345};
  auto json_view      = yogi::JsonView(json);
  auto view           = yogi::PayloadView(json_view);

  auto s = json.dump();
  EXPECT_EQ(s, view.data());
  EXPECT_EQ(view.encoding(), yogi::Encoding::kJson);
}

TEST_F(PayloadViewTest, ConstructFromMsgpackView) {
  std::string msgpack = "Hello";
  auto msgpack_view   = yogi::MsgpackView(msgpack);
  auto view           = yogi::PayloadView(msgpack_view);

  EXPECT_EQ(msgpack, std::string(view.data(), static_cast<std::size_t>(view.size())));
  EXPECT_EQ(view.encoding(), yogi::Encoding::kMsgpack);
}

TEST_F(PayloadViewTest, ClassComparisonOperators) {
  const char* ab = "Hello";
  yogi::PayloadView a_view(ab, std::strlen(ab), yogi::Encoding::kJson);
  yogi::PayloadView b_view(ab, std::strlen(ab), yogi::Encoding::kMsgpack);

  std::string c = ab;
  yogi::PayloadView c_view(c.c_str(), c.size(), yogi::Encoding::kJson);

  std::string d = c + " you";
  yogi::PayloadView d_view(d.c_str(), d.size(), yogi::Encoding::kJson);

  std::string e = "olleH";
  yogi::PayloadView e_view(e.c_str(), e.size(), yogi::Encoding::kJson);

  EXPECT_TRUE(a_view == a_view);
  EXPECT_FALSE(a_view == b_view);
  EXPECT_TRUE(a_view == c_view);
  EXPECT_FALSE(a_view == d_view);
  EXPECT_FALSE(a_view == e_view);

  EXPECT_FALSE(a_view != a_view);
  EXPECT_TRUE(a_view != b_view);
  EXPECT_FALSE(a_view != c_view);
  EXPECT_TRUE(a_view != d_view);
  EXPECT_TRUE(a_view != e_view);
}

TEST_F(PayloadViewTest, FreeComparisonOperators) {
  const char* json = "json";
  yogi::JsonView json_view(json);
  yogi::PayloadView json_payload(json_view);

  EXPECT_TRUE(json_payload == json_view);
  EXPECT_TRUE(json_view == json_payload);
  EXPECT_FALSE(json_payload != json_view);
  EXPECT_FALSE(json_view != json_payload);

  const char* msgpack = "msgpack";
  yogi::MsgpackView msgpack_view(msgpack, std::strlen(msgpack));
  yogi::PayloadView msgpack_payload(msgpack_view);

  EXPECT_TRUE(msgpack_payload == msgpack_view);
  EXPECT_TRUE(msgpack_view == msgpack_payload);
  EXPECT_FALSE(msgpack_payload != msgpack_view);
  EXPECT_FALSE(msgpack_view != msgpack_payload);
}
