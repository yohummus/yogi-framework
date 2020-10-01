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


class TestOperationId(TestCase):
    def test_from_value(self):
        oid = yogi.OperationId(5)
        self.assertEqual(oid.value, 5)
        self.assertTrue(oid.is_valid)

        oid = yogi.OperationId(0)
        self.assertEqual(oid.value, 0)
        self.assertFalse(oid.is_valid)

        oid = yogi.OperationId(-10)
        self.assertEqual(oid.value, -10)
        self.assertFalse(oid.is_valid)

    def test_comparison_operators(self):
        a = yogi.OperationId(4)
        b = yogi.OperationId(4)
        c = yogi.OperationId(1)

        self.assertTrue(a == b)
        self.assertFalse(a == c)

        self.assertFalse(a != b)
        self.assertTrue(a != c)

    def test_hash(self):
        a = yogi.OperationId(4)
        b = yogi.OperationId(7)
        self.assertNotEqual(hash(a), hash(b))
