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

#include <test/common.h>

#include <chrono>
#include <cstring>
using namespace std::chrono_literals;

class DurationTest : public Test {};

TEST_F(DurationTest, Zero) {
  auto dur = Duration::kZero;
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.nanoseconds_count(), 0);
}

TEST_F(DurationTest, Infinity) {
  auto dur = Duration::kInf;
  EXPECT_FALSE(dur.is_finite());
  EXPECT_TRUE(dur > Duration::kZero);
}

TEST_F(DurationTest, NegativeInfinity) {
  auto dur = Duration::kNegInf;
  EXPECT_FALSE(dur.is_finite());
  EXPECT_TRUE(dur < Duration::kZero);
}

TEST_F(DurationTest, FromNanoseconds) {
  auto dur = Duration::from_nanoseconds(123);
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.nanoseconds_count(), 123);

  dur = Duration::from_nanoseconds(444.56);
  EXPECT_EQ(dur.nanoseconds_count(), 444);  // Note: no rounding

  dur = Duration::from_nanoseconds(std::numeric_limits<double>::infinity());
  EXPECT_EQ(dur, Duration::kInf);

  dur = Duration::from_nanoseconds(-std::numeric_limits<double>::infinity());
  EXPECT_EQ(dur, Duration::kNegInf);
}

TEST_F(DurationTest, FromMicroseconds) {
  auto dur = Duration::from_microseconds(123);
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.total_microseconds(), 123);

  dur = Duration::from_microseconds(std::numeric_limits<double>::infinity());
  EXPECT_EQ(dur, Duration::kInf);

  dur = Duration::from_microseconds(0.5);
  EXPECT_EQ(dur.nanoseconds_count(), 500);
}

TEST_F(DurationTest, FromMilliseconds) {
  auto dur = Duration::from_milliseconds(123);
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.total_milliseconds(), 123);

  dur = Duration::from_milliseconds(std::numeric_limits<double>::infinity());
  EXPECT_EQ(dur, Duration::kInf);

  dur = Duration::from_milliseconds(0.5);
  EXPECT_EQ(dur.total_microseconds(), 500);
}

TEST_F(DurationTest, FromSeconds) {
  auto dur = Duration::from_seconds(123);
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.total_seconds(), 123);

  dur = Duration::from_seconds(std::numeric_limits<double>::infinity());
  EXPECT_EQ(dur, Duration::kInf);

  dur = Duration::from_seconds(0.5);
  EXPECT_EQ(dur.total_milliseconds(), 500);
}

TEST_F(DurationTest, FromMinutes) {
  auto dur = Duration::from_minutes(123);
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.total_minutes(), 123);

  dur = Duration::from_minutes(std::numeric_limits<double>::infinity());
  EXPECT_EQ(dur, Duration::kInf);

  dur = Duration::from_minutes(0.5);
  EXPECT_EQ(dur.total_seconds(), 30);
}

TEST_F(DurationTest, FromHours) {
  auto dur = Duration::from_hours(123);
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.total_hours(), 123);

  dur = Duration::from_hours(std::numeric_limits<double>::infinity());
  EXPECT_EQ(dur, Duration::kInf);

  dur = Duration::from_hours(0.5);
  EXPECT_EQ(dur.total_minutes(), 30);
}

TEST_F(DurationTest, FromDays) {
  auto dur = Duration::from_days(123);
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.total_days(), 123);

  dur = Duration::from_days(std::numeric_limits<double>::infinity());
  EXPECT_EQ(dur, Duration::kInf);

  dur = Duration::from_days(0.5);
  EXPECT_EQ(dur.total_hours(), 12);
}

TEST_F(DurationTest, DefaultConstructor) {
  auto dur = Duration{};
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.nanoseconds_count(), 0);
}

TEST_F(DurationTest, ChronoConstructor) {
  auto dur = Duration{12s};
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.total_seconds(), 12);

  dur = Duration{33ms};
  EXPECT_TRUE(dur.is_finite());
  EXPECT_EQ(dur.total_milliseconds(), 33);
}

TEST_F(DurationTest, Nanoseconds) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_EQ(dur.nanoseconds(), 887);
}

TEST_F(DurationTest, Microseconds) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_EQ(dur.microseconds(), 465);
}

TEST_F(DurationTest, Milliseconds) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_EQ(dur.milliseconds(), 132);
}

TEST_F(DurationTest, Seconds) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_EQ(dur.seconds(), 9);
}

TEST_F(DurationTest, Minutes) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_EQ(dur.minutes(), 33);
}

TEST_F(DurationTest, Hours) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_EQ(dur.hours(), 21);
}

TEST_F(DurationTest, Days) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_EQ(dur.days(), 1428);
}

TEST_F(DurationTest, nanoseconds_count) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_EQ(dur.nanoseconds_count(), 123456789132465887LL);
}

TEST_F(DurationTest, TotalNanoseconds) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_DOUBLE_EQ(dur.total_nanoseconds(), 123456789132465887.0);

  EXPECT_EQ(Duration::kInf.total_nanoseconds(), std::numeric_limits<double>::infinity());
  EXPECT_EQ(Duration::kNegInf.total_nanoseconds(), -std::numeric_limits<double>::infinity());
}

TEST_F(DurationTest, TotalMicroseconds) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_DOUBLE_EQ(dur.total_microseconds(), 123456789132465.887);

  EXPECT_EQ(Duration::kInf.total_microseconds(), std::numeric_limits<double>::infinity());
  EXPECT_EQ(Duration::kNegInf.total_microseconds(), -std::numeric_limits<double>::infinity());
}

TEST_F(DurationTest, TotalMilliseconds) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_DOUBLE_EQ(dur.total_milliseconds(), 123456789132.465887);

  EXPECT_EQ(Duration::kInf.total_milliseconds(), std::numeric_limits<double>::infinity());
  EXPECT_EQ(Duration::kNegInf.total_milliseconds(), -std::numeric_limits<double>::infinity());
}

TEST_F(DurationTest, TotalSeconds) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_DOUBLE_EQ(dur.total_seconds(), 123456789.132465887);

  EXPECT_EQ(Duration::kInf.total_seconds(), std::numeric_limits<double>::infinity());
  EXPECT_EQ(Duration::kNegInf.total_seconds(), -std::numeric_limits<double>::infinity());
}

TEST_F(DurationTest, TotalMinutes) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_DOUBLE_EQ(dur.total_minutes(), 2057613.1522077648);

  EXPECT_EQ(Duration::kInf.total_minutes(), std::numeric_limits<double>::infinity());
  EXPECT_EQ(Duration::kNegInf.total_minutes(), -std::numeric_limits<double>::infinity());
}

TEST_F(DurationTest, TotalHours) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_DOUBLE_EQ(dur.total_hours(), 34293.55253679608);

  EXPECT_EQ(Duration::kInf.total_hours(), std::numeric_limits<double>::infinity());
  EXPECT_EQ(Duration::kNegInf.total_hours(), -std::numeric_limits<double>::infinity());
}

TEST_F(DurationTest, TotalDays) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_DOUBLE_EQ(dur.total_days(), 1428.8980223665033);

  EXPECT_EQ(Duration::kInf.total_days(), std::numeric_limits<double>::infinity());
  EXPECT_EQ(Duration::kNegInf.total_days(), -std::numeric_limits<double>::infinity());
}

TEST_F(DurationTest, Negated) {
  auto dur     = Duration{123ns};
  auto neg_dur = dur.negated();
  EXPECT_EQ(neg_dur.nanoseconds_count(), -dur.nanoseconds_count());
  EXPECT_TRUE(neg_dur.is_finite());
}

TEST_F(DurationTest, ToChronoDuration) {
  auto dur = Duration{123456789132465887ns};
  EXPECT_EQ(dur.to_chrono_duration<>(), 123456789132465887ns);
  EXPECT_EQ(dur.to_chrono_duration<std::chrono::seconds>(), 123456789s);

  EXPECT_EQ(Duration::kInf.to_chrono_duration<>(), (std::chrono::nanoseconds::max)());
  EXPECT_EQ(Duration::kInf.to_chrono_duration<std::chrono::minutes>(), (std::chrono::minutes::max)());
  EXPECT_EQ(Duration::kNegInf.to_chrono_duration<>(), (std::chrono::nanoseconds::min)());
  EXPECT_EQ(Duration::kNegInf.to_chrono_duration<std::chrono::minutes>(), (std::chrono::minutes::min)());
}

TEST_F(DurationTest, IsInfinite) {
  EXPECT_TRUE(Duration::kZero.is_finite());
  EXPECT_TRUE(Duration{123ns}.is_finite());
  EXPECT_FALSE(Duration::kInf.is_finite());
  EXPECT_FALSE(Duration::kNegInf.is_finite());
}

TEST_F(DurationTest, Format) {
  MOCK_FormatDuration(
      [](long long dur, int neg, const char** str, int* strsize, const char* durfmt, const char* inffmt) -> int {
        EXPECT_EQ(dur, 123456789123456789);
        EXPECT_EQ(neg, 0);
        EXPECT_NE(str, nullptr);
        EXPECT_NE(strsize, nullptr);
        EXPECT_EQ(durfmt, nullptr);
        EXPECT_EQ(inffmt, nullptr);
        *str     = "hello";
        *strsize = static_cast<int>(strlen(*str) + 1);
        return YOGI_OK;
      });

  EXPECT_EQ(Duration{123456789123456789ns}.format(), "hello");

  MOCK_FormatDuration(
      [](long long, int, const char** str, int* strsize, const char* durfmt, const char* inffmt) -> int {
        EXPECT_STREQ(durfmt, "%S");
        EXPECT_STREQ(inffmt, "abc");
        *str     = "bla";
        *strsize = static_cast<int>(strlen(*str) + 1);
        return YOGI_OK;
      });

  EXPECT_EQ(Duration{123456789123456789ns}.format("%S", "abc"), "bla");

  MOCK_FormatDuration([](long long dur, int neg, const char** str, int* strsize, const char*, const char*) -> int {
    EXPECT_EQ(dur, -1);
    EXPECT_EQ(neg, 1);
    *str     = "xyz";
    *strsize = static_cast<int>(strlen(*str) + 1);
    return YOGI_OK;
  });

  EXPECT_EQ(Duration::kNegInf.format(), "xyz");
}

TEST_F(DurationTest, FormatError) {
  MOCK_FormatDuration(
      [](long long, int, const char**, int*, const char*, const char*) -> int { return YOGI_ERR_WRONG_OBJECT_TYPE; });

  EXPECT_THROW(Duration::kNegInf.format(), FailureException);
}

TEST_F(DurationTest, ToString) {
  MOCK_FormatDuration(
      [](long long dur, int neg, const char** str, int* strsize, const char* durfmt, const char* inffmt) -> int {
        EXPECT_EQ(dur, 123456789123456789);
        EXPECT_EQ(neg, 0);
        EXPECT_NE(str, nullptr);
        EXPECT_NE(strsize, nullptr);
        EXPECT_EQ(durfmt, nullptr);
        EXPECT_EQ(inffmt, nullptr);
        *str     = "hello";
        *strsize = static_cast<int>(strlen(*str) + 1);
        return YOGI_OK;
      });

  EXPECT_EQ(Duration{123456789123456789ns}.to_string(), "hello");
}

TEST_F(DurationTest, PlusOperator) {
  auto dur1 = Duration{10ns};
  auto dur2 = Duration{3us};
  EXPECT_EQ((dur1 + dur2).nanoseconds_count(), 3010);

  EXPECT_EQ(dur1 + Duration::kInf, Duration::kInf);
  EXPECT_EQ(Duration::kInf + Duration::kInf, Duration::kInf);
  EXPECT_EQ(dur1 + Duration::kNegInf, Duration::kNegInf);
  EXPECT_EQ(Duration::kNegInf + Duration::kNegInf, Duration::kNegInf);
  EXPECT_THROW(Duration::kInf + Duration::kNegInf, ArithmeticException);
}

TEST_F(DurationTest, MinusOperator) {
  auto dur1 = Duration{10ns};
  auto dur2 = Duration{3us};
  EXPECT_EQ((dur1 - dur2).nanoseconds_count(), -2990);

  EXPECT_EQ(dur1 - Duration::kInf, Duration::kNegInf);
  EXPECT_EQ(Duration::kInf - Duration::kNegInf, Duration::kInf);
  EXPECT_EQ(dur1 - Duration::kNegInf, Duration::kInf);
  EXPECT_EQ(Duration::kNegInf - Duration::kInf, Duration::kNegInf);
  EXPECT_THROW(Duration::kNegInf - Duration::kNegInf, ArithmeticException);
}

TEST_F(DurationTest, MultiplicationOperator) {
  auto dur = Duration{8ns};

  EXPECT_EQ((dur * 3).nanoseconds_count(), 24);
  EXPECT_EQ((dur * 3.5).nanoseconds_count(), 28);

  EXPECT_EQ(Duration::kInf * 5, Duration::kInf);
  EXPECT_EQ(Duration::kInf * -5, Duration::kNegInf);
  EXPECT_THROW(Duration::kInf * 0, ArithmeticException);
}

TEST_F(DurationTest, DivisionOperator) {
  auto dur = Duration{28ns};

  EXPECT_EQ((dur / 2).nanoseconds_count(), 14);
  EXPECT_EQ((dur / 3.5).nanoseconds_count(), 8);

  EXPECT_EQ(Duration::kInf / 5, Duration::kInf);
  EXPECT_EQ(Duration::kInf / -5, Duration::kNegInf);
  EXPECT_THROW(dur / 0, ArithmeticException);
}

TEST_F(DurationTest, PlusEqualsOperator) {
  auto dur1 = Duration{10ns};
  auto dur2 = Duration{3us};
  auto dur3 = dur1 += dur2;
  EXPECT_EQ(dur1, dur3);
  EXPECT_EQ(dur1.nanoseconds_count(), 3010);
}

TEST_F(DurationTest, MinusEqualsOperator) {
  auto dur1 = Duration{10ns};
  auto dur2 = Duration{3us};
  auto dur3 = dur1 -= dur2;
  EXPECT_EQ(dur1, dur3);
  EXPECT_EQ(dur1.nanoseconds_count(), -2990);
}

TEST_F(DurationTest, MultiplyEqualsOperator) {
  auto dur1 = Duration{3ns};
  auto dur2 = dur1 *= 3;
  EXPECT_EQ(dur1, dur2);
  EXPECT_EQ(dur1.nanoseconds_count(), 9);
}

TEST_F(DurationTest, DivideEqualsOperator) {
  auto dur1 = Duration{12ns};
  auto dur2 = dur1 /= 3;
  EXPECT_EQ(dur1, dur2);
  EXPECT_EQ(dur1.nanoseconds_count(), 4);
}

TEST_F(DurationTest, EqualityOperator) {
  auto dur1 = Duration{1000ns};
  auto dur2 = Duration{1us};
  auto dur3 = Duration{2ns};
  EXPECT_TRUE(dur1 == dur2);
  EXPECT_FALSE(dur1 == dur3);
}

TEST_F(DurationTest, NotEqualsOperator) {
  auto dur1 = Duration{1000ns};
  auto dur2 = Duration{1us};
  auto dur3 = Duration{2ns};
  EXPECT_FALSE(dur1 != dur2);
  EXPECT_TRUE(dur1 != dur3);
}

TEST_F(DurationTest, LessThanOperator) {
  auto dur1 = Duration{1ns};
  auto dur2 = Duration{1ns};
  auto dur3 = Duration{2ns};
  EXPECT_FALSE(dur1 < dur2);
  EXPECT_TRUE(dur2 < dur3);
  EXPECT_FALSE(dur3 < dur1);

  EXPECT_FALSE(Duration::kInf < Duration::kInf);
  EXPECT_TRUE(Duration::kNegInf < Duration::kInf);
  EXPECT_FALSE(Duration::kInf < Duration::kZero);
  EXPECT_TRUE(Duration::kNegInf < Duration::kZero);
}

TEST_F(DurationTest, GreaterThanOperator) {
  auto dur1 = Duration{1ns};
  auto dur2 = Duration{1ns};
  auto dur3 = Duration{2ns};
  EXPECT_FALSE(dur1 > dur2);
  EXPECT_FALSE(dur2 > dur3);
  EXPECT_TRUE(dur3 > dur1);

  EXPECT_FALSE(Duration::kInf > Duration::kInf);
  EXPECT_FALSE(Duration::kNegInf > Duration::kInf);
  EXPECT_TRUE(Duration::kInf > Duration::kZero);
  EXPECT_FALSE(Duration::kNegInf > Duration::kZero);
}

TEST_F(DurationTest, LessOrEqualOperator) {
  auto dur1 = Duration{1ns};
  auto dur2 = Duration{1ns};
  auto dur3 = Duration{2ns};
  EXPECT_TRUE(dur1 <= dur2);
  EXPECT_TRUE(dur2 <= dur3);
  EXPECT_FALSE(dur3 <= dur1);

  EXPECT_TRUE(Duration::kInf <= Duration::kInf);
  EXPECT_TRUE(Duration::kNegInf <= Duration::kInf);
  EXPECT_FALSE(Duration::kInf <= Duration::kZero);
  EXPECT_TRUE(Duration::kNegInf <= Duration::kZero);
}

TEST_F(DurationTest, GreaterOrEqualOperator) {
  auto dur1 = Duration{1ns};
  auto dur2 = Duration{1ns};
  auto dur3 = Duration{2ns};
  EXPECT_TRUE(dur1 >= dur2);
  EXPECT_FALSE(dur2 >= dur3);
  EXPECT_TRUE(dur3 >= dur1);

  EXPECT_TRUE(Duration::kInf >= Duration::kInf);
  EXPECT_FALSE(Duration::kNegInf >= Duration::kInf);
  EXPECT_TRUE(Duration::kInf >= Duration::kZero);
  EXPECT_FALSE(Duration::kNegInf >= Duration::kZero);
}
