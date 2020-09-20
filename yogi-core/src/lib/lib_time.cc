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

#include <src/lib/lib_helpers.h>
#include <src/objects/timer.h>
#include <src/util/time.h>

YOGI_API int YOGI_GetCurrentTime(long long* timestamp) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(timestamp != nullptr);

  *timestamp = Timestamp::now().ns_since_epoch();

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_FormatTime(long long timestamp, const char** str, int* strsize, const char* timefmt) {
  BEGIN_CHECKED_API_FUNCTION

  auto t = Timestamp{timestamp};
  auto s = t.format(timefmt);
  set_api_buffer(std::move(s), str, strsize);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ParseTime(long long* timestamp, const char* str, const char* timefmt) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(timestamp != nullptr);
  CHECK_PARAM(str != nullptr);

  *timestamp = Timestamp::parse(str, timefmt).ns_since_epoch();

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_FormatDuration(long long dur, int neg, const char** str, int* strsize, const char* durfmt,
                                 const char* inffmt) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(dur >= -1);
  CHECK_PARAM(neg == YOGI_TRUE || neg == YOGI_FALSE);

  auto d = Duration{dur, neg == YOGI_TRUE};
  auto s = d.format(durfmt, inffmt);
  set_api_buffer(std::move(s), str, strsize);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_TimerCreate(void** timer, void* context) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(timer != nullptr);
  CHECK_PARAM(context != nullptr);

  auto ctx = ObjectRegister::get<Context>(context);
  auto tmr = Timer::create(ctx);
  *timer   = ObjectRegister::register_object(tmr);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_TimerStartAsync(void* timer, long long duration, void (*fn)(int res, void* userarg), void* userarg) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(timer != nullptr);
  CHECK_PARAM(duration >= -1);
  CHECK_PARAM(fn != nullptr);

  auto tmr = ObjectRegister::get<Timer>(timer);
  tmr->start_async(Duration{duration}, fn, userarg);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_TimerCancel(void* timer) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(timer != nullptr);

  auto tmr = ObjectRegister::get<Timer>(timer);
  if (!tmr->cancel()) {
    throw Error{YOGI_ERR_TIMER_EXPIRED};
  }

  END_CHECKED_API_FUNCTION
}
