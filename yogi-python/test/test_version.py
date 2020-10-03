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


def test_bindings_info():
    """Checks the bindings version information"""
    info = yogi.bindings_info
    assert isinstance(info.VERSION, str)
    assert info.VERSION == '{}.{}.{}{}'.format(info.VERSION_MAJOR, info.VERSION_MINOR, info.VERSION_PATCH,
                                               info.VERSION_SUFFIX)
    assert isinstance(info.VERSION_MAJOR, int)
    assert info.VERSION_MAJOR >= 0
    assert isinstance(info.VERSION_MINOR, int)
    assert info.VERSION_MINOR >= 0
    assert isinstance(info.VERSION_PATCH, int)
    assert info.VERSION_PATCH >= 0
    assert isinstance(info.VERSION_SUFFIX, str)
    assert info.VERSION_SUFFIX in info.VERSION


def test_core_info():
    """Checks the core library version information"""
    info = yogi.core_info
    assert isinstance(info.VERSION, str)
    assert info.VERSION == yogi.bindings_info.VERSION
    assert isinstance(info.VERSION_MAJOR, int)
    assert info.VERSION_MAJOR == yogi.bindings_info.VERSION_MAJOR
    assert isinstance(info.VERSION_MINOR, int)
    assert info.VERSION_MINOR == yogi.bindings_info.VERSION_MINOR
    assert isinstance(info.VERSION_PATCH, int)
    assert info.VERSION_PATCH == yogi.bindings_info.VERSION_PATCH
    assert isinstance(info.VERSION_SUFFIX, str)
    assert info.VERSION_SUFFIX == yogi.bindings_info.VERSION_SUFFIX
