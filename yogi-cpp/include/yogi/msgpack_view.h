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

#ifndef _YOGI_MSGPACK_VIEW_H
#define _YOGI_MSGPACK_VIEW_H

//! \file
//!
//! Helpers for passing MessagePack-encoded data to functions.

#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace yogi {

////////////////////////////////////////////////////////////////////////////////
/// Helper class for passing different types of MessagePack data to functions
/// that internally require a buffer holding the serialized MessagePack data.
///
/// \attention
///   It is imperative that the objects passed to any of the view's constructors
///   outlive the view object!
////////////////////////////////////////////////////////////////////////////////
class MsgpackView {
 public:
  /// Constructs a view that evaluates to a nullptr.
  MsgpackView() : MsgpackView(nullptr, 0) {
  }

  /// Constructs a view from a vector.
  ///
  /// \param data Buffer to use.
  MsgpackView(const std::vector<char>& data) : MsgpackView(data.data(), data.size()) {
  }

  /// Constructs a view from a buffer.
  ///
  /// \param data Buffer to use.
  /// \param size Size of the buffer in bytes.
  MsgpackView(const char* data, int size) : data_(data), size_(size){};

  /// Constructs a view from a buffer.
  ///
  /// \param data Buffer to use.
  /// \param size Size of the buffer in bytes.
  MsgpackView(const char* data, std::size_t size) : MsgpackView(data, static_cast<int>(size)){};

  /// Constructs a view from a standard string.
  ///
  /// \param s Referenced string.
  MsgpackView(const std::string& s) : data_(s.c_str()), size_(static_cast<int>(s.size())) {
  }

  /// Constructs a view from a std::stringstream.
  ///
  /// \param ss The string stream to reference.
  MsgpackView(const std::stringstream& ss)
      : tmp_(ss.str()), data_(tmp_.c_str()), size_(static_cast<int>(tmp_.size())){};

  /// Returns a buffer holding the serialized MessagePack data.
  ///
  /// \attention
  ///   The returned value is only valid as long as both the view object and the
  ///   parameters passed to any of its constructors are valid.
  ///
  /// \returns Buffer holding the serialized MessagePack data.
  const char* data() const {
    return data_;
  }

  /// Returns the length of the serialized MessagePack data in bytes.
  ///
  /// \returns Size of the serialized MessagePack data in bytes.
  int size() const {
    return size_;
  }

  bool operator==(const MsgpackView& rhs) const {
    if (size() != rhs.size()) return false;
    return std::memcmp(data(), rhs.data(), static_cast<std::size_t>(size())) == 0;
  }

  bool operator!=(const MsgpackView& rhs) const {
    return !(*this == rhs);
  }

 private:
  std::string tmp_;
  const char* data_;
  int size_;
};

}  // namespace yogi

#endif  // _YOGI_MSGPACK_VIEW_H
