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

from enum import IntEnum


class DocIntEnum(IntEnum):
    def __new__(cls, value, doc=None):
        self = int.__new__(cls, value)
        self._value_ = value
        if doc is not None:
            self.__doc__ = doc
        return self


class ErrorCode(DocIntEnum):
    """Error codes returned by functions from the Yogi Core library"""
    # :CODEGEN_BEGIN:
    OK = 0, 'Success'
    UNKNOWN = -1, 'Unknown internal error occured'
    OBJECT_STILL_USED = -2, 'The object is still being used by another object'
    BAD_ALLOC = -3, 'Insufficient memory to complete the operation'
    INVALID_PARAM = -4, 'Invalid parameter'
    INVALID_HANDLE = -5, 'Invalid Handle'
    WRONG_OBJECT_TYPE = -6, 'Object is of the wrong type'
    CANCELED = -7, 'The operation has been canceled'
    BUSY = -8, 'Operation failed because the object is busy'
    TIMEOUT = -9, 'The operation timed out'
    TIMER_EXPIRED = -10, 'The timer has not been started or already expired'
    BUFFER_TOO_SMALL = -11, 'The supplied buffer is too small'
    OPEN_SOCKET_FAILED = -12, 'Could not open a socket'
    BIND_SOCKET_FAILED = -13, 'Could not bind a socket'
    LISTEN_SOCKET_FAILED = -14, 'Could not listen on socket'
    SET_SOCKET_OPTION_FAILED = -15, 'Could not set a socket option'
    INVALID_REGEX = -16, 'Invalid regular expression'
    READ_FILE_FAILED = -17, 'Could not read from file'
    RW_SOCKET_FAILED = -18, 'Could not read from or write to socket'
    CONNECT_SOCKET_FAILED = -19, 'Could not connect a socket'
    INVALID_MAGIC_PREFIX = -20, 'The magic prefix sent when establishing a connection is wrong'
    INCOMPATIBLE_VERSION = -21, 'The Yogi versions are not compatible'
    DESERIALIZE_MSG_FAILED = -22, 'Could not deserialize a message'
    ACCEPT_SOCKET_FAILED = -23, 'Could not accept a socket'
    LOOPBACK_CONNECTION = -24, 'Attempting to connect branch to itself'
    PASSWORD_MISMATCH = -25, 'The passwords of the local and remote branch do not match'
    NET_NAME_MISMATCH = -26, 'The net names of the local and remote branch do not match'
    DUPLICATE_BRANCH_NAME = -27, 'A branch with the same name is already active'
    DUPLICATE_BRANCH_PATH = -28, 'A branch with the same path is already active'
    PAYLOAD_TOO_LARGE = -29, 'Message payload is too large'
    PARSING_CMDLINE_FAILED = -30, 'Parsing the command line failed'
    PARSING_JSON_FAILED = -31, 'Parsing a JSON string failed'
    PARSING_FILE_FAILED = -32, 'Parsing a configuration file failed'
    CONFIG_NOT_VALID = -33, 'The configuration is not valid'
    HELP_REQUESTED = -34, 'Help/usage text requested'
    WRITE_FILE_FAILED = -35, 'Could not write to file'
    UNDEFINED_VARIABLES = -36, 'One or more configuration variables are undefined or could not be resolved'
    NO_VARIABLE_SUPPORT = -37, 'Support for configuration variables has been disabled'
    VARIABLE_USED_IN_KEY = -38, 'A configuration variable has been used in a key'
    INVALID_TIME_FORMAT = -39, 'Invalid time format'
    PARSING_TIME_FAILED = -40, 'Could not parse time string'
    TX_QUEUE_FULL = -41, 'A send queue for a remote branch is full'
    INVALID_OPERATION_ID = -42, 'Invalid operation ID'
    OPERATION_NOT_RUNNING = -43, 'Operation is not running'
    INVALID_USER_MSGPACK = -44, 'User-supplied data is not valid MessagePack'
    JOIN_MULTICAST_GROUP_FAILED = -45, 'Joining UDP multicast group failed'
    ENUMERATE_NETWORK_INTERFACES_FAILED = -46, 'Enumerating network interfaces failed'
    CONFIGURATION_SECTION_NOT_FOUND = -47, 'The section could not be found in the configuration'
    CONFIGURATION_VALIDATION_FAILED = -48, 'Validating the configuration failed'
    WORKER_ALREADY_ADDED = -49, 'The context has already been added as a worker'
    OPEN_FILE_FAILED = -50, 'Could not open file'
    # :CODEGEN_END:


class Verbosity(DocIntEnum):
    """Levels of how verbose logging output is"""
    # :CODEGEN_BEGIN:
    NONE = -1, 'Used to disable logging'
    FATAL = 0, 'Errors that require a process restart'
    ERROR = 1, 'Errors that the system can recover from'
    WARNING = 2, 'Warnings'
    INFO = 3, 'General information about the system state'
    DEBUG = 4, 'Information for debugging'
    TRACE = 5, 'Detailed debugging'
    # :CODEGEN_END:


class Stream(DocIntEnum):
    """Output streams for writing to the terminal"""
    # :CODEGEN_BEGIN:
    STDOUT = 0, 'Standard output'
    STDERR = 1, 'Standard error output'
    # :CODEGEN_END:


class Schema(DocIntEnum):
    """Built-in JSON Schemas.

    JSON Schemas used for describing and validating various forms of data such
    as configurations or results passed back to the library user (e.g. branch
    information).
    """
    # :CODEGEN_BEGIN:
    BRANCH_PROPERTIES = 0, 'All possible branch properties'
    BRANCH_CONFIG = 1, 'Properties for creating a branch'
    LOCAL_BRANCH_INFO = 2, 'Information about a local branch'
    REMOTE_BRANCH_INFO = 3, 'Information about a remote branch'
    BRANCH_EVENT = 4, 'Information about a branch event'
    # :CODEGEN_END:


class Encoding(DocIntEnum):
    """Possible data/payload encoding types"""
    # :CODEGEN_BEGIN:
    JSON = 0, 'Data is encoded as JSON'
    MSGPACK = 1, 'Data is encoded as MessagePack'
    # :CODEGEN_END:


class HttpStatus(DocIntEnum):
    """HTTP response status codes"""
    # :CODEGEN_BEGIN:
    HTTP_200 = 200, 'OK'
    HTTP_201 = 201, 'Created'
    HTTP_202 = 202, 'Accepted'
    HTTP_203 = 203, 'Non-Authoritative Information'
    HTTP_204 = 204, 'No Content'
    HTTP_205 = 205, 'Reset Content'
    HTTP_206 = 206, 'Partial Content'
    HTTP_300 = 300, 'Multiple Choices'
    HTTP_301 = 301, 'Moved Permanently'
    HTTP_302 = 302, 'Found'
    HTTP_303 = 303, 'See Other'
    HTTP_304 = 304, 'Not Modified'
    HTTP_307 = 307, 'Temporary Redirect'
    HTTP_308 = 308, 'Permanent Redirect'
    HTTP_400 = 400, 'Bad Request'
    HTTP_401 = 401, 'Unauthorized'
    HTTP_403 = 403, 'Forbidden'
    HTTP_404 = 404, 'Not Found'
    HTTP_405 = 405, 'Method Not Allowed'
    HTTP_406 = 406, 'Not Acceptable'
    HTTP_407 = 407, 'Proxy Authentication Required'
    HTTP_408 = 408, 'Request Timeout'
    HTTP_409 = 409, 'Conflict'
    HTTP_410 = 410, 'Gone'
    HTTP_411 = 411, 'Length Required'
    HTTP_412 = 412, 'Precondition Failed'
    HTTP_413 = 413, 'Payload Too Large'
    HTTP_414 = 414, 'URI Too Long'
    HTTP_415 = 415, 'Unsupported Media Type'
    HTTP_416 = 416, 'Range Not Satisfiable'
    HTTP_417 = 417, 'Expectation Failed'
    HTTP_418 = 418, 'I am a teapot'
    HTTP_422 = 422, 'Unprocessable Entity'
    HTTP_425 = 425, 'Too Early'
    HTTP_426 = 426, 'Upgrade Required'
    HTTP_428 = 428, 'Precondition Required'
    HTTP_429 = 429, 'Too Many Requests'
    HTTP_431 = 431, 'Request Header Fields Too Large'
    HTTP_451 = 451, 'Unavailable For Legal Reasons'
    HTTP_500 = 500, 'Internal Server Error'
    HTTP_501 = 501, 'Not Implemented'
    HTTP_502 = 502, 'Bad Gateway'
    HTTP_503 = 503, 'Service Unavailable'
    HTTP_504 = 504, 'Gateway Timeout'
    HTTP_505 = 505, 'HTTP Version Not Supported'
    HTTP_511 = 511, 'Network Authentication Required'
    # :CODEGEN_END:


class WebProcessAction(DocIntEnum):
    """Actions on a web process, originating from a web client"""
    # :CODEGEN_BEGIN:
    START = 0, 'Client requests to start a process'
    INPUT = 1, 'Client sent some input to the process'
    CANCELED = 2, 'Client canceled the process'
    # :CODEGEN_END:


class WebProcessUpdate(DocIntEnum):
    """Updates from a web process, originating from the web server"""
    # :CODEGEN_BEGIN:
    STARTED = 0, 'Process has been started'
    FAILED_TO_START = 1, 'Process could not be started'
    FINISHED = 2, 'Process finished gracefully'
    CRASHED = 3, 'Process finished by crashing'
    CANCELED = 4, 'Process finished by cancelation'
    OUTPUT = 5, 'Process created some output'
    # :CODEGEN_END:


class Signals(DocIntEnum):
    """Signals"""
    # :CODEGEN_BEGIN:
    NONE = 0, 'No signal'
    INT = 1 << 0, 'Interrupt (e.g. by pressing STRG + C)'
    TERM = 1 << 1, 'Termination request'
    USR1 = 1 << 24, 'User-defined signal 1'
    USR2 = 1 << 25, 'User-defined signal 2'
    USR3 = 1 << 26, 'User-defined signal 3'
    USR4 = 1 << 27, 'User-defined signal 4'
    USR5 = 1 << 28, 'User-defined signal 5'
    USR6 = 1 << 29, 'User-defined signal 6'
    USR7 = 1 << 30, 'User-defined signal 7'
    USR8 = 1 << 31, 'User-defined signal 8'
    ALL = INT[0] | TERM[0] | USR1[0] | USR2[0] | USR3[0] | USR4[0] | USR5[0] | USR6[0] | USR7[0] | USR8[0], 'All signals'
    # :CODEGEN_END:


class ConfigurationFlags(DocIntEnum):
    """Flags used to change a configuration object's behaviour"""
    # :CODEGEN_BEGIN:
    NONE = 0, 'No flags'
    DISABLE_VARIABLES = 1 << 0, 'Disables support for variables in the configuration'
    MUTABLE_CMD_LINE = 1 << 1, 'Makes configuration options given directly on the command line overridable'
    # :CODEGEN_END:


class CommandLineOptions(DocIntEnum):
    """Flags used to adjust how command line options are parsed"""
    # :CODEGEN_BEGIN:
    NONE = 0, 'No options'
    LOGGING = 1 << 0, 'Include the --log... switches for configuring file logging'
    BRANCH_NAME = 1 << 1, 'Include the --name switch for setting the branch name'
    BRANCH_DESCRIPTION = 1 << 2, 'Include the --description switch for setting the branch description'
    BRANCH_NETWORK = 1 << 3, 'Include the --network switch for setting the network name'
    BRANCH_PASSWORD = 1 << 4, 'Include the --password switch for setting the network password'
    BRANCH_PATH = 1 << 5, 'Include the --path switch for setting the branch path'
    BRANCH_ADV_IFS = 1 << 6, 'Include the --adv-interfaces switch for setting the branch advertising interfaces'
    BRANCH_ADV_ADDR = 1 << 7, 'Include the --adv-addr switch for setting the branch advertising address'
    BRANCH_ADV_PORT = 1 << 8, 'Include the --adv-port switch for setting the branch advertising port'
    BRANCH_ADV_INT = 1 << 9, 'Include the --adv-int switch for setting the branch advertising interval'
    BRANCH_TIMEOUT = 1 << 10, 'Include the --timeout switch for setting the branch timeout'
    BRANCH_GHOST_MODE = 1 << 11, 'Include the --ghost_mode switch for enabling ghost mode for the branch'
    FILES = 1 << 12, 'Parse configuration files given on the command line'
    FILES_REQUIRED = 1 << 13, 'At least one configuration file must be given'
    OVERRIDES = 1 << 14, 'Include the --override switch for overriding arbitrary configuration sections'
    VARIABLES = 1 << 15, 'Include the --var switch for setting variables'
    BRANCH_ALL = BRANCH_NAME[0] | BRANCH_DESCRIPTION[0] | BRANCH_NETWORK[0] | BRANCH_PASSWORD[0] | BRANCH_PATH[0] | BRANCH_ADV_IFS[
        0] | BRANCH_ADV_ADDR[0] | BRANCH_ADV_PORT[0] | BRANCH_ADV_INT[0] | BRANCH_TIMEOUT[0] | BRANCH_GHOST_MODE[0], 'Combination of all branch flags'
    ALL = LOGGING[0] | BRANCH_ALL[0] | FILES[0] | FILES_REQUIRED[0] | OVERRIDES[0] | VARIABLES[0], 'Combination of all flags'
    # :CODEGEN_END:


class BranchEvents(DocIntEnum):
    """Events that can be observed on a branch"""
    # :CODEGEN_BEGIN:
    NONE = 0, 'No event (passed to event handler function if wait operation failed)'
    BRANCH_DISCOVERED = 1 << 0, 'A new branch has been discovered'
    BRANCH_QUERIED = 1 << 1, 'Querying a new branch for information finished (successfully or not)'
    CONNECT_FINISHED = 1 << 2, 'Connecting to a branch finished (successfully or not)'
    CONNECTION_LOST = 1 << 3, 'The connection to a branch was lost'
    ALL = BRANCH_DISCOVERED[0] | BRANCH_QUERIED[0] | CONNECT_FINISHED[0] | CONNECTION_LOST[0], 'All branch events'
    # :CODEGEN_END:


class HttpMethods(DocIntEnum):
    """HTTP request methods"""
    # :CODEGEN_BEGIN:
    NONE = 0, 'No methods'
    GET = 1 << 0, 'GET method for retrieving data'
    HEAD = 1 << 1, 'HEAD method for retrieving the same header as with the GET request but'
    POST = 1 << 2, 'POST method for submitting data'
    PUT = 1 << 3, 'PUT method for modifying data'
    DELETE = 1 << 4, 'DELETE method for deleting data'
    PATCH = 1 << 5, 'PATCH method for partially modifying data'
    # :CODEGEN_END:
