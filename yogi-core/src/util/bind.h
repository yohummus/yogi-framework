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

#include <functional>
#include <memory>
#include <utility>

template <typename Obj, typename... Args>
inline std::function<void(Args...)> bind_weak(void (Obj::*fn)(Args...), Obj* obj) {
  auto shared_obj = obj->shared_from_this();
  auto weak_obj   = std::weak_ptr<typename decltype(shared_obj)::element_type>{shared_obj};

  std::function<void(Args...)> wrapper = [weak_obj, fn](Args&&... args) {
    auto shared_obj = std::static_pointer_cast<Obj>(weak_obj.lock());
    if (!shared_obj) return;

    ((*shared_obj).*fn)(std::forward<Args>(args)...);
  };

  return wrapper;
}

template <typename Obj, typename... Args>
inline std::function<void(Args...)> bind_strong(void (Obj::*fn)(Args...), Obj* obj) {
  auto p                               = std::static_pointer_cast<Obj>(obj->shared_from_this());
  std::function<void(Args...)> wrapper = [p, fn](Args&&... args) { ((*p).*fn)(std::forward<Args>(args)...); };

  return wrapper;
}
