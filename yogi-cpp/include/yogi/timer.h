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

#ifndef _YOGI_TIMER_H
#define _YOGI_TIMER_H

//! \file
//!
//! Timer implementation.

#include "context.h"
#include "duration.h"
#include "enums.h"
#include "object.h"

#include <functional>
#include <memory>

namespace yogi {

class Timer;

/// Shared pointer to a timer.
using TimerPtr = std::shared_ptr<Timer>;

////////////////////////////////////////////////////////////////////////////////
/// Simple timer implementation.
////////////////////////////////////////////////////////////////////////////////
class Timer : public ObjectT<Timer> {
 public:
  /// Handler function to be called once a timer expires.
  ///
  /// \param res %Result of the wait operation.
  using HandlerFn = std::function<void(const Result& res)>;

  /// Creates a timer.
  ///
  /// \param context The context to use.
  ///
  /// \returns The created timer.
  static TimerPtr create(ContextPtr context) {
    return TimerPtr(new Timer(context));
  }

  /// Starts the timer.
  ///
  /// If the timer is already running, the timer will be canceled first,
  /// as if cancel() were called explicitly.
  ///
  /// \param duration Time when the timer expires.
  /// \param fn       Handler function to call after the given time passed.
  void start_async(const Duration& duration, HandlerFn fn) {
    struct CallbackData {
      HandlerFn fn;
    };

    auto data = std::make_unique<CallbackData>();
    data->fn  = fn;

    int res = detail::YOGI_TimerStartAsync(
        handle(), duration.nanoseconds_count(),
        [](int res, void* userarg) {
          auto data = std::unique_ptr<CallbackData>(static_cast<CallbackData*>(userarg));
          if (!data->fn) return;

          detail::with_error_code_to_result(res, [&](const auto& result) { data->fn(result); });
        },
        data.get());

    detail::check_error_code(res);
    data.release();
  }

  /// Cancels the timer.
  ///
  /// Canceling the timer will result in the handler function registered via
  /// start() to be called with a cancellation error. Note that if the handler
  /// is already scheduled for execution, it will be called without an error.
  ///
  /// \returns True if the timer was canceled successfully and false otherwise
  ///          (i.e. the timer has not been started or it already expired).
  bool cancel() {
    int res = detail::YOGI_TimerCancel(handle());
    return detail::false_if_specific_error_else_throw(res, ErrorCode::kTimerExpired);
  }

 private:
  Timer(ContextPtr context)
      : ObjectT(detail::call_api_create(detail::YOGI_TimerCreate, get_foreign_handle(context)), {context}) {
  }
};

}  // namespace yogi

#endif  // _YOGI_TIMER_H
