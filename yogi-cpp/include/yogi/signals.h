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

#ifndef _YOGI_SIGNALS_H
#define _YOGI_SIGNALS_H

//! \file
//!
//! Signal handling.

#include "context.h"
#include "enums.h"
#include "object.h"

#include <functional>
#include <memory>

namespace yogi {
namespace detail {

class SigargWrapper {
 public:
  virtual ~SigargWrapper() {
  }
};

template <typename T>
class SigargWrapperT : public SigargWrapper {
 public:
  SigargWrapperT(T&& data) : data_(std::forward<T>(data)) {
  }

  T& data() {
    return data_;
  }

 private:
  T data_;
};

}  // namespace detail

/// Handler function to be called once the signal raised via raise_signal() has
/// been delivered to all signal sets.
using RaiseSignalFinishedFn = std::function<void()>;

/// \addtogroup freefn
/// @{

/// Raises a signal.
///
/// Signals in Yogi are intended to be used similarly to POSIX signals. They
/// have to be raised explicitly by the user (e.g. when receiving an actual
/// POSIX signal like SIGINT) via this function. A signal will be received by
/// all signal sets containing that signal.
///
/// The cleanup handler \p fn will be called once all signal handlers have been
/// called.
///
/// \note
///   The cleanup handler \p fn can get called either from within the
///   raise_signal() function or from any context within the program.
///
/// \param signal The signal to raise.
/// \param fn     Function to call after all signal sets have been notified.
inline void raise_signal(Signals signal, RaiseSignalFinishedFn fn = {}) {
  struct CallbackData {
    RaiseSignalFinishedFn fn;
  };

  auto data = std::make_unique<CallbackData>();
  data->fn  = fn;

  int res = detail::YOGI_RaiseSignal(
      static_cast<int>(signal), nullptr,
      [](void*, void* userarg) {
        auto data = std::unique_ptr<CallbackData>(static_cast<CallbackData*>(userarg));
        if (!data->fn) return;
        data->fn();
      },
      data.get());

  detail::check_error_code(res);
  data.release();
}

/// @} freefn

/// Handler function to be called once the signal raised via raise_signal() has
/// been delivered to all signal sets.
///
/// \tparam Sigarg Type of the \p sigarg signal argument passed to
///                raise_signal_with_arg().
///
/// \param sigarg Signal argument passed to raise_signal_with_arg(); will be
///               nullptr if no signal argument was specified when raising the
///               signal.
template <typename Sigarg>
using RaiseSignalFinishedFnT = std::function<void(Sigarg* sigarg)>;

/// \addtogroup freefn
/// @{

/// Raises a signal.
///
/// Signals in Yogi are intended to be used similarly to POSIX signals. They
/// have to be raised explicitly by the user (e.g. when receiving an actual
/// POSIX signal like SIGINT) via this function. A signal will be received by
/// all signal sets containing that signal.
///
/// The \p sigarg parameter can be used to deliver user-defined data to the
/// signal handlers.
///
/// The cleanup handler \p fn will be called once all signal handlers have been
/// called.
///
/// \note
///   The cleanup handler \p fn can get called either from within the
///   raise_signal() function or from any context within the program.
///
/// \tparam Sigarg Type of the \p sigarg signal argument.
///
/// \param signal The signal to raise.
/// \param sigarg Signal argument to pass to the signal handlers.
/// \param fn     Function to call after all signal sets have been notified.
template <typename Sigarg>
inline void raise_signal_with_arg(Signals signal, Sigarg&& sigarg, RaiseSignalFinishedFnT<Sigarg> fn = {}) {
  struct CallbackData {
    detail::SigargWrapperT<Sigarg> sigarg_wrapper;
    RaiseSignalFinishedFnT<Sigarg> fn;

    CallbackData(Sigarg&& sigarg) : sigarg_wrapper(std::forward<Sigarg>(sigarg)) {
    }
  };

  auto data = std::make_unique<CallbackData>(std::forward<Sigarg>(sigarg));
  data->fn  = fn;

  detail::SigargWrapper* sigarg_wrapper_p = &data->sigarg_wrapper;

  int res = detail::YOGI_RaiseSignal(
      static_cast<int>(signal), sigarg_wrapper_p,
      [](void* sigarg, void* userarg) {
        auto data = std::unique_ptr<CallbackData>(static_cast<CallbackData*>(userarg));
        if (!data->fn) return;

        auto sigarg_wrapper_p   = static_cast<detail::SigargWrapper*>(sigarg);
        auto sigarg_wrapper_ptr = static_cast<detail::SigargWrapperT<Sigarg>*>(sigarg_wrapper_p);

        data->fn(&sigarg_wrapper_ptr->data());
      },
      data.get());

  detail::check_error_code(res);
  data.release();
}

/// @} freefn

class SignalSet;

/// Shared pointer to a signal set.
using SignalSetPtr = std::shared_ptr<SignalSet>;

////////////////////////////////////////////////////////////////////////////////
/// A set of signals to receive when raised via raise_signal() and
/// raise_signal_with_arg().
///
/// \note
///   The signals are queued until they can be delivered by means of calls to
///   await_signal().
////////////////////////////////////////////////////////////////////////////////
class SignalSet : public ObjectT<SignalSet> {
 public:
  /// Handler function used for the await_signal() function.
  ///
  /// \param res    %Result of the wait operation.
  /// \param signal The raised signal.
  using SignalHandlerFn = std::function<void(const Result& res, Signals signal)>;

  /// Handler function used for the await_signal<Sigarg>() function.
  ///
  /// \tparam Type of the signal argument passed to raise_signal_with_arg().
  ///
  /// \param res    %Result of the wait operation.
  /// \param signal The raised signal.
  /// \param sigarg Value of the signal argument passed to raise_signal_with_arg();
  ///               will be nullptr if no signal argument was specified when
  ///               raising the signal.
  template <typename Sigarg>
  using SignalHandlerFnT = std::function<void(const Result& res, Signals signal, Sigarg* sigarg)>;

  /// Create a signal set.
  ///
  /// \param context The context to use.
  /// \param signals Signals in the signal set.
  ///
  /// \returns The created signal set.
  static SignalSetPtr create(ContextPtr context, Signals signals) {
    return SignalSetPtr(new SignalSet(context, signals));
  }

  /// Waits for a signal to be raised.
  ///
  /// The handler \p fn will be called after one of the signals in the set is
  /// caught.
  ///
  /// \param fn Handler function to call.
  void await_signal(SignalHandlerFn fn) {
    struct CallbackData {
      SignalHandlerFn fn;
    };

    auto data = std::make_unique<CallbackData>();
    data->fn  = fn;

    int res = detail::YOGI_SignalSetAwaitSignalAsync(
        handle(),
        [](int res, int sig, void*, void* userarg) {
          auto data = std::unique_ptr<CallbackData>(static_cast<CallbackData*>(userarg));
          if (!data->fn) return;

          detail::with_error_code_to_result(res,
                                            [&](const auto& result) { data->fn(result, static_cast<Signals>(sig)); });
        },
        data.get());

    detail::check_error_code(res);
    data.release();
  }

  /// Waits for a signal to be raised.
  ///
  /// The handler \p fn will be called after one of the signals in the set is
  /// caught. The signal argument will be casted to the given template type T.
  /// If casting to T fails, then an exception will be thrown. In order to
  /// receive any type of signal argument, chose the default Sigarg type for T.
  ///
  /// \tparam Sigarg Type of the signal argument passed to raise_signal_with_arg().
  ///
  /// \param fn Handler function to call.
  template <typename Sigarg>
  void await_signal(SignalHandlerFnT<Sigarg> fn) {
    struct CallbackData {
      SignalHandlerFnT<Sigarg> fn;
    };

    auto data = std::make_unique<CallbackData>();
    data->fn  = fn;

    int res = detail::YOGI_SignalSetAwaitSignalAsync(
        handle(),
        [](int res, int sig, void* sigarg, void* userarg) {
          auto data = std::unique_ptr<CallbackData>(static_cast<CallbackData*>(userarg));
          if (!data->fn) return;

          auto sigarg_wrapper_p = static_cast<detail::SigargWrapper*>(sigarg);

          Sigarg* typed_sigarg = nullptr;
          if (sigarg_wrapper_p) {
            // Next line throws if Sigarg is incompatible with the type used
            // for raise_signal_with_arg()
            auto& dummy = dynamic_cast<detail::SigargWrapperT<Sigarg>&>(*sigarg_wrapper_p);
            (void)(dummy);  // Unused

            auto sigarg_wrapper_ptr = static_cast<detail::SigargWrapperT<Sigarg>*>(sigarg_wrapper_p);

            typed_sigarg = &sigarg_wrapper_ptr->data();
          }

          detail::with_error_code_to_result(
              res, [&](const auto& result) { data->fn(result, static_cast<Signals>(sig), typed_sigarg); });
        },
        data.get());

    detail::check_error_code(res);
    data.release();
  }

  /// Cancels waiting for a signal.
  ///
  /// This causes the handler function registered via await_signal() to be
  /// called with a cancellation error.
  ///
  /// \return True if the wait operation was cancelled successfully.
  bool cancel_await_signal() {
    int res = detail::YOGI_SignalSetCancelAwaitSignal(handle());
    return detail::false_if_specific_error_else_throw(res, ErrorCode::kOperationNotRunning);
  }

 private:
  SignalSet(ContextPtr context, Signals signals)
      : ObjectT(detail::call_api_create(detail::YOGI_SignalSetCreate, get_foreign_handle(context),
                                        static_cast<int>(signals)),
                {context}) {
  }
};

}  // namespace yogi

#endif  // _YOGI_SIGNALS_H
