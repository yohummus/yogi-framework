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

from typing import Callable, Any
from ctypes import c_void_p, byref

from ._context import Context
from ._duration import Duration, duration_to_api_duration
from ._errors import Result, FailureException, ErrorCode, false_if_specific_ec_else_raise
from ._handler import Handler
from ._library import yogi_core
from ._object import Object


class Timer(Object):
    """Simple timer implementation."""

    def __init__(self, context: Context):
        """Creates the timer.

        Args:
            context: The context to use.
        """
        handle = c_void_p()
        yogi_core.YOGI_TimerCreate(byref(handle), context._handle)
        Object.__init__(self, handle, [context])

    def start_async(self, duration: Duration, fn: Callable[[Result], Any]) -> None:
        """Starts the timer.

        If the timer is already running, the timer will be canceled first, as
        if cancel() were called explicitly.

        Args:
            duration: Time when the timer expires.
            fn:       Handler function to call after the given time passed.
        """
        dur = duration_to_api_duration(duration)
        with Handler(yogi_core.YOGI_TimerStartAsync.argtypes[2], fn) as handler:
            yogi_core.YOGI_TimerStartAsync(self._handle, dur, handler, None)

    def cancel(self) -> bool:
        """Cancels the given timer.

        Canceling a timer will result in the handler function registered via
        start_async() to be called with a cancellation error. Note that if the
        handler is already scheduled for execution, it will be called without
        an error.

        Returns:
            True if the timer was canceled successfully and False otherwise
            (i.e. the timer has not been started or it already expired).
        """
        res = yogi_core.YOGI_TimerCancel(self._handle)
        return false_if_specific_ec_else_raise(res, ErrorCode.TIMER_EXPIRED)
