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

#ifndef _YOGI_IO_H
#define _YOGI_IO_H

//! \file
//!
//! Functions for printing various Yogi types.

#include "detail/sfinae.h"

#include <string>

namespace yogi {

_YOGI_DEFINE_SFINAE_METHOD_TESTER(HasToStringMethod, .to_string() == std::string())

/// \addtogroup freefn
/// @{

/// Converts a given Yogi enum value or object to a string.
///
/// \tparam T Type of the enum or object.
///
/// \param printable The object to convert to a string.
///
/// \returns Human-readable string name or description of the object.
template <typename T>
inline std::string to_string(T printable) {
  static_assert(internal::HasToStringMethod<T>::value, "T has no usable to_string() method.");

  return printable.to_string();
}

/// @} freefn

}  // namespace yogi

#endif  // _YOGI_IO_H
