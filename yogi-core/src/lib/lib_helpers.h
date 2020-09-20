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

#pragma once

#include <src/config.h>

#include <src/api/errors.h>
#include <src/api/object.h>

#include <stdio.h>
#include <regex>
#include <stdexcept>

// ========== HELPER MACROS ==========
// clang-format off
#define BEGIN_CHECKED_API_FUNCTION \
  try {                            \
    ([&]() -> void {               \
      /* begin function body */

#define END_CHECKED_API_FUNCTION                                 \
      /* end function body */                                    \
    })();                                                        \
                                                                 \
    return update_last_error_details(Success());                 \
  } catch (const DescriptiveError& e) {                          \
    return update_last_error_details(e);                         \
  } catch (const Error& e) {                                     \
    return update_last_error_details(e);                         \
  } catch (const std::bad_alloc&) {                              \
    return update_last_error_details(Error{YOGI_ERR_BAD_ALLOC}); \
  } CATCH_INTERNAL_ERRORS                                        \
// clang-format on

#ifdef NDEBUG
#  define CATCH_INTERNAL_ERRORS                                                            \
    catch (const std::exception& e) {                                                      \
      fprintf(stderr, "%s:%i: INTERNAL ERROR: %s\n", __FILE__, __LINE__, e.what());        \
      return update_last_error_details(DescriptiveError(YOGI_ERR_UNKNOWN) << e.what());    \
    }                                                                                      \
    catch (...) {                                                                          \
      fprintf(stderr, "%s:%i: INTERNAL ERROR: %s\n", __FILE__, __LINE__, "Unknown error"); \
      return update_last_error_details(Error(YOGI_ERR_UNKNOWN));                           \
    }
#else
#  define CATCH_INTERNAL_ERRORS
#endif

#define CHECK_PARAM(cond)                             \
  {                                                   \
    if (!(cond)) throw Error{YOGI_ERR_INVALID_PARAM}; \
  }

#define CHECK_BUFFER_PARAMS(buffer, size)        \
  {                                              \
    CHECK_PARAM(buffer != nullptr || size == 0); \
    CHECK_PARAM(size >= 0);                      \
  }

#define CHECK_FLAGS(flags, all_flags) \
  { CHECK_PARAM(!(flags & ~(all_flags))); }

#define CHECK_EXACTLY_ONE_BIT_SET(var) \
  { CHECK_PARAM(var && !(var & (var - 1))); }

// ========== HELPER FUNCTIONS ==========
int update_last_error_details(const DescriptiveError& e);
int update_last_error_details(const Result& res);
const char* get_last_error_details();

void set_api_buffer(std::string&& buffer, const char** data_ptr, int* size_ptr);
