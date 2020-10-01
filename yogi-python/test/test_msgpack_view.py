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

from .common import TestCase


class TestMsgpackView(TestCase):
    def test_from_bytes(self):
        view = yogi.MsgpackView(bytes([0x92, 0x1, 0x2]))
        self.assertEqual(view.data, bytes([0x92, 0x1, 0x2]))
        self.assertEqual(view.size, 3)

    def test_from_object(self):
        view = yogi.MsgpackView([1, 2])
        self.assertEqual(view.data, bytes([0x92, 0x1, 0x2]))
        self.assertEqual(view.size, 3)

    def test_len_operator(self):
        view = yogi.MsgpackView([1, 2, 3])
        self.assertEqual(len(view), view.size)

    def test_comparison_operators(self):
        a = yogi.MsgpackView([1])
        b = yogi.MsgpackView([1])
        c = yogi.MsgpackView([2])

        self.assertTrue(a == b)
        self.assertFalse(a == c)

        self.assertFalse(a != b)
        self.assertTrue(a != c)

    def test_hash(self):
        a = yogi.MsgpackView([1])
        b = yogi.MsgpackView([2])
        self.assertNotEqual(hash(a), hash(b))
