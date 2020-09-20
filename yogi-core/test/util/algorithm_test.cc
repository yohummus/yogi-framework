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

#include <src/util/algorithm.h>

#include <vector>

class AlgorithmTest : public ::testing::Test {
 protected:
  std::vector<int> vec_ = {1, 2, 3, 3, 4, 3, 5};
};

TEST_F(AlgorithmTest, Find) {
  EXPECT_EQ(find(vec_, 3), vec_.begin() + 2);
  EXPECT_EQ(find(vec_, 0), vec_.end());
}

TEST_F(AlgorithmTest, FindIf) {
  auto it = find_if(vec_, [](int val) { return val == 3; });
  EXPECT_EQ(it, vec_.begin() + 2);

  it = find_if(vec_, [](int) { return false; });
  EXPECT_EQ(it, vec_.end());
}

TEST_F(AlgorithmTest, Contains) {
  EXPECT_TRUE(contains(vec_, 3));
  EXPECT_FALSE(contains(vec_, 7));
}

TEST_F(AlgorithmTest, ContainsIf) {
  bool contains = contains_if(vec_, [](int val) { return val == 3; });
  EXPECT_TRUE(contains);

  contains = contains_if(vec_, [](int) { return false; });
  EXPECT_FALSE(contains);
}

TEST_F(AlgorithmTest, RemoveErase) {
  auto it = remove_erase(vec_, 3);
  EXPECT_EQ(vec_, (std::vector<int>{1, 2, 4, 5}));
  EXPECT_EQ(it, vec_.end());
}

TEST_F(AlgorithmTest, RemoveEraseIf) {
  auto it = remove_erase_if(vec_, [](int val) { return val > 3; });
  EXPECT_EQ(vec_, (std::vector<int>{1, 2, 3, 3, 3}));
  EXPECT_EQ(it, vec_.end());
}
