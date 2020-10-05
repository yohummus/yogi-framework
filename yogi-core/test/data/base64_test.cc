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

#include <src/data/base64.h>

#include <vector>

class Base64Test : public TestFixture {
 protected:
  std::string plain_   = "hello";
  std::string encoded_ = "aGVsbG8=";
};

TEST_F(Base64Test, Encode) {
  auto encoded = encode_base64(plain_);
  EXPECT_EQ(encoded, encoded_);
}

TEST_F(Base64Test, Decode) {
  auto plain = decode_base64(encoded_);
  EXPECT_EQ(plain, plain_);
}
