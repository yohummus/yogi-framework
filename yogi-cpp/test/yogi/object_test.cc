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

#include <regex>

class ObjectTest : public Test {
 protected:
  yogi::ContextPtr object_ = create_context();
};

TEST_F(ObjectTest, Format) {
  MOCK_FormatObject([](void* obj, const char** str, int* strsize, const char* objfmt, const char* nullstr) {
    EXPECT_EQ(obj, kPointer);
    EXPECT_NE(str, nullptr);
    EXPECT_NE(strsize, nullptr);
    EXPECT_EQ(objfmt, nullptr);
    EXPECT_EQ(nullstr, nullptr);
    *str     = "x";
    *strsize = static_cast<int>(strlen(*str) + 1);
    return YOGI_OK;
  });

  EXPECT_EQ(object_->format(), "x");
}

TEST_F(ObjectTest, FormatError) {
  MOCK_FormatObject([](void*, const char**, int*, const char* objfmt, const char* nullstr) {
    EXPECT_STREQ(objfmt, "foo");
    EXPECT_STREQ(nullstr, "bar");
    return YOGI_ERR_UNKNOWN;
  });

  EXPECT_THROW(object_->format("foo", "bar"), yogi::FailureException);
}

TEST_F(ObjectTest, ToString) {
  MOCK_FormatObject([](void* obj, const char** str, int* strsize, const char* objfmt, const char* nullstr) {
    EXPECT_EQ(obj, kPointer);
    EXPECT_NE(str, nullptr);
    EXPECT_NE(strsize, nullptr);
    EXPECT_EQ(objfmt, nullptr);
    EXPECT_EQ(nullstr, nullptr);
    *str     = "x";
    *strsize = static_cast<int>(strlen(*str) + 1);
    return YOGI_OK;
  });

  EXPECT_EQ(object_->to_string(), "x");
}

TEST_F(ObjectTest, Destroy) {
  static bool called;

  called = false;
  MOCK_Destroy([](void* obj) {
    EXPECT_EQ(obj, kPointer);
    called = true;
    return YOGI_OK;
  });

  object_.reset();
  EXPECT_TRUE(called);
}
