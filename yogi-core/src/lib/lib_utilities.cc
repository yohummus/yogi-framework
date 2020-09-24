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

#include <src/api/constants.h>
#include <src/api/version.h>
#include <src/lib/lib_helpers.h>
#include <src/schemas/schemas.h>

YOGI_API const char* YOGI_GetVersion() {
  return constants::kVersion;
}

YOGI_API int YOGI_CheckBindingsCompatibility(const char* bindver) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(bindver != nullptr);

  check_bindings_compatibility(bindver);

  END_CHECKED_API_FUNCTION
}

YOGI_API const char* YOGI_GetErrorString(int res) {
  return Result(res).description();
}

YOGI_API const char* YOGI_GetLastErrorDetails() {
  return get_last_error_details();
}

YOGI_API int YOGI_GetConstant(void* dest, int constant) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(dest != nullptr);

  get_constant(dest, constant);

  END_CHECKED_API_FUNCTION
}

YOGI_API const char* YOGI_GetSchema(int schema) {
  return get_schema(schema);
}
