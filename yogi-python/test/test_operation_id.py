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


def test_from_value():
    """Verifies that an OperationId can be constructed from a numeric value"""
    oid = yogi.OperationId(5)
    assert oid.value == 5
    assert oid.is_valid

    oid = yogi.OperationId(0)
    assert oid.value == 0
    assert not oid.is_valid

    oid = yogi.OperationId(-10)
    assert oid.value == -10
    assert not oid.is_valid


def test_comparison_operators():
    """Verifies that two OperationId instances can be compared for equality/non-equality"""
    a = yogi.OperationId(4)
    b = yogi.OperationId(4)
    c = yogi.OperationId(1)

    assert a == b
    assert not a == c

    assert not a != b
    assert a != c


def test_hash():
    """Verifies that a hash can be computed for an OperationId"""
    a = yogi.OperationId(4)
    b = yogi.OperationId(7)
    assert hash(a) != hash(b)
