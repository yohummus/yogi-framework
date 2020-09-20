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
#include <src/objects/context.h>
#include <src/util/time.h>

YOGI_API int YOGI_ContextCreate(void** context) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);

  auto ctx = Context::create();
  *context = ObjectRegister::register_object(ctx);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ContextPoll(void* context, int* count) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);

  auto ctx = ObjectRegister::get<Context>(context);

  int n = ctx->poll();
  if (count) *count = n;

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ContextPollOne(void* context, int* count) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);

  auto ctx = ObjectRegister::get<Context>(context);

  int n = ctx->poll_one();
  if (count) *count = n;

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ContextRun(void* context, int* count, long long duration) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);
  CHECK_PARAM(duration >= -1);

  auto ctx = ObjectRegister::get<Context>(context);

  int n = ctx->run(Duration{duration});
  if (count) *count = n;

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ContextRunOne(void* context, int* count, long long duration) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);
  CHECK_PARAM(duration >= -1);

  auto ctx = ObjectRegister::get<Context>(context);

  int n = ctx->run_one(Duration{duration});
  if (count) *count = n;

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ContextRunInBackground(void* context) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);

  auto ctx = ObjectRegister::get<Context>(context);
  ctx->run_in_background();

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ContextStop(void* context) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);

  auto ctx = ObjectRegister::get<Context>(context);
  ctx->stop();

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ContextWaitForRunning(void* context, long long duration) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);
  CHECK_PARAM(duration >= -1);

  auto ctx = ObjectRegister::get<Context>(context);
  if (!ctx->wait_for_running(Duration{duration})) {
    throw Error{YOGI_ERR_TIMEOUT};
  }

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ContextWaitForStopped(void* context, long long duration) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);
  CHECK_PARAM(duration >= -1);

  auto ctx = ObjectRegister::get<Context>(context);
  if (!ctx->wait_for_stopped(Duration{duration})) {
    throw Error{YOGI_ERR_TIMEOUT};
  }

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ContextPost(void* context, void (*fn)(void* userarg), void* userarg) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(context != nullptr);
  CHECK_PARAM(fn != nullptr);

  auto ctx = ObjectRegister::get<Context>(context);
  ctx->post([=] { fn(userarg); });

  END_CHECKED_API_FUNCTION
}
