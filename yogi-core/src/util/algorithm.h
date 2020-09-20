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

#include <algorithm>

template <typename Container, typename T>
inline typename Container::iterator find(Container& container, const T& value) {
  return std::find(container.begin(), container.end(), value);
}

template <typename Container, typename T>
inline typename Container::const_iterator find(const Container& container, const T& value) {
  return std::find(container.begin(), container.end(), value);
}

template <typename Container, typename Predicate>
inline typename Container::iterator find_if(Container& container, Predicate pred) {
  return std::find_if(container.begin(), container.end(), pred);
}

template <typename Container, typename T>
inline bool contains(Container& container, const T& value) {
  return find(container, value) != container.end();
}

template <typename Container, typename Predicate>
inline bool contains_if(Container& container, Predicate pred) {
  return find_if(container, pred) != container.end();
}

template <typename Container, typename T>
inline typename Container::iterator remove_erase(Container& container, const T& value) {
  return container.erase(std::remove(container.begin(), container.end(), value), container.end());
}

template <typename Container, typename Predicate>
inline typename Container::iterator remove_erase_if(Container& container, Predicate pred) {
  return container.erase(std::remove_if(container.begin(), container.end(), pred), container.end());
}
