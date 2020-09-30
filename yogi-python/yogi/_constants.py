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

from typing import Union
from ctypes import c_char_p, c_int, c_longlong, byref

from ._library import yogi_core
from ._enums import Verbosity
from ._duration import Duration


def get_constant(api_id, api_type) -> Union[str, int]:
    c = api_type()
    yogi_core.YOGI_GetConstant(byref(c), api_id)
    return c.value.decode() if api_type is c_char_p else c.value


class Constants:
    # :BEGIN_CODEGEN: gen_python_constants
    """Constants built into the Yogi Core library.

    Attributes:
        VERSION                        Whole version string of the core library
        VERSION_MAJOR                  Major version number of the core library
        VERSION_MINOR                  Major version number of the core library
        VERSION_PATCH                  Major version number of the core library
        VERSION_SUFFIX                 Suffix of the core library version
        DEFAULT_ADV_INTERFACES         Default network interfaces to use for advertising
        DEFAULT_ADV_ADDRESS            Default multicast addresses to use for advertising
        DEFAULT_ADV_PORT               Default port to use for advertising via UDP IPv6 multicasts
        DEFAULT_ADV_INTERVAL           Default advertising interval in nanoseconds
        DEFAULT_CONNECTION_TIMEOUT     Default connection timeout in nanoseconds
        DEFAULT_LOGGER_VERBOSITY       Default logging verbosity
        DEFAULT_LOG_TIME_FORMAT        Default textual format for timestamps in log entries
        DEFAULT_LOG_FORMAT             Default textual format for log entries
        MAX_MESSAGE_PAYLOAD_SIZE       Maximum size of the payload in a message
        DEFAULT_TIME_FORMAT            Default textual format for timestamps
        DEFAULT_INF_DURATION_STRING    Default string to denote an infinite duration
        DEFAULT_DURATION_FORMAT        Default textual format for duration strings
        DEFAULT_INVALID_HANDLE_STRING  Default string to denote an invalid object handle
        DEFAULT_OBJECT_FORMAT          Default textual format for strings describing an object
        MIN_TX_QUEUE_SIZE              Minimum size of a send queue for a remote branch
        MAX_TX_QUEUE_SIZE              Maximum size of a send queue for a remote branch
        DEFAULT_TX_QUEUE_SIZE          Default size of a send queue for a remote branch
        MIN_RX_QUEUE_SIZE              Minimum size of a receive queue for a remote branch
        MAX_RX_QUEUE_SIZE              Maximum size of a receive queue for a remote branch
        DEFAULT_RX_QUEUE_SIZE          Default size of a receive queue for a remote branch
        DEFAULT_WEB_PORT               Default port for the web server to listen on for client connections
        DEFAULT_WEB_INTERFACES         Default network interfaces to use for the web server
        DEFAULT_WEB_TIMEOUT            Default timeout for web server connections in nanoseconds
        DEFAULT_HTTP_HEADER_LIMIT      Default maximum HTTP header size of incoming requests in bytes
        DEFAULT_HTTP_BODY_LIMIT        Default maximum HTTP body size of incoming requests in bytes
        DEFAULT_WEB_CACHE_SIZE         Default size of the web server cache in bytes
        MAX_WEB_CACHE_SIZE             Maximum size of the web server cache in bytes
        DEFAULT_ADMIN_USER             Default user name for the administrator account
        DEFAULT_ADMIN_PASSWORD         Default password for the administrator account
        DEFAULT_SSL_PRIVATE_KEY        Default private key to use for SSL connections
        DEFAULT_SSL_CERTIFICATE_CHAIN  Default certificate chain to use for SSL connections
        DEFAULT_SSL_DH_PARAMS          Default DH parameters to use for SSL connections
    """
    VERSION: str = get_constant(1, c_char_p)
    VERSION_MAJOR: int = get_constant(2, c_int)
    VERSION_MINOR: int = get_constant(3, c_int)
    VERSION_PATCH: int = get_constant(4, c_int)
    VERSION_SUFFIX: str = get_constant(5, c_char_p)
    DEFAULT_ADV_INTERFACES: str = get_constant(6, c_char_p)
    DEFAULT_ADV_ADDRESS: str = get_constant(7, c_char_p)
    DEFAULT_ADV_PORT: int = get_constant(8, c_int)
    DEFAULT_ADV_INTERVAL: Duration = Duration.from_nanoseconds(get_constant(9, c_longlong))
    DEFAULT_CONNECTION_TIMEOUT: Duration = Duration.from_nanoseconds(get_constant(10, c_longlong))
    DEFAULT_LOGGER_VERBOSITY: Verbosity = Verbosity(get_constant(11, c_int))
    DEFAULT_LOG_TIME_FORMAT: str = get_constant(12, c_char_p)
    DEFAULT_LOG_FORMAT: str = get_constant(13, c_char_p)
    MAX_MESSAGE_PAYLOAD_SIZE: int = get_constant(14, c_int)
    DEFAULT_TIME_FORMAT: str = get_constant(15, c_char_p)
    DEFAULT_INF_DURATION_STRING: str = get_constant(16, c_char_p)
    DEFAULT_DURATION_FORMAT: str = get_constant(17, c_char_p)
    DEFAULT_INVALID_HANDLE_STRING: str = get_constant(18, c_char_p)
    DEFAULT_OBJECT_FORMAT: str = get_constant(19, c_char_p)
    MIN_TX_QUEUE_SIZE: int = get_constant(20, c_int)
    MAX_TX_QUEUE_SIZE: int = get_constant(21, c_int)
    DEFAULT_TX_QUEUE_SIZE: int = get_constant(22, c_int)
    MIN_RX_QUEUE_SIZE: int = get_constant(23, c_int)
    MAX_RX_QUEUE_SIZE: int = get_constant(24, c_int)
    DEFAULT_RX_QUEUE_SIZE: int = get_constant(25, c_int)
    DEFAULT_WEB_PORT: int = get_constant(26, c_int)
    DEFAULT_WEB_INTERFACES: str = get_constant(27, c_char_p)
    DEFAULT_WEB_TIMEOUT: Duration = Duration.from_nanoseconds(get_constant(28, c_longlong))
    DEFAULT_HTTP_HEADER_LIMIT: int = get_constant(29, c_int)
    DEFAULT_HTTP_BODY_LIMIT: int = get_constant(30, c_int)
    DEFAULT_WEB_CACHE_SIZE: int = get_constant(31, c_int)
    MAX_WEB_CACHE_SIZE: int = get_constant(32, c_int)
    DEFAULT_ADMIN_USER: str = get_constant(33, c_char_p)
    DEFAULT_ADMIN_PASSWORD: str = get_constant(34, c_char_p)
    DEFAULT_SSL_PRIVATE_KEY: str = get_constant(35, c_char_p)
    DEFAULT_SSL_CERTIFICATE_CHAIN: str = get_constant(36, c_char_p)
    DEFAULT_SSL_DH_PARAMS: str = get_constant(37, c_char_p)
    # :END_CODEGEN:


constants = Constants()
