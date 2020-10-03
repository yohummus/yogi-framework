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


def test_from_string():
    """Verifies that a JsonView can be constructed from a string"""
    view = yogi.JsonView("[1]")
    assert view.data == '[1]\0'.encode()
    assert view.size == 4


def test_from_object():
    """Verifies that a JsonView can be constructed from an object"""
    view = yogi.JsonView([5])
    assert view.data == '[5]\0'.encode()
    assert view.size == 4


def test_len_operator():
    """Verifies that len() returns a sensible value"""
    view = yogi.JsonView("[1]")
    assert len(view) == view.size


def test_comparison_operators():
    """Checks the equal and non-equal comparison operators"""
    a = yogi.JsonView("[1]")
    b = yogi.JsonView("[1]")
    c = yogi.JsonView("[2]")

    assert a == b
    assert not a == c

    assert not a != b
    assert a != c


def test_hash():
    """Verifies that a hash can be computed over the JsonView"""
    a = yogi.JsonView("[1]")
    b = yogi.JsonView("[2]")
    assert hash(a) != hash(b)
