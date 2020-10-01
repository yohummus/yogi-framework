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

from typing import NamedTuple

from ._library import yogi_core
from ._constants import constants


class BindingsInfo(NamedTuple):
    """Information about the Python bindings"""
    # :CODEGEN_BEGIN:
    VERSION: str = '0.0.1-alpha'
    VERSION_MAJOR: int = 0
    VERSION_MINOR: int = 0
    VERSION_PATCH: int = 1
    VERSION_SUFFIX: str = '-alpha'
    # :CODEGEN_END:


bindings_info = BindingsInfo()


class CoreInfo(NamedTuple):
    """Information about the Yogi Core library"""
    VERSION: str = constants.VERSION
    VERSION_MAJOR: int = constants.VERSION_MAJOR
    VERSION_MINOR: int = constants.VERSION_MINOR
    VERSION_PATCH: int = constants.VERSION_PATCH
    VERSION_SUFFIX: str = constants.VERSION_SUFFIX


core_info = CoreInfo()
