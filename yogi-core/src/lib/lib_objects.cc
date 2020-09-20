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

#include <src/api/object.h>
#include <src/lib/lib_helpers.h>

YOGI_API int YOGI_FormatObject(void* object, const char** str, int* strsize, const char* objfmt, const char* nullstr) {
  BEGIN_CHECKED_API_FUNCTION

  std::string s;
  if (object == nullptr) {
    s = nullstr ? nullstr : constants::kDefaultInvalidHandleString;
  } else {
    auto obj = ObjectRegister::get(object);
    s        = objfmt ? obj->format(objfmt) : obj->format();
  }

  set_api_buffer(std::move(s), str, strsize);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_Destroy(void* object) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(object != nullptr);

  ObjectRegister::destroy(object);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_DestroyAll() {
  BEGIN_CHECKED_API_FUNCTION

  ObjectRegister::destroy_all();

  END_CHECKED_API_FUNCTION
}
