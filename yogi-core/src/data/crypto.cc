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

#include <src/data/crypto.h>
#include <src/util/hex.h>

#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <limits>
#include <mutex>
#include <random>

namespace {

SmallBuffer make_sha256_impl(const void* data, std::size_t len) {
  SmallBuffer hash(SHA256_DIGEST_LENGTH);

  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, data, len);
  SHA256_Final(hash.data(), &sha256);

  return hash;
}

}  // anonymous namespace

SmallBuffer make_sha256(const Buffer& data) { return make_sha256_impl(data.data(), data.size()); }

SmallBuffer make_sha256(std::string_view data) { return make_sha256_impl(data.data(), data.size()); }

Buffer generate_random_bytes(std::size_t n) {
  Buffer bytes(n);

  static std::mutex mutex;  // For libcrypto's RAND_bytes() and error info
  std::lock_guard<std::mutex> lock(mutex);
  auto res = RAND_bytes(bytes.data(), static_cast<int>(bytes.size()));
  if (res != 1) {
    // char str[128];
    // ERR_error_string_n(ERR_get_error(), str, sizeof(str));
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<> dist;
    for (auto& byte : bytes) {
      byte = static_cast<Byte>(dist(gen) % 256);
    }
  }

  return bytes;
}
