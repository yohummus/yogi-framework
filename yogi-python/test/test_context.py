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


class TestContext(TestCase):
    def setUp(self):
        super().setUp()
        self.context = self.create_context()

    def test_poll(self):
        def fn(context, count):
            self.assertEqual(context, self.pointer.value)
            self.assertTrue(count)
            count.contents.value = 5
            return yogi.ErrorCode.OK

        self.MOCK_ContextPoll(fn)
        self.assertEqual(self.context.poll(), 5)

    def test_poll_error(self):
        self.MOCK_ContextPoll(self.error_mock)
        self.assertRaises(yogi.FailureException, self.context.poll)

    def test_poll_one(self):
        def fn(context, count):
            self.assertEqual(context, self.pointer.value)
            self.assertTrue(count)
            count.contents.value = 1
            return yogi.ErrorCode.OK

        self.MOCK_ContextPollOne(fn)
        self.assertEqual(self.context.poll_one(), 1)

    def test_poll_one_error(self):
        self.MOCK_ContextPollOne(self.error_mock)
        self.assertRaises(yogi.FailureException, self.context.poll_one)

    def test_run(self):
        def fn(context, count, duration):
            self.assertEqual(context, self.pointer.value)
            self.assertTrue(count)
            self.assertEqual(1000000, duration)
            count.contents.value = 5
            return yogi.ErrorCode.OK

        self.MOCK_ContextRun(fn)
        self.assertEqual(self.context.run(yogi.Duration.from_milliseconds(1)), 5)

    def test_run_error(self):
        def fn(context, count, duration):
            self.assertEqual(-1, duration)
            return yogi.ErrorCode.UNKNOWN

        self.MOCK_ContextRun(fn)
        self.assertRaises(yogi.FailureException, self.context.run)

    def test_run_one(self):
        def fn(context, count, duration):
            self.assertEqual(context, self.pointer.value)
            self.assertTrue(count)
            self.assertEqual(1000000, duration)
            count.contents.value = 1
            return yogi.ErrorCode.OK

        self.MOCK_ContextRunOne(fn)
        self.assertEqual(self.context.run_one(yogi.Duration.from_milliseconds(1)), 1)

    def test_run_one_error(self):
        def fn(context, count, duration):
            self.assertEqual(-1, duration)
            return yogi.ErrorCode.UNKNOWN

        self.MOCK_ContextRunOne(fn)
        self.assertRaises(yogi.FailureException, self.context.run_one)

    def test_run_in_background(self):
        def fn(context):
            self.assertEqual(context, self.pointer.value)
            return yogi.ErrorCode.OK

        self.MOCK_ContextRunInBackground(fn)
        self.context.run_in_background()

    def test_run_in_background_error(self):
        self.MOCK_ContextRunInBackground(self.error_mock)
        self.assertRaises(yogi.FailureException, self.context.run_in_background)

    def test_stop(self):
        def fn(context):
            self.assertEqual(context, self.pointer.value)
            return yogi.ErrorCode.OK

        self.MOCK_ContextStop(fn)
        self.context.stop()

    def test_stop_error(self):
        self.MOCK_ContextStop(self.error_mock)
        self.assertRaises(yogi.FailureException, self.context.stop)

    def test_wait_for_running(self):
        def fn(context, duration):
            self.assertEqual(context, self.pointer.value)
            self.assertEqual(1000000, duration)
            return yogi.ErrorCode.OK

        self.MOCK_ContextWaitForRunning(fn)
        self.assertTrue(self.context.wait_for_running(yogi.Duration.from_milliseconds(1)))

    def test_wait_for_running_timeout(self):
        def fn(context, duration):
            return yogi.ErrorCode.TIMEOUT

        self.MOCK_ContextWaitForRunning(fn)
        self.assertFalse(self.context.wait_for_running(yogi.Duration.from_milliseconds(1)))

    def test_wait_for_running_error(self):
        def fn(context, duration):
            self.assertEqual(-1, duration)
            return yogi.ErrorCode.UNKNOWN

        self.MOCK_ContextWaitForRunning(fn)
        self.assertRaises(yogi.FailureException, self.context.wait_for_running)

    def test_wait_for_stopped(self):
        def fn(context, duration):
            self.assertEqual(context, self.pointer.value)
            self.assertEqual(1000000, duration)
            return yogi.ErrorCode.OK

        self.MOCK_ContextWaitForStopped(fn)
        self.assertTrue(self.context.wait_for_stopped(yogi.Duration.from_milliseconds(1)))

    def test_wait_for_stopped_timeout(self):
        def fn(context, duration):
            return yogi.ErrorCode.TIMEOUT

        self.MOCK_ContextWaitForStopped(fn)
        self.assertFalse(self.context.wait_for_stopped(yogi.Duration.from_milliseconds(1)))

    def test_wait_for_stopped_error(self):
        def fn(context, duration):
            self.assertEqual(-1, duration)
            return yogi.ErrorCode.UNKNOWN

        self.MOCK_ContextWaitForStopped(fn)
        self.assertRaises(yogi.FailureException, self.context.wait_for_stopped)

    def test_post(self):
        def fn(context, fn, userarg):
            self.assertEqual(context, self.pointer.value)
            self.assertTrue(fn)
            fn(userarg)
            return yogi.ErrorCode.OK

        self.MOCK_ContextPost(fn)

        called = False

        def handlerFn():
            nonlocal called
            called = True

        self.context.post(handlerFn)
        self.assertTrue(called)

    def test_post_error(self):
        self.MOCK_ContextPost(self.error_mock)
        self.assertRaises(yogi.FailureException, lambda: self.context.post(lambda: None))
