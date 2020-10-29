/*
 * This file is part of the Yogi distribution https://github.com/yohummus/yogi.
 * Copyright (c) 2019 Johannes Bergmann.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _YOGI_INTERNAL_QUERY_STRING_H
#define _YOGI_INTERNAL_QUERY_STRING_H

//! \file
//!
//! Helpers for obtaining strings from the Yogi Core library.

#include "../errors.h"
#include "error_helpers.h"

#include <string>

namespace yogi {
namespace detail {

template <typename Fn>
inline std::string query_string(Fn fn) {
  std::vector<char> v;
  int size = 64;

  int res;
  do {
    size *= 2;
    v = std::vector<char>{};
    v.resize(static_cast<std::size_t>(size));

    res = fn(v.data(), size);
  } while (res == static_cast<int>(ErrorCode::kBufferTooSmall));

  detail::check_error_code(res);

  return std::string(v.data());
}

}  // namespace detail
}  // namespace yogi

#endif  // _YOGI_INTERNAL_QUERY_STRING_H
