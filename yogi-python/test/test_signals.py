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

from .common import TestCase


class TestSignals(TestCase):
    def create_signal_set(self):
        def fn(timer, context, signals):
            timer.contents.value = self.pointer.value
            return yogi.ErrorCode.OK

        self.MOCK_SignalSetCreate(fn)
        return yogi.SignalSet(self.create_context(), signals=yogi.Signals.TERM)

    def test_raise_signal(self):
        called = False

        def handler_fn():
            nonlocal called
            called = True

        def fn(signal, sigarg, fn_, userarg):
            self.assertEqual(signal, yogi.Signals.TERM)
            self.assertIsNone(sigarg)
            self.assertTrue(fn_)
            self.assertIsNone(userarg)
            fn_(sigarg, userarg)
            return yogi.ErrorCode.OK

        self.MOCK_RaiseSignal(fn)
        yogi.raise_signal(yogi.Signals.TERM, None, handler_fn)
        self.assertTrue(called)

        def handler_fn2(sigarg):  # This handler has a parameter
            self.assertIsNone(sigarg)

        yogi.raise_signal(yogi.Signals.TERM, None, handler_fn2)

    def test_raise_signal_no_handler(self):
        called = False

        def fn(signal, sigarg, fn_, userarg):
            nonlocal called
            called = True
            return yogi.ErrorCode.OK

        self.MOCK_RaiseSignal(fn)
        yogi.raise_signal(yogi.Signals.TERM)
        self.assertTrue(called)

        called = False
        yogi.raise_signal(yogi.Signals.TERM, 'foo')
        self.assertTrue(called)

    def test_raise_signal_with_sigarg(self):
        called = False

        def handler_fn():
            nonlocal called
            called = True

        def fn(signal, sigarg, fn_, userarg):
            self.assertEqual(signal, yogi.Signals.TERM)
            self.assertIsNotNone(sigarg)
            self.assertTrue(fn_)
            self.assertIsNone(userarg)
            fn_(sigarg, userarg)
            return yogi.ErrorCode.OK

        self.MOCK_RaiseSignal(fn)
        yogi.raise_signal(yogi.Signals.TERM, 'foo', handler_fn)
        self.assertTrue(called)

        def handler_fn2(sigarg):
            self.assertEqual(sigarg, 'foo')

        yogi.raise_signal(yogi.Signals.TERM, 'foo', handler_fn2)

    def test_create_signal_set(self):
        def fn(timer, context, signals):
            self.assertTrue(timer)
            self.assertEqual(context, self.pointer.value)
            self.assertEqual(signals, yogi.Signals.USR3)
            timer.contents.value = self.pointer.value
            return yogi.ErrorCode.OK

        self.MOCK_SignalSetCreate(fn)
        return yogi.SignalSet(self.create_context(), yogi.Signals.USR3)

    def test_await_signal(self):
        sigset = self.create_signal_set()
        called = False

        def handler_fn(res, signal):
            self.assertIsInstance(res, yogi.Failure)
            self.assertEqual(res.error_code, yogi.ErrorCode.WRONG_OBJECT_TYPE)
            self.assertEqual(signal, yogi.Signals.USR6)
            nonlocal called
            called = True

        def fn(sigset, fn_, userarg):
            self.assertEqual(sigset, self.pointer.value)
            self.assertTrue(fn_)
            self.assertIsNone(userarg)
            fn_(yogi.ErrorCode.WRONG_OBJECT_TYPE, yogi.Signals.USR6, None, userarg)
            return yogi.ErrorCode.OK

        self.MOCK_SignalSetAwaitSignalAsync(fn)
        sigset.await_signal_async(handler_fn)
        self.assertTrue(called)

        called = False

        def handler_fn2(res, signal, sigarg):
            self.assertIsNone(sigarg)
            nonlocal called
            called = True

        sigset.await_signal_async(handler_fn2)
        self.assertTrue(called)

    def test_await_signal_with_sigarg(self):
        sigset = self.create_signal_set()
        called = False

        def handler_fn(res, signal, sigarg):
            self.assertIsInstance(res, yogi.Failure)
            self.assertEqual(res.error_code, yogi.ErrorCode.WRONG_OBJECT_TYPE)
            self.assertEqual(signal, yogi.Signals.USR6)
            self.assertEqual(sigarg, 'foo')
            nonlocal called
            called = True

        def fn(sigset, fn_, userarg):
            self.assertEqual(sigset, self.pointer.value)
            self.assertTrue(fn_)
            self.assertIsNone(userarg)
            fn_(yogi.ErrorCode.WRONG_OBJECT_TYPE, yogi.Signals.USR6, 'foo', userarg)
            return yogi.ErrorCode.OK

        self.MOCK_SignalSetAwaitSignalAsync(fn)
        sigset.await_signal_async(handler_fn)
        self.assertTrue(called)

    def test_cancel_await_signal(self):
        sigset = self.create_signal_set()

        def fn(sigset):
            self.assertEqual(sigset, self.pointer.value)
            return yogi.ErrorCode.OK

        self.MOCK_SignalSetCancelAwaitSignal(fn)
        self.assertTrue(sigset.cancel_await_signal())

        def fn2(sigset):
            self.assertEqual(sigset, self.pointer.value)
            return yogi.ErrorCode.OPERATION_NOT_RUNNING

        self.MOCK_SignalSetCancelAwaitSignal(fn2)
        self.assertFalse(sigset.cancel_await_signal())
