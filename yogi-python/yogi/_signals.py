# This file is part of the Yogi Framework
# https://github.com/yohummus/yogi-framework.
#
# Copyright (c) 2020 Johannes Bergmann.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

from typing import Callable, Any, TypeVar, Union
from ctypes import c_void_p, byref
from inspect import signature

from ._context import Context
from ._enums import Signals
from ._errors import Result, ErrorCode, false_if_specific_ec_else_raise
from ._handler import Handler, inc_ref_cnt, dec_ref_cnt, py_object
from ._library import yogi_core
from ._object import Object


T = TypeVar('T')


def raise_signal(signal: Signals, sigarg: T = None, fn: Union[Callable[[], Any], Callable[[T], Any]] = None) -> None:
    """Raises a signal.

    Signals in Yogi are intended to be used similarly to POSIX signals. They
    have to be raised explicitly by the user (e.g. when receiving an actual
    POSIX signal like SIGINT) via this function. A signal will be received by
    all signal sets containing that signal.

    The sigarg parameter can be used to deliver user-defined data to the
    signal handlers.

    The cleanup handler fn will be called once all signal handlers have been
    called. Once fn has been called, the sigarg parameter is not used any more
    and can be cleaned up.

    Note: The cleanup handler fn can get called either from within the
          raise_signal() function or from any context within the program.

    Args:
        signal: The signal to raise (must be a single signal).
        sigarg: User-defined data to pass to the signal handlers.
        fn:     Function to be called once all signal handlers have been
                executed.
    """
    sigarg = py_object(sigarg)
    inc_ref_cnt(sigarg)

    fn_obj = None

    def clb(sigarg, userarg):
        dec_ref_cnt(fn_obj)
        dec_ref_cnt(py_object(sigarg))

        if fn is not None:
            if len(signature(fn).parameters) == 1:
                fn(sigarg)
            else:
                fn()

    wrapped_fn = yogi_core.YOGI_RaiseSignal.argtypes[2](clb)
    fn_obj = py_object(wrapped_fn)
    inc_ref_cnt(fn_obj)

    try:
        yogi_core.YOGI_RaiseSignal(signal, sigarg, wrapped_fn, None)
    except Exception:
        dec_ref_cnt(fn_obj)
        dec_ref_cnt(sigarg)
        raise


class SignalSet(Object):
    """Manages a set of signals that the user can wait on.

    Signal sets are used to receive signals raised via raise_signal(). The
    signals are queued until they can be delivered by means of calls to
    await_signal_async().
    """

    def __init__(self, context: Context, signals: Signals):
        """Creates the signal set.

        Args:
            context: The context to use.
            signals: The signals to listen for.
        """
        handle = c_void_p()
        yogi_core.YOGI_SignalSetCreate(byref(handle), context._handle, signals)
        super().__init__(handle, [context])

    def await_signal_async(self,
                           fn: Union[Callable[[Result, Signals], Any], Callable[[Result, Signals, T], Any]]) -> None:
        """Waits for a signal to be raised.

        The handler fn will be called after one of the signals in the signal
        set is caught.

        Args:
            fn: Handler function to call.
        """
        def wrapped_fn(res, signals, sigarg):
            if len(signature(fn).parameters) == 2:
                fn(res, Signals(signals))
            else:
                fn(res, Signals(signals), sigarg)

        with Handler(yogi_core.YOGI_SignalSetAwaitSignalAsync.argtypes[1], wrapped_fn) as handler:
            yogi_core.YOGI_SignalSetAwaitSignalAsync(self._handle, handler, None)

    def cancel_await_signal(self) -> bool:
        """Cancels waiting for a signal.

        This causes the handler function registered via await_signal_async() to
        be called with a cancellation error.

        Returns:
            True if the wait operation was cancelled successfully.
        """
        res = yogi_core.YOGI_SignalSetCancelAwaitSignal(self._handle)
        return false_if_specific_ec_else_raise(res, ErrorCode.OPERATION_NOT_RUNNING)
