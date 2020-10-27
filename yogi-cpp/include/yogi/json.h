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

#ifndef _YOGI_JSON_H
#define _YOGI_JSON_H

//! \file
//!
//! Includes Niels Lohmann's JSON library.

#ifdef NLOHMANN_JSON_VERSION_MAJOR

#  if NLOHMANN_JSON_VERSION_MAJOR != 3
#    error "Incompatible version of Niels Lohmann's JSON library has been included"
#  endif

#else

// Disable all warnings
#  if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Weverything"
#  elif defined(__GNUG__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wall"
#    pragma GCC diagnostic ignored "-Wextra"
#    pragma GCC diagnostic ignored "-Wsign-conversion"
#    pragma GCC diagnostic ignored "-Wold-style-cast"
#  elif defined(_MSC_VER)
#    pragma warning(push, 0)
#  endif

// Include the JSON library
#  include <nlohmann/json.hpp>

// Enabled the warnings again
#  if defined(__clang__)
#    pragma clang diagnostic pop
#  elif defined(__GNUG__)
#    pragma GCC diagnostic pop
#  elif defined(_MSC_VER)
#    pragma warning(pop)
#  endif

#endif  // NLOHMANN_JSON_VERSION_MAJOR

namespace yogi {

/// Alias for Niels Lohmann's JSON library.
using Json = ::nlohmann::json;

}  // namespace yogi

#endif  // _YOGI_JSON_H
