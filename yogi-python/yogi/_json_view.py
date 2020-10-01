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

from typing import Union
import json


class JsonView:
    """Helper class for passing different types of JSON to functions."""

    def __init__(self, jsn: Union[str, object]):
        """Constructs a view from either a string  or an arbitrary object.

        Args:
            jsn: Serialized JSON or an arbitrary object that can be serialized as JSON.
        """
        s = jsn if isinstance(jsn, str) else json.dumps(jsn)
        if s[-1] != '\0':
            s += '\0'

        self._memview = memoryview(s.encode())

    @property
    def data(self) -> memoryview:
        """Serialized JSON data, including the trailing '\0'."""
        return self._memview

    @property
    def size(self) -> int:
        """Length of the serialized JSON data in bytes.

        Includes the trailing '\0'.
        """
        return self._memview.nbytes

    def __len__(self) -> int:  # pylint: disable=invalid-length-returned
        return self.size

    def __eq__(self, rhs: 'JsonView') -> bool:
        return self._memview == rhs._memview

    def __ne__(self, rhs: 'JsonView') -> bool:
        return not (self == rhs)

    def __hash__(self) -> int:
        return hash(self._memview)
