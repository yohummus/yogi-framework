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
#include <src/objects/signal_set.h>

YOGI_API int YOGI_RaiseSignal(int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg), void* userarg) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_FLAGS(signal, YOGI_SIG_ALL);
  CHECK_EXACTLY_ONE_BIT_SET(signal);

  SignalSet::raise_signal(signal, sigarg, fn, userarg);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_SignalSetCreate(void** sigset, void* context, int signals) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(sigset != nullptr);
  CHECK_PARAM(context != nullptr);
  CHECK_FLAGS(signals, YOGI_SIG_ALL);

  auto ctx = ObjectRegister::get<Context>(context);
  auto set = SignalSet::create(ctx, signals);
  *sigset  = ObjectRegister::register_object(set);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_SignalSetAwaitSignalAsync(void* sigset, void (*fn)(int res, int sig, void* sigarg, void* userarg),
                                            void* userarg) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(sigset != nullptr);
  CHECK_PARAM(fn != nullptr);

  auto set = ObjectRegister::get<SignalSet>(sigset);
  set->await_async(fn, userarg);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_SignalSetCancelAwaitSignal(void* sigset) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(sigset != nullptr);

  auto set = ObjectRegister::get<SignalSet>(sigset);
  if (!set->cancel_await()) {
    throw Error{YOGI_ERR_OPERATION_NOT_RUNNING};
  }

  END_CHECKED_API_FUNCTION
}
