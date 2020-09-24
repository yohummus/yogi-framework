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

#include <yogi_core.h>

class TimerTest : public Test {
 protected:
  TimerPtr create_timer() {
    MOCK_TimerCreate([](void** timer, void*) {
      *timer = kPointer;
      return YOGI_OK;
    });

    return Timer::create(create_context());
  }
};

TEST_F(TimerTest, Create) {
  MOCK_TimerCreate([](void** timer, void* context) {
    EXPECT_NE(timer, nullptr);
    EXPECT_EQ(context, kPointer);
    return YOGI_OK;
  });
  Timer::create(create_context());
}

TEST_F(TimerTest, CreateError) {
  MOCK_TimerCreate([](void**, void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(Timer::create(create_context()), FailureException);
}

TEST_F(TimerTest, StartAsync) {
  auto timer = create_timer();

  bool called = false;
  auto fn     = [&](const Result& res) {
    EXPECT_EQ(res.error_code(), ErrorCode::kWrongObjectType);
    called = true;
  };

  MOCK_TimerStartAsync([](void* timer, long long duration, void (*fn)(int res, void* userarg), void* userarg) {
    EXPECT_EQ(timer, kPointer);
    EXPECT_EQ(duration, 1234);
    EXPECT_NE(fn, nullptr);
    EXPECT_NE(userarg, nullptr);
    fn(YOGI_ERR_WRONG_OBJECT_TYPE, userarg);
    return YOGI_OK;
  });
  timer->start_async(Duration::from_nanoseconds(1234), fn);
  EXPECT_TRUE(called);
}

TEST_F(TimerTest, StartAsyncError) {
  auto timer = create_timer();

  MOCK_TimerStartAsync([](void*, long long, void (*)(int, void*), void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(timer->start_async(Duration::from_nanoseconds(1234), {}), FailureException);
}

TEST_F(TimerTest, Cancel) {
  auto timer = create_timer();

  MOCK_TimerCancel([](void* timer) {
    EXPECT_EQ(timer, kPointer);
    return YOGI_OK;
  });
  EXPECT_TRUE(timer->cancel());

  MOCK_TimerCancel([](void*) { return YOGI_ERR_TIMER_EXPIRED; });
  EXPECT_FALSE(timer->cancel());
}

TEST_F(TimerTest, CancelError) {
  auto timer = create_timer();

  MOCK_TimerCancel([](void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(timer->cancel(), FailureException);
}
