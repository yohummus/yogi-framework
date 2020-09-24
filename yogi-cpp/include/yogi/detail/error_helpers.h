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

#ifndef _YOGI_DETAIL_ERROR_HELPERS_H
#define _YOGI_DETAIL_ERROR_HELPERS_H

//! \file
//!
//! Helpers for handling errors.

#include "../errors.h"

#include <array>

namespace yogi {
namespace detail {

inline void check_error_code(int res) {
  if (res >= 0) return;

  auto details = YOGI_GetLastErrorDetails();
  if (details[0] == '\0') {
    throw FailureException(static_cast<ErrorCode>(res));
  } else {
    throw DetailedFailureException(static_cast<ErrorCode>(res), details);
  }
}

inline bool false_if_specific_error_else_throw(int res, ErrorCode ec) {
  if (res == static_cast<int>(ec)) return false;
  check_error_code(res);
  return true;
}

template <typename Fn, typename... Args>
inline void with_error_code_to_result(int res, Fn fn, Args&&... args) {
  if (res < 0) {
    fn(yogi::Failure(static_cast<ErrorCode>(res)), std::forward<Args>(args)...);
  } else {
    fn(yogi::Success(res), std::forward<Args>(args)...);
  }
}

}  // namespace detail
}  // namespace yogi

#endif  // _YOGI_DETAIL_ERROR_HELPERS_H
