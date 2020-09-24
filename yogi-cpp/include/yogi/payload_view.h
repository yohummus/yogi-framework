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

#ifndef _YOGI_PAYLOAD_VIEW_H
#define _YOGI_PAYLOAD_VIEW_H

//! \file
//!
//! Helpers for passing and converting user payload to functions.

#include "enums.h"
#include "json_view.h"
#include "msgpack_view.h"

namespace yogi {

////////////////////////////////////////////////////////////////////////////////
/// Helper class for passing and converting different types of user payload.
///
/// \attention
///   It is imperative that the objects passed to any of the view's constructors
///   outlive the view object!
////////////////////////////////////////////////////////////////////////////////
class PayloadView {
 public:
  /// Constructs a view that evaluates to a nullptr.
  PayloadView() : PayloadView(nullptr, 0, Encoding::kJson) {
  }

  /// Constructs a view from a buffer.
  ///
  /// \param data Buffer to use.
  PayloadView(const std::vector<char>& data, Encoding enc) : PayloadView(data.data(), data.size(), enc) {
  }

  /// Constructs a view from a buffer.
  ///
  /// \param data Buffer to use.
  /// \param size Size of the buffer in bytes.
  /// \param enc  Encoding.
  PayloadView(const char* data, int size, Encoding enc) : data_(data), size_(size), enc_(enc) {
  }

  /// Constructs a view from a buffer.
  ///
  /// \param data Buffer to use.
  /// \param size Size of the buffer in bytes.
  /// \param enc  Encoding.
  PayloadView(const char* data, std::size_t size, Encoding enc) : PayloadView(data, static_cast<int>(size), enc) {
  }

  /// Constructs a view from JSON data.
  ///
  /// \param json The JSON data.
  PayloadView(const JsonView& json) : PayloadView(json.data(), json.size(), Encoding::kJson) {
  }

  /// Constructs a view from MessagePack data.
  ///
  /// \param msgpack The MessagePack data.
  PayloadView(const MsgpackView& msgpack) : PayloadView(msgpack.data(), msgpack.size(), Encoding::kMsgpack) {
  }

  /// Returns a buffer holding the payload data.
  ///
  /// \attention
  ///   The returned value is only valid as long as both the view object and the
  ///   parameters passed to any of its constructors are valid.
  ///
  /// \returns Buffer holding the payload data.
  const char* data() const {
    return data_;
  }

  /// Returns the size of the payload data in bytes.
  ///
  /// \returns Size of the payload data in bytes.
  int size() const {
    return size_;
  };

  /// Returns the encoding of the payload data.
  ///
  /// \returns Encoding of the payload data.
  Encoding encoding() const {
    return enc_;
  }

  bool operator==(const PayloadView& rhs) const {
    if ((encoding() != rhs.encoding())) return false;
    if (size() != rhs.size()) return false;
    return std::memcmp(data(), rhs.data(), static_cast<std::size_t>(size())) == 0;
  }

  bool operator!=(const PayloadView& rhs) const {
    return !(*this == rhs);
  }

 private:
  const char* data_;
  int size_;
  Encoding enc_;
};

inline bool operator==(const PayloadView& lhs, const JsonView& rhs) {
  return lhs == PayloadView(rhs);
}

inline bool operator!=(const PayloadView& lhs, const JsonView& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const JsonView& lhs, const PayloadView& rhs) {
  return rhs == lhs;
}

inline bool operator!=(const JsonView& lhs, const PayloadView& rhs) {
  return rhs != lhs;
}

inline bool operator==(const PayloadView& lhs, const MsgpackView& rhs) {
  return lhs == PayloadView(rhs);
}

inline bool operator!=(const PayloadView& lhs, const MsgpackView& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const MsgpackView& lhs, const PayloadView& rhs) {
  return rhs == lhs;
}

inline bool operator!=(const MsgpackView& lhs, const PayloadView& rhs) {
  return rhs != lhs;
}

}  // namespace yogi

#endif  // _YOGI_PAYLOAD_VIEW_H
