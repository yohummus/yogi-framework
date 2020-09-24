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

#include <type_traits>

class ErrorsTest : public Test {
 public:
  ErrorsTest() {
    MOCK_GetErrorString([](int err) { return "foobar"; });
  }
};

TEST_F(ErrorsTest, Result) {
  EXPECT_FALSE(Result(-1));
  EXPECT_TRUE(Result(0));
  EXPECT_TRUE(Result(1));
  EXPECT_FALSE(!Result(1));
  EXPECT_EQ(Result(10).value(), 10);
  EXPECT_EQ(Result(ErrorCode::kBusy).error_code(), ErrorCode::kBusy);
  EXPECT_EQ(Result(10).error_code(), ErrorCode::kOk);
  EXPECT_EQ(Result(3), Result(3));
  EXPECT_NE(Result(2), Result(3));
  EXPECT_GT(Result(-3).to_string().size(), 5u);
  EXPECT_EQ(Result(0).error_code(), ErrorCode::kOk);
  EXPECT_EQ(Result(0).error_code(), Result(1).error_code());
}

TEST_F(ErrorsTest, Failure) {
  EXPECT_FALSE(Failure(ErrorCode::kBadAlloc));
  EXPECT_EQ(Failure(ErrorCode::kBadAlloc).to_string(), Result(ErrorCode::kBadAlloc).to_string());
  EXPECT_EQ(Failure(ErrorCode::kBadAlloc), Result(ErrorCode::kBadAlloc));
  EXPECT_TRUE((std::is_base_of<Result, Failure>::value));
}

TEST_F(ErrorsTest, DetailedFailure) {
  EXPECT_EQ(DetailedFailure(ErrorCode::kBusy, "ab").details(), "ab");
  EXPECT_TRUE((std::is_base_of<Failure, DetailedFailure>::value));
  EXPECT_NE(DetailedFailure(ErrorCode::kBusy, "xyz").to_string().find(Failure(ErrorCode::kBusy).to_string()),
            std::string::npos);
  EXPECT_NE(DetailedFailure(ErrorCode::kBusy, "xyz").to_string().find("xyz"), std::string::npos);
}

TEST_F(ErrorsTest, Exception) {
  EXPECT_TRUE((std::is_base_of<std::exception, Exception>::value));
}

TEST_F(ErrorsTest, FailureException) {
  EXPECT_TRUE((std::is_base_of<Exception, FailureException>::value));
  EXPECT_EQ(FailureException(ErrorCode::kBusy).what(), Failure(ErrorCode::kBusy).to_string());
  EXPECT_EQ(FailureException(ErrorCode::kBusy).failure(), Failure(ErrorCode::kBusy));
}

TEST_F(ErrorsTest, DetailedFailureException) {
  EXPECT_TRUE((std::is_base_of<FailureException, DetailedFailureException>::value));
  EXPECT_EQ(DetailedFailureException(ErrorCode::kBusy, "xyz").what(),
            DetailedFailure(ErrorCode::kBusy, "xyz").to_string());
  EXPECT_EQ(DetailedFailureException(ErrorCode::kBusy, "xyz").failure(), DetailedFailure(ErrorCode::kBusy, "xyz"));
}

TEST_F(ErrorsTest, Success) {
  EXPECT_TRUE(Success(0));
  EXPECT_TRUE(Success(1));
  EXPECT_EQ(Success(1).to_string(), Result(1).to_string());
  EXPECT_EQ(Success(1), Result(1));
  EXPECT_TRUE((std::is_base_of<Result, Success>::value));
}

TEST_F(ErrorsTest, ArithmeticException) {
  EXPECT_TRUE((std::is_base_of<Exception, ArithmeticException>::value));
  EXPECT_STREQ(ArithmeticException("abc").what(), "abc");
}
