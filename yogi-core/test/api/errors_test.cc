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

#include <src/api/errors.h>

#include <sstream>
#include <thread>
#include <type_traits>

// :CODEGEN_BEGIN:
int kLastError = YOGI_ERR_OPEN_FILE_FAILED;
// :CODEGEN_END:

TEST(ErrorsTest, DefaultResultConstructor) {
  Result res;
  EXPECT_EQ(res.value(), YOGI_ERR_UNKNOWN);
}

TEST(ErrorsTest, GetErrorString) {
  EXPECT_STRNE(YOGI_GetErrorString(-2), YOGI_GetErrorString(-1));
  EXPECT_STREQ(YOGI_GetErrorString(0), YOGI_GetErrorString(1));
}

TEST(ErrorsTest, DescriptionForEachError) {
  auto inv_err_str = YOGI_GetErrorString(kLastError - 1);

  for (int i = 0; i >= kLastError; --i) {
    EXPECT_STRNE(YOGI_GetErrorString(i), inv_err_str) << "Error code: " << i;
  }

  for (int i = kLastError - 1; i < kLastError - 10; --i) {
    EXPECT_STREQ(YOGI_GetErrorString(i), inv_err_str) << "Error code: " << i;
  }
}

TEST(ErrorsTest, NoDuplicates) {
  for (int i = 0; i >= kLastError; --i) {
    for (int j = 0; j > i; --j) {
      EXPECT_STRNE(YOGI_GetErrorString(i), YOGI_GetErrorString(j));
    }
  }
}

TEST(ErrorsTest, Comparison) {
  Result res1{12};
  Result res2{12};
  Result res3{44};

  EXPECT_TRUE(res1 == res2);
  EXPECT_FALSE(res1 != res2);
  EXPECT_FALSE(res1 == res3);
  EXPECT_TRUE(res1 != res3);
}

TEST(ErrorsTest, SuccessErrorCheck) {
  Result res{123};
  EXPECT_TRUE(res.is_success());
  EXPECT_FALSE(res.is_error());

  Success ok;
  EXPECT_TRUE(ok.is_success());
  EXPECT_FALSE(ok.is_error());

  Error err{-1};
  EXPECT_FALSE(err.is_success());
  EXPECT_TRUE(err.is_error());
}

TEST(ErrorsTest, OstreamOperators) {
  std::stringstream ss;

  Result res{12};
  ss << res;
  EXPECT_EQ(ss.str(), res.description());

  Error err{-1};
  ss.str("");
  ss << err;
  EXPECT_EQ(ss.str(), err.description());

  DescriptiveError desc_err{-2};
  ss.str("");
  ss << desc_err;
  EXPECT_EQ(ss.str(), desc_err.description());

  ss.str("");
  desc_err << "Blabla";
  ss << desc_err;
  EXPECT_CONTAINS(ss.str(), desc_err.description());
  EXPECT_CONTAINS(ss.str(), desc_err.details());
}

TEST(ErrorsTest, ExceptionWhat) {
  Error err{-1};
  EXPECT_STREQ(err.what(), err.description());
}

TEST(ErrorsTest, ConstructDescriptiveError) {
  DescriptiveError err1{-1};
  err1 << "Blabla";

  DescriptiveError err2{err1};
  EXPECT_EQ(err2.description(), err1.description());
  EXPECT_EQ(err2.details(), err1.details());

  DescriptiveError err3{std::move(err1)};
  EXPECT_EQ(err3.description(), err2.description());
  EXPECT_EQ(err3.details(), err2.details());
}

TEST(ErrorsTest, ToError) {
  Result res{-1};

  auto err = res.to_error();
  static_assert(std::is_same_v<decltype(err), Error>);
  EXPECT_EQ(err.error_code(), res.error_code());
}

TEST(ErrorsTest, GetLastErrorDetails) {
  std::string details1;
  std::string details2;

  std::thread th1{[&] {
    int res = YOGI_CheckBindingsCompatibility("foo");
    EXPECT_NE(res, YOGI_OK);
    details1 = YOGI_GetLastErrorDetails();
    EXPECT_CONTAINS(details1, "not have a valid format");
  }};

  std::thread th2{[&] {
    int res = YOGI_CheckBindingsCompatibility("9999.0.0");
    EXPECT_NE(res, YOGI_OK);
    details2 = YOGI_GetLastErrorDetails();
    EXPECT_CONTAINS(details2, "is incompatible with");
  }};

  th1.join();
  th2.join();

  EXPECT_NE(details1, "") << details1;
  EXPECT_NE(details2, "") << details2;
  EXPECT_NE(details1, details2) << details1 << "\n" << details2;
}
