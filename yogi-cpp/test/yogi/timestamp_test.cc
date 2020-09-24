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

using yogi::ArithmeticException;
using yogi::Duration;
using yogi::Timestamp;
using namespace std::chrono_literals;

class TimestampTest : public Test {};

TEST_F(TimestampTest, FromDurationSinceEpoch) {
  EXPECT_EQ(Timestamp::from_duration_since_epoch(123ns).duration_since_epoch(), 123ns);
  EXPECT_THROW(Timestamp::from_duration_since_epoch(Duration::kInf), ArithmeticException);
  EXPECT_THROW(Timestamp::from_duration_since_epoch(yogi::Duration(-1ns)), ArithmeticException);
}

TEST_F(TimestampTest, Now) {
  MOCK_GetCurrentTime([](long long* timestamp) {
    EXPECT_NE(timestamp, nullptr);
    *timestamp = 5555555555ll;
    return YOGI_OK;
  });

  EXPECT_EQ(Timestamp::now().duration_since_epoch().nanoseconds_count(), 5555555555ll);
}

TEST_F(TimestampTest, NowError) {
  MOCK_GetCurrentTime([](long long*) { return YOGI_ERR_UNKNOWN; });

  EXPECT_THROW(Timestamp::now(), FailureException);
}

TEST_F(TimestampTest, Parse) {
  MOCK_ParseTime([](long long* timestamp, const char* str, const char* timefmt) {
    EXPECT_NE(timestamp, nullptr);
    EXPECT_STREQ(str, "foo");
    EXPECT_EQ(timefmt, nullptr);
    *timestamp = 1234356789123000000ll;
    return YOGI_OK;
  });

  auto t = Timestamp::parse("foo");
  EXPECT_EQ(t.duration_since_epoch(), 1234356789123000000ns);
}

TEST_F(TimestampTest, ParseError) {
  MOCK_ParseTime([](long long*, const char*, const char* timefmt) {
    EXPECT_STREQ(timefmt, "bar");
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(Timestamp::parse("foo", "bar"), yogi::FailureException);
}

TEST_F(TimestampTest, Constructor) {
  EXPECT_EQ(Timestamp().duration_since_epoch(), 0ns);
}

TEST_F(TimestampTest, DurationSinceEpoch) {
  EXPECT_EQ(Timestamp::from_duration_since_epoch(123ns).duration_since_epoch(), 123ns);
}

TEST_F(TimestampTest, Nanoseconds) {
  EXPECT_EQ(Timestamp::from_duration_since_epoch(123456789ns).nanoseconds(), 789);
}

TEST_F(TimestampTest, Microseconds) {
  EXPECT_EQ(Timestamp::from_duration_since_epoch(123456789ns).microseconds(), 456);
}

TEST_F(TimestampTest, Milliseconds) {
  EXPECT_EQ(Timestamp::from_duration_since_epoch(123456789ns).milliseconds(), 123);
}

TEST_F(TimestampTest, Format) {
  auto t = Timestamp::from_duration_since_epoch(123456789123456789ns);

  MOCK_FormatTime([](long long timestamp, const char** str, int* strsize, const char* timefmt) {
    EXPECT_EQ(timestamp, 123456789123456789ll);
    EXPECT_NE(str, nullptr);
    EXPECT_NE(strsize, nullptr);
    EXPECT_EQ(timefmt, nullptr);
    *str     = "x";
    *strsize = static_cast<int>(strlen(*str) + 1);
    return YOGI_OK;
  });

  EXPECT_EQ(t.format(), "x");
}

TEST_F(TimestampTest, FormatError) {
  auto t = Timestamp::from_duration_since_epoch(123456789123456789ns);

  MOCK_FormatTime([](long long, const char**, int*, const char* timefmt) {
    EXPECT_STREQ(timefmt, "foo");
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(t.format("foo"), FailureException);
}

TEST_F(TimestampTest, ToString) {
  auto t = Timestamp::from_duration_since_epoch(123456789123456789ns);

  MOCK_FormatTime([](long long timestamp, const char** str, int* strsize, const char* timefmt) {
    EXPECT_EQ(timestamp, 123456789123456789ll);
    EXPECT_NE(str, nullptr);
    EXPECT_NE(strsize, nullptr);
    EXPECT_EQ(timefmt, nullptr);
    *str     = "x";
    *strsize = static_cast<int>(strlen(*str) + 1);
    return YOGI_OK;
  });

  EXPECT_EQ(t.to_string(), "x");
}

TEST_F(TimestampTest, Operators) {
  auto t = Timestamp();
  t += 8ns;
  EXPECT_EQ(t.duration_since_epoch(), 8ns);
  EXPECT_THROW(t += -1ms, ArithmeticException);
  t -= 1ns;
  EXPECT_EQ(t.duration_since_epoch(), 7ns);
  EXPECT_THROW(t -= 1ms, ArithmeticException);

  EXPECT_EQ(t + 5ns, Timestamp::from_duration_since_epoch(12ns));
  EXPECT_THROW(t + -1ms, ArithmeticException);
  EXPECT_EQ(t - 2ns, Timestamp::from_duration_since_epoch(5ns));
  EXPECT_EQ(t - Timestamp::from_duration_since_epoch(3ns), 4ns);
  EXPECT_THROW(t - 1ms, ArithmeticException);

  EXPECT_TRUE(t == Timestamp::from_duration_since_epoch(7ns));
  EXPECT_FALSE(t == Timestamp::from_duration_since_epoch(8ns));

  EXPECT_FALSE(t != Timestamp::from_duration_since_epoch(7ns));
  EXPECT_TRUE(t != Timestamp::from_duration_since_epoch(8ns));

  EXPECT_TRUE(Timestamp::from_duration_since_epoch(2ns) < Timestamp::from_duration_since_epoch(3ns));
  EXPECT_FALSE(Timestamp::from_duration_since_epoch(3ns) < Timestamp::from_duration_since_epoch(2ns));
  EXPECT_FALSE(Timestamp::from_duration_since_epoch(3ns) < Timestamp::from_duration_since_epoch(3ns));

  EXPECT_FALSE(Timestamp::from_duration_since_epoch(2ns) > Timestamp::from_duration_since_epoch(3ns));
  EXPECT_TRUE(Timestamp::from_duration_since_epoch(3ns) > Timestamp::from_duration_since_epoch(2ns));
  EXPECT_FALSE(Timestamp::from_duration_since_epoch(3ns) > Timestamp::from_duration_since_epoch(3ns));

  EXPECT_TRUE(Timestamp::from_duration_since_epoch(2ns) <= Timestamp::from_duration_since_epoch(3ns));
  EXPECT_TRUE(Timestamp::from_duration_since_epoch(3ns) <= Timestamp::from_duration_since_epoch(3ns));
  EXPECT_FALSE(Timestamp::from_duration_since_epoch(3ns) <= Timestamp::from_duration_since_epoch(2ns));

  EXPECT_FALSE(Timestamp::from_duration_since_epoch(2ns) >= Timestamp::from_duration_since_epoch(3ns));
  EXPECT_TRUE(Timestamp::from_duration_since_epoch(3ns) >= Timestamp::from_duration_since_epoch(3ns));
  EXPECT_TRUE(Timestamp::from_duration_since_epoch(3ns) >= Timestamp::from_duration_since_epoch(2ns));
}
