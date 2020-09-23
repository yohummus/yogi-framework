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

// MSVC-specific
#ifdef _MSC_VER
#  pragma warning(disable : 4250 4503 4127)
#endif

// Include the main DLL header file for the error codes
#ifndef YOGI_API
#  ifdef _MSC_VER
#    define YOGI_API __declspec(dllexport)
#  else
#    define YOGI_API __attribute__((visibility("default")))
#  endif
#endif

#include <yogi_core.h>

// Debug & development
#ifdef _MSC_VER
#  define YOGI_DEBUG_BREAK __debugbreak();
#else
#  include <cassert>
#  define YOGI_DEBUG_BREAK assert(false);
#endif

#ifndef NDEBUG
#  include <iostream>
#  include <string>
#  define YOGI_TRACE                                                             \
    {                                                                            \
      std::string file(__FILE__);                                                \
      file = file.substr(file.find_last_of("\\/") + 1);                          \
      std::cerr << file << ":" << __LINE__ << ": " << __FUNCTION__ << std::endl; \
    }

#  define YOGI_TRACE_VAL(val)                                                                                \
    {                                                                                                        \
      std::string file(__FILE__);                                                                            \
      file = file.substr(file.find_last_of("\\/") + 1);                                                      \
      std::cerr << file << ":" << __LINE__ << ": " << __FUNCTION__ << ": " #val " = " << (val) << std::endl; \
    }

#  define YOGI_ASSERT(x)                                                                                   \
    {                                                                                                      \
      if (!(x)) {                                                                                          \
        std::cerr << "ASSERTION \"" #x "\" in " << __FILE__ << ":" << __LINE__ << " FAILED." << std::endl; \
        YOGI_DEBUG_BREAK;                                                                                  \
      }                                                                                                    \
    }

#  define YOGI_DEBUG_ONLY(...) __VA_ARGS__
#else
#  define YOGI_ASSERT(x)
#  define YOGI_DEBUG_ONLY(...)
#endif

#define YOGI_NEVER_REACHED YOGI_ASSERT(std::string("NEVER") == "REACHED")
#define YOGI_UNUSED(x) ((void)(x))
