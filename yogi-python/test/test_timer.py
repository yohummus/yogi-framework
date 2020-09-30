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

import yogi

from .common import TestCase


class TestTimer(TestCase):
    def create_timer(self):
        def fn(timer, context):
            timer.contents.value = self.pointer.value
            return yogi.ErrorCode.OK

        self.MOCK_TimerCreate(fn)
        return yogi.Timer(self.create_context())

    def test_create(self):
        called = False

        def fn(timer, context):
            self.assertTrue(timer)
            self.assertEqual(context, self.pointer.value)
            timer.contents.value = self.pointer.value
            nonlocal called
            called = True
            return yogi.ErrorCode.OK

        self.MOCK_TimerCreate(fn)
        yogi.Timer(self.create_context())
        self.assertTrue(called)

    def test_start_async(self):
        timer = self.create_timer()
        called = False

        def handler_fn(result):
            self.assertIsInstance(result, yogi.Success)
            self.assertEqual(result.error_code, yogi.ErrorCode.OK)
            nonlocal called
            called = True

        def fn(timer, duration, fn_, userarg):
            self.assertEqual(timer, self.pointer.value)
            self.assertEqual(duration, 1234)
            self.assertTrue(fn_)
            self.assertIsNone(userarg)
            fn_(yogi.ErrorCode.OK, userarg)
            return yogi.ErrorCode.OK

        self.MOCK_TimerStartAsync(fn)
        timer.start_async(yogi.Duration.from_nanoseconds(1234), handler_fn)
        self.assertTrue(called)

    def test_start_async_callback_cancel(self):
        timer = self.create_timer()
        called = False

        def handler_fn(result):
            self.assertIsInstance(result, yogi.Failure)
            self.assertEqual(result.error_code, yogi.ErrorCode.CANCELED)
            nonlocal called
            called = True

        def fn(timer, duration, fn_, userarg):
            self.assertTrue(fn_)
            fn_(yogi.ErrorCode.CANCELED, userarg)
            return yogi.ErrorCode.OK

        self.MOCK_TimerStartAsync(fn)
        timer.start_async(yogi.Duration.from_nanoseconds(1234), handler_fn)
        self.assertTrue(called)

    def test_cancel(self):
        timer = self.create_timer()

        def fn(timer):
            self.assertEqual(timer, self.pointer.value)
            return yogi.ErrorCode.OK

        self.MOCK_TimerCancel(fn)
        self.assertTrue(timer.cancel())

        def fn2(timer):
            self.assertEqual(timer, self.pointer.value)
            return yogi.ErrorCode.TIMER_EXPIRED

        self.MOCK_TimerCancel(fn2)
        self.assertFalse(timer.cancel())
