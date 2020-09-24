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

#ifndef _YOGI_JSON_VIEW_H
#define _YOGI_JSON_VIEW_H

//! \file
//!
//! Helpers for passing JSON-encoded data to functions.

#include <cstring>
#include <string>
#include <vector>

namespace yogi {

////////////////////////////////////////////////////////////////////////////////
/// Helper class for passing different types of JSON to functions that
/// internally require a C string holding the serialized JSON data.
///
/// \attention
///   It is imperative that the objects passed to any of the view's constructors
///   outlive the view object!
////////////////////////////////////////////////////////////////////////////////
class JsonView {
 public:
  /// Constructs a view that evaluates to a nullptr.
  JsonView() : s_(nullptr), size_(0) {
  }

  /// Constructs a view from a vector.
  ///
  /// \param data Buffer to use.
  JsonView(const std::vector<char>& data) : s_(data.data()), size_(static_cast<int>(data.size())) {
  }

  /// Constructs a view from a standard string.
  ///
  /// \param s Referenced string.
  JsonView(const std::string& s) : s_(s.c_str()), size_(static_cast<int>(s.size() + 1)) {
  }

  /// Constructs a view from a NULL-terminated string.
  ///
  /// \param s Referenced NULL-terminated string.
  JsonView(const char* s) : s_(s), size_(static_cast<int>(strlen(s) + 1)){};

// Define a constructor that takes a nlohmann::json object if that library has been included
#if NLOHMANN_JSON_VERSION_MAJOR
  /// Constructs a view from a JSON object.
  ///
  /// \param json The JSON object to reference.
  JsonView(const ::nlohmann::json& json)
      : tmp_(json.dump()), s_(tmp_.c_str()), size_(static_cast<int>(tmp_.size() + 1)){};
#endif

  /// Returns a NULL-terminated string holding the serialized JSON data.
  ///
  /// \attention
  ///   The returned value is only valid as long as both the view object and the
  ///   parameter passed to any of its constructors are valid.
  ///
  /// \returns NULL-terminated string holding the serialized JSON data.
  const char* data() const {
    return s_;
  }

  /// Returns the length of the serialized JSON data in bytes.
  ///
  /// \returns Length of the serialized JSON data, including the trailing '\0'.
  int size() const {
    return size_;
  }

  /// Returns a NULL-terminated string holding the serialized JSON data.
  ///
  /// \attention
  ///   The returned value is only valid as long as both the view object and the
  ///   parameter passed to any of its constructors are valid.
  ///
  /// \returns NULL-terminated string holding the serialized JSON data.
  operator const char*() const {
    return s_;
  }

  bool operator==(const JsonView& rhs) const {
    if (size() != rhs.size()) return false;
    return std::memcmp(data(), rhs.data(), static_cast<std::size_t>(size())) == 0;
  }

  bool operator!=(const JsonView& rhs) const {
    return !(*this == rhs);
  }

 private:
  std::string tmp_;
  const char* s_;
  int size_;
};

}  // namespace yogi

#endif  // _YOGI_JSON_VIEW_H
