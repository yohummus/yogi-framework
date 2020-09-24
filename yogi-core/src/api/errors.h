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

#include <sstream>
#include <stdexcept>

class Error;

class Result {
 public:
  Result() : ec_(YOGI_ERR_UNKNOWN) {
  }

  explicit Result(int error_code) : ec_(error_code) {
  }

  bool operator==(const Result& rhs) const {
    return ec_ == rhs.ec_;
  }

  bool operator!=(const Result& rhs) const {
    return ec_ != rhs.ec_;
  }

  int value() const {
    return ec_;
  }

  int error_code() const {
    return ec_ > 0 ? 0 : ec_;
  }
  const char* description() const;
  bool is_success() const {
    return ec_ >= 0;
  }

  bool is_error() const {
    return ec_ < 0;
  }
  Error to_error() const;

 private:
  int ec_;
};

class Success : public Result {
 public:
  Success(int res = YOGI_OK) : Result(res) {
  }
};

class Error : public Result, public std::exception {
 public:
  explicit Error(int error_code) : Result(error_code) {
    YOGI_ASSERT(error_code < 0);
  }

  virtual const char* what() const noexcept override {
    return description();
  }
};

class DescriptiveError : public Error {
 public:
  explicit DescriptiveError(int error_code) : Error(error_code) {
  }

  DescriptiveError(const DescriptiveError& err) : Error(err.value()), oss_(err.oss_.str()) {
  }

  DescriptiveError(DescriptiveError&& err) : Error(err.value()), oss_(std::move(err.oss_)) {
  }

  template <typename T>
  DescriptiveError& operator<<(T&& val) {
    oss_ << std::forward<T>(val);
    return *this;
  }

  std::string details() const {
    return oss_.str();
  }

 private:
  std::ostringstream oss_;
};

inline Error Result::to_error() const {
  YOGI_ASSERT(is_error());
  return Error{ec_};
}

std::ostream& operator<<(std::ostream& os, const Result& res);
std::ostream& operator<<(std::ostream& os, const Error& err);
std::ostream& operator<<(std::ostream& os, const DescriptiveError& err);
