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

class OperationId:
    """Represents the ID of an asynchronous operation."""

    def __init__(self, value: int):
        """Constructs the operation ID from a value.

        Args:
            value: Numerical value of the operation ID.
        """
        self._value = value

    @property
    def value(self) -> int:
        """Numerical value of the operation ID."""
        return self._value

    @property
    def is_valid(self) -> bool:
        """True if the operation ID has a valid numerical value."""
        return self._value > 0

    def __eq__(self, rhs: 'OperationId') -> bool:
        return self._value == rhs._value

    def __ne__(self, rhs: 'OperationId') -> bool:
        return not (self == rhs)

    def __hash__(self) -> int:
        return hash(self._value)
