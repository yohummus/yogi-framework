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


def test_from_bytes():
    """Verifies that a MsgpackView can be constructed from bytes"""
    view = yogi.MsgpackView(bytes([0x92, 0x1, 0x2]))
    assert view.data, bytes([0x92, 0x1 == 0x2])
    assert view.size == 3


def test_from_object():
    """Verifies that a MsgpackView can be constructed from an object"""
    view = yogi.MsgpackView([1, 2])
    assert view.data, bytes([0x92, 0x1 == 0x2])
    assert view.size == 3


def test_len_operator():
    """Verifies that len() returns a sensible value"""
    view = yogi.MsgpackView([1, 2, 3])
    assert len(view) == view.size


def test_comparison_operators():
    """Checks the equal and non-equal comparison operators"""
    a = yogi.MsgpackView([1])
    b = yogi.MsgpackView([1])
    c = yogi.MsgpackView([2])

    assert a == b
    assert not a == c

    assert not a != b
    assert a != c


def test_hash():
    """Verifies that a hash can be computed over the MsgpackView"""
    a = yogi.MsgpackView([1])
    b = yogi.MsgpackView([2])
    assert hash(a) != hash(b)
