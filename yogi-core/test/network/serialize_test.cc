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

#include <src/network/serialize.h>

#include <boost/uuid/uuid_generators.hpp>

class SerializeTest : public TestFixture {
 protected:
  template <typename T>
  void check(T val) {
    Buffer buffer;
    serialize(&buffer, val);

    T des_val;
    auto it = buffer.cbegin();
    EXPECT_TRUE(deserialize(&des_val, buffer, &it));
    EXPECT_EQ(des_val, val);
    EXPECT_EQ(it, buffer.end());

    EXPECT_FALSE(deserialize(&des_val, buffer, &it));
  }
};

TEST_F(SerializeTest, bool) {
  check<bool>(true);
  check<bool>(false);
}

TEST_F(SerializeTest, unsigned_short) {
  check<unsigned short>(12345);
}

TEST_F(SerializeTest, int) {
  check<int>(-1234567890);
}

TEST_F(SerializeTest, size_t) {
  check<size_t>(1234567890);
}

TEST_F(SerializeTest, nanoseconds) {
  check(std::chrono::nanoseconds(1234567890));
}

TEST_F(SerializeTest, Timestamp) {
  check(Timestamp::now());
}

TEST_F(SerializeTest, string) {
  check<std::string>("Hello");
}

TEST_F(SerializeTest, uuid) {
  check(boost::uuids::random_generator()());
}
