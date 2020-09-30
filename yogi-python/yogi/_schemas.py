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

from ._library import yogi_core
from ._enums import Schema, ErrorCode
from ._errors import FailureException


def get_schema(schema: Schema) -> str:
    """Returns a built-in JSON Schema.

    The built-in schemas may contain references to other schemas. These
    references are based on the schema filenames as set in the $id property.

    The schema will be returned formatted and with whitespace.

    Args:
        schema: Schema to retrieve.

    Returns:
        Schema as serialized JSON.
    """
    scm = yogi_core.YOGI_GetSchema(schema).decode()
    if not scm:
        raise FailureException(ErrorCode.INVALID_PARAM)

    return scm
