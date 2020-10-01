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

from typing import Union, Optional

from ._json_view import JsonView
from ._msgpack_view import MsgpackView
from ._enums import Encoding


class PayloadView:
    """Helper class for passing and converting different types of user payload."""

    def __init__(self, buf_or_view: Union[bytearray, JsonView, MsgpackView],
                 size: Optional[int] = None, encoding: Optional[Encoding] = None):
        """Constructs a view from a buffer, a JsonView or a MsgpackView.

        Args:
            buf_or_view: Byte array, JsonView or MsgpackView.
            size:        Data size in the bytes array.
            encoding:    Encoding of the data in the bytes array.
        """
        if isinstance(buf_or_view, JsonView):
            assert size is None
            assert encoding is None
            self._memview = buf_or_view.data
            self._size = buf_or_view.size
            self._encoding = Encoding.JSON
        elif isinstance(buf_or_view, MsgpackView):
            assert size is None
            assert encoding is None
            self._memview = buf_or_view.data
            self._size = buf_or_view.size
            self._encoding = Encoding.MSGPACK
        elif isinstance(buf_or_view, bytearray):
            assert size is not None
            assert encoding is not None
            self._memview = memoryview(buf_or_view)[:size]
            self._size = size
            self._encoding = encoding
        else:
            raise TypeError(
                'Invalid type ({}) passed to constructor.'.format(
                    type(buf_or_view)))

    @property
    def data(self) -> memoryview:
        """Buffer holding the payload data."""
        return self._memview

    @property
    def size(self) -> int:
        """Size of the payload data in bytes."""
        return self._size

    @property
    def encoding(self) -> Encoding:
        """Encoding of the payload data."""
        return self._encoding

    def __len__(self) -> int:  # pylint: disable=invalid-length-returned
        return self.size

    def __eq__(self, rhs: Union['PayloadView', JsonView, MsgpackView]) -> bool:
        if isinstance(rhs, JsonView):
            rhs_enc = Encoding.JSON
        elif isinstance(rhs, MsgpackView):
            rhs_enc = Encoding.MSGPACK
        else:
            rhs_enc = rhs.encoding

        return self._encoding == rhs_enc and self._memview == rhs._memview

    def __ne__(self, rhs: Union['PayloadView', JsonView, MsgpackView]) -> bool:
        return not (self == rhs)
