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

import yogi

from .conftest import Mocks


def test_create(mocks: Mocks, context: yogi.Context):
    """Verifies that Timer can be instantiated"""
    called = False

    def fn(timer, context):
        assert timer
        assert context == 1234
        timer.contents.value = 1234
        nonlocal called
        called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_TimerCreate(fn)
    yogi.Timer(context)
    assert called


def test_start_async(mocks: Mocks, timer: yogi.Timer):
    """Verifies that a timer can be started with a callback function"""
    called = False

    def handler_fn(result):
        assert isinstance(result, yogi.Success)
        assert result.error_code == yogi.ErrorCode.OK
        nonlocal called
        called = True

    def fn(timer, duration, fn_, userarg):
        assert timer == 1234
        assert duration == 1234
        assert fn_
        assert userarg is None
        fn_(yogi.ErrorCode.OK, userarg)
        return yogi.ErrorCode.OK

    mocks.MOCK_TimerStartAsync(fn)
    timer.start_async(yogi.Duration.from_nanoseconds(1234), handler_fn)
    assert called


def test_start_async_callback_cancel(mocks: Mocks, timer: yogi.Timer):
    """Verifies that canceled timers call the previously registered callback function with an error"""
    called = False

    def handler_fn(result):
        assert isinstance(result, yogi.Failure)
        assert result.error_code == yogi.ErrorCode.CANCELED
        nonlocal called
        called = True

    def fn(timer, duration, fn_, userarg):
        assert fn_
        fn_(yogi.ErrorCode.CANCELED, userarg)
        return yogi.ErrorCode.OK

    mocks.MOCK_TimerStartAsync(fn)
    timer.start_async(yogi.Duration.from_nanoseconds(1234), handler_fn)
    assert called


def test_cancel(mocks: Mocks, timer: yogi.Timer):
    """Verifies that a timer can be canceled"""
    def fn(timer):
        assert timer == 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_TimerCancel(fn)
    assert timer.cancel()

    def fn2(timer):
        assert timer == 1234
        return yogi.ErrorCode.TIMER_EXPIRED

    mocks.MOCK_TimerCancel(fn2)
    assert not timer.cancel()
