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

#ifndef _YOGI_OBJECT_H
#define _YOGI_OBJECT_H

//! \file
//!
//! Base classes for Yogi objects.

#include "detail/api.h"
#include "detail/error_helpers.h"

#include <stdio.h>
#include <chrono>
#include <initializer_list>
#include <memory>
#include <string>

namespace yogi {
namespace detail {

template <typename Fn, typename... Args>
inline void* call_api_create(Fn fn, Args&&... args) {
  void* handle = nullptr;
  int res      = fn(&handle, std::forward<Args>(args)...);
  check_error_code(res);
  return handle;
}

}  // namespace detail

class Object;

/// Shared pointer to an object.
using ObjectPtr = std::shared_ptr<Object>;

////////////////////////////////////////////////////////////////////////////////
/// Base class for all "creatable" objects.
///
/// "Creatable" Yogi objects are objects that get instantiated and live until
/// they are destroyed by the user.
////////////////////////////////////////////////////////////////////////////////
class Object : public std::enable_shared_from_this<Object> {
 public:
  virtual ~Object() {
    if (handle_ == nullptr) return;
    int res = detail::YOGI_Destroy(handle_);
    if (res != 0) {
      std::string str = "an object";
      try {
        str = format();
      } catch (const std::exception&) {
      }

      fprintf(stderr, "ERROR: Could not destroy %s: %s\n", str.c_str(), detail::YOGI_GetErrorString(res));
    }
  }

  /// Creates a string describing the object.
  ///
  /// The \p objfmt parameter describes the format of the string. The following
  /// placeholders are supported:
  ///  - *$T*: Type of the object (e.g. %Branch)
  ///  - *$x*: Handle of the object in lower-case hex notation
  ///  - *$X*: Handle of the object in upper-case hex notation
  ///
  /// By default, the object will be formatted in the format "Branch [44fdde]"
  /// with the hex value in brackets being the object's handle.
  ///
  /// If, for any reason, the object's handle is NULL, this function returns
  /// the nullstr parameter value ("INVALID HANDLE" by default).
  ///
  /// \param fmt     Format of the string
  /// \param nullstr String to use if the object's handle is NULL
  ///
  /// \returns Formatted string.
  std::string format(const StringView& fmt = {}, const StringView& nullstr = {}) const {
    const char* str;
    int strsize;
    int res = detail::YOGI_FormatObject(handle_, &str, &strsize, fmt, nullstr);
    detail::check_error_code(res);
    return std::string(str, static_cast<std::string::size_type>(strsize - 1));
  }

  /// Returns a human-readable string identifying the object.
  ///
  /// \returns Human-readable string identifying the object.
  virtual std::string to_string() const {
    return format();
  }

 protected:
  static void* get_foreign_handle(const ObjectPtr& other) {
    return other ? other->handle() : nullptr;
  }
  static void* get_foreign_handle(const Object& other) {
    return other.handle();
  }

  Object(void* handle, std::initializer_list<ObjectPtr> dependencies) : handle_(handle), dependencies_(dependencies) {
  }

  void* handle() const {
    return handle_;
  }

 private:
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;

  void* handle_;
  const std::initializer_list<ObjectPtr> dependencies_;
};

////////////////////////////////////////////////////////////////////////////////
/// Templated base class for all "creatable" objects.
///
/// "Creatable" Yogi objects are objects that get instantiated and live until
/// they are destroyed by the user.
///
/// \tparam T Class that derives from ObjectT.
////////////////////////////////////////////////////////////////////////////////
template <typename T>
class ObjectT : public Object {
 public:
  using Object::Object;
};

}  // namespace yogi

#endif  // _YOGI_OBJECT_H
