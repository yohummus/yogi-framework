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

#ifndef _YOGI_SCHEMAS_H
#define _YOGI_SCHEMAS_H

//! \file
//!
//! Functions for getting built-in JSON Schemas from Yogi Core.

#include "detail/api.h"
#include "enums.h"

namespace yogi {

/// \addtogroup freefn
/// @{

/// Returns a built-in JSON Schema.
///
/// The built-in schemas may contain references to other schemas. These
/// references are based on the schema filenames as set in the $id property.
///
/// The schema will be returned formatted and with whitespace.
///
/// \param schema The schema to retrieve.
/// \returns Schema as serialized JSON.
inline std::string get_schema(Schema schema) {
  auto scm = detail::YOGI_GetSchema(static_cast<int>(schema));
  return {scm};
}

/// @} freefn

}  // namespace yogi

#endif  // _YOGI_SCHEMAS_H
