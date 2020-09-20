/*
 * This file is part of the Yogi Framework
 * https://github.com/yohummus/yogi-framework.
 *
 * Copyright (c) 2020 Johannes Bergmann.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <src/api/errors.h>

const char* Result::description() const {
  // clang-format off
  switch (error_code()) {
    // :CODEGEN_BEGIN:
    case YOGI_OK: return "Success";
    case YOGI_ERR_UNKNOWN: return "Unknown internal error occured";
    case YOGI_ERR_OBJECT_STILL_USED: return "The object is still being used by another object";
    case YOGI_ERR_BAD_ALLOC: return "Insufficient memory to complete the operation";
    case YOGI_ERR_INVALID_PARAM: return "Invalid parameter";
    case YOGI_ERR_INVALID_HANDLE: return "Invalid Handle";
    case YOGI_ERR_WRONG_OBJECT_TYPE: return "Object is of the wrong type";
    case YOGI_ERR_CANCELED: return "The operation has been canceled";
    case YOGI_ERR_BUSY: return "Operation failed because the object is busy";
    case YOGI_ERR_TIMEOUT: return "The operation timed out";
    case YOGI_ERR_TIMER_EXPIRED: return "The timer has not been started or already expired";
    case YOGI_ERR_BUFFER_TOO_SMALL: return "The supplied buffer is too small";
    case YOGI_ERR_OPEN_SOCKET_FAILED: return "Could not open a socket";
    case YOGI_ERR_BIND_SOCKET_FAILED: return "Could not bind a socket";
    case YOGI_ERR_LISTEN_SOCKET_FAILED: return "Could not listen on socket";
    case YOGI_ERR_SET_SOCKET_OPTION_FAILED: return "Could not set a socket option";
    case YOGI_ERR_INVALID_REGEX: return "Invalid regular expression";
    case YOGI_ERR_READ_FILE_FAILED: return "Could not read from file";
    case YOGI_ERR_RW_SOCKET_FAILED: return "Could not read from or write to socket";
    case YOGI_ERR_CONNECT_SOCKET_FAILED: return "Could not connect a socket";
    case YOGI_ERR_INVALID_MAGIC_PREFIX: return "The magic prefix sent when establishing a connection is wrong";
    case YOGI_ERR_INCOMPATIBLE_VERSION: return "The Yogi versions are not compatible";
    case YOGI_ERR_DESERIALIZE_MSG_FAILED: return "Could not deserialize a message";
    case YOGI_ERR_ACCEPT_SOCKET_FAILED: return "Could not accept a socket";
    case YOGI_ERR_LOOPBACK_CONNECTION: return "Attempting to connect branch to itself";
    case YOGI_ERR_PASSWORD_MISMATCH: return "The passwords of the local and remote branch do not match";
    case YOGI_ERR_NET_NAME_MISMATCH: return "The net names of the local and remote branch do not match";
    case YOGI_ERR_DUPLICATE_BRANCH_NAME: return "A branch with the same name is already active";
    case YOGI_ERR_DUPLICATE_BRANCH_PATH: return "A branch with the same path is already active";
    case YOGI_ERR_PAYLOAD_TOO_LARGE: return "Message payload is too large";
    case YOGI_ERR_PARSING_CMDLINE_FAILED: return "Parsing the command line failed";
    case YOGI_ERR_PARSING_JSON_FAILED: return "Parsing a JSON string failed";
    case YOGI_ERR_PARSING_FILE_FAILED: return "Parsing a configuration file failed";
    case YOGI_ERR_CONFIG_NOT_VALID: return "The configuration is not valid";
    case YOGI_ERR_HELP_REQUESTED: return "Help/usage text requested";
    case YOGI_ERR_WRITE_FILE_FAILED: return "Could not write to file";
    case YOGI_ERR_UNDEFINED_VARIABLES: return "One or more configuration variables are undefined or could not be resolved";
    case YOGI_ERR_NO_VARIABLE_SUPPORT: return "Support for configuration variables has been disabled";
    case YOGI_ERR_VARIABLE_USED_IN_KEY: return "A configuration variable has been used in a key";
    case YOGI_ERR_INVALID_TIME_FORMAT: return "Invalid time format";
    case YOGI_ERR_PARSING_TIME_FAILED: return "Could not parse time string";
    case YOGI_ERR_TX_QUEUE_FULL: return "A send queue for a remote branch is full";
    case YOGI_ERR_INVALID_OPERATION_ID: return "Invalid operation ID";
    case YOGI_ERR_OPERATION_NOT_RUNNING: return "Operation is not running";
    case YOGI_ERR_INVALID_USER_MSGPACK: return "User-supplied data is not valid MessagePack";
    case YOGI_ERR_JOIN_MULTICAST_GROUP_FAILED: return "Joining UDP multicast group failed";
    case YOGI_ERR_ENUMERATE_NETWORK_INTERFACES_FAILED: return "Enumerating network interfaces failed";
    case YOGI_ERR_CONFIGURATION_SECTION_NOT_FOUND: return "The section could not be found in the configuration";
    case YOGI_ERR_CONFIGURATION_VALIDATION_FAILED: return "Validating the configuration failed";
    case YOGI_ERR_WORKER_ALREADY_ADDED: return "The context has already been added as a worker";
    case YOGI_ERR_OPEN_FILE_FAILED: return "Could not open file";
    // :CODEGEN_END:
  }
  // clang-format on

  return "Invalid error code";
}

std::ostream& operator<<(std::ostream& os, const Result& res) {
  os << res.description();
  return os;
}

std::ostream& operator<<(std::ostream& os, const Error& err) {
  os << static_cast<const Result&>(err);
  return os;
}

std::ostream& operator<<(std::ostream& os, const DescriptiveError& err) {
  os << static_cast<const Error&>(err);

  auto details = err.details();
  if (!details.empty()) os << ": " << details;

  return os;
}
