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

#include <src/util/time.h>

#include <chrono>
#include <thread>
using namespace std::chrono_literals;

TEST(TimeTest, GetCurrentTime) {
  long long time_a = 0;
  int res          = YOGI_GetCurrentTime(&time_a);
  EXPECT_OK(res);
  EXPECT_GT(time_a, 0);

  std::this_thread::sleep_for(100us);

  long long time_b = 0;
  res              = YOGI_GetCurrentTime(&time_b);
  EXPECT_OK(res);
  EXPECT_GT(time_b, time_a);
}

TEST(TimeTest, FormatTime) {
  long long timestamp = 1234356789123456789LL;
  const char* str;
  int strsize;

  int res = YOGI_FormatTime(timestamp, &str, &strsize, nullptr);
  EXPECT_OK(res);
  EXPECT_STREQ(str, "2009-02-11T12:53:09.123Z");
  EXPECT_EQ(strsize, strlen(str) + 1);

  res = YOGI_FormatTime(timestamp, &str, &strsize, "%Y%m%d%H%M%S%3%6%9");
  EXPECT_OK(res);
  EXPECT_STREQ(str, "20090211125309123456789");

  res = YOGI_FormatTime(timestamp, &str, &strsize, "%FT%T.%3Z");
  EXPECT_OK(res);
  EXPECT_STREQ(str, "2009-02-11T12:53:09.123Z");
}

TEST(TimeTest, ParseTime) {
  long long timestamp;
  int res = YOGI_ParseTime(&timestamp, "2009-02-11T12:53:09.123Z", nullptr);
  EXPECT_OK(res);
  EXPECT_EQ(timestamp, 1234356789123000000LL);

  res = YOGI_ParseTime(&timestamp, "20090211125309123456789", "%Y%m%d%H%M%S%3%6%9");
  EXPECT_OK(res);
  EXPECT_EQ(timestamp, 1234356789123456789LL);

  res = YOGI_ParseTime(&timestamp, "01", "%S");
  EXPECT_OK(res);
  EXPECT_EQ(timestamp, 2678401000000000LL);

  res = YOGI_ParseTime(&timestamp, "555", "%6");
  EXPECT_OK(res);
  EXPECT_EQ(timestamp, 2678400000555000LL);

  res = YOGI_ParseTime(&timestamp, "123", "%X");
  EXPECT_ERR(res, YOGI_ERR_INVALID_TIME_FORMAT);

  res = YOGI_ParseTime(&timestamp, "123", nullptr);
  EXPECT_ERR(res, YOGI_ERR_PARSING_TIME_FAILED);
}

TEST(TimeTest, FormatDuration) {
  long long dur = 123456789123456789;
  const char* str;
  int strsize;

  int res = YOGI_FormatDuration(dur, YOGI_FALSE, &str, &strsize, nullptr, nullptr);
  EXPECT_OK(res);
  EXPECT_STREQ(str, "1428d 21:33:09.123456789");
  EXPECT_EQ(strsize, strlen(str) + 1);

  res = YOGI_FormatDuration(dur, YOGI_FALSE, &str, &strsize, "%+%-%D%d%H%M%S%T%3%6%9", "abc");
  EXPECT_OK(res);
  EXPECT_STREQ(str, "+1428142821330921:33:09123456789");

  res = YOGI_FormatDuration(-1, YOGI_FALSE, &str, &strsize, nullptr, nullptr);
  EXPECT_OK(res);
  EXPECT_STREQ(str, "inf");

  res = YOGI_FormatDuration(-1, YOGI_TRUE, &str, &strsize, nullptr, nullptr);
  EXPECT_OK(res);
  EXPECT_STREQ(str, "-inf");

  res = YOGI_FormatDuration(-1, YOGI_FALSE, &str, &strsize, nullptr, "%+%-abc");
  EXPECT_OK(res);
  EXPECT_STREQ(str, "+abc");

  res = YOGI_FormatDuration(-1, YOGI_TRUE, &str, &strsize, nullptr, "%+%-abc");
  EXPECT_OK(res);
  EXPECT_STREQ(str, "--abc");

  res = YOGI_FormatDuration(dur, YOGI_FALSE, &str, &strsize, "%+%-%D%d", "abc");
  EXPECT_OK(res);
  EXPECT_STREQ(str, "+14281428");

  res = YOGI_FormatDuration(dur, YOGI_TRUE, &str, &strsize, "%+%-%D%d", nullptr);
  EXPECT_OK(res);
  EXPECT_STREQ(str, "--14281428");

  res = YOGI_FormatDuration(dur, YOGI_FALSE, &str, &strsize, "%+%-%D%d", nullptr);
  EXPECT_OK(res);
  EXPECT_STREQ(str, "+14281428");

  res = YOGI_FormatDuration(123, YOGI_FALSE, &str, &strsize, "%D%d", nullptr);
  EXPECT_OK(res);
  EXPECT_STREQ(str, "0");
}
