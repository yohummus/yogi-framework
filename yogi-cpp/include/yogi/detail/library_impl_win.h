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

#ifndef _YOGI_DETAIL_LIBRARY_IMPL_WIN_H
#define _YOGI_DETAIL_LIBRARY_IMPL_WIN_H

//! \file
//!
//! Windows-specific parts for handling the Yogi-Core library.

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#  undef WIN32_LEAN_AND_MEAN
#else
#  include <Windows.h>
#endif

#include <string>

namespace yogi {
namespace detail {

class LibraryImpl {
 protected:
  typedef HMODULE LibraryHandle;
  typedef FARPROC ProcAddress;

  static const char* get_filename() {
#pragma warning(push)
#pragma warning(disable : 4996)  // 'getenv': This function or variable may be unsafe.
    const char* s = std::getenv("YOGI_CORE_LIBRARY");
#pragma warning(pop)

    if (!s) {
      s = sizeof(void*) == 4 ? "yogi-core-x86.dll" : "yogi-core-x64.dll";
    }

    return s;
  }

  static LibraryHandle load_library(const char* filename) {
    return ::LoadLibraryA(filename);
  }

  static ProcAddress get_proc_address(LibraryHandle handle, const char* name) {
    return ::GetProcAddress(handle, name);
  }

  static std::string get_last_error_string() {
    std::string s;
    auto err = ::GetLastError();
    if (err) {
      LPSTR msg = nullptr;
      auto n    = ::FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err,
          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPTSTR>(&msg), 0, nullptr);
      if (n) {
        s.assign(msg, msg + n);
        ::LocalFree(msg);
      }
    }

    return s;
  }
};

}  // namespace detail
}  // namespace yogi

#endif  // _YOGI_DETAIL_LIBRARY_IMPL_WIN_H
