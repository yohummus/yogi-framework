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


def test_raise_signal(mocks: Mocks):
    """Verifies that the signals can be raised"""
    called = False

    def handler_fn():
        nonlocal called
        called = True

    def fn(signal, sigarg, fn_, userarg):
        assert signal == yogi.Signals.TERM
        assert sigarg is None
        assert fn_
        assert userarg is None
        fn_(sigarg, userarg)
        return yogi.ErrorCode.OK

    mocks.MOCK_RaiseSignal(fn)
    yogi.raise_signal(yogi.Signals.TERM, None, handler_fn)
    assert called

    def handler_fn2(sigarg):  # This handler has a parameter
        assert sigarg is None

    yogi.raise_signal(yogi.Signals.TERM, None, handler_fn2)


def test_create_signal_set(context: yogi.Context):
    """Verifies that SignalSet can be instantiated"""
    pass  # Already tested by requesting the context fixture


def test_raise_signal_no_handler(mocks: Mocks, context: yogi.Context):
    """Verifies that signals can be raised without having registered a signal handler"""
    called = False

    def fn(signal, sigarg, fn_, userarg):
        nonlocal called
        called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_RaiseSignal(fn)
    yogi.raise_signal(yogi.Signals.TERM)
    assert called

    called = False
    yogi.raise_signal(yogi.Signals.TERM, 'foo')
    assert called


def test_raise_signal_with_sigarg(mocks: Mocks):
    """Verifies that signals can be raised with a signal argument"""
    called = False

    def handler_fn():
        nonlocal called
        called = True

    def fn(signal, sigarg, fn_, userarg):
        assert signal == yogi.Signals.TERM
        assert sigarg is not None
        assert fn_
        assert userarg is None
        fn_(sigarg, userarg)
        return yogi.ErrorCode.OK

    mocks.MOCK_RaiseSignal(fn)
    yogi.raise_signal(yogi.Signals.TERM, 'foo', handler_fn)
    assert called

    def handler_fn2(sigarg):
        assert sigarg == 'foo'

    yogi.raise_signal(yogi.Signals.TERM, 'foo', handler_fn2)


def test_await_signal(mocks: Mocks, signal_set: yogi.SignalSet):
    """Verifies that we can wait for a signal to arrive"""
    called = False

    def handler_fn(res, signal):
        assert isinstance(res, yogi.Failure)
        assert res.error_code == yogi.ErrorCode.WRONG_OBJECT_TYPE
        assert signal == yogi.Signals.USR6
        nonlocal called
        called = True

    def fn(sigset, fn_, userarg):
        assert sigset == 1111
        assert fn_
        assert userarg is None
        fn_(yogi.ErrorCode.WRONG_OBJECT_TYPE, yogi.Signals.USR6, None, userarg)
        return yogi.ErrorCode.OK

    mocks.MOCK_SignalSetAwaitSignalAsync(fn)
    signal_set.await_signal_async(handler_fn)
    assert called

    called = False

    def handler_fn2(res, signal, sigarg):
        assert sigarg is None
        nonlocal called
        called = True

    signal_set.await_signal_async(handler_fn2)
    assert called


def test_await_signal_with_sigarg(mocks: Mocks, signal_set: yogi.SignalSet):
    """Verifies that we can wait for a signal to arrive with a signal argument"""
    called = False

    def handler_fn(res, signal, sigarg):
        assert isinstance(res, yogi.Failure)
        assert res.error_code == yogi.ErrorCode.WRONG_OBJECT_TYPE
        assert signal == yogi.Signals.USR6
        assert sigarg == 'foo'
        nonlocal called
        called = True

    def fn(sigset, fn_, userarg):
        assert sigset == 1111
        assert fn_
        assert userarg is None
        fn_(yogi.ErrorCode.WRONG_OBJECT_TYPE, yogi.Signals.USR6, 'foo', userarg)
        return yogi.ErrorCode.OK

    mocks.MOCK_SignalSetAwaitSignalAsync(fn)
    signal_set.await_signal_async(handler_fn)
    assert called


def test_cancel_await_signal(mocks: Mocks, signal_set: yogi.SignalSet):
    """Verifies that waiting for a signal to arrive can be interrupted"""
    def fn(sigset):
        assert sigset == 1111
        return yogi.ErrorCode.OK

    mocks.MOCK_SignalSetCancelAwaitSignal(fn)
    assert signal_set.cancel_await_signal()

    def fn2(sigset):
        assert sigset == 1111
        return yogi.ErrorCode.OPERATION_NOT_RUNNING

    mocks.MOCK_SignalSetCancelAwaitSignal(fn2)
    assert not signal_set.cancel_await_signal()
