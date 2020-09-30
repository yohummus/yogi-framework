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

from typing import NamedTuple

from ._library import yogi_core
from ._constants import constants


class BindingsInfo(NamedTuple):
    """Information about the Python bindings"""
    # :BEGIN_CODEGEN: gen_python_version
    VERSION: str = '0.0.1-alpha'
    VERSION_MAJOR: int = 0
    VERSION_MINOR: int = 0
    VERSION_PATCH: int = 1
    VERSION_SUFFIX: str = '-alpha'
    # :END_CODEGEN:


bindings_info = BindingsInfo()


class CoreInfo(NamedTuple):
    """Information about the Yogi Core library"""
    VERSION: str = constants.VERSION
    VERSION_MAJOR: int = constants.VERSION_MAJOR
    VERSION_MINOR: int = constants.VERSION_MINOR
    VERSION_PATCH: int = constants.VERSION_PATCH
    VERSION_SUFFIX: str = constants.VERSION_SUFFIX


core_info = CoreInfo()
