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

#ifndef _YOGI_INTERNAL_SFINAE_H
#define _YOGI_INTERNAL_SFINAE_H

//! \file
//!
//! Helpers related to Substitution Failure Is Not An Error.

#include <type_traits>

#define _YOGI_DEFINE_SFINAE_METHOD_TESTER(class_name, test_expression)                       \
  namespace detail {                                                                         \
  template <typename T>                                                                      \
  struct class_name {                                                                        \
   private:                                                                                  \
    template <typename U>                                                                    \
    static auto test(int) -> decltype(std::declval<U>() test_expression, std::true_type());  \
                                                                                             \
    template <typename>                                                                      \
    static std::false_type test(...);                                                        \
                                                                                             \
   public:                                                                                   \
    static constexpr bool value = std::is_same<decltype(test<T>(0)), std::true_type>::value; \
  };                                                                                         \
  }  // namespace internal

#endif  // _YOGI_INTERNAL_SFINAE_H
