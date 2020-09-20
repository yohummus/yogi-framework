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

#include <src/api/object.h>
#include <src/objects/context.h>

class Dummy : public ExposedObjectT<Dummy, ObjectType::kDummy> {};

class MyObject : public ExposedObjectT<MyObject, ObjectType::kTimer> {
 public:
  MyObject(int) {}
  ~MyObject() { ++dtor_calls; }

  static int dtor_calls;
};

int MyObject::dtor_calls = 0;

class ObjectTest : public testing::Test {
 protected:
  virtual void TearDown() override { ObjectRegister::destroy_all(); }
};

TEST_F(ObjectTest, Create) {
  auto obj = MyObject::create(123);
  EXPECT_EQ(obj.use_count(), 1);
}

TEST_F(ObjectTest, Type) {
  auto obj = MyObject::create(123);
  EXPECT_EQ(obj->type(), ObjectType::kTimer);
  EXPECT_EQ(MyObject::static_type(), ObjectType::kTimer);
}

TEST_F(ObjectTest, Handle) {
  auto obj = MyObject::create(123);
  EXPECT_NE(obj->handle(), nullptr);
}

TEST_F(ObjectTest, Cast) {
  auto my_obj   = MyObject::create(123);
  auto obj      = std::dynamic_pointer_cast<ExposedObject>(my_obj);
  auto my_obj_2 = obj->cast<MyObject>();
  EXPECT_EQ(my_obj, my_obj_2);

  EXPECT_THROW(my_obj->cast<Dummy>(), Error);
  EXPECT_THROW(obj->cast<Dummy>(), Error);
}

TEST_F(ObjectTest, MakeWeakPtr) {
  auto obj  = MyObject::create(123);
  auto weak = obj->make_weak_ptr();
  EXPECT_EQ(weak.lock(), obj);
}

TEST_F(ObjectTest, RegisterAndDestroyObject) {
  auto dtor_calls = MyObject::dtor_calls;

  auto obj    = MyObject::create(123);
  auto handle = ObjectRegister::register_object(obj);
  EXPECT_GT(obj.use_count(), 1);
  EXPECT_EQ(MyObject::dtor_calls, dtor_calls);
  EXPECT_EQ(handle, obj->handle());

  EXPECT_THROW(ObjectRegister::destroy(nullptr), Error);
  EXPECT_THROW(ObjectRegister::destroy(handle), Error);

  obj.reset();
  EXPECT_EQ(MyObject::dtor_calls, dtor_calls);
  EXPECT_NO_THROW(ObjectRegister::destroy(handle));
  EXPECT_EQ(MyObject::dtor_calls, dtor_calls + 1);

  EXPECT_THROW(ObjectRegister::destroy(handle), Error);
}

TEST_F(ObjectTest, GetObject) {
  EXPECT_THROW(ObjectRegister::get(nullptr), Error);

  auto obj    = MyObject::create(123);
  auto handle = ObjectRegister::register_object(obj);

  ObjectPtr obj2 = ObjectRegister::get(handle);
  EXPECT_EQ(obj2, obj);

  std::shared_ptr<MyObject> obj3 = ObjectRegister::get<MyObject>(handle);
  EXPECT_EQ(obj3, obj);

  EXPECT_THROW(ObjectRegister::get<Dummy>(handle), Error);
}

TEST_F(ObjectTest, GetAllObjects) {
  auto obj1 = MyObject::create(123);
  ObjectRegister::register_object(obj1);
  auto obj2 = MyObject::create(456);
  ObjectRegister::register_object(obj2);
  auto dummy = Dummy::create();
  ObjectRegister::register_object(dummy);

  auto vec = ObjectRegister::get_all<MyObject>();
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(std::count(vec.begin(), vec.end(), obj1), 1);
  EXPECT_EQ(std::count(vec.begin(), vec.end(), obj2), 1);
}

TEST_F(ObjectTest, DestroyAllObjects) {
  EXPECT_NO_THROW(ObjectRegister::destroy_all());

  auto obj1 = MyObject::create(123);
  auto obj2 = MyObject::create(456);

  auto dtor_calls = MyObject::dtor_calls;
  ObjectRegister::register_object(obj2);
  ObjectRegister::register_object(obj1);
  EXPECT_EQ(MyObject::dtor_calls, dtor_calls);

  obj1.reset();
  obj2.reset();
  ObjectRegister::destroy_all();
  EXPECT_EQ(MyObject::dtor_calls, dtor_calls + 2);
}

TEST_F(ObjectTest, Format) {
  void* context;
  int res = YOGI_ContextCreate(&context);
  EXPECT_OK(res);

  const char* str;
  int strsize;
  res = YOGI_FormatObject(context, &str, &strsize, nullptr, nullptr);
  EXPECT_OK(res);
  std::string s1 = str;
  EXPECT_EQ(strsize, strlen(str) + 1);
  EXPECT_NE(s1.find("Context"), std::string::npos);
  EXPECT_NE(s1.find("["), std::string::npos);
  EXPECT_NE(s1.find("]"), std::string::npos);
  EXPECT_EQ(s1.find("[]"), std::string::npos);

  res = YOGI_FormatObject(context, &str, &strsize, "$T$x$X", nullptr);
  EXPECT_OK(res);
  std::string s2 = str;
  EXPECT_EQ(s2.find("Context"), 0);
  EXPECT_GT(s2.size(), sizeof("Context") + 2);

  res = YOGI_FormatObject(context, &str, &strsize, "$T$X$x", nullptr);
  EXPECT_OK(res);
  std::string s3 = str;

  std::string tmp = s3.substr(sizeof("Context"));
  if (tmp.find_first_of("abcdef") == std::string::npos) {
    EXPECT_EQ(s3, s2);
  } else {
    EXPECT_NE(s3, s2);
  }

  res = YOGI_FormatObject(nullptr, &str, &strsize, nullptr, "abc");
  EXPECT_OK(res);
  EXPECT_STREQ(str, "abc");

  res = YOGI_FormatObject(nullptr, &str, &strsize, "$T $x", "abc");
  EXPECT_OK(res);
  EXPECT_STREQ(str, "abc");
}
