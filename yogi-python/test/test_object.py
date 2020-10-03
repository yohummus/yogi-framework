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


def test_format(mocks: Mocks, context: yogi.Context, hello_bytes: bytes):
    """Verifies that an object can be formatted as a string"""
    def fn(obj, str_, strsize, objfmt, nullstr):
        assert obj == 1234
        assert objfmt is None
        assert nullstr is None
        assert not strsize
        str_.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_FormatObject(fn)
    assert context.format() == 'hello'


def test_format_error(mocks: Mocks, context: yogi.Context):
    """Verifies that errors while formatting an object as a string get reported"""
    def fn(obj, str_, strsize, objfmt, nullstr):
        assert objfmt == b'foo'
        assert nullstr == b'bar'
        assert not strsize
        return yogi.ErrorCode.UNKNOWN

    mocks.MOCK_FormatObject(fn)

    with pytest.raises(yogi.FailureException):
        context.format('foo', 'bar')


def test_str(mocks: Mocks, context: yogi.Context, hello_bytes: bytes):
    """Checks the default string conversion"""
    def fn(obj, str_, strsize, objfmt, nullstr):
        assert obj == 1234
        assert objfmt is None
        assert nullstr is None
        assert not strsize
        str_.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_FormatObject(fn)
    assert str(context) == 'hello'


def test_destroy(mocks: Mocks):
    """Verifies that objects can be destroyed"""
    def fn(context):
        context.contents.value = 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextCreate(fn)
    context = yogi.Context()

    called = False

    def fn2(obj):
        assert obj == 1234
        nonlocal called
        called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_Destroy(fn2)
    del context
    assert called
