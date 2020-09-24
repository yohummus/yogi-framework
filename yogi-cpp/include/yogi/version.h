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

#ifndef _YOGI_VERSION_H
#define _YOGI_VERSION_H

#include "constants.h"
#include "detail/macros.h"

#include <string>

//! \file
//!
//! Version information.

// :CODEGEN_BEGIN:

/// Whole version string of the bindings
#define YOGI_BINDINGS_VERSION "0.0.1-alpha"

/// Major version number of the bindings
#define YOGI_BINDINGS_VERSION_MAJOR 0

/// Minor version number of the bindings
#define YOGI_BINDINGS_VERSION_MINOR 0

/// Patch version number of the bindings
#define YOGI_BINDINGS_VERSION_PATCH 1

/// Version suffix of the bindings
#define YOGI_BINDINGS_VERSION_SUFFIX "-alpha"

// :CODEGEN_END:

namespace yogi {

////////////////////////////////////////////////////////////////////////////////
/// Information about the C++ bindings.
////////////////////////////////////////////////////////////////////////////////
struct bindings_info {
  static const std::string kVersion;        ///< Whole version string of the bindings
  static const int kVersionMajor;           ///< Major version number of the bindings
  static const int kVersionMinor;           ///< Minor version number of the bindings
  static const int kVersionPatch;           ///< Patch version number of the bindings
  static const std::string kVersionSuffix;  ///< Version suffix of the bindings
};

_YOGI_WEAK_SYMBOL const std::string bindings_info::kVersion       = YOGI_BINDINGS_VERSION;
_YOGI_WEAK_SYMBOL const int bindings_info::kVersionMajor          = YOGI_BINDINGS_VERSION_MAJOR;
_YOGI_WEAK_SYMBOL const int bindings_info::kVersionMinor          = YOGI_BINDINGS_VERSION_MINOR;
_YOGI_WEAK_SYMBOL const int bindings_info::kVersionPatch          = YOGI_BINDINGS_VERSION_PATCH;
_YOGI_WEAK_SYMBOL const std::string bindings_info::kVersionSuffix = YOGI_BINDINGS_VERSION_SUFFIX;

////////////////////////////////////////////////////////////////////////////////
/// Information about the Yogi Core library.
////////////////////////////////////////////////////////////////////////////////
struct core_info {
  static const std::string kVersion;        ///< Whole version string of the Yogi Core library
  static const int kVersionMajor;           ///< Major version number of the Yogi Core library
  static const int kVersionMinor;           ///< Minor version number of the Yogi Core library
  static const int kVersionPatch;           ///< Patch version number of the Yogi Core library
  static const std::string kVersionSuffix;  ///< Version suffix of the Yogi Core library
};

_YOGI_WEAK_SYMBOL const std::string core_info::kVersion       = constants::kVersion;
_YOGI_WEAK_SYMBOL const int core_info::kVersionMajor          = constants::kVersionMajor;
_YOGI_WEAK_SYMBOL const int core_info::kVersionMinor          = constants::kVersionMinor;
_YOGI_WEAK_SYMBOL const int core_info::kVersionPatch          = constants::kVersionPatch;
_YOGI_WEAK_SYMBOL const std::string core_info::kVersionSuffix = constants::kVersionSuffix;

}  // namespace yogi

#endif  // _YOGI_VERSION_H
