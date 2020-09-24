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
#include <regex>
#include <thread>
using namespace std::chrono_literals;

class ContextTest : public Test {
 public:
  yogi::ContextPtr context_ = create_context();
};

TEST_F(ContextTest, Poll) {
  MOCK_ContextPoll([](void* context, int* count) {
    EXPECT_NE(context, nullptr);
    EXPECT_NE(count, nullptr);
    *count = 5;
    return YOGI_OK;
  });

  EXPECT_EQ(context_->poll(), 5);
}

TEST_F(ContextTest, PollError) {
  MOCK_ContextPoll([](void*, int*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(context_->poll(), yogi::FailureException);
}

TEST_F(ContextTest, PollOne) {
  MOCK_ContextPollOne([](void* context, int* count) {
    EXPECT_NE(context, nullptr);
    EXPECT_NE(count, nullptr);
    *count = 1;
    return YOGI_OK;
  });

  EXPECT_EQ(context_->poll_one(), 1);
}

TEST_F(ContextTest, PollOneError) {
  MOCK_ContextPollOne([](void*, int*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(context_->poll_one(), yogi::FailureException);
}

TEST_F(ContextTest, Run) {
  MOCK_ContextRun([](void* context, int* count, long long duration) {
    EXPECT_NE(context, nullptr);
    EXPECT_NE(count, nullptr);
    EXPECT_EQ(duration, 1000000ll);
    *count = 5;
    return YOGI_OK;
  });

  EXPECT_EQ(context_->run(1ms), 5);
}

TEST_F(ContextTest, RunError) {
  MOCK_ContextRun([](void*, int*, long long duration) {
    EXPECT_EQ(-1, duration);
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(context_->run(), yogi::FailureException);
}

TEST_F(ContextTest, RunOne) {
  MOCK_ContextRunOne([](void* context, int* count, long long duration) {
    EXPECT_NE(context, nullptr);
    EXPECT_NE(count, nullptr);
    EXPECT_EQ(duration, 1000000ll);
    *count = 1;
    return YOGI_OK;
  });

  EXPECT_EQ(context_->run_one(1ms), 1);
}

TEST_F(ContextTest, RunOneError) {
  MOCK_ContextRunOne([](void*, int*, long long duration) {
    EXPECT_EQ(-1, duration);
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(context_->run_one(), yogi::FailureException);
}

TEST_F(ContextTest, RunInBackground) {
  MOCK_ContextRunInBackground([](void* context) {
    EXPECT_NE(context, nullptr);
    return YOGI_OK;
  });

  EXPECT_NO_THROW(context_->run_in_background());
}

TEST_F(ContextTest, RunInBackgroundError) {
  MOCK_ContextRunInBackground([](void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(context_->run_in_background(), yogi::FailureException);
}

TEST_F(ContextTest, Stop) {
  MOCK_ContextStop([](void* context) {
    EXPECT_NE(context, nullptr);
    return YOGI_OK;
  });

  EXPECT_NO_THROW(context_->stop());
}

TEST_F(ContextTest, StopError) {
  MOCK_ContextStop([](void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(context_->stop(), yogi::FailureException);
}

TEST_F(ContextTest, WaitForRunning) {
  MOCK_ContextWaitForRunning([](void* context, long long duration) {
    EXPECT_NE(context, nullptr);
    EXPECT_EQ(duration, 1000000ll);
    return YOGI_OK;
  });

  EXPECT_TRUE(context_->wait_for_running(1ms));
}

TEST_F(ContextTest, WaitForRunningTimeout) {
  MOCK_ContextWaitForRunning([](void*, long long) { return YOGI_ERR_TIMEOUT; });

  EXPECT_FALSE(context_->wait_for_running(1ms));
}

TEST_F(ContextTest, WaitForRunningError) {
  MOCK_ContextWaitForRunning([](void*, long long duration) {
    EXPECT_EQ(-1, duration);
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(context_->wait_for_running(), yogi::FailureException);
}

TEST_F(ContextTest, WaitForStopped) {
  MOCK_ContextWaitForStopped([](void* context, long long duration) {
    EXPECT_NE(context, nullptr);
    EXPECT_EQ(duration, 1000000ll);
    return YOGI_OK;
  });

  EXPECT_TRUE(context_->wait_for_stopped(1ms));
}

TEST_F(ContextTest, WaitForStoppedTimeout) {
  MOCK_ContextWaitForStopped([](void*, long long) { return YOGI_ERR_TIMEOUT; });

  EXPECT_FALSE(context_->wait_for_stopped(1ms));
}

TEST_F(ContextTest, WaitForStoppedError) {
  MOCK_ContextWaitForStopped([](void*, long long duration) {
    EXPECT_EQ(-1, duration);
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(context_->wait_for_stopped(), yogi::FailureException);
}

TEST_F(ContextTest, Post) {
  MOCK_ContextPost([](void* context, void (*fn)(void* userarg), void* userarg) {
    EXPECT_EQ(context, kPointer);
    EXPECT_NE(fn, nullptr);
    EXPECT_NE(userarg, nullptr);
    fn(userarg);
    return YOGI_OK;
  });

  bool called = false;
  auto fn     = [&] { called = true; };
  context_->post(fn);
  EXPECT_TRUE(called);
}

TEST_F(ContextTest, PostError) {
  MOCK_ContextPost([](void*, void (*)(void*), void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(context_->post({}), FailureException);
}
