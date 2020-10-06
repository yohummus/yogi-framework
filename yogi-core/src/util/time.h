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

#include <chrono>
#include <string>
#include <string_view>

class Timestamp {
 public:
  static Timestamp now();
  static Timestamp parse(std::string_view str, const char* fmt);

  Timestamp() : ns_since_epoch_() {
  }

  explicit Timestamp(long long ns_since_epoch) : ns_since_epoch_{ns_since_epoch} {
  }

  long long ns_since_epoch() const {
    return ns_since_epoch_;
  }

  std::string format(const char* fmt) const;
  std::string to_javascript_string() const;

  bool operator==(const Timestamp& rhs) const {
    return ns_since_epoch_ == rhs.ns_since_epoch_;
  }

 private:
  long long ns_since_epoch_;
};

class Duration {
 public:
  static Duration inf() {
    return Duration{-1};
  }

  explicit Duration(long long ns, bool is_negative = false) : ns_{ns}, is_negative_{is_negative} {
  }

  template <typename Rep, typename Period>
  Duration(const std::chrono::duration<Rep, Period>& dur)
      : Duration(std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count()) {
  }

  long long ns() const {
    return ns_;
  }

  bool is_neg() const {
    return is_negative_;
  }

  bool is_inf() const {
    return ns_ < 0;
  }

  std::chrono::nanoseconds to_chrono_duration() const;

  std::string format(const char* durfmt, const char* infstr) const;

 private:
  long long ns_;
  bool is_negative_;
};
