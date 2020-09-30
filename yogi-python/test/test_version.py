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


class TestVersion(TestCase):
    def test_bindings_info(self):
        info = yogi.bindings_info
        self.assertIsInstance(info.VERSION, str)
        self.assertEqual(info.VERSION, '{}.{}.{}{}'.format(info.VERSION_MAJOR,
                                                           info.VERSION_MINOR, info.VERSION_PATCH, info.VERSION_SUFFIX))
        self.assertIsInstance(info.VERSION_MAJOR, int)
        self.assertGreaterEqual(info.VERSION_MAJOR, 0)
        self.assertIsInstance(info.VERSION_MINOR, int)
        self.assertGreaterEqual(info.VERSION_MINOR, 0)
        self.assertIsInstance(info.VERSION_PATCH, int)
        self.assertGreaterEqual(info.VERSION_PATCH, 0)
        self.assertIsInstance(info.VERSION_SUFFIX, str)
        self.assertIn(info.VERSION_SUFFIX, info.VERSION)

    def test_core_info(self):
        info = yogi.core_info
        self.assertIsInstance(info.VERSION, str)
        self.assertEqual(info.VERSION, yogi.bindings_info.VERSION)
        self.assertIsInstance(info.VERSION_MAJOR, int)
        self.assertEqual(info.VERSION_MAJOR, yogi.bindings_info.VERSION_MAJOR)
        self.assertIsInstance(info.VERSION_MINOR, int)
        self.assertEqual(info.VERSION_MINOR, yogi.bindings_info.VERSION_MINOR)
        self.assertIsInstance(info.VERSION_PATCH, int)
        self.assertEqual(info.VERSION_PATCH, yogi.bindings_info.VERSION_PATCH)
        self.assertIsInstance(info.VERSION_SUFFIX, str)
        self.assertEqual(info.VERSION_SUFFIX, yogi.bindings_info.VERSION_SUFFIX)
