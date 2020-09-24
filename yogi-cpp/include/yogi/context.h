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

#ifndef _YOGI_CONTEXT_H
#define _YOGI_CONTEXT_H

//! \file
//!
//! Contexts (schedulers for the execution of asynchronous operations).

#include "detail/api.h"
#include "detail/error_helpers.h"
#include "duration.h"
#include "object.h"

#include <chrono>
#include <functional>
#include <memory>

namespace yogi {
namespace detail {

inline long long to_core_duration(const Duration& duration) {
  if (duration < Duration::kZero) {
    throw FailureException(ErrorCode::kInvalidParam);
  }

  return duration.is_finite() ? duration.nanoseconds_count() : -1;
}

}  // namespace detail

class Context;

/// Shared pointer to a context.
using ContextPtr = std::shared_ptr<Context>;

////////////////////////////////////////////////////////////////////////////////
/// Scheduler for the execution of asynchronous operations.
///
/// Once an asynchronous operation finishes, the corresponding handler function
/// is added to the context's event queue and executed through once of the
/// Poll... or Run... functions.
////////////////////////////////////////////////////////////////////////////////
class Context : public ObjectT<Context> {
 public:
  using HandlerFn = std::function<void()>;

  /// Creates the context
  ///
  /// \returns The created context.
  static ContextPtr create() {
    return ContextPtr(new Context());
  }

  /// Runs the context's event processing loop to execute ready handlers.
  ///
  /// This function runs handlers (internal and user-supplied such as functions
  /// registered through post()) that are ready to run, without blocking, until
  /// the stop() function has been called or there are no more ready handlers.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  ///
  /// \returns Number of executed handlers.
  int poll() {
    int count;
    int res = detail::YOGI_ContextPoll(handle(), &count);
    detail::check_error_code(res);
    return count;
  }

  /// Runs the context's event processing loop to execute at most one ready
  /// handler.
  ///
  /// This function runs at most one handler (internal and user-supplied such as
  /// functions registered through post()) that are ready to run, without
  /// blocking.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  ///
  /// \returns Number of executed handlers.
  int poll_one() {
    int count;
    int res = detail::YOGI_ContextPollOne(handle(), &count);
    detail::check_error_code(res);
    return count;
  }

  /// Runs the context's event processing loop for the specified duration.
  ///
  /// This function blocks while running the context's event processing loop and
  /// calling dispatched handlers (internal and user-supplied such as functions
  /// registered through post()) for the specified duration unless stop() is
  /// called within that time.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  ///
  /// \param duration Duration.
  ///
  /// \returns Number of executed handlers.
  int run(const Duration& duration) {
    int count;
    int res = detail::YOGI_ContextRun(handle(), &count, detail::to_core_duration(duration));
    detail::check_error_code(res);
    return count;
  }

  /// Runs the context's event processing loop.
  ///
  /// This function blocks while running the context's event processing loop and
  /// calling dispatched handlers (internal and user-supplied such as functions
  /// registered through post()) until stop() is called.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  ///
  /// \returns Number of executed handlers.
  int run() {
    return run(Duration::kInf);
  }

  /// Runs the context's event processing loop for the specified duration to
  /// execute at most one handler.
  ///
  /// This function blocks while running the context's event processing loop and
  /// calling dispatched handlers (internal and user-supplied such as functions
  /// registered through post()) for the specified duration until a single
  /// handler function has been executed, unless stop() is called within that
  /// time.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  ///
  /// \param duration Duration.
  ///
  /// \returns Number of executed handlers.
  int run_one(const Duration& duration) {
    int count;
    int res = detail::YOGI_ContextRunOne(handle(), &count, detail::to_core_duration(duration));
    detail::check_error_code(res);
    return count;
  }

  /// Runs the context's event processing loop to execute at most one handler.
  ///
  /// This function blocks while running the context's event processing loop and
  /// calling dispatched handlers (internal and user-supplied such as functions
  /// registered through post()) until a single handler function has been
  /// executed, unless stop() is called.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  ///
  /// \returns Number of executed handlers.
  int run_one() {
    return run_one(Duration::kInf);
  }

  /// Starts an internal thread for running the context's event processing loop.
  ///
  /// This function starts a threads that runs the context's event processing
  /// loop in the background. It relieves the user from having to start a thread
  /// and calling the appropriate Run... or Poll... functions themself. The
  /// thread can be stopped using stop().
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  void run_in_background() {
    int res = detail::YOGI_ContextRunInBackground(handle());
    detail::check_error_code(res);
  }

  /// Stops the context's event processing loop.
  ///
  /// This function signals the context to stop running its event processing
  /// loop. This causes Run... functions to return as soon as possible and it
  /// terminates the thread started via run_in_background().
  void stop() {
    int res = detail::YOGI_ContextStop(handle());
    detail::check_error_code(res);
  }

  /// Blocks until the context's event processing loop is being run or until the
  /// specified timeout is reached.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  ///
  /// \param duration Maximum time to wait.
  ///
  /// \returns True if the context's event processing loop is running within the
  ///          specified duration and false otherwise.
  bool wait_for_running(const Duration& duration) {
    int res = detail::YOGI_ContextWaitForRunning(handle(), detail::to_core_duration(duration));
    if (res == static_cast<int>(ErrorCode::kTimeout)) return false;
    detail::check_error_code(res);
    return true;
  }

  /// Blocks until the context's event processing loop is being run.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  void wait_for_running() {
    wait_for_running(Duration::kInf);
  }

  /// Blocks until no thread is running the context's event processing
  /// loop or until the specified timeout is reached.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  ///
  /// \param duration Maximum time to wait.
  ///
  /// \returns True if the context's event processing loop is not running within
  ///          the specified duration and false otherwise.
  bool wait_for_stopped(const Duration& duration) {
    int res = detail::YOGI_ContextWaitForStopped(handle(), detail::to_core_duration(duration));
    if (res == static_cast<int>(ErrorCode::kTimeout)) return false;
    detail::check_error_code(res);
    return true;
  }

  /// Blocks until no thread is running the context's event processing loop.
  ///
  /// This function must be called from outside any handler functions that are
  /// being executed through the context.
  void wait_for_stopped() {
    wait_for_stopped(Duration::kInf);
  }

  /// Adds the given function to the context's event processing queue to
  /// be executed and returns immediately.
  ///
  /// The handler \p fn will only be executed after this function returns and
  /// only by a thread running the context's event processing loop.
  ///
  /// \param fn Function to call from within the context.
  void post(HandlerFn fn) {
    auto fn_ptr         = new HandlerFn(fn);
    static auto wrapper = [](void* userarg) {
      std::unique_ptr<HandlerFn> fn_ptr(static_cast<HandlerFn*>(userarg));
      (*fn_ptr)();
    };

    int res = detail::YOGI_ContextPost(handle(), wrapper, fn_ptr);
    detail::check_error_code(res);
  }

 private:
  Context() : ObjectT(detail::call_api_create(detail::YOGI_ContextCreate), {}) {
  }
};

}  // namespace yogi

#endif  // _YOGI_CONTEXT_H
