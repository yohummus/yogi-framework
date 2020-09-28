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

#include <src/api/object.h>
#include <src/objects/context.h>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <limits>

const std::string& ExposedObject::type_name() const {
  switch (type()) {
    // clang-format off
    case ObjectType::kDummy:         { static const std::string s = "Dummy";         return s; }
    case ObjectType::kContext:       { static const std::string s = "Context";       return s; }
    case ObjectType::kLogger:        { static const std::string s = "Logger";        return s; }
    case ObjectType::kTimer:         { static const std::string s = "Timer";         return s; }
    case ObjectType::kBranch:        { static const std::string s = "Branch";        return s; }
    case ObjectType::kConfiguration: { static const std::string s = "Configuration"; return s; }
    case ObjectType::kSignalSet:     { static const std::string s = "SignalSet";     return s; }
    case ObjectType::kWebServer:     { static const std::string s = "WebServer";     return s; }
    case ObjectType::kWebRoute:      { static const std::string s = "WebRoute";      return s; }
    case ObjectType::kWebProcess:    { static const std::string s = "WebProcess";    return s; }
      // clang-format on
  }

  YOGI_NEVER_REACHED;
  static const std::string s;
  return s;
}

std::string ExposedObject::format(std::string fmt) const {
  boost::replace_all(fmt, "$T", type_name());

  char buf[24];
  sprintf(buf, "%llx", reinterpret_cast<unsigned long long>(this));
  boost::replace_all(fmt, "$x", buf);
  sprintf(buf, "%llX", reinterpret_cast<unsigned long long>(this));
  boost::replace_all(fmt, "$X", buf);

  return fmt;
}

std::mutex ObjectRegister::mutex_;
// NOLINTNEXTLINE(cert-err58-cpp)
ObjectRegister::ObjectsMap ObjectRegister::objects_;

ObjectHandle ObjectRegister::register_object(const ObjectPtr& obj) {
  std::lock_guard<std::mutex> lock{mutex_};

  auto handle = obj->handle();
  YOGI_ASSERT(!objects_.count(handle));
  objects_[handle] = obj;

  return handle;
}

void ObjectRegister::destroy(ObjectHandle handle) {
  ObjectPtr obj;  // Hold it so it gets destroyed AFTER the lock is released

  std::lock_guard<std::mutex> lock{mutex_};
  auto it = objects_.find(handle);

  if (it == objects_.end()) {
    throw Error{YOGI_ERR_INVALID_HANDLE};
  }

  if (it->second.use_count() > 1) {
    throw Error{YOGI_ERR_OBJECT_STILL_USED};
  }

  obj = it->second;
  objects_.erase(it);
}

void ObjectRegister::destroy_all() {
  std::vector<ObjectPtr> objs = take_objects();
  stop_all_contexts(objs);

  while (remove_unused_objects(&objs) || poll_all_contexts(objs))
    ;

  if (!objs.empty()) {
    YOGI_DEBUG_ONLY(print_objects_still_in_use(objs));
    throw Error{YOGI_ERR_OBJECT_STILL_USED};
  }
}

ObjectRegister::ObjectsVector ObjectRegister::take_objects() {
  ObjectsVector objs;

  std::lock_guard<std::mutex> lock{mutex_};
  for (auto& entry : objects_) {
    objs.push_back(std::move(entry.second));
  }
  objects_.clear();

  return objs;
}

bool ObjectRegister::remove_unused_objects(ObjectsVector* objs) {
  bool destroyed_some = false;

  auto it = objs->begin();
  while (it != objs->end()) {
    if (it->use_count() == 1) {
      it             = objs->erase(it);
      destroyed_some = true;
    } else {
      ++it;
    }
  }

  return destroyed_some;
}

void ObjectRegister::stop_all_contexts(const ObjectsVector& objs) {
  for (auto& context : get_all<Context>(objs)) {
    context->stop();
    bool stopped = context->wait_for_stopped(Duration::inf());
    YOGI_ASSERT(stopped);
    YOGI_UNUSED(stopped);
  }
}

bool ObjectRegister::poll_all_contexts(const ObjectsVector& objs) {
  bool polled_some = false;
  for (auto& context : get_all<Context>(objs)) {
    int n = context->poll();
    if (n) polled_some = true;
  }

  return polled_some;
}

void ObjectRegister::print_objects_still_in_use(const ObjectsVector& objs) {
  std::cout << "Objects still in use after DestroyAll():" << std::endl;
  for (auto& obj : objs) {
    std::cout << obj->format() << std::endl;
  }
  std::cout << std::endl;
}
