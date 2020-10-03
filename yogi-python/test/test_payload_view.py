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


def test_from_json_view():
    """Verifies that a payload can be constructed from a JsonView"""
    jsn = yogi.JsonView("[1]")
    view = yogi.PayloadView(jsn)
    assert view.data == jsn.data
    assert view.size == jsn.size


def test_from_msgpack_view():
    """Verifies that a payload can be constructed from a MsgpackView"""
    mp = yogi.MsgpackView([1, 2])
    view = yogi.PayloadView(mp)
    assert view.data == mp.data
    assert view.size == mp.size


def test_from_buffer():
    """Verifies that a payload can be constructed from a byte array"""
    buf = bytearray([1, 2, 0])
    view = yogi.PayloadView(buf, len(buf), yogi.Encoding.JSON)
    assert view.data == buf
    assert view.size == len(buf)
    assert view.encoding == yogi.Encoding.JSON

    buf = bytearray([0x92, 1, 2])
    view = yogi.PayloadView(buf, len(buf), yogi.Encoding.MSGPACK)
    assert view.data == buf
    assert view.size == len(buf)
    assert view.encoding == yogi.Encoding.MSGPACK


def test_len_operator():
    """Verifies that len() returns a sensible value"""
    view = yogi.MsgpackView([1, 2, 3])
    assert len(view) == view.size


def test_comparison_operators_payload_view():
    """Verifies that two PayloadView instances can be compared for equality"""
    a = yogi.PayloadView(bytearray([0x92, 1, 0]), 3, yogi.Encoding.MSGPACK)
    b = yogi.PayloadView(bytearray([0x92, 1, 0]), 3, yogi.Encoding.MSGPACK)
    c = yogi.PayloadView(bytearray([0x92, 1, 0]), 3, yogi.Encoding.JSON)

    assert a == b
    assert not a == c

    assert not a != b
    assert a != c


def test_comparison_operators_json_view():
    """Verifies that a PayloadView instance can be compared to a JsonView for equality"""
    view = yogi.PayloadView(yogi.JsonView("[1]"))
    a = yogi.JsonView("[1]")
    b = yogi.JsonView("[2]")

    assert view == a
    assert not view == b

    assert not view != a
    assert view != b


def test_comparison_operators_msgpack_view():
    """Verifies that a PayloadView instance can be compared to a MsgpackView for equality"""
    view = yogi.PayloadView(yogi.MsgpackView([1]))
    a = yogi.MsgpackView([1])
    b = yogi.MsgpackView([2])

    assert view == a
    assert not view == b

    assert not view != a
    assert view != b
