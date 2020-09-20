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

#include <src/api/errors.h>
#include <src/system/process.h>

#if defined(_WIN32)
#  include <windows.h>  // Needs to go first

#  include <process.h>
#  include <processthreadsapi.h>
#elif defined(__APPLE__)
#  include <pthread.h>
#  include <unistd.h>
#else
#  include <sys/syscall.h>
#  include <sys/types.h>
#  include <unistd.h>
#endif

int get_process_id() {
#ifdef _WIN32
  return _getpid();
#else
  return getpid();
#endif
}

int get_thread_id() {
#if defined(_WIN32)
  auto id = ::GetCurrentThreadId();
#elif defined(__APPLE__)
  std::uint64_t id;
  pthread_threadid_np(NULL, &id);
#else
  auto id = syscall(SYS_gettid);
#endif

  return static_cast<int>(id);
}
