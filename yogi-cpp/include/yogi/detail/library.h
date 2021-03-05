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

#ifndef _YOGI_DETAIL_LIBRARY_H
#define _YOGI_DETAIL_LIBRARY_H

//! \file
//!
//! Helpers for handling the Yogi Core library.

#ifdef _WIN32
#  include "library_impl_win.h"
#else
#  include "library_impl_unix.h"
#endif

#include "macros.h"

#include <cstdlib>
#include <iostream>
#include <string>

namespace yogi {
namespace detail {

class Library final : LibraryImpl {
 public:
  template <typename Fn>
  static Fn get_function_address(const char* name) {
    using namespace std::string_literals;

    if (!lib_handle_) {
      lib_handle_ = load_yogi_core();
      check_version_compatibility();
    }

    auto addr = get_proc_address(lib_handle_, name);
    if (!addr) {
      die_with_error_message("Could not find function "s + name + " in the loaded yogi-core library");
    }

    return reinterpret_cast<Fn>(addr);
  }

 private:
  static LibraryHandle lib_handle_;

  static LibraryHandle load_yogi_core() {
    using namespace std::string_literals;

    auto filename = get_filename();
    auto handle   = load_library(filename);
    if (!handle) {
      die_with_error_message("Could not load "s + filename);
    }

    return handle;
  }

  static void check_version_compatibility() {
    // :CODEGEN_BEGIN: gen_cpp_version
    const char* bindings_version = "0.0.1-alpha";
    // :CODEGEN_END:

    auto compat_fn = get_function_address<int (*)(const char*)>("YOGI_CheckBindingsCompatibility");
    auto error_fn  = get_function_address<const char* (*)()>("YOGI_GetLastErrorDetails");

    if (compat_fn(bindings_version) != 0) {
      std::cerr << "FATAL: " << error_fn() << std::endl;
      std::exit(1);
    }
  }

  static void die_with_error_message(std::string prefix) {
    std::cerr << "FATAL: " << prefix << ": " << get_last_error_string() << std::endl;
    std::exit(1);
  }
};

_YOGI_WEAK_SYMBOL Library::LibraryHandle Library::lib_handle_;

}  // namespace detail
}  // namespace yogi

#endif  // _YOGI_DETAIL_LIBRARY_H
