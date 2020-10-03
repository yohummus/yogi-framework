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
import pytest

from .conftest import Mocks


def test_poll(mocks: Mocks, context: yogi.Context):
    """Checks the poll() function in the error-free case"""
    def fn(context, count):
        assert context == 1234
        assert count
        count.contents.value = 5
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextPoll(fn)
    assert context.poll() == 5


def test_poll_error(mocks: Mocks, context: yogi.Context):
    """Checks the poll() function in the error case"""
    mocks.MOCK_ContextPoll(lambda *_: yogi.ErrorCode.WRONG_OBJECT_TYPE)
    with pytest.raises(yogi.FailureException):
        context.poll()


def test_poll_one(mocks: Mocks, context: yogi.Context):
    """Checks the poll_one() function in the error-free case"""
    def fn(context, count):
        assert context == 1234
        assert count
        count.contents.value = 1
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextPollOne(fn)
    assert context.poll_one() == 1


def test_poll_one_error(mocks: Mocks, context: yogi.Context):
    """Checks the poll_one() function in the error case"""
    mocks.MOCK_ContextPollOne(lambda *_: yogi.ErrorCode.WRONG_OBJECT_TYPE)
    with pytest.raises(yogi.FailureException):
        context.poll_one()


def test_run(mocks: Mocks, context: yogi.Context):
    """Checks the run() function in the error-free case"""
    def fn(context, count, duration):
        assert context == 1234
        assert count
        assert 1000000 == duration
        count.contents.value = 5
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextRun(fn)
    assert context.run(yogi.Duration.from_milliseconds(1)) == 5


def test_run_error(mocks: Mocks, context: yogi.Context):
    """Checks the run() function in the error case"""
    def fn(context, count, duration):
        assert -1 == duration
        return yogi.ErrorCode.UNKNOWN

    mocks.MOCK_ContextRun(fn)
    with pytest.raises(yogi.FailureException):
        context.run()


def test_run_one(mocks: Mocks, context: yogi.Context):
    """Checks the run_one() function in the error-free case"""
    def fn(context, count, duration):
        assert context == 1234
        assert count
        assert 1000000 == duration
        count.contents.value = 1
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextRunOne(fn)
    assert context.run_one(yogi.Duration.from_milliseconds(1)) == 1


def test_run_one_error(mocks: Mocks, context: yogi.Context):
    """Checks the run_one() function in the error case"""
    def fn(context, count, duration):
        assert -1 == duration
        return yogi.ErrorCode.UNKNOWN

    mocks.MOCK_ContextRunOne(fn)
    with pytest.raises(yogi.FailureException):
        context.run_one()


def test_run_in_background(mocks: Mocks, context: yogi.Context):
    """Checks the run_in_background() function in the error-free case"""
    def fn(context):
        assert context == 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextRunInBackground(fn)
    context.run_in_background()


def test_run_in_background_error(mocks: Mocks, context: yogi.Context):
    """Checks the run_in_background() function in the error case"""
    mocks.MOCK_ContextRunInBackground(lambda *_: yogi.ErrorCode.WRONG_OBJECT_TYPE)
    with pytest.raises(yogi.FailureException):
        context.run_in_background()


def test_stop(mocks: Mocks, context: yogi.Context):
    """Checks the stop() function in the error-free case"""
    def fn(context):
        assert context == 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextStop(fn)
    context.stop()


def test_stop_error(mocks: Mocks, context: yogi.Context):
    """Checks the stop() function in the error case"""
    mocks.MOCK_ContextStop(lambda *_: yogi.ErrorCode.WRONG_OBJECT_TYPE)
    with pytest.raises(yogi.FailureException):
        context.stop()


def test_wait_for_running(mocks: Mocks, context: yogi.Context):
    """Checks the wait_for_running() function in the error-free case"""
    def fn(context, duration):
        assert context == 1234
        assert 1000000 == duration
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextWaitForRunning(fn)
    assert context.wait_for_running(yogi.Duration.from_milliseconds(1))


def test_wait_for_running_timeout(mocks: Mocks, context: yogi.Context):
    """Checks the wait_for_running() function in the timeout case"""
    def fn(context, duration):
        return yogi.ErrorCode.TIMEOUT

    mocks.MOCK_ContextWaitForRunning(fn)
    assert not context.wait_for_running(yogi.Duration.from_milliseconds(1))


def test_wait_for_running_error(mocks: Mocks, context: yogi.Context):
    """Checks the wait_for_running() function in the error case"""
    def fn(context, duration):
        assert -1 == duration
        return yogi.ErrorCode.UNKNOWN

    mocks.MOCK_ContextWaitForRunning(fn)
    with pytest.raises(yogi.FailureException):
        context.wait_for_running()


def test_wait_for_stopped(mocks: Mocks, context: yogi.Context):
    """Checks the wait_for_stopped() function in the error-free case"""
    def fn(context, duration):
        assert context == 1234
        assert 1000000 == duration
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextWaitForStopped(fn)
    assert context.wait_for_stopped(yogi.Duration.from_milliseconds(1))


def test_wait_for_stopped_timeout(mocks: Mocks, context: yogi.Context):
    """Checks the wait_for_stopped() function in the timeout case"""
    def fn(context, duration):
        return yogi.ErrorCode.TIMEOUT

    mocks.MOCK_ContextWaitForStopped(fn)
    assert not context.wait_for_stopped(yogi.Duration.from_milliseconds(1))


def test_wait_for_stopped_error(mocks: Mocks, context: yogi.Context):
    """Checks the wait_for_stopped() function in the error case"""
    def fn(context, duration):
        assert -1 == duration
        return yogi.ErrorCode.UNKNOWN

    mocks.MOCK_ContextWaitForStopped(fn)
    with pytest.raises(yogi.FailureException):
        context.wait_for_stopped()


def test_post(mocks: Mocks, context: yogi.Context):
    """Checks the post() function in the error-free case"""
    def fn(context, fn, userarg):
        assert context == 1234
        assert fn
        fn(userarg)
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextPost(fn)

    called = False

    def handlerFn():
        nonlocal called
        called = True

    context.post(handlerFn)
    assert called


def test_post_error(mocks: Mocks, context: yogi.Context):
    """Checks the post_error() function in the error case"""
    mocks.MOCK_ContextPost(lambda *_: yogi.ErrorCode.WRONG_OBJECT_TYPE)
    with pytest.raises(yogi.FailureException):
        context.post(lambda: None)
