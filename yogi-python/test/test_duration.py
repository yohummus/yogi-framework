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
import datetime
import math

from .conftest import Mocks


def test_zero():
    """Checks the special zero duration value"""
    dur = yogi.Duration.ZERO
    assert dur.is_finite
    assert dur.nanoseconds_count == 0


def test_infinity():
    """Checks the special infinity duration value"""
    dur = yogi.Duration.INF
    assert not dur.is_finite
    assert dur > yogi.Duration.ZERO


def test_negative_infinity():
    """Checks the special negative infinity duration value"""
    dur = yogi.Duration.NEG_INF
    assert not dur.is_finite
    assert dur < yogi.Duration.ZERO


def test_from_nanoseconds():
    """Verifies that a Duration object can be constructed from a number of nanoseconds"""
    dur = yogi.Duration.from_nanoseconds(123)
    assert dur.is_finite
    assert dur.nanoseconds_count == 123

    dur = yogi.Duration.from_nanoseconds(444.56)
    assert dur.nanoseconds_count == 444  # Note: no rounding

    dur = yogi.Duration.from_nanoseconds(math.inf)
    assert dur == yogi.Duration.INF

    dur = yogi.Duration.from_nanoseconds(-math.inf)
    assert dur == yogi.Duration.NEG_INF


def test_from_microseconds():
    """Verifies that a Duration object can be constructed from a number of microseconds"""
    dur = yogi.Duration.from_microseconds(123)
    assert dur.is_finite
    assert dur.total_microseconds == 123

    dur = yogi.Duration.from_microseconds(math.inf)
    assert dur == yogi.Duration.INF

    dur = yogi.Duration.from_microseconds(0.5)
    assert dur.nanoseconds_count == 500


def test_from_milliseconds():
    """Verifies that a Duration object can be constructed from a number of milliseconds"""
    dur = yogi.Duration.from_milliseconds(123)
    assert dur.is_finite
    assert dur.total_milliseconds == 123

    dur = yogi.Duration.from_milliseconds(math.inf)
    assert dur == yogi.Duration.INF

    dur = yogi.Duration.from_milliseconds(0.5)
    assert dur.total_microseconds == 500


def test_from_seconds():
    """Verifies that a Duration object can be constructed from a number of seconds"""
    dur = yogi.Duration.from_seconds(123)
    assert dur.is_finite
    assert dur.total_seconds == 123

    dur = yogi.Duration.from_seconds(math.inf)
    assert dur == yogi.Duration.INF

    dur = yogi.Duration.from_seconds(0.5)
    assert dur.total_milliseconds == 500


def test_from_minutes():
    """Verifies that a Duration object can be constructed from a number of minutes"""
    dur = yogi.Duration.from_minutes(123)
    assert dur.is_finite
    assert dur.total_minutes == 123

    dur = yogi.Duration.from_minutes(math.inf)
    assert dur == yogi.Duration.INF

    dur = yogi.Duration.from_minutes(0.5)
    assert dur.total_seconds == 30


def test_from_hours():
    """Verifies that a Duration object can be constructed from a number of hours"""
    dur = yogi.Duration.from_hours(123)
    assert dur.is_finite
    assert dur.total_hours == 123

    dur = yogi.Duration.from_hours(math.inf)
    assert dur == yogi.Duration.INF

    dur = yogi.Duration.from_hours(0.5)
    assert dur.total_minutes == 30


def test_from_days():
    """Verifies that a Duration object can be constructed from a number of days"""
    dur = yogi.Duration.from_days(123)
    assert dur.is_finite
    assert dur.total_days == 123

    dur = yogi.Duration.from_days(math.inf)
    assert dur == yogi.Duration.INF

    dur = yogi.Duration.from_days(0.5)
    assert dur.total_hours == 12


def test_constructor():
    """Checks the Duration constructor"""
    dur = yogi.Duration()
    assert dur.is_finite
    assert dur.nanoseconds_count == 0

    dur = yogi.Duration(datetime.timedelta(seconds=12))
    assert dur.is_finite
    assert dur.total_seconds == 12

    dur = yogi.Duration(datetime.timedelta(milliseconds=33))
    assert dur.is_finite
    assert dur.total_milliseconds == 33


def test_nanoseconds():
    """Verifies the nanoseconds property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.nanoseconds == 887


def test_microseconds():
    """Verifies the microseconds property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.microseconds == 465


def test_milliseconds():
    """Verifies the milliseconds property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.milliseconds == 132


def test_seconds():
    """Verifies the seconds property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.seconds == 9


def test_minutes():
    """Verifies the minutes property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.minutes == 33


def test_hours():
    """Verifies the hours property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.hours == 21


def test_days():
    """Verifies the days property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.days == 1428


def test_nanoseconds_count():
    """Verifies the nanoseconds_count property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.nanoseconds_count == 123456789132465887


def test_total_nanoseconds():
    """Verifies the total_nanoseconds property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.total_nanoseconds == pytest.approx(123456789132465887)

    assert yogi.Duration.INF.total_nanoseconds == math.inf
    assert yogi.Duration.NEG_INF.total_nanoseconds == -math.inf


def test_total_microseconds():
    """Verifies the total_microseconds property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.total_microseconds == pytest.approx(123456789132465.887)

    assert yogi.Duration.INF.total_microseconds == math.inf
    assert yogi.Duration.NEG_INF.total_microseconds == -math.inf


def test_total_milliseconds():
    """Verifies the total_milliseconds property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.total_milliseconds == pytest.approx(123456789132.465887)

    assert yogi.Duration.INF.total_milliseconds == math.inf
    assert yogi.Duration.NEG_INF.total_milliseconds == -math.inf


def test_total_seconds():
    """Verifies the total_seconds property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.total_seconds == pytest.approx(123456789.132465887)

    assert yogi.Duration.INF.total_seconds == math.inf
    assert yogi.Duration.NEG_INF.total_seconds == -math.inf


def test_total_minutes():
    """Verifies the total_minutes property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.total_minutes == pytest.approx(2057613.1522077648)

    assert yogi.Duration.INF.total_minutes == math.inf
    assert yogi.Duration.NEG_INF.total_minutes == -math.inf


def test_total_hours():
    """Verifies the total_hours property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.total_hours == pytest.approx(34293.55253679608)

    assert yogi.Duration.INF.total_hours == math.inf
    assert yogi.Duration.NEG_INF.total_hours == -math.inf


def test_total_days():
    """Verifies the total_days property"""
    dur = yogi.Duration.from_nanoseconds(123456789132465887)
    assert dur.total_days == pytest.approx(1428.8980223665033)

    assert yogi.Duration.INF.total_days == math.inf
    assert yogi.Duration.NEG_INF.total_days == -math.inf


def test_negated():
    """Verifies the negated property"""
    dur = yogi.Duration.from_nanoseconds(123)
    neg_dur = dur.negated
    assert -neg_dur.nanoseconds_count == dur.nanoseconds_count
    assert neg_dur.is_finite


def test_to_timedelta():
    """Verifies that the duration can be converted to a datetime.timedelta"""
    dur = yogi.Duration.from_nanoseconds(123456789132465000)
    assert dur.to_timedelta() == datetime.timedelta(microseconds=123456789132465)

    assert yogi.Duration.INF.to_timedelta() == datetime.timedelta.max
    assert yogi.Duration.NEG_INF.to_timedelta() == datetime.timedelta.min


def test_is_finite():
    """Verifies the is_finite property"""
    assert yogi.Duration.ZERO.is_finite
    assert yogi.Duration.from_nanoseconds(123).is_finite
    assert not yogi.Duration.INF.is_finite
    assert not yogi.Duration.NEG_INF.is_finite


def test_format(mocks: Mocks, hello_bytes: bytes):
    """Verifies that a duration can be formatted as a string"""
    dur = yogi.Duration.from_nanoseconds(123456789123456789)

    def fn(dur, neg, str_, strsize, durfmt, inffmt):
        assert dur == 123456789123456789
        assert neg == 0
        assert str_
        assert not strsize
        assert durfmt == b'bla'
        assert inffmt == b'moo'
        str_.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_FormatDuration(fn)

    s = dur.format('bla', 'moo')
    assert s == 'hello'


def test_format_error(mocks: Mocks):
    """Verifies that errors during formatting are reported"""
    dur = yogi.Duration.from_nanoseconds(123456789123456789)

    mocks.MOCK_FormatDuration(lambda *_: yogi.ErrorCode.WRONG_OBJECT_TYPE)
    with pytest.raises(yogi.FailureException):
        dur.format()


def test_format_inf(mocks: Mocks, hello_bytes: bytes):
    """Checks the formatted string for an infinite duration"""
    def fn(dur, neg, str_, strsize, durfmt, inffmt):
        assert dur == -1
        assert neg == 1
        assert str_
        str_.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_FormatDuration(fn)

    assert yogi.Duration.NEG_INF.format() == 'hello'


def test_str(mocks: Mocks, hello_bytes: bytes):
    """Checks the default conversion to a string"""
    dur = yogi.Duration.from_nanoseconds(123456789123456789)

    def fn(dur, neg, str_, strsize, durfmt, inffmt):
        assert dur == 123456789123456789
        assert neg == 0
        assert str_
        assert not strsize
        assert durfmt is None
        assert inffmt is None
        str_.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_FormatDuration(fn)

    s = str(dur)
    assert s == 'hello'


def test_add():
    """Verifies that two durations can be added"""
    dur1 = yogi.Duration.from_nanoseconds(10)
    dur2 = yogi.Duration.from_microseconds(3)
    assert (dur1 + dur2).nanoseconds_count == 3010

    assert dur1 + yogi.Duration.INF == yogi.Duration.INF
    assert yogi.Duration.INF + yogi.Duration.INF == yogi.Duration.INF
    assert dur1 + yogi.Duration.NEG_INF == yogi.Duration.NEG_INF
    assert yogi.Duration.NEG_INF + yogi.Duration.NEG_INF == yogi.Duration.NEG_INF

    with pytest.raises(ArithmeticError):
        yogi.Duration.INF + yogi.Duration.NEG_INF


def test_sub():
    """Verifies that a duration can be subtracted from another duration"""
    dur1 = yogi.Duration.from_nanoseconds(10)
    dur2 = yogi.Duration.from_microseconds(3)
    assert (dur1 - dur2).nanoseconds_count == -2990

    assert dur1 - yogi.Duration.INF == yogi.Duration.NEG_INF
    assert yogi.Duration.INF - yogi.Duration.NEG_INF == yogi.Duration.INF
    assert dur1 - yogi.Duration.NEG_INF == yogi.Duration.NEG_INF
    assert yogi.Duration.NEG_INF - yogi.Duration.INF == yogi.Duration.NEG_INF

    with pytest.raises(ArithmeticError):
        yogi.Duration.NEG_INF - yogi.Duration.NEG_INF


def test_mul():
    """Verifies that a duration can be multiplied by a number"""
    dur = yogi.Duration.from_nanoseconds(8)

    assert (dur * 3).nanoseconds_count == 24
    assert (dur * 3.5).nanoseconds_count == 28

    assert yogi.Duration.INF * 5 == yogi.Duration.INF
    assert yogi.Duration.INF * -5 == yogi.Duration.NEG_INF

    with pytest.raises(ArithmeticError):
        yogi.Duration.INF * 0


def test_truediv():
    """Verifies that a duration can be divided by a number"""
    dur = yogi.Duration.from_nanoseconds(28)

    assert (dur / 2).nanoseconds_count == 14
    assert (dur / 3.5).nanoseconds_count == 8

    assert yogi.Duration.INF / 5 == yogi.Duration.INF
    assert yogi.Duration.INF / -5 == yogi.Duration.NEG_INF

    with pytest.raises(ZeroDivisionError):
        dur / 0


def test_eq():
    """Checks the equal comparison operator between two durations"""
    dur1 = yogi.Duration.from_nanoseconds(1000)
    dur2 = yogi.Duration.from_microseconds(1)
    dur3 = yogi.Duration.from_nanoseconds(2)
    assert dur1 == dur2
    assert not dur1 == dur3


def test_ne():
    """Checks the not-equal comparison operator between two durations"""
    dur1 = yogi.Duration.from_nanoseconds(1000)
    dur2 = yogi.Duration.from_microseconds(1)
    dur3 = yogi.Duration.from_nanoseconds(2)
    assert not dur1 != dur2
    assert dur1 != dur3


def test_lt():
    """Checks the less-than comparison operator between two durations"""
    dur1 = yogi.Duration.from_nanoseconds(1)
    dur2 = yogi.Duration.from_nanoseconds(1)
    dur3 = yogi.Duration.from_nanoseconds(2)
    assert not dur1 < dur2
    assert dur2 < dur3
    assert not dur3 < dur1

    assert not yogi.Duration.INF < yogi.Duration.INF
    assert yogi.Duration.NEG_INF < yogi.Duration.INF
    assert not yogi.Duration.INF < yogi.Duration.ZERO
    assert yogi.Duration.NEG_INF < yogi.Duration.ZERO


def test_gt():
    """Checks the greater-than comparison operator between two durations"""
    dur1 = yogi.Duration.from_nanoseconds(1)
    dur2 = yogi.Duration.from_nanoseconds(1)
    dur3 = yogi.Duration.from_nanoseconds(2)
    assert not dur1 > dur2
    assert not dur2 > dur3
    assert dur3 > dur1

    assert not yogi.Duration.INF > yogi.Duration.INF
    assert not yogi.Duration.NEG_INF > yogi.Duration.INF
    assert yogi.Duration.INF > yogi.Duration.ZERO
    assert not yogi.Duration.NEG_INF > yogi.Duration.ZERO


def test_le():
    """Checks the less-or-equal comparison operator between two durations"""
    dur1 = yogi.Duration.from_nanoseconds(1)
    dur2 = yogi.Duration.from_nanoseconds(1)
    dur3 = yogi.Duration.from_nanoseconds(2)
    assert dur1 <= dur2
    assert dur2 <= dur3
    assert not dur3 <= dur1

    assert yogi.Duration.INF <= yogi.Duration.INF
    assert yogi.Duration.NEG_INF <= yogi.Duration.INF
    assert not yogi.Duration.INF <= yogi.Duration.ZERO
    assert yogi.Duration.NEG_INF <= yogi.Duration.ZERO


def test_ge():
    """Checks the greater-or-equal comparison operator between two durations"""
    dur1 = yogi.Duration.from_nanoseconds(1)
    dur2 = yogi.Duration.from_nanoseconds(1)
    dur3 = yogi.Duration.from_nanoseconds(2)
    assert dur1 >= dur2
    assert not dur2 >= dur3
    assert dur3 >= dur1

    assert yogi.Duration.INF >= yogi.Duration.INF
    assert not yogi.Duration.NEG_INF >= yogi.Duration.INF
    assert yogi.Duration.INF >= yogi.Duration.ZERO
    assert not yogi.Duration.NEG_INF >= yogi.Duration.ZERO


def test_hash():
    """Verifies that a hash can be computed from a duration"""
    dur1 = yogi.Duration.from_days(1)
    dur2 = yogi.Duration.from_days(2)
    assert hash(dur1) != hash(dur2)
