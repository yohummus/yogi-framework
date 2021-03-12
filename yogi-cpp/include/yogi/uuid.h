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

#ifndef _YOGI_UUID_H
#define _YOGI_UUID_H

//! \file
//!
//! Representation of Universal Unique IDentifiers.

#include "errors.h"
#include "string_view.h"

#include <cstring>

namespace yogi {
namespace detail {

inline char nibble_to_hex_char(unsigned char nibble) {
  char ch = static_cast<char>(nibble);
  ch      = static_cast<char>(ch + (nibble <= 9 ? '0' : ('a' - 10)));
  return ch;
}

}  // namespace detail

////////////////////////////////////////////////////////////////////////////////
/// Represents a Universal Unique IDentifier.
///
/// A UUID is a Universal Unique IDentifier that is used to unambiguously
/// identify certain Yogi objects like branches.
///
/// \note
///   This class is a POD.
////////////////////////////////////////////////////////////////////////////////
class Uuid {
 public:
  typedef unsigned char value_type;
  typedef value_type* iterator;
  typedef value_type const* const_iterator;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  // UUID example: 123e4567-e89b-12d3-a456-426655440000
  static Uuid parse(const StringView& str) {
    const char* s = str;

    if (strlen(s) != 36 || s[8] != '-' || s[13] != '-' || s[18] != '-' || s[23] != '-') {
      throw DetailedFailureException(ErrorCode::kInvalidParam, "Invalid UUID string format");
    }

    const char raw[32] = {
        // clang-format off
      s[ 0], s[ 1], s[ 2], s[ 3], s[ 4], s[ 5], s[ 6], s[ 7],
      s[ 9], s[10], s[11], s[12],
      s[14], s[15], s[16], s[17],
      s[19], s[20], s[21], s[22],
      s[24], s[25], s[26], s[27], s[28], s[29], s[30], s[31], s[32], s[33], s[34], s[35],
    };  // clang-format on

    Uuid uuid;
    for (int i = 0; i < 32; ++i) {
      char ch = raw[i];
      char nibble;

      if ('0' <= ch && ch <= '9') {
        nibble = ch - '0';
      } else if ('a' <= ch && ch <= 'f') {
        nibble = ch - 'a' + 10;
      } else if ('A' <= ch && ch <= 'F') {
        nibble = ch - 'F' + 10;
      } else {
        throw DetailedFailureException(ErrorCode::kInvalidParam, "Invalid hex character in UUID string");
      }

      if (i % 2 == 0) {
        uuid.data_[i / 2] = nibble << 4;
      } else {
        uuid.data_[i / 2] |= nibble;
      }
    }

    return uuid;
  }

  static constexpr size_type static_size() noexcept {
    return sizeof(Uuid);
  }

  constexpr std::size_t size() const {
    return static_size();
  }

  iterator begin() noexcept {
    return data_;
  }

  const_iterator begin() const noexcept {
    return data_;
  }

  iterator end() noexcept {
    return data_ + size();
  }

  const_iterator end() const noexcept {
    return data_ + size();
  }

  bool operator==(const Uuid& rhs) const noexcept {
    return memcmp(data_, rhs.data_, sizeof(data_)) == 0;
  }

  bool operator!=(const Uuid& rhs) const noexcept {
    return !(*this == rhs);
  }

  bool operator<(const Uuid& rhs) const noexcept {
    return memcmp(data_, rhs.data_, sizeof(data_)) < 0;
  }

  bool operator>(const Uuid& rhs) const noexcept {
    return rhs < *this;
  }

  bool operator<=(const Uuid& rhs) const noexcept {
    return !(rhs < *this);
  }

  bool operator>=(const Uuid& rhs) const noexcept {
    return !(rhs > *this);
  }

  void swap(Uuid& rhs) noexcept {
    Uuid tmp = *this;
    *this    = rhs;
    rhs      = tmp;
  }

  value_type* data() noexcept {
    return data_;
  }

  const value_type* data() const noexcept {
    return data_;
  }

  /// Returns a string in the format "6ba7b810-9dad-11d1-80b4-00c04fd430c8".
  ///
  /// \returns String representation of the UUID.
  std::string to_string() const {
    std::string s;
    s.reserve(36);

    for (std::size_t i = 0; i < size(); ++i) {
      s += detail::nibble_to_hex_char((data_[i] >> 4) & 0x0F);
      s += detail::nibble_to_hex_char(data_[i] & 0x0F);

      if (i == 3 || i == 5 || i == 7 || i == 9) {
        s += '-';
      }
    }

    return s;
  }

 private:
  value_type data_[16];
};

}  // namespace yogi

namespace std {

template <>
struct hash<yogi::Uuid> {
  size_t operator()(const yogi::Uuid& uuid) const {
    std::size_t seed = 0;
    for (yogi::Uuid::const_iterator it = uuid.begin(); it != uuid.end(); ++it) {
      seed ^= static_cast<std::size_t>(*it) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    return seed;
  }
};

}  // namespace std

#endif  // _YOGI_UUID_H
