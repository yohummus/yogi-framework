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

#pragma once

#include <src/config.h>

#include <src/api/constants.h>
#include <src/api/errors.h>

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

enum class ObjectType {
  kDummy,  // For testing
  kContext,
  kLogger,
  kTimer,
  kBranch,
  kConfiguration,
  kSignalSet,
  kWebServer,
  kWebRoute,
  kWebProcess,
};

typedef void* ObjectHandle;

class ExposedObject : public std::enable_shared_from_this<ExposedObject> {
 public:
  ExposedObject()          = default;
  virtual ~ExposedObject() = default;

  virtual ObjectType type() const = 0;

  const std::string& type_name() const;
  std::string format(std::string fmt = constants::kDefaultObjectFormat) const;
  ObjectHandle handle() { return static_cast<ObjectHandle>(this); }

  template <typename TO>
  std::shared_ptr<TO> cast() {
    // Specialization for ExposedObject type is below this class
    if (type() != TO::static_type()) {
      throw Error{YOGI_ERR_WRONG_OBJECT_TYPE};
    }

    return std::static_pointer_cast<TO>(this->shared_from_this());
  }

 private:
  // noncopyable
  ExposedObject(const ExposedObject&) = delete;
  void operator=(const ExposedObject&) = delete;
};

template <>
inline std::shared_ptr<ExposedObject> ExposedObject::cast() {
  return std::static_pointer_cast<ExposedObject>(this->shared_from_this());
}

typedef std::shared_ptr<ExposedObject> ObjectPtr;

template <typename TO, ObjectType TK>
class ExposedObjectT : public ExposedObject {
 public:
  static constexpr ObjectType static_type() { return TK; }

  template <typename... TArgs>
  static std::shared_ptr<TO> create(TArgs&&... args) {
    return std::make_shared<TO>(std::forward<TArgs>(args)...);
  }

  virtual ObjectType type() const override { return static_type(); };

  std::shared_ptr<TO> make_shared_ptr() { return std::static_pointer_cast<TO>(this->shared_from_this()); }
  std::weak_ptr<TO> make_weak_ptr() { return {make_shared_ptr()}; }
};

class ObjectRegister {
 public:
  template <typename TO = ExposedObject>
  static std::shared_ptr<TO> get(ObjectHandle handle) {
    std::lock_guard<std::mutex> lock{mutex_};
    auto obj = objects_.find(handle);

    if (obj == objects_.end()) {
      throw Error{YOGI_ERR_INVALID_HANDLE};
    }

    YOGI_ASSERT(obj->second->handle() == handle);
    return obj->second->cast<TO>();
  }

  template <typename TO, typename TP>
  static std::vector<std::shared_ptr<TO>> get_matching(TP predicate) {
    std::lock_guard<std::mutex> lock{mutex_};

    std::vector<std::shared_ptr<TO>> vec;
    for (auto& obj : objects_) {
      if (obj.second->type() == TO::static_type()) {
        auto typed_obj = std::static_pointer_cast<TO>(obj.second);
        if (predicate(typed_obj)) {
          vec.emplace_back(std::move(typed_obj));
        }
      }
    }

    return vec;
  }

  template <typename TO>
  static std::vector<std::shared_ptr<TO>> get_all() {
    return get_matching<TO>([](auto&) { return true; });
  }

  static ObjectHandle register_object(const ObjectPtr& obj);
  static void destroy(ObjectHandle handle);
  static void destroy_all();

 private:
  typedef std::unordered_map<ObjectHandle, ObjectPtr> ObjectsMap;
  typedef std::vector<ObjectPtr> ObjectsVector;

  template <typename TO>
  static std::vector<std::shared_ptr<TO>> get_all(const ObjectsVector& objs) {
    std::vector<std::shared_ptr<TO>> vec;
    for (auto& obj : objs) {
      if (obj->type() == TO::static_type()) {
        vec.push_back(std::static_pointer_cast<TO>(obj));
      }
    }
    return vec;
  }

  static ObjectsVector take_objects();
  static bool remove_unused_objects(ObjectsVector* objs);
  static void stop_all_contexts(const ObjectsVector& objs);
  static bool poll_all_contexts(const ObjectsVector& objs);
  static void print_objects_still_in_use(const ObjectsVector& objs);

  static std::mutex mutex_;
  static ObjectsMap objects_;
};
