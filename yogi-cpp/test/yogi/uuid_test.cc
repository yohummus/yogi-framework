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

#include <type_traits>
#include <vector>

yogi::Uuid make_uuid(std::vector<yogi::Uuid::value_type> v) {
  yogi::Uuid uuid = {};
  for (std::size_t i = 0; i < v.size(); ++i) {
    uuid.data()[16 - v.size() + i] = v[i];
  }
  return uuid;
}

TEST(UuidTest, Pod) {
  EXPECT_TRUE(std::is_pod<yogi::Uuid>::value);
  EXPECT_EQ(sizeof(yogi::Uuid), 16u);
}

TEST(UuidTest, Size) {
  EXPECT_EQ(sizeof(yogi::Uuid), 16u);
  EXPECT_EQ(yogi::Uuid::static_size(), 16u);
  EXPECT_EQ(yogi::Uuid().size(), 16u);
}

TEST(UuidTest, Iterators) {
  auto uuid = make_uuid({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
  for (int i = 0; i < 16; ++i) {
    EXPECT_EQ(*(uuid.begin() + i), i);
  }
  EXPECT_EQ(uuid.begin() + 16, uuid.end());
}

TEST(UuidTest, Operators) {
  auto uuid_a = make_uuid({1});
  auto uuid_b = make_uuid({2});

  EXPECT_TRUE(uuid_a == make_uuid({1}));
  EXPECT_FALSE(uuid_b == make_uuid({1}));

  EXPECT_FALSE(uuid_a != make_uuid({1}));
  EXPECT_TRUE(uuid_b != make_uuid({1}));

  EXPECT_TRUE(uuid_a < uuid_b);
  EXPECT_FALSE(uuid_b < uuid_a);

  EXPECT_FALSE(uuid_a > uuid_b);
  EXPECT_TRUE(uuid_b > uuid_a);

  EXPECT_TRUE(uuid_a <= uuid_a);
  EXPECT_TRUE(uuid_a <= uuid_b);
  EXPECT_FALSE(uuid_b <= uuid_a);

  EXPECT_TRUE(uuid_a >= uuid_a);
  EXPECT_FALSE(uuid_a >= uuid_b);
  EXPECT_TRUE(uuid_b >= uuid_a);
}

TEST(UuidTest, Swap) {
  auto uuid_a = make_uuid({1});
  auto uuid_b = make_uuid({2});

  std::swap(uuid_a, uuid_b);

  EXPECT_EQ(uuid_a, make_uuid({2}));
  EXPECT_EQ(uuid_b, make_uuid({1}));
}

TEST(UuidTest, Data) {
  auto uuid = make_uuid({1});

  EXPECT_EQ(uuid.data()[15], 1);
  uuid.data()[15] = 55;
  EXPECT_EQ(uuid.data()[15], 55);
}

TEST(UuidTest, ToString) {
  auto uuid =
      make_uuid({0x6b, 0xa7, 0xb8, 0x10, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8});
  auto s = uuid.to_string();
  EXPECT_EQ(s, "6ba7b810-9dad-11d1-80b4-00c04fd430c8");
}
