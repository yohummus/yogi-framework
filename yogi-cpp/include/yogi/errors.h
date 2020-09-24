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

#ifndef _YOGI_ERRORS_H
#define _YOGI_ERRORS_H

//! \file
//!
//! Functionality related to error handling, including exceptions.

#include "detail/api.h"
#include "enums.h"
#include "string_view.h"

#include <exception>
#include <string>

namespace yogi {

////////////////////////////////////////////////////////////////////////////////
/// Represents a result of an operation.
///
/// This is a wrapper around the result code returned by the functions from
/// the Yogi Core library. A result is represented by a number which is >= 0
/// in case of success and < 0 in case of a failure.
////////////////////////////////////////////////////////////////////////////////
class Result {
 public:
  /// Constructor
  ///
  /// \param value Result code as returned by the Yogi Core library function.
  explicit Result(int value) : value_(value) {
  }

  /// Constructor
  ///
  /// \param ec Error code.
  explicit Result(ErrorCode ec) : Result(static_cast<int>(ec)) {
  }

  virtual ~Result() {
  }

  /// Returns the result code.
  ///
  /// \returns The number as returned by the Yogi Core library function.
  int value() const {
    return value_;
  }

  /// Error code associated with this result.
  ///
  /// \returns Associated error code.
  ErrorCode error_code() const {
    return value_ >= 0 ? ErrorCode::kOk : static_cast<enum ErrorCode>(value_);
  }

  /// Returns a human-readable string describing the result.
  ///
  /// \return Description of the result.
  virtual std::string to_string() const {
    return detail::YOGI_GetErrorString(value_);
  }

  /// Returns _true_ if the error code >= 0 (i.e. the operation succeeded).
  ///
  /// \returns _true_ if the error code >= 0.
  explicit operator bool() const {
    return value_ >= 0;
  }

  /// Returns _true_ if the error code < 0 (i.e. the operation failed).
  ///
  /// \returns _true_ if the error code < 0.
  bool operator!() const {
    return value_ < 0;
  }

  bool operator==(const Result& rhs) const {
    return value_ == rhs.value_;
  }

  bool operator!=(const Result& rhs) const {
    return value_ != rhs.value_;
  }

  bool operator<(const Result& rhs) const {
    return value_ < rhs.value_;
  }

  bool operator<=(const Result& rhs) const {
    return value_ <= rhs.value_;
  }

  bool operator>(const Result& rhs) const {
    return value_ > rhs.value_;
  }

  bool operator>=(const Result& rhs) const {
    return value_ >= rhs.value_;
  }

 private:
  int value_;
};

////////////////////////////////////////////////////////////////////////////////
/// Represents the failure of an operation.
///
/// The success of an operation is associated with a result code >= 0.
////////////////////////////////////////////////////////////////////////////////
class Success : public Result {
 public:
  /// Constructor
  ///
  /// \param value Result code as returned by the Yogi Core library function.
  explicit Success(int value) : Result(value) {
  }

  /// Default constructor
  Success() : Success(static_cast<int>(ErrorCode::kOk)) {
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Represents the failure of an operation.
///
/// The failure of an operation is associated with a result code < 0.
////////////////////////////////////////////////////////////////////////////////
class Failure : public Result {
 public:
  /// Constructor
  ///
  /// \param ec Error code.
  explicit Failure(enum ErrorCode ec) : Result(ec) {
  }
};

////////////////////////////////////////////////////////////////////////////////
/// A failure of an operation that includes a detailed description.
///
/// Some functions in the Yogi Core library provide information in addition to
/// the error code in case of a failure. This class contains both the error
/// code and the additional information.
////////////////////////////////////////////////////////////////////////////////
class DetailedFailure : public Failure {
 public:
  /// Constructor
  ///
  /// \param ec      Error code.
  /// \param details Error details.
  DetailedFailure(enum ErrorCode ec, const StringView& details) : Failure(ec), details_(details) {
  }

  /// Returns the error details.
  ///
  /// \returns Detailed error description.
  const std::string& details() const {
    return details_;
  }

  /// Returns a human-readable string describing the failure, including details.
  ///
  /// \return Detailed description of the failure.
  virtual std::string to_string() const override {
    return Failure::to_string() + ". Details: " + details_;
  }

 private:
  const std::string details_;
};

////////////////////////////////////////////////////////////////////////////////
/// Base class for all Yogi exceptions.
///
/// All exceptions thrown by Yogi functions are derived from this class.
////////////////////////////////////////////////////////////////////////////////
class Exception : public std::exception {
 public:
  /// Returns a description of the error.
  ///
  /// \returns Description of the error.
  virtual const char* what() const noexcept override = 0;
};

/// %Exception wrapping a Failure object.
///
/// This exception type is used for failures without a detailed description.
class FailureException : public Exception {
 public:
  /// Constructor
  ///
  /// \param ec Error code.
  FailureException(ErrorCode ec) : failure_(ec) {
  }

  /// Returns the wrapped Failure object.
  ///
  /// \returns The wrapped Failure object.
  virtual const Failure& failure() const {
    return failure_;
  }

  virtual const char* what() const noexcept override {
    return detail::YOGI_GetErrorString(failure_.value());
  }

 private:
  const class Failure failure_;
};

////////////////////////////////////////////////////////////////////////////////
/// An exception class wrapping a DetailedFailure object.
///
/// This exception type is used for failures that have detailed information
/// available.
////////////////////////////////////////////////////////////////////////////////
class DetailedFailureException : public FailureException {
 public:
  /// Constructor
  ///
  /// \param ec      Error code.
  /// \param details Error details.
  DetailedFailureException(ErrorCode ec, const StringView& details)
      : FailureException(ec), failure_(ec, details), what_(failure_.to_string()) {
  }

  virtual const class Failure& failure() const override {
    return failure_;
  }

  virtual const char* what() const noexcept override {
    return what_.c_str();
  }

 private:
  const DetailedFailure failure_;
  const std::string what_;
};

////////////////////////////////////////////////////////////////////////////////
/// An exception class for arithmetic errors.
///
/// This exception type is used when arithmetic errors occur in Yogi classes
/// such as Duration and Timestamp.
////////////////////////////////////////////////////////////////////////////////
class ArithmeticException : public Exception {
 public:
  /// Constructor
  ///
  /// \param what Description of the error.
  ArithmeticException(const char* what) : what_(what) {
  }

  virtual const char* what() const noexcept override {
    return what_;
  }

 private:
  const char* what_;
};

}  // namespace yogi

#endif  // _YOGI_ERRORS_H
