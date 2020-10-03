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
import re

from .conftest import Mocks


def test_some(mocks: Mocks):
    """Checks the values of some constants with different types"""
    assert len(yogi.constants.VERSION) > 4
    assert yogi.constants.DEFAULT_ADV_PORT == 1
    assert yogi.constants.DEFAULT_LOGGER_VERBOSITY != yogi.Verbosity.FATAL
    assert yogi.constants.DEFAULT_ADV_INTERVAL.total_milliseconds > 1


def test_types(mocks: Mocks):
    """Verifies that the constants actually have the type they claim in their type annotations"""
    Constants = type(yogi.constants)
    names = [name for name in Constants.__dict__.keys() if re.match(r'^[A-Z_]+$', name)]
    for name in names:
        claimed_type = Constants.__annotations__[name]  # pylint: disable=no-member
        actual_type = type(getattr(Constants, name))
        assert claimed_type == actual_type
