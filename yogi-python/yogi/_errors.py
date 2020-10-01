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

from ctypes import create_string_buffer
from typing import Callable, Union

from ._enums import ErrorCode


class Result:
    """Represents a result of an operation.

    This is a wrapper around the result code returned by the functions from
    the Yogi Core library. A result is represented by a number which is >= 0
    in case of success and < 0 in case of a failure.
    """

    def __init__(self, value_or_ec: Union[int, ErrorCode]):
        """Constructs the result.

        Args:
            value: Number as returned by the Yogi Core library functions.
        """
        if isinstance(value_or_ec, int):
            self._value = value_or_ec
        else:
            self._value = value_or_ec.value

    @property
    def value(self) -> int:
        """The number as returned by the Yogi Core library functions."""
        return self._value

    @property
    def error_code(self) -> ErrorCode:
        """Error code associated with this result."""
        return ErrorCode(self._value) if self._value < 0 else ErrorCode.OK

    def __bool__(self) -> bool:
        return self._value >= 0

    def __eq__(self, other) -> bool:
        return isinstance(other, Result) and self._value == other.value

    def __ne__(self, other) -> bool:
        return not (self == other)

    def __str__(self) -> str:
        from ._library import yogi_core
        return yogi_core.YOGI_GetErrorString(self._value).decode()

    def __hash__(self) -> int:
        return hash(self._value)


class Success(Result):
    """Represents the success of an operation."""

    def __init__(self, value: int = 0):
        super().__init__(value)


class Failure(Result):
    """Represents the failure of an operation."""

    def __init__(self, ec: ErrorCode):
        """Constructs the Failure.

        Args:
            ec: Error code.
        """
        super().__init__(ec.value)

    def __str__(self) -> str:
        return Result.__str__(self)


class DetailedFailure(Failure):
    """A failure of an operation that includes a detailed description."""

    def __init__(self, ec: ErrorCode, details: str):
        """Constructs the DetailedFailure.

        Args:
            ec: Error code.
            details: Additional information about the failure.
        """
        super().__init__(ec)
        self._details = details

    @property
    def details(self) -> str:
        """More detailed information about the error."""
        return self._details

    def __str__(self) -> str:
        return Failure.__str__(self) + ". Details: " + self._details


class Exception(Exception):
    """Base class for all Yogi exceptions."""
    @property
    def failure(self) -> Failure:
        """Wrapped Failure or DetailedFailure object."""
        raise NotImplementedError()


class FailureException(Exception):
    """Exception wrapping a Failure object.

    This exception type is used for failures without a detailed description.
    """

    def __init__(self, ec: ErrorCode):
        """Constructs the exception.

        Args:
            ec: Error code.
        """
        self._failure = Failure(ec)

    @property
    def failure(self) -> Failure:
        """Wrapped Failure object."""
        return self._failure


class DetailedFailureException(FailureException):
    """Exception wrapping a DetailedFailure object.

    This exception type is used for failures that have detailed information available.
    """

    def __init__(self, ec: ErrorCode, details: str):
        """Constructs the exception.

        Args:
            ec: Error code.
            details: Additional information about the failure.
        """
        self._failure = DetailedFailure(ec, details)

    @property
    def failure(self) -> DetailedFailure:
        """Wrapped DetailedFailure object."""
        return self._failure


def api_result_handler(res: int) -> Success:
    if res >= 0:
        return Success(res)
    else:
        from ._library import yogi_core
        details = yogi_core.YOGI_GetLastErrorDetails().decode()
        if details:
            raise DetailedFailureException(ErrorCode(res), details)
        else:
            raise FailureException(ErrorCode(res))


def error_code_to_result(res: int) -> Result:
    if res < 0:
        return Failure(ErrorCode(res))
    else:
        return Success(res)


def false_if_specific_ec_else_raise(res: int, ec: ErrorCode) -> Result:
    if res == ec:
        return False
    elif res < 0:
        raise FailureException(ErrorCode(res))
    else:
        return True
