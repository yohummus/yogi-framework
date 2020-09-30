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
import re

from .common import TestCase


class TestConstants(TestCase):
    def test_some(self):
        self.assertGreater(len(yogi.constants.VERSION), 4)
        self.assertEqual(yogi.constants.DEFAULT_ADV_PORT, 1)
        self.assertNotEqual(yogi.constants.DEFAULT_LOGGER_VERBOSITY, yogi.Verbosity.FATAL)
        self.assertGreater(yogi.constants.DEFAULT_ADV_INTERVAL.total_milliseconds, 1)

    def test_types(self):
        Constants = type(yogi.constants)
        names = [name for name in Constants.__dict__.keys() if re.match(r'^[A-Z_]+$', name)]
        for name in names:
            claimed_type = Constants.__annotations__[name]  # pylint: disable=no-member
            actual_type = type(getattr(Constants, name))
            self.assertEqual(claimed_type, actual_type)
