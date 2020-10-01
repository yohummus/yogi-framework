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


class TestErrors(TestCase):
    def test_result(self):
        self.assertFalse(yogi.Result(-1))
        self.assertFalse(yogi.Result(yogi.ErrorCode.BAD_ALLOC))
        self.assertTrue(yogi.Result(0))
        self.assertTrue(yogi.Result(1))
        self.assertEqual(yogi.Result(10).value, 10)
        self.assertEqual(yogi.Result(3), yogi.Result(3))
        self.assertNotEqual(yogi.Result(2), yogi.Result(3))
        self.assertEqual(str(yogi.Result(yogi.ErrorCode.BAD_ALLOC)), 'Dummy error string')
        self.assertNotEqual(hash(yogi.Result(1)), hash(yogi.Result(2)))
        self.assertIsInstance(yogi.Result(yogi.ErrorCode.BAD_ALLOC).error_code, yogi.ErrorCode)
        self.assertEqual(yogi.Result(0).error_code, yogi.Result(1).error_code)

    def test_failure(self):
        self.assertFalse(yogi.Failure(yogi.ErrorCode.BAD_ALLOC))
        self.assertEqual(str(yogi.Failure(yogi.ErrorCode.BAD_ALLOC)), str(yogi.Result(yogi.ErrorCode.BAD_ALLOC)))
        self.assertEqual(yogi.Failure(yogi.ErrorCode.BAD_ALLOC), yogi.Result(yogi.ErrorCode.BAD_ALLOC))
        self.assertIsInstance(yogi.Failure(yogi.ErrorCode.BAD_ALLOC), yogi.Result)

    def test_detailed_failure(self):
        self.assertEqual(yogi.DetailedFailure(yogi.ErrorCode.BAD_ALLOC, "ab").details, "ab")
        self.assertIsInstance(yogi.DetailedFailure(yogi.ErrorCode.BAD_ALLOC, "ab"), yogi.Failure)

    def test_success(self):
        self.assertTrue(yogi.Success(1))
        self.assertTrue(yogi.Success(0))
        self.assertEqual(str(yogi.Success(1)), str(yogi.Result(1)))
        self.assertEqual(yogi.Success(1), yogi.Result(1))
        self.assertIsInstance(yogi.Success(1), yogi.Result)