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

#include <src/util/bind.h>

#include <memory>

struct Base : public std::enable_shared_from_this<Base> {};

struct Derived : public Base {
  void function_with_no_args() {
    called = true;
  }

  void function_with_one_arg(std::string s) {
    EXPECT_EQ(s, "hello");
    called = true;
  }

  void function_with_two_args(std::string s, int x) {
    EXPECT_EQ(s, "hello");
    EXPECT_EQ(x, 123);
    called = true;
  }

  bool called = false;
};

class BindTest : public TestFixture {
 protected:
  std::shared_ptr<Derived> obj_ = std::make_shared<Derived>();
};

TEST_F(BindTest, WeakFunctionWithNoArguments) {
  auto fn = bind_weak(&Derived::function_with_no_args, obj_.get());
  fn();
  EXPECT_TRUE(obj_->called);
}

TEST_F(BindTest, StrongFunctionWithNoArguments) {
  auto fn = bind_strong(&Derived::function_with_no_args, obj_.get());
  fn();
  EXPECT_TRUE(obj_->called);
}

TEST_F(BindTest, WeakFunctionWithOneArgument) {
  auto fn = bind_weak(&Derived::function_with_one_arg, obj_.get());
  fn("hello");
  EXPECT_TRUE(obj_->called);
}

TEST_F(BindTest, StrongFunctionWithOneArgument) {
  auto fn = bind_strong(&Derived::function_with_one_arg, obj_.get());
  fn("hello");
  EXPECT_TRUE(obj_->called);
}

TEST_F(BindTest, WeakFunctionWithTwoArguments) {
  auto fn = bind_weak(&Derived::function_with_two_args, obj_.get());
  fn("hello", 123);
  EXPECT_TRUE(obj_->called);
}

TEST_F(BindTest, StrongFunctionWithTwoArguments) {
  auto fn = bind_strong(&Derived::function_with_two_args, obj_.get());
  fn("hello", 123);
  EXPECT_TRUE(obj_->called);
}

TEST_F(BindTest, WeakExpired) {
  auto fn = bind_weak(&Derived::function_with_no_args, obj_.get());
  obj_    = {};
  EXPECT_NO_THROW(fn());  // Should not crash
}
