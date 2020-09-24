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

#ifndef _YOGI_DETAIL_LIBRARY_IMPL_UNIX_H
#define _YOGI_DETAIL_LIBRARY_IMPL_UNIX_H

//! \file
//!
//! Unix-specific parts for handling the Yogi-Core library.

#include <dlfcn.h>
#include <string>

namespace yogi {
namespace detail {

class LibraryImpl {
 protected:
  typedef void* LibraryHandle;
  typedef void* ProcAddress;

  static const char* get_filename() {
    const char* s = std::getenv("YOGI_CORE_LIBRARY");
    if (s) return s;

#ifdef __APPLE__
    return "libyogi-core.dylib";
#else
    return "libyogi-core.so";
#endif
  }

  static LibraryHandle load_library(const char* filename) {
    dlerror();  // Clear any previous errors
    return dlopen(filename, RTLD_NOW);
  }

  static ProcAddress get_proc_address(LibraryHandle handle, const char* name) {
    dlerror();  // Clear any previous errors
    return dlsym(handle, name);
  }

  static std::string get_last_error_string() {
    std::string s;
    auto err = dlerror();
    if (err) {
      s.assign(err);
    }

    return s;
  }
};

}  // namespace detail
}  // namespace yogi

#endif  // _YOGI_DETAIL_LIBRARY_IMPL_UNIX_H
