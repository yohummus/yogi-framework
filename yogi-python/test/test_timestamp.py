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
import datetime

from .common import TestCase


class TestTimestamp(TestCase):
    def test_from_duration_since_epoch(self):
        self.assertEqual(yogi.Timestamp.from_duration_since_epoch(
            yogi.Duration.from_days(123)).duration_since_epoch, yogi.Duration.from_days(123))
        self.assertRaises(ArithmeticError, lambda: yogi.Timestamp.from_duration_since_epoch(yogi.Duration.INF))
        self.assertRaises(ArithmeticError, lambda: yogi.Timestamp.from_duration_since_epoch(
            yogi.Duration.from_days(-1)))

    def test_now(self):
        def fn(timestamp):
            timestamp.contents.value = 5555555555
            return yogi.ErrorCode.OK

        self.MOCK_GetCurrentTime(fn)
        self.assertEqual(5555555555, yogi.Timestamp.now().duration_since_epoch.nanoseconds_count)

    def test_now_error(self):
        self.MOCK_GetCurrentTime(self.error_mock)
        with self.assertRaises(yogi.FailureException):
            yogi.Timestamp.now()

    def test_parse(self):
        def fn(timestamp, str_, timefmt):
            self.assertEqual(str_, b'foo')
            self.assertIsNone(timefmt)
            timestamp.contents.value = 1234356789123000000
            return yogi.ErrorCode.OK

        self.MOCK_ParseTime(fn)
        self.assertEqual(1234356789123000000, yogi.Timestamp.parse('foo').duration_since_epoch.nanoseconds_count)

    def test_parse_error(self):
        def fn(timestamp, str, timefmt):
            self.assertEqual(timefmt, b'bar')
            return yogi.ErrorCode.UNKNOWN

        self.MOCK_ParseTime(fn)
        with self.assertRaises(yogi.FailureException):
            yogi.Timestamp.parse('foo', 'bar')

    def test_constructor(self):
        t = yogi.Timestamp()
        self.assertEqual(t.duration_since_epoch.nanoseconds_count, 0)

    def test_duration_since_epoch(self):
        dur = yogi.Duration.from_days(123)
        t = yogi.Timestamp.from_duration_since_epoch(dur)
        self.assertEqual(t.duration_since_epoch, dur)

    def test_nanoseconds(self):
        dur = yogi.Duration.from_nanoseconds(123456789)
        t = yogi.Timestamp.from_duration_since_epoch(dur)
        self.assertEqual(t.nanoseconds, 789)

    def test_microseconds(self):
        dur = yogi.Duration.from_nanoseconds(123456789)
        t = yogi.Timestamp.from_duration_since_epoch(dur)
        self.assertEqual(t.microseconds, 456)

    def test_milliseconds(self):
        dur = yogi.Duration.from_nanoseconds(123456789)
        t = yogi.Timestamp.from_duration_since_epoch(dur)
        self.assertEqual(t.milliseconds, 123)

    def test_to_datetime(self):
        dur = yogi.Duration.from_nanoseconds(123456789123456789)
        dt = yogi.Timestamp.from_duration_since_epoch(dur).to_datetime()
        self.assertIsInstance(dt, datetime.datetime)
        self.assertEqual(dt.microsecond, 123456)

    def test_format(self):
        def fn(timestamp, str_, strsize, timefmt):
            self.assertEqual(timestamp, 123456789123456789)
            self.assertFalse(timefmt)
            self.assertFalse(strsize)
            str_.contents.value = self.hello_bytes
            return yogi.ErrorCode.OK

        self.MOCK_FormatTime(fn)
        t = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_nanoseconds(123456789123456789))
        self.assertEqual(t.format(), 'hello')

    def test_format_error(self):
        def fn(timestamp, str_, strsize, timefmt):
            self.assertEqual(timefmt, b'foo')
            return yogi.ErrorCode.UNKNOWN

        self.MOCK_FormatTime(fn)
        t = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_nanoseconds(123456789123456789))
        with self.assertRaises(yogi.FailureException):
            t.format('foo')

    def test_str(self):
        def fn(timestamp, str_, strsize, timefmt):
            self.assertEqual(timestamp, 123456789123456789)
            self.assertFalse(timefmt)
            self.assertFalse(strsize)
            str_.contents.value = self.hello_bytes
            return yogi.ErrorCode.OK

        self.MOCK_FormatTime(fn)
        t = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_nanoseconds(123456789123456789))
        self.assertEqual(str(t), 'hello')

    def test_add(self):
        t = yogi.Timestamp()
        t = t + yogi.Duration.from_nanoseconds(8)
        self.assertEqual(t.duration_since_epoch.nanoseconds_count, 8)
        self.assertRaises(ArithmeticError, lambda: t + yogi.Duration.from_milliseconds(-1))

    def test_sub(self):
        t = yogi.Timestamp()
        t += yogi.Duration.from_nanoseconds(8)
        t = t - yogi.Duration.from_nanoseconds(2)
        self.assertEqual(t.duration_since_epoch.nanoseconds_count, 6)
        self.assertEqual(
            (t - yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_nanoseconds(2))).nanoseconds_count, 4)
        self.assertRaises(ArithmeticError, lambda: t - yogi.Duration.from_milliseconds(1))

    def test_eq(self):
        t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
        t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

        self.assertTrue(t1 == t1)
        self.assertFalse(t2 == t1)

    def test_ne(self):
        t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
        t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

        self.assertFalse(t1 != t1)
        self.assertTrue(t2 != t1)

    def test_lt(self):
        t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
        t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

        self.assertTrue(t1 < t2)
        self.assertFalse(t2 < t1)
        self.assertFalse(t2 < t2)

    def test_gt(self):
        t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
        t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

        self.assertFalse(t1 > t2)
        self.assertTrue(t2 > t1)
        self.assertFalse(t2 > t2)

    def test_le(self):
        t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
        t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

        self.assertTrue(t1 <= t2)
        self.assertFalse(t2 <= t1)
        self.assertTrue(t2 <= t2)

    def test_ge(self):
        t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
        t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

        self.assertFalse(t1 >= t2)
        self.assertTrue(t2 >= t1)
        self.assertTrue(t2 >= t2)

    def test_hash(self):
        self.assertNotEqual(hash(yogi.Timestamp.from_duration_since_epoch(
            yogi.Duration.from_seconds(1))), hash(yogi.Timestamp()))
