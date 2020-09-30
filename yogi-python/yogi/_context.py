# This file is part of the Yogi distribution https://github.com/yohummus/yogi.
# Copyright (c) 2019 Johannes Bergmann.
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.

from typing import Callable, Any
from ctypes import c_void_p, c_int, byref

from ._errors import ErrorCode, FailureException
from ._object import Object
from ._library import yogi_core
from ._duration import Duration, duration_to_api_duration
from ._handler import py_object, inc_ref_cnt, dec_ref_cnt


class Context(Object):
    """Scheduler for the execution of asynchronous operations."""

    def __init__(self):
        """Creates the context."""
        handle = c_void_p()
        yogi_core.YOGI_ContextCreate(byref(handle))
        super().__init__(handle)

    def poll(self) -> int:
        """Runs the context's event processing loop to execute ready handlers.

        This function runs handlers (internal and user-supplied such as
        functions registered through the post() function) that are ready to
        run, without blocking, until the stop() function has been called or
        there are no more ready handlers.

        This function must be called from outside any handler functions that
        are being executed through the context.

        Returns:
            Number of executed handlers.
        """
        n = c_int()
        yogi_core.YOGI_ContextPoll(self._handle, byref(n))
        return n.value

    def poll_one(self) -> int:
        """Runs the context's event processing loop to execute at most one
        ready handler.

        This function runs at most one handler (internal and user-supplied
        such as functions registered through the post() function) that is
        ready to run, without blocking.

        This function must be called from outside any handler functions that
        are being executed through the context.

        Returns:
            Number of executed handlers (either 1 or 0).
        """
        n = c_int()
        yogi_core.YOGI_ContextPollOne(self._handle, byref(n))
        return n.value

    def run(self, duration: Duration = None) -> int:
        """Runs the context's event processing loop for the specified
        duration.

        This function blocks while running the context's event processing loop
        and calling dispatched handlers (internal and user-supplied such as
        functions registered through the post() function) for the specified
        duration unless the stop() function is called within that time.

        This function must be called from outside any handler functions that
        are being executed through the context.

        Args:
            duration: Maximum amount of time to run.

        Returns:
            Number of executed handlers.
        """
        n = c_int()
        dur = duration_to_api_duration(duration, Duration.INF)
        yogi_core.YOGI_ContextRun(self._handle, byref(n), dur)
        return n.value

    def run_one(self, duration: Duration = None) -> int:
        """Runs the context's event processing loop for the specified
        duration to execute at most one handler.

        This function blocks while running the context's event processing loop
        and calling dispatched handlers (internal and user-supplied such as
        functions registered through the post() function) for the specified
        duration until a single handler function has been executed, unless the
        stop() function is called within that time.

        This function must be called from outside any handler functions that
        are being executed through the context.

        Args:
            duration: Maximum amount of time to run.

        Returns:
            Number of executed handlers (either 1 or 0).
        """
        n = c_int()
        dur = duration_to_api_duration(duration, Duration.INF)
        yogi_core.YOGI_ContextRunOne(self._handle, byref(n), dur)
        return n.value

    def run_in_background(self) -> None:
        """Starts an internal thread for running the context's event
        processing loop.

        This function starts a thread that runs the context's event processing
        loop in the background. It relieves the user from having to start a
        thread and calling the appropriate run(), run_one(), poll() and
        poll_one() functions themself. The thread can be stopped using the
        stop() function.

        This function must be called from outside any handler functions that
        are being executed through the context.
        """
        yogi_core.YOGI_ContextRunInBackground(self._handle)

    def stop(self) -> None:
        """Stops the context's event processing loop.

        This function signals the context to stop running its event processing
        loop. This causes run() and run_one() functions to return as soon as
        possible and it terminates the thread started via run_in_background().
        """
        yogi_core.YOGI_ContextStop(self._handle)

    def wait_for_running(self, duration: Duration = None) -> bool:
        """Blocks until the context's event processing loop is being run or
        until the specified timeout is reached.

        This function must be called from outside any handler functions that
        are being executed through the context.

        Args:
            duration: Maximum time to wait.

        Returns:
            True if the context's event processing loop is running within
            the specified duration and False otherwise.
        """
        dur = duration_to_api_duration(duration, Duration.INF)

        try:
            yogi_core.YOGI_ContextWaitForRunning(self._handle, dur)
        except FailureException as e:
            if e.failure.error_code is ErrorCode.TIMEOUT:
                return False
            else:
                raise

        return True

    def wait_for_stopped(self, duration: Duration = None) -> bool:
        """Blocks until no thread is running the context's event processing
        loop or until the specified timeout is reached.

        This function must be called from outside any handler functions that
        are being executed through the context.

        Args:
            duration: Maximum time to wait.

        Returns:
            True if the context's event processing loop is not running within
            the specified duration and False otherwise.
        """
        dur = duration_to_api_duration(duration, Duration.INF)

        try:
            yogi_core.YOGI_ContextWaitForStopped(self._handle, dur)
        except FailureException as e:
            if e.failure.error_code is ErrorCode.TIMEOUT:
                return False
            else:
                raise

        return True

    def post(self, fn: Callable[[], Any]) -> None:
        """Adds the given function to the context's event processing queue to
        be executed and returns immediately.

        The handler fn will only be executed after this function returns and
        only by a thread running the context's event processing loop.

        Args:
            fn: The function to call from within the context.
        """
        fn_obj = None

        def clb(userarg):
            dec_ref_cnt(fn_obj)
            fn()

        wrapped_fn = yogi_core.YOGI_ContextPost.argtypes[1](clb)
        fn_obj = py_object(wrapped_fn)
        inc_ref_cnt(fn_obj)

        try:
            yogi_core.YOGI_ContextPost(self._handle, wrapped_fn, None)
        except Exception:
            dec_ref_cnt(fn_obj)
            raise
