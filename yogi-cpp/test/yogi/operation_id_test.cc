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

class OperationIdTest : public testing::Test {
 protected:
  const yogi::OperationId oid_ = yogi::detail::make_operation_id(123);
};

TEST_F(OperationIdTest, DefaultConstructor) {
  yogi::OperationId oid;
  EXPECT_FALSE(oid.is_valid());
  EXPECT_EQ(oid.value(), 0);
}

TEST_F(OperationIdTest, Value) {
  EXPECT_EQ(oid_.value(), 123);
}

TEST_F(OperationIdTest, is_valid) {
  EXPECT_TRUE(oid_.is_valid());

  auto oid = yogi::detail::make_operation_id(0);
  EXPECT_FALSE(oid.is_valid());

  oid = yogi::detail::make_operation_id(-1);
  EXPECT_FALSE(oid.is_valid());
}

TEST_F(OperationIdTest, Copy) {
  yogi::OperationId oid;
  oid = oid_;
  EXPECT_EQ(oid.value(), 123);
  EXPECT_TRUE(oid.is_valid());
}

TEST_F(OperationIdTest, Operators) {
  auto fn = [](int val) { return yogi::detail::make_operation_id(val); };

  EXPECT_TRUE(fn(5) == fn(5));
  EXPECT_FALSE(fn(5) == fn(10));

  EXPECT_TRUE(fn(5) != fn(10));
  EXPECT_FALSE(fn(5) != fn(5));

  EXPECT_TRUE(fn(5) < fn(10));
  EXPECT_FALSE(fn(5) < fn(5));
  EXPECT_FALSE(fn(5) < fn(1));

  EXPECT_TRUE(fn(5) <= fn(10));
  EXPECT_TRUE(fn(5) <= fn(5));
  EXPECT_FALSE(fn(5) <= fn(1));

  EXPECT_FALSE(fn(5) > fn(10));
  EXPECT_FALSE(fn(5) > fn(5));
  EXPECT_TRUE(fn(5) > fn(1));

  EXPECT_FALSE(fn(5) >= fn(10));
  EXPECT_TRUE(fn(5) >= fn(5));
  EXPECT_TRUE(fn(5) >= fn(1));
}
