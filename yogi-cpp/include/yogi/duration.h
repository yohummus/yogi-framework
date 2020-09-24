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

#ifndef _YOGI_DURATION_H
#define _YOGI_DURATION_H

//! \file
//!
//! Representation of time durations.

#include "detail/api.h"
#include "detail/error_helpers.h"
#include "string_view.h"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <type_traits>

static_assert(std::numeric_limits<double>::has_infinity,
              "Yogi needs a platform with support for infinite double values.");

namespace yogi {

class Duration;

namespace detail {

template <typename T>
inline bool is_finite(T) {
  return true;
}

template <>
inline bool is_finite<float>(float val) {
  return std::isfinite(val);
}

template <>
inline bool is_finite<double>(double val) {
  return std::isfinite(val);
}

template <typename T>
inline bool is_nan(T) {
  return false;
}

template <>
inline bool is_nan<float>(float val) {
  return std::isnan(val);
}

template <>
inline bool is_nan<double>(double val) {
  return std::isnan(val);
}

enum InfinityType {
  kNegative = -1,
  kNone     = 0,
  kPositive = 1,
};

inline long long add_safely(long long a, long long b) {
  if (a > 0 && b > (std::numeric_limits<long long>::max)() - a) {
    throw ArithmeticException("Duration value overflow");
  } else if (a < 0 && b < (std::numeric_limits<long long>::min)() - a) {
    throw ArithmeticException("Duration value underflow");
  }

  return a + b;
}

template <long long Divisor>
inline double to_total_time_unit(InfinityType inf_type, long long ns_count) {
  switch (inf_type) {
    case InfinityType::kPositive:
      return std::numeric_limits<double>::infinity();

    case InfinityType::kNegative:
      return -std::numeric_limits<double>::infinity();

    default:
      return static_cast<double>(ns_count) / Divisor;
  }
}

template <typename T>
inline long long multiply(long long val, T multiplicator) {
  return static_cast<long long>(val * multiplicator);
}

template <>
inline long long multiply<float>(long long val, float multiplicator) {
  return static_cast<long long>(static_cast<float>(val) * multiplicator);
}

template <>
inline long long multiply<double>(long long val, double multiplicator) {
  return static_cast<long long>(static_cast<double>(val) * multiplicator);
}

template <typename T>
inline long long divide(long long val, T divisor) {
  return static_cast<long long>(val / divisor);
}

template <>
inline long long divide<float>(long long val, float multiplicator) {
  return static_cast<long long>(static_cast<float>(val) / multiplicator);
}

template <>
inline long long divide<double>(long long val, double multiplicator) {
  return static_cast<long long>(static_cast<double>(val) / multiplicator);
}

template <typename T>
inline long long multiply_safely(long long val, T multiplicator) {
  if (is_nan(multiplicator)) {
    throw ArithmeticException("Trying to multiply duration value and NaN");
  }

  if (multiplicator == T{}) {
    return 0;
  }

  if (std::abs(multiplicator) > static_cast<T>(1)) {
    auto max_val = divide((std::numeric_limits<long long>::max)(), multiplicator);
    if (std::abs(val) > max_val) {
      throw ArithmeticException("Duration value overflow");
    }
  }

  return multiply(val, multiplicator);
}

template <typename T>
void check_divisor(T divisor) {
  if (is_nan(divisor)) {
    throw ArithmeticException("Trying to divide duration value by NaN");
  }

  if (divisor == T{}) {
    throw ArithmeticException("Trying to divide duration value by zero");
  }
}

template <typename T>
inline long long divide_safely(long long val, T divisor) {
  check_divisor(divisor);

  if (!is_finite(divisor)) {
    return 0;
  }

  if (std::abs(divisor) < static_cast<T>(1)) {
    auto max_val = multiply((std::numeric_limits<long long>::max)(), divisor);
    if (std::abs(val) > max_val) {
      throw ArithmeticException("Duration value overflow.");
    }
  }

  return divide(val, divisor);
}

template <long long Multiplicator, typename T>
inline Duration duration_from_time_unit(T val);

}  // namespace detail

////////////////////////////////////////////////////////////////////////////////
/// Represents a time duration.
///
/// The resolution of a time duration is in nanoseconds. Durations can be
/// positive or negative. Exceptions are thrown in case of arithmetic errors.
////////////////////////////////////////////////////////////////////////////////
class Duration {
 public:
  /// Zero duration
  static const Duration kZero;

  /// Infinite duration
  static const Duration kInf;

  /// Negative infinite duration
  static const Duration kNegInf;

  /// Construct a duration from a number of nanoseconds
  ///
  /// \tparam T Arithmetic type of \p ns
  ///
  /// \param ns Number of nanoseconds
  ///
  /// \returns Duration instance
  template <typename T>
  static Duration from_nanoseconds(T ns) {
    return detail::duration_from_time_unit<1LL>(ns);
  }

  /// Construct a duration from a number of microseconds
  ///
  /// \tparam T Arithmetic type of \p us
  ///
  /// \param us Number of microseconds
  ///
  /// \returns Duration instance
  template <typename T>
  static Duration from_microseconds(T us) {
    return detail::duration_from_time_unit<1'000LL>(us);
  }

  /// Construct a duration from a number of milliseconds
  ///
  /// \tparam T Arithmetic type of \p ms
  ///
  /// \param ms Number of milliseconds
  ///
  /// \returns Duration instance
  template <typename T>
  static Duration from_milliseconds(T ms) {
    return detail::duration_from_time_unit<1'000'000LL>(ms);
  }

  /// Construct a duration from a number of seconds
  ///
  /// \tparam T Arithmetic type of \p seconds
  ///
  /// \param seconds Number of seconds
  ///
  /// \returns Duration instance
  template <typename T>
  static Duration from_seconds(T seconds) {
    return detail::duration_from_time_unit<1'000'000'000LL>(seconds);
  }

  /// Construct a duration from a number of minutes
  ///
  /// \tparam T Arithmetic type of \p minutes
  ///
  /// \param minutes Number of minutes
  ///
  /// \returns Duration instance
  template <typename T>
  static Duration from_minutes(T minutes) {
    return detail::duration_from_time_unit<60 * 1'000'000'000LL>(minutes);
  }

  /// Construct a duration from a number of hours
  ///
  /// \tparam T Arithmetic type of \p hours
  ///
  /// \param hours Number of hours
  ///
  /// \returns Duration instance
  template <typename T>
  static Duration from_hours(T hours) {
    return detail::duration_from_time_unit<60 * 60 * 1'000'000'000LL>(hours);
  }

  /// Construct a duration from a number of days
  ///
  /// \tparam T Arithmetic type of \p days
  ///
  /// \param days Number of days
  ///
  /// \returns Duration instance
  template <typename T>
  static Duration from_days(T days) {
    return detail::duration_from_time_unit<24 * 60 * 60 * 1'000'000'000LL>(days);
  }

  /// Constructs a zero nanoseconds duration
  Duration() : Duration(detail::kNone) {
  }

  /// Constructs a duration from a duration value from the standard library
  ///
  /// \tparam Rep    Arithmetic type representing the number of ticks
  /// \tparam Period Ratio type representing the tick period
  ///
  /// \param dur Duration
  template <typename Rep, typename Period>
  Duration(const std::chrono::duration<Rep, Period>& dur) : Duration(detail::kNone) {
    ns_count_ = std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
  }

  /// nanoseconds fraction of the duration
  ///
  /// If the duration is infinite, this function will return an undefined value.
  ///
  /// \returns Nanoseconds fraction (0-999)
  int nanoseconds() const {
    return static_cast<int>(nanoseconds_count() % 1'000);
  }

  /// microseconds fraction of the duration
  ///
  /// If the duration is infinite, this function will return an undefined value.
  ///
  /// \returns Microseconds fraction (0-999)
  int microseconds() const {
    return static_cast<int>((nanoseconds_count() / 1'000LL) % 1'000);
  }

  /// milliseconds fraction of the duration
  ///
  /// If the duration is infinite, this function will return an undefined value.
  ///
  /// \returns Milliseconds fraction (0-999)
  int milliseconds() const {
    return static_cast<int>((nanoseconds_count() / 1'000'000LL) % 1'000);
  }

  /// Seconds fraction of the duration
  ///
  /// If the duration is infinite, this function will return an undefined value.
  ///
  /// \returns Seconds fraction (0-59)
  int seconds() const {
    return static_cast<int>(nanoseconds_count() / 1'000'000'000LL) % 60;
  }

  /// Minutes fraction of the duration
  ///
  /// \returns Minutes fraction (0-59)
  int minutes() const {
    return static_cast<int>(nanoseconds_count() / (60 * 1'000'000'000LL)) % 60;
  }

  /// Hours fraction of the duration
  ///
  /// If the duration is infinite, this function will return an undefined value.
  ///
  /// \returns Hours fraction (0-23)
  int hours() const {
    return static_cast<int>(nanoseconds_count() / (60 * 60 * 1'000'000'000LL)) % 24;
  }

  /// Days fraction of the duration
  ///
  /// If the duration is infinite, this function will return an undefined value.
  ///
  /// \returns Days fraction
  int days() const {
    return static_cast<int>(nanoseconds_count() / (24 * 60 * 60 * 1'000'000'000LL));
  }

  /// Total number of nanoseconds
  ///
  /// If the duration is infinite, this function will return an undefined value.
  ///
  /// \returns Total number of nanoseconds
  long long nanoseconds_count() const {
    return ns_count_;
  }

  /// Total number of nanoseconds
  ///
  /// If the duration is infinite, this function will return the appropriate
  /// infinite value.
  ///
  /// \returns Total number of nanoseconds
  double total_nanoseconds() const {
    return detail::to_total_time_unit<1LL>(inf_type_, ns_count_);
  }

  /// Total number of microseconds
  ///
  /// If the duration is infinite, this function will return the appropriate
  /// infinite value.
  ///
  /// \returns Total number of microseconds
  double total_microseconds() const {
    return detail::to_total_time_unit<1'000LL>(inf_type_, ns_count_);
  }

  /// Total number of milliseconds
  ///
  /// If the duration is infinite, this function will return the appropriate
  /// infinite value.
  ///
  /// \returns Total number of milliseconds
  double total_milliseconds() const {
    return detail::to_total_time_unit<1'000'000LL>(inf_type_, ns_count_);
  }

  /// Total number of seconds
  ///
  /// If the duration is infinite, this function will return the appropriate
  /// infinite value.
  ///
  /// \returns Total number of seconds
  double total_seconds() const {
    return detail::to_total_time_unit<1'000'000'000LL>(inf_type_, ns_count_);
  }

  /// Total number of minutes
  ///
  /// If the duration is infinite, this function will return the appropriate
  /// infinite value.
  ///
  /// \returns Total number of minutes
  double total_minutes() const {
    return detail::to_total_time_unit<60 * 1'000'000'000LL>(inf_type_, ns_count_);
  }

  /// Total number of hours
  ///
  /// If the duration is infinite, this function will return the appropriate
  /// infinite value.
  ///
  /// \returns Total number of hours
  double total_hours() const {
    return detail::to_total_time_unit<60 * 60 * 1'000'000'000LL>(inf_type_, ns_count_);
  }

  /// Total number of days
  ///
  /// If the duration is infinite, this function will return the appropriate
  /// infinite value.
  ///
  /// \returns Total number of days
  double total_days() const {
    return detail::to_total_time_unit<24 * 60 * 60 * 1'000'000'000LL>(inf_type_, ns_count_);
  }

  /// Returns the negated duration, i.e. multiplied by -1
  ///
  /// \returns Negated duration
  Duration negated() const {
    switch (inf_type_) {
      case detail::InfinityType::kPositive:
        return kNegInf;

      case detail::InfinityType::kNegative:
        return kInf;

      default:
        return from_nanoseconds(-ns_count_);
    }
  }

  /// Converts the duration to a standard library duration
  ///
  /// In case that the duration is infinite, the respective min
  ///  or max value of the requested type will be returned.
  ///
  /// \tparam T Standard library duration type
  ///
  /// \returns Duration as a standard library duration
  template <typename T = std::chrono::nanoseconds>
  T to_chrono_duration() const {
    switch (inf_type_) {
      case detail::InfinityType::kPositive:
        return (T::max)();

      case detail::InfinityType::kNegative:
        return (T::min)();

      default:  // detail::kNone
        return std::chrono::duration_cast<T>(std::chrono::nanoseconds(ns_count_));
    }
  }

  /// Checks if the duration is finite or not
  ///
  /// \returns True if the duration is finite
  bool is_finite() const {
    return inf_type_ == detail::kNone;
  }

  /// Converts the duration to a string.
  ///
  /// The \p dur_fmt parameter describes the format of the conversion.
  /// The following placeholders are supported:
  ///  - *%+*: Plus sign if duration is positive; minus sign if it is negative
  ///  - *%-*: Minus sign (only) if duration is negative
  ///  - *%d*: Total number of days
  ///  - *%D*: Total number of days if days > 0
  ///  - *%H*: Fractional hours (range 00 to 23)
  ///  - *%M*: Fractional minutes (range 00 to 59)
  ///  - *%S*: Fractional seconds (range 00 to 59)
  ///  - *%T*: Equivalent to %H:%M:%S
  ///  - *%3*: Fractional milliseconds (range 000 to 999)
  ///  - *%6*: Fractional microseconds (range 000 to 999)
  ///  - *%9*: Fractional nanoseconds (range 000 to 999)
  ///
  /// The \p inf_fmt parameter describes the format to use for infinite
  /// durations. The following placeholders are supported:
  ///  - *%+*: Plus sign if duration is positive; minus sign if it is negative
  ///  - *%-*: Minus sign (only) if duration is negative
  ///
  /// By default, the duration will be formatted in the format
  /// "-23d 04:19:33.123456789". If the duration is infinite, then the string
  /// constructed using \p inf_fmt will be returned ("inf" and "-inf"
  /// respectively by default).
  ///
  /// \param[in] dur_fmt Format of the duration string
  /// \param[in] inf_fmt Format to use for infinity
  ///
  /// \returns The formatted duration string
  std::string format(const StringView& dur_fmt = {}, const StringView& inf_fmt = {}) const {
    const char* str;
    int strsize;
    int res = detail::YOGI_FormatDuration(is_finite() ? ns_count_ : -1, ns_count_ < 0 ? 1 : 0, &str, &strsize, dur_fmt,
                                          inf_fmt);
    detail::check_error_code(res);
    return std::string(str, static_cast<std::string::size_type>(strsize - 1));
  }

  /// Converts the duration to a string
  ///
  /// The format of the string will be "-23d 04:19:33.123456789".
  ///
  /// \returns Duration as a string
  std::string to_string() const {
    return format();
  }

  Duration operator+(const Duration& rhs) const {
    if (inf_type_ == detail::kNone && rhs.inf_type_ == detail::kNone) {
      return from_nanoseconds(detail::add_safely(ns_count_, rhs.ns_count_));
    }

    auto inf_type_sum = inf_type_ + rhs.inf_type_;
    if (inf_type_sum == detail::kNone) {
      throw ArithmeticException("Trying to add positive and negative infinite duration values.");
    }

    return inf_type_sum > 0 ? kInf : kNegInf;
  }

  Duration operator-(const Duration& rhs) const {
    return *this + rhs.negated();
  }

  template <typename T>
  Duration operator*(T rhs) const {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type.");

    if (inf_type_ != detail::kNone && rhs == T{}) {
      throw ArithmeticException("Trying to multiply infinite duration value and zero.");
    }

    if (inf_type_ == detail::kNone && detail::is_finite(rhs)) {
      return from_nanoseconds(detail::multiply_safely(ns_count_, rhs));
    }

    auto rhs_inf_type = rhs > T{} ? detail::InfinityType::kPositive : detail::InfinityType::kNegative;
    return inf_type_ == rhs_inf_type ? kInf : kNegInf;
  }

  template <typename T>
  Duration operator/(T rhs) const {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type.");

    if (inf_type_ == detail::kNone) {
      return from_nanoseconds(detail::divide_safely(ns_count_, rhs));
    }

    detail::check_divisor(rhs);
    auto rhs_inf_type = rhs > T{} ? detail::InfinityType::kPositive : detail::InfinityType::kNegative;
    return inf_type_ == rhs_inf_type ? kInf : kNegInf;
  }

  Duration& operator+=(const Duration& rhs) {
    *this = *this + rhs;
    return *this;
  }

  Duration& operator-=(const Duration& rhs) {
    *this = *this - rhs;
    return *this;
  }

  Duration& operator*=(int rhs) {
    *this = *this * rhs;
    return *this;
  }

  Duration& operator*=(float rhs) {
    *this = *this * rhs;
    return *this;
  }

  Duration& operator/=(int rhs) {
    *this = *this / rhs;
    return *this;
  }

  Duration& operator/=(float rhs) {
    *this = *this / rhs;
    return *this;
  }

  bool operator==(const Duration& rhs) const {
    if (inf_type_ == rhs.inf_type_ && inf_type_ != detail::kNone) {
      return true;
    }

    return ns_count_ == rhs.ns_count_;
  }

  bool operator!=(const Duration& rhs) const {
    return !(*this == rhs);
  }

  bool operator<(const Duration& rhs) const {
    if (inf_type_ == rhs.inf_type_ && inf_type_ == detail::kNone) {
      return ns_count_ < rhs.ns_count_;
    }

    return inf_type_ < rhs.inf_type_;
  }

  bool operator>(const Duration& rhs) const {
    if (inf_type_ == rhs.inf_type_ && inf_type_ == detail::kNone) {
      return ns_count_ > rhs.ns_count_;
    }

    return inf_type_ > rhs.inf_type_;
  }

  bool operator<=(const Duration& rhs) const {
    return !(*this > rhs);
  }

  bool operator>=(const Duration& rhs) const {
    return !(*this < rhs);
  }

 private:
  explicit Duration(detail::InfinityType inf_type) : inf_type_(inf_type) {
    switch (inf_type) {
      case detail::InfinityType::kPositive:
        ns_count_ = (std::numeric_limits<long long>::max)();
        break;

      case detail::InfinityType::kNegative:
        ns_count_ = (std::numeric_limits<long long>::min)();
        break;

      default:  // detail::kNone
        ns_count_ = 0;
        break;
    }
  }

  detail::InfinityType inf_type_;
  long long ns_count_;
};

_YOGI_WEAK_SYMBOL const Duration Duration::kZero;
_YOGI_WEAK_SYMBOL const Duration Duration::kInf(detail::InfinityType::kPositive);
_YOGI_WEAK_SYMBOL const Duration Duration::kNegInf(detail::InfinityType::kNegative);

namespace detail {

template <long long Multiplicator, typename T>
inline Duration duration_from_time_unit_impl(T val) {
  static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type.");

  if (is_nan(val)) {
    throw ArithmeticException("Cannot construct duration from NaN");
  }

  if (is_finite(val)) {
    auto max_val = static_cast<T>((std::numeric_limits<T>::max)() / Multiplicator);
    if (std::abs(val) > max_val) {
      throw ArithmeticException("Duration value overflow");
    }

    return Duration{std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(val * Multiplicator))};
  }

  // infinite
  return val < 0 ? Duration::kNegInf : Duration::kInf;
}

template <bool IsIntegral>
struct DurationFromTimeUnitSelector {
  template <long long Multiplicator, typename T>
  inline static Duration Fn(T val) {
    return duration_from_time_unit_impl<Multiplicator, T>(val);
  }
};

template <>
struct DurationFromTimeUnitSelector<true> {
  template <long long Multiplicator, typename T>
  inline static Duration Fn(T val) {
    return duration_from_time_unit_impl<Multiplicator, long long>(val);
  }
};

template <long long Multiplicator, typename T>
inline Duration duration_from_time_unit(T val) {
  using Selector = DurationFromTimeUnitSelector<std::is_integral<T>::value>;
  return Selector::template Fn<Multiplicator>(val);
}

}  // namespace detail
}  // namespace yogi

#endif  // _YOGI_DURATION_H
