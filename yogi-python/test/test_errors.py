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


def test_result():
    """Checks the Result class"""
    assert not yogi.Result(-1)
    assert not yogi.Result(yogi.ErrorCode.BAD_ALLOC)
    assert yogi.Result(0)
    assert yogi.Result(1)
    assert yogi.Result(10).value == 10
    assert yogi.Result(3) == yogi.Result(3)
    assert yogi.Result(2) != yogi.Result(3)
    assert str(yogi.Result(yogi.ErrorCode.BAD_ALLOC)) == 'Dummy error string'
    assert hash(yogi.Result(1)) != hash(yogi.Result(2))
    assert isinstance(yogi.Result(yogi.ErrorCode.BAD_ALLOC).error_code, yogi.ErrorCode)
    assert yogi.Result(0).error_code == yogi.Result(1).error_code


def test_failure():
    """Checks the Failure class"""
    assert not yogi.Failure(yogi.ErrorCode.BAD_ALLOC)
    assert str(yogi.Failure(yogi.ErrorCode.BAD_ALLOC)) == str(yogi.Result(yogi.ErrorCode.BAD_ALLOC))
    assert yogi.Failure(yogi.ErrorCode.BAD_ALLOC) == yogi.Result(yogi.ErrorCode.BAD_ALLOC)
    assert isinstance(yogi.Failure(yogi.ErrorCode.BAD_ALLOC), yogi.Result)


def test_detailed_failure():
    """Checks the DetailedFailure class"""
    assert yogi.DetailedFailure(yogi.ErrorCode.BAD_ALLOC, "ab").details == "ab"
    assert isinstance(yogi.DetailedFailure(yogi.ErrorCode.BAD_ALLOC, "ab"), yogi.Failure)


def test_success():
    """Checks the Success class"""
    assert yogi.Success(1)
    assert yogi.Success(0)
    assert str(yogi.Success(1)) == str(yogi.Result(1))
    assert yogi.Success(1) == yogi.Result(1)
    assert isinstance(yogi.Success(1), yogi.Result)
