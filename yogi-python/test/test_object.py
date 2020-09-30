# This file is part of the Yogi distribution https://github.com/yohummus/yogi.
# Copyright (c) 2019 Johannes Bergmann.
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.

import yogi

from .common import TestCase


class TestContext(TestCase):
    def setUp(self):
        self.obj = self.create_context()

    def test_format(self):
        def fn(obj, str_, strsize, objfmt, nullstr):
            self.assertEqual(obj, self.pointer.value)
            self.assertIsNone(objfmt)
            self.assertIsNone(nullstr)
            self.assertFalse(strsize)
            str_.contents.value = self.hello_bytes
            return yogi.ErrorCode.OK

        self.MOCK_FormatObject(fn)
        self.assertEqual(self.obj.format(), 'hello')

    def test_format_error(self):
        def fn(obj, str_, strsize, objfmt, nullstr):
            self.assertEqual(objfmt, b'foo')
            self.assertEqual(nullstr, b'bar')
            self.assertFalse(strsize)
            return yogi.ErrorCode.UNKNOWN

        self.MOCK_FormatObject(fn)
        self.assertRaises(yogi.FailureException, lambda: self.obj.format('foo', 'bar'))

    def test_str(self):
        def fn(obj, str_, strsize, objfmt, nullstr):
            self.assertEqual(obj, self.pointer.value)
            self.assertIsNone(objfmt)
            self.assertIsNone(nullstr)
            self.assertFalse(strsize)
            str_.contents.value = self.hello_bytes
            return yogi.ErrorCode.OK

        self.MOCK_FormatObject(fn)
        self.assertEqual(str(self.obj), 'hello')

    def test_destroy(self):
        called = False

        def fn(obj):
            self.assertEqual(obj, self.pointer.value)
            nonlocal called
            called = True
            return yogi.ErrorCode.OK

        self.MOCK_Destroy(fn)
        del self.obj
        self.assertTrue(called)
