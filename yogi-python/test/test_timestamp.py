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
import datetime
import pytest

from .conftest import Mocks


def test_from_duration_since_epoch():
    """Verifies that a Timestamp instance can be created from a duration"""
    assert yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(123)).duration_since_epoch \
        == yogi.Duration.from_days(123)

    with pytest.raises(ArithmeticError):
        yogi.Timestamp.from_duration_since_epoch(yogi.Duration.INF)

    with pytest.raises(ArithmeticError):
        yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(-1))


def test_now(mocks: Mocks):
    """Checks that we can retrieve the current time"""
    def fn(timestamp):
        timestamp.contents.value = 5555555555
        return yogi.ErrorCode.OK

    mocks.MOCK_GetCurrentTime(fn)
    assert 5555555555 == yogi.Timestamp.now().duration_since_epoch.nanoseconds_count


def test_now_error(mocks: Mocks):
    """Verifies that errors that occur when getting the current time get reported"""
    mocks.MOCK_GetCurrentTime(lambda *_: yogi.ErrorCode.WRONG_OBJECT_TYPE)

    with pytest.raises(yogi.FailureException):
        yogi.Timestamp.now()


def test_parse(mocks: Mocks):
    """Verifies that a Timestamp instance can be created from a string"""
    def fn(timestamp, str_, timefmt):
        assert str_ == b'foo'
        assert timefmt is None
        timestamp.contents.value = 1234356789123000000
        return yogi.ErrorCode.OK

    mocks.MOCK_ParseTime(fn)
    assert 1234356789123000000 == yogi.Timestamp.parse('foo').duration_since_epoch.nanoseconds_count


def test_parse_error(mocks: Mocks):
    """Verifies that errors that occur while parsing a string get reported"""
    def fn(timestamp, str, timefmt):
        assert timefmt == b'bar'
        return yogi.ErrorCode.UNKNOWN

    mocks.MOCK_ParseTime(fn)

    with pytest.raises(yogi.FailureException):
        yogi.Timestamp.parse('foo', 'bar')


def test_constructor():
    """Verifies that the default constructor creates a timestamp representing the epoch"""
    t = yogi.Timestamp()
    assert t.duration_since_epoch.nanoseconds_count == 0


def test_duration_since_epoch():
    """Verifies that a Duration instance can be obtained from the epoch to the time that the timestamp represents"""
    dur = yogi.Duration.from_days(123)
    t = yogi.Timestamp.from_duration_since_epoch(dur)
    assert t.duration_since_epoch == dur


def test_nanoseconds():
    """Verifies that the nanoseconds fraction of a timestamp can be retrieved"""
    dur = yogi.Duration.from_nanoseconds(123456789)
    t = yogi.Timestamp.from_duration_since_epoch(dur)
    assert t.nanoseconds == 789


def test_microseconds():
    """Verifies that the microseconds fraction of a timestamp can be retrieved"""
    dur = yogi.Duration.from_nanoseconds(123456789)
    t = yogi.Timestamp.from_duration_since_epoch(dur)
    assert t.microseconds == 456


def test_milliseconds():
    """Verifies that the milliseconds fraction of a timestamp can be retrieved"""
    dur = yogi.Duration.from_nanoseconds(123456789)
    t = yogi.Timestamp.from_duration_since_epoch(dur)
    assert t.milliseconds == 123


def test_to_datetime():
    """Verifies that a Timestamp instance can be converted to a datetime.datetime instance"""
    dur = yogi.Duration.from_nanoseconds(123456789123456789)
    dt = yogi.Timestamp.from_duration_since_epoch(dur).to_datetime()
    assert isinstance(dt, datetime.datetime)
    assert dt.microsecond == 123456


def test_format(mocks: Mocks, hello_bytes: bytes):
    """Verifies that a Timestamp instance can be formatted as a string"""
    def fn(timestamp, str_, strsize, timefmt):
        assert timestamp == 123456789123456789
        assert not timefmt
        assert not strsize
        str_.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_FormatTime(fn)
    t = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_nanoseconds(123456789123456789))
    assert t.format() == 'hello'


def test_format_error(mocks: Mocks):
    """Verifies that errors that occur while formatting a Timestamp instance to a string get reported"""
    def fn(timestamp, str_, strsize, timefmt):
        assert timefmt == b'foo'
        return yogi.ErrorCode.UNKNOWN

    mocks.MOCK_FormatTime(fn)
    t = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_nanoseconds(123456789123456789))
    with pytest.raises(yogi.FailureException):
        t.format('foo')


def test_str(mocks: Mocks, hello_bytes: bytes):
    """Verifies that a Timestmap instance can be converted to a string"""
    def fn(timestamp, str_, strsize, timefmt):
        assert timestamp == 123456789123456789
        assert not timefmt
        assert not strsize
        str_.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_FormatTime(fn)
    t = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_nanoseconds(123456789123456789))
    assert str(t) == 'hello'


def test_add():
    """Verifies that a Duration instance can be added to a Timestmap instance"""
    t = yogi.Timestamp()
    t = t + yogi.Duration.from_nanoseconds(8)
    assert t.duration_since_epoch.nanoseconds_count == 8

    with pytest.raises(ArithmeticError):
        t + yogi.Duration.from_milliseconds(-1)


def test_sub():
    """Verifies that a Duration instance can be subtracted from a Timestmap instance"""
    t = yogi.Timestamp()
    t += yogi.Duration.from_nanoseconds(8)
    t = t - yogi.Duration.from_nanoseconds(2)
    assert t.duration_since_epoch.nanoseconds_count == 6
    assert (t - yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_nanoseconds(2))).nanoseconds_count == 4

    with pytest.raises(ArithmeticError):
        t - yogi.Duration.from_milliseconds(1)


def test_eq():
    """Verifies that Timestamp implements the equality operator"""
    t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
    t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

    assert t1 == t1
    assert not t2 == t1


def test_ne():
    """Verifies that Timestamp implements the inequality operator"""
    t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
    t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

    assert not t1 != t1
    assert t2 != t1


def test_lt():
    """Verifies that Timestamp implements the less-than operator"""
    t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
    t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

    assert t1 < t2
    assert not t2 < t1
    assert not t2 < t2


def test_gt():
    """Verifies that Timestamp implements the greater-than operator"""
    t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
    t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

    assert not t1 > t2
    assert t2 > t1
    assert not t2 > t2


def test_le():
    """Verifies that Timestamp implements the less-or-equal operator"""
    t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
    t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

    assert t1 <= t2
    assert not t2 <= t1
    assert t2 <= t2


def test_ge():
    """Verifies that Timestamp implements the greater-or-equal operator"""
    t1 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(2))
    t2 = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_days(3))

    assert not t1 >= t2
    assert t2 >= t1
    assert t2 >= t2


def test_hash():
    """Verifies that a hash can be computed from a Timestamp instance"""
    assert hash(yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_seconds(1))) != hash(yogi.Timestamp())
