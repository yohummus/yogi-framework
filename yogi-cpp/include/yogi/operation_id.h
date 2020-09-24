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

#ifndef _YOGI_OPERATION_ID_H
#define _YOGI_OPERATION_ID_H

//! \file
//!
//! Classes for handling IDs for asynchronous operations.

namespace yogi {

class OperationId;

namespace detail {

inline OperationId make_operation_id(int val);

}  // namespace detail

////////////////////////////////////////////////////////////////////////////////
/// Represents the ID of an asynchronous operation.
////////////////////////////////////////////////////////////////////////////////
class OperationId {
  friend OperationId detail::make_operation_id(int);

 public:
  /// Constructs an invalid operation ID.
  OperationId() : id_(0) {
  }

  /// Returns the numerical value of the ID.
  ///
  /// \returns Numerical value of the ID.
  int value() const {
    return id_;
  }

  /// Returns true if the operation ID is valid.
  ///
  /// \returns True if the operation ID is valid.
  bool is_valid() const {
    return id_ > 0;
  }

  bool operator==(const OperationId& rhs) const {
    return id_ == rhs.id_;
  }

  bool operator!=(const OperationId& rhs) const {
    return id_ != rhs.id_;
  }

  bool operator<(const OperationId& rhs) const {
    return id_ < rhs.id_;
  }

  bool operator<=(const OperationId& rhs) const {
    return id_ <= rhs.id_;
  }

  bool operator>(const OperationId& rhs) const {
    return id_ > rhs.id_;
  }

  bool operator>=(const OperationId& rhs) const {
    return id_ >= rhs.id_;
  }

 private:
  OperationId(int val) : id_(val) {
  }

  int id_;
};

namespace detail {

inline OperationId make_operation_id(int val) {
  return OperationId(val);
}

}  // namespace detail
}  // namespace yogi

#endif  // _YOGI_OPERATION_ID_H
