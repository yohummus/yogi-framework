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

#ifndef _YOGI_ENUMS_h
#define _YOGI_ENUMS_h

//! \file
//!
//! Functionality related to error handling, including exceptions.

#include "io.h"

#include <exception>
#include <string>

namespace yogi {

/// \addtogroup enums
/// @{

////////////////////////////////////////////////////////////////////////////////
/// Error codes
///
/// Yogi error codes indicating failures are always < 0. A human-readable
/// description can be obtained via the Result class.
////////////////////////////////////////////////////////////////////////////////
enum class ErrorCode {
  // clang-format off
  // :CODEGEN_BEGIN:
  kOk                               =   0, ///< Success
  kUnknown                          =  -1, ///< Unknown internal error occured
  kObjectStillUsed                  =  -2, ///< The object is still being used by another object
  kBadAlloc                         =  -3, ///< Insufficient memory to complete the operation
  kInvalidParam                     =  -4, ///< Invalid parameter
  kInvalidHandle                    =  -5, ///< Invalid Handle
  kWrongObjectType                  =  -6, ///< Object is of the wrong type
  kCanceled                         =  -7, ///< The operation has been canceled
  kBusy                             =  -8, ///< Operation failed because the object is busy
  kTimeout                          =  -9, ///< The operation timed out
  kTimerExpired                     = -10, ///< The timer has not been started or already expired
  kBufferTooSmall                   = -11, ///< The supplied buffer is too small
  kOpenSocketFailed                 = -12, ///< Could not open a socket
  kBindSocketFailed                 = -13, ///< Could not bind a socket
  kListenSocketFailed               = -14, ///< Could not listen on socket
  kSetSocketOptionFailed            = -15, ///< Could not set a socket option
  kInvalidRegex                     = -16, ///< Invalid regular expression
  kReadFileFailed                   = -17, ///< Could not read from file
  kRwSocketFailed                   = -18, ///< Could not read from or write to socket
  kConnectSocketFailed              = -19, ///< Could not connect a socket
  kInvalidMagicPrefix               = -20, ///< The magic prefix sent when establishing a connection is wrong
  kIncompatibleVersion              = -21, ///< The Yogi versions are not compatible
  kDeserializeMsgFailed             = -22, ///< Could not deserialize a message
  kAcceptSocketFailed               = -23, ///< Could not accept a socket
  kLoopbackConnection               = -24, ///< Attempting to connect branch to itself
  kPasswordMismatch                 = -25, ///< The passwords of the local and remote branch do not match
  kNetNameMismatch                  = -26, ///< The net names of the local and remote branch do not match
  kDuplicateBranchName              = -27, ///< A branch with the same name is already active
  kDuplicateBranchPath              = -28, ///< A branch with the same path is already active
  kPayloadTooLarge                  = -29, ///< Message payload is too large
  kParsingCmdlineFailed             = -30, ///< Parsing the command line failed
  kParsingJsonFailed                = -31, ///< Parsing a JSON string failed
  kParsingFileFailed                = -32, ///< Parsing a configuration file failed
  kConfigNotValid                   = -33, ///< The configuration is not valid
  kHelpRequested                    = -34, ///< Help/usage text requested
  kWriteFileFailed                  = -35, ///< Could not write to file
  kUndefinedVariables               = -36, ///< One or more configuration variables are undefined or could not be resolved
  kNoVariableSupport                = -37, ///< Support for configuration variables has been disabled
  kVariableUsedInKey                = -38, ///< A configuration variable has been used in a key
  kInvalidTimeFormat                = -39, ///< Invalid time format
  kParsingTimeFailed                = -40, ///< Could not parse time string
  kTxQueueFull                      = -41, ///< A send queue for a remote branch is full
  kInvalidOperationId               = -42, ///< Invalid operation ID
  kOperationNotRunning              = -43, ///< Operation is not running
  kInvalidUserMsgpack               = -44, ///< User-supplied data is not valid MessagePack
  kJoinMulticastGroupFailed         = -45, ///< Joining UDP multicast group failed
  kEnumerateNetworkInterfacesFailed = -46, ///< Enumerating network interfaces failed
  kConfigurationSectionNotFound     = -47, ///< The section could not be found in the configuration
  kConfigurationValidationFailed    = -48, ///< Validating the configuration failed
  kWorkerAlreadyAdded               = -49, ///< The context has already been added as a worker
  kOpenFileFailed                   = -50, ///< Could not open file
  // :CODEGEN_END:
  // clang-format on
};

template <>
inline std::string to_string<ErrorCode>(ErrorCode val) {
  // clang-format off
  switch (val) {
  // :CODEGEN_BEGIN:
  case ErrorCode::kOk:                               return "kOk";
  case ErrorCode::kUnknown:                          return "kUnknown";
  case ErrorCode::kObjectStillUsed:                  return "kObjectStillUsed";
  case ErrorCode::kBadAlloc:                         return "kBadAlloc";
  case ErrorCode::kInvalidParam:                     return "kInvalidParam";
  case ErrorCode::kInvalidHandle:                    return "kInvalidHandle";
  case ErrorCode::kWrongObjectType:                  return "kWrongObjectType";
  case ErrorCode::kCanceled:                         return "kCanceled";
  case ErrorCode::kBusy:                             return "kBusy";
  case ErrorCode::kTimeout:                          return "kTimeout";
  case ErrorCode::kTimerExpired:                     return "kTimerExpired";
  case ErrorCode::kBufferTooSmall:                   return "kBufferTooSmall";
  case ErrorCode::kOpenSocketFailed:                 return "kOpenSocketFailed";
  case ErrorCode::kBindSocketFailed:                 return "kBindSocketFailed";
  case ErrorCode::kListenSocketFailed:               return "kListenSocketFailed";
  case ErrorCode::kSetSocketOptionFailed:            return "kSetSocketOptionFailed";
  case ErrorCode::kInvalidRegex:                     return "kInvalidRegex";
  case ErrorCode::kReadFileFailed:                   return "kReadFileFailed";
  case ErrorCode::kRwSocketFailed:                   return "kRwSocketFailed";
  case ErrorCode::kConnectSocketFailed:              return "kConnectSocketFailed";
  case ErrorCode::kInvalidMagicPrefix:               return "kInvalidMagicPrefix";
  case ErrorCode::kIncompatibleVersion:              return "kIncompatibleVersion";
  case ErrorCode::kDeserializeMsgFailed:             return "kDeserializeMsgFailed";
  case ErrorCode::kAcceptSocketFailed:               return "kAcceptSocketFailed";
  case ErrorCode::kLoopbackConnection:               return "kLoopbackConnection";
  case ErrorCode::kPasswordMismatch:                 return "kPasswordMismatch";
  case ErrorCode::kNetNameMismatch:                  return "kNetNameMismatch";
  case ErrorCode::kDuplicateBranchName:              return "kDuplicateBranchName";
  case ErrorCode::kDuplicateBranchPath:              return "kDuplicateBranchPath";
  case ErrorCode::kPayloadTooLarge:                  return "kPayloadTooLarge";
  case ErrorCode::kParsingCmdlineFailed:             return "kParsingCmdlineFailed";
  case ErrorCode::kParsingJsonFailed:                return "kParsingJsonFailed";
  case ErrorCode::kParsingFileFailed:                return "kParsingFileFailed";
  case ErrorCode::kConfigNotValid:                   return "kConfigNotValid";
  case ErrorCode::kHelpRequested:                    return "kHelpRequested";
  case ErrorCode::kWriteFileFailed:                  return "kWriteFileFailed";
  case ErrorCode::kUndefinedVariables:               return "kUndefinedVariables";
  case ErrorCode::kNoVariableSupport:                return "kNoVariableSupport";
  case ErrorCode::kVariableUsedInKey:                return "kVariableUsedInKey";
  case ErrorCode::kInvalidTimeFormat:                return "kInvalidTimeFormat";
  case ErrorCode::kParsingTimeFailed:                return "kParsingTimeFailed";
  case ErrorCode::kTxQueueFull:                      return "kTxQueueFull";
  case ErrorCode::kInvalidOperationId:               return "kInvalidOperationId";
  case ErrorCode::kOperationNotRunning:              return "kOperationNotRunning";
  case ErrorCode::kInvalidUserMsgpack:               return "kInvalidUserMsgpack";
  case ErrorCode::kJoinMulticastGroupFailed:         return "kJoinMulticastGroupFailed";
  case ErrorCode::kEnumerateNetworkInterfacesFailed: return "kEnumerateNetworkInterfacesFailed";
  case ErrorCode::kConfigurationSectionNotFound:     return "kConfigurationSectionNotFound";
  case ErrorCode::kConfigurationValidationFailed:    return "kConfigurationValidationFailed";
  case ErrorCode::kWorkerAlreadyAdded:               return "kWorkerAlreadyAdded";
  case ErrorCode::kOpenFileFailed:                   return "kOpenFileFailed";
  // :CODEGEN_END:
  }
  // clang-format on

  return "INVALID ENUM VALUE";
}

////////////////////////////////////////////////////////////////////////////////
/// Levels of how verbose logging output is.
///
/// The term _severity_ refers to the same type.
////////////////////////////////////////////////////////////////////////////////
enum class Verbosity {
  // clang-format off
  // :CODEGEN_BEGIN:
  kNone    =  -1, ///< Used to disable logging
  kFatal   =   0, ///< Errors that require a process restart
  kError   =   1, ///< Errors that the system can recover from
  kWarning =   2, ///< Warnings
  kInfo    =   3, ///< General information about the system state
  kDebug   =   4, ///< Information for debugging
  kTrace   =   5, ///< Detailed debugging
  // :CODEGEN_END:
  // clang-format on
};

template <>
inline std::string to_string<Verbosity>(Verbosity val) {
  // clang-format off
  switch (val) {
  // :CODEGEN_BEGIN:
  case Verbosity::kNone:    return "kNone";
  case Verbosity::kFatal:   return "kFatal";
  case Verbosity::kError:   return "kError";
  case Verbosity::kWarning: return "kWarning";
  case Verbosity::kInfo:    return "kInfo";
  case Verbosity::kDebug:   return "kDebug";
  case Verbosity::kTrace:   return "kTrace";
  // :CODEGEN_END:
  }
  // clang-format on

  return "INVALID ENUM VALUE";
}

////////////////////////////////////////////////////////////////////////////////
/// Output streams for writing to the terminal.
////////////////////////////////////////////////////////////////////////////////
enum class Stream {
  // clang-format off
  // :CODEGEN_BEGIN:
  kStdout =   0, ///< Standard output
  kStderr =   1, ///< Standard error output
  // :CODEGEN_END:
  // clang-format on
};

template <>
inline std::string to_string<Stream>(Stream val) {
  // clang-format off
  switch (val) {
  // :CODEGEN_BEGIN:
  case Stream::kStdout: return "kStdout";
  case Stream::kStderr: return "kStderr";
  // :CODEGEN_END:
  }
  // clang-format on

  return "INVALID ENUM VALUE";
}

////////////////////////////////////////////////////////////////////////////////
/// Built-in JSON Schemas.
///
/// JSON Schemas used for describing and validating various forms of data such
/// as configurations or results passed back to the library user (e.g. branch
/// information).
////////////////////////////////////////////////////////////////////////////////
enum class Schema {
  // clang-format off
  // :CODEGEN_BEGIN:
  kBranchProperties =   0, ///< All possible branch properties
  kBranchConfig     =   1, ///< Properties for creating a branch
  kLocalBranchInfo  =   2, ///< Information about a local branch
  kRemoteBranchInfo =   3, ///< Information about a remote branch
  kBranchEvent      =   4, ///< Information about a branch event
  // :CODEGEN_END:
  // clang-format on
};

template <>
inline std::string to_string<Schema>(Schema val) {
  // clang-format off
  switch (val) {
  // :CODEGEN_BEGIN:
  case Schema::kBranchProperties: return "kBranchProperties";
  case Schema::kBranchConfig:     return "kBranchConfig";
  case Schema::kLocalBranchInfo:  return "kLocalBranchInfo";
  case Schema::kRemoteBranchInfo: return "kRemoteBranchInfo";
  case Schema::kBranchEvent:      return "kBranchEvent";
  // :CODEGEN_END:
  }
  // clang-format on

  return "INVALID ENUM VALUE";
}

////////////////////////////////////////////////////////////////////////////////
/// Possible data/payload encoding types.
////////////////////////////////////////////////////////////////////////////////
enum class Encoding {
  // clang-format off
  // :CODEGEN_BEGIN:
  kJson    =   0, ///< Data is encoded as JSON
  kMsgpack =   1, ///< Data is encoded as MessagePack
  // :CODEGEN_END:
  // clang-format on
};

template <>
inline std::string to_string<Encoding>(Encoding val) {
  // clang-format off
  switch (val) {
  // :CODEGEN_BEGIN:
  case Encoding::kJson:    return "kJson";
  case Encoding::kMsgpack: return "kMsgpack";
  // :CODEGEN_END:
  }
  // clang-format on

  return "INVALID ENUM VALUE";
}

////////////////////////////////////////////////////////////////////////////////
/// HTTP response status codes.
////////////////////////////////////////////////////////////////////////////////
enum class HttpStatus {
  // clang-format off
  // :CODEGEN_BEGIN:
  k200 = 200, ///< OK
  k201 = 201, ///< Created
  k202 = 202, ///< Accepted
  k203 = 203, ///< Non-Authoritative Information
  k204 = 204, ///< No Content
  k205 = 205, ///< Reset Content
  k206 = 206, ///< Partial Content
  k300 = 300, ///< Multiple Choices
  k301 = 301, ///< Moved Permanently
  k302 = 302, ///< Found
  k303 = 303, ///< See Other
  k304 = 304, ///< Not Modified
  k307 = 307, ///< Temporary Redirect
  k308 = 308, ///< Permanent Redirect
  k400 = 400, ///< Bad Request
  k401 = 401, ///< Unauthorized
  k403 = 403, ///< Forbidden
  k404 = 404, ///< Not Found
  k405 = 405, ///< Method Not Allowed
  k406 = 406, ///< Not Acceptable
  k407 = 407, ///< Proxy Authentication Required
  k408 = 408, ///< Request Timeout
  k409 = 409, ///< Conflict
  k410 = 410, ///< Gone
  k411 = 411, ///< Length Required
  k412 = 412, ///< Precondition Failed
  k413 = 413, ///< Payload Too Large
  k414 = 414, ///< URI Too Long
  k415 = 415, ///< Unsupported Media Type
  k416 = 416, ///< Range Not Satisfiable
  k417 = 417, ///< Expectation Failed
  k418 = 418, ///< I am a teapot
  k422 = 422, ///< Unprocessable Entity
  k425 = 425, ///< Too Early
  k426 = 426, ///< Upgrade Required
  k428 = 428, ///< Precondition Required
  k429 = 429, ///< Too Many Requests
  k431 = 431, ///< Request Header Fields Too Large
  k451 = 451, ///< Unavailable For Legal Reasons
  k500 = 500, ///< Internal Server Error
  k501 = 501, ///< Not Implemented
  k502 = 502, ///< Bad Gateway
  k503 = 503, ///< Service Unavailable
  k504 = 504, ///< Gateway Timeout
  k505 = 505, ///< HTTP Version Not Supported
  k511 = 511, ///< Network Authentication Required
  // :CODEGEN_END:
  // clang-format on
};

template <>
inline std::string to_string<HttpStatus>(HttpStatus val) {
  // clang-format off
  switch (val) {
  // :CODEGEN_BEGIN:
  case HttpStatus::k200: return "k200";
  case HttpStatus::k201: return "k201";
  case HttpStatus::k202: return "k202";
  case HttpStatus::k203: return "k203";
  case HttpStatus::k204: return "k204";
  case HttpStatus::k205: return "k205";
  case HttpStatus::k206: return "k206";
  case HttpStatus::k300: return "k300";
  case HttpStatus::k301: return "k301";
  case HttpStatus::k302: return "k302";
  case HttpStatus::k303: return "k303";
  case HttpStatus::k304: return "k304";
  case HttpStatus::k307: return "k307";
  case HttpStatus::k308: return "k308";
  case HttpStatus::k400: return "k400";
  case HttpStatus::k401: return "k401";
  case HttpStatus::k403: return "k403";
  case HttpStatus::k404: return "k404";
  case HttpStatus::k405: return "k405";
  case HttpStatus::k406: return "k406";
  case HttpStatus::k407: return "k407";
  case HttpStatus::k408: return "k408";
  case HttpStatus::k409: return "k409";
  case HttpStatus::k410: return "k410";
  case HttpStatus::k411: return "k411";
  case HttpStatus::k412: return "k412";
  case HttpStatus::k413: return "k413";
  case HttpStatus::k414: return "k414";
  case HttpStatus::k415: return "k415";
  case HttpStatus::k416: return "k416";
  case HttpStatus::k417: return "k417";
  case HttpStatus::k418: return "k418";
  case HttpStatus::k422: return "k422";
  case HttpStatus::k425: return "k425";
  case HttpStatus::k426: return "k426";
  case HttpStatus::k428: return "k428";
  case HttpStatus::k429: return "k429";
  case HttpStatus::k431: return "k431";
  case HttpStatus::k451: return "k451";
  case HttpStatus::k500: return "k500";
  case HttpStatus::k501: return "k501";
  case HttpStatus::k502: return "k502";
  case HttpStatus::k503: return "k503";
  case HttpStatus::k504: return "k504";
  case HttpStatus::k505: return "k505";
  case HttpStatus::k511: return "k511";
  // :CODEGEN_END:
  }
  // clang-format on

  return "INVALID ENUM VALUE";
}

////////////////////////////////////////////////////////////////////////////////
/// Actions on a web process, originating from a web client.
////////////////////////////////////////////////////////////////////////////////
enum class WebProcessAction {
  // clang-format off
  // :CODEGEN_BEGIN:
  kStart    =   0, ///< Client requests to start a process
  kInput    =   1, ///< Client sent some input to the process
  kCanceled =   2, ///< Client canceled the process
  // :CODEGEN_END:
  // clang-format on
};

template <>
inline std::string to_string<WebProcessAction>(WebProcessAction val) {
  // clang-format off
  switch (val) {
  // :CODEGEN_BEGIN:
  case WebProcessAction::kStart:    return "kStart";
  case WebProcessAction::kInput:    return "kInput";
  case WebProcessAction::kCanceled: return "kCanceled";
  // :CODEGEN_END:
  }
  // clang-format on

  return "INVALID ENUM VALUE";
}

////////////////////////////////////////////////////////////////////////////////
/// Updates from a web process, originating from the web server.
////////////////////////////////////////////////////////////////////////////////
enum class WebProcessUpdate {
  // clang-format off
  // :CODEGEN_BEGIN:
  kStarted       =   0, ///< Process has been started
  kFailedToStart =   1, ///< Process could not be started
  kFinished      =   2, ///< Process finished gracefully
  kCrashed       =   3, ///< Process finished by crashing
  kCanceled      =   4, ///< Process finished by cancelation
  kOutput        =   5, ///< Process created some output
  // :CODEGEN_END:
  // clang-format on
};

template <>
inline std::string to_string<WebProcessUpdate>(WebProcessUpdate val) {
  // clang-format off
  switch (val) {
  // :CODEGEN_BEGIN:
  case WebProcessUpdate::kStarted:       return "kStarted";
  case WebProcessUpdate::kFailedToStart: return "kFailedToStart";
  case WebProcessUpdate::kFinished:      return "kFinished";
  case WebProcessUpdate::kCrashed:       return "kCrashed";
  case WebProcessUpdate::kCanceled:      return "kCanceled";
  case WebProcessUpdate::kOutput:        return "kOutput";
  // :CODEGEN_END:
  }
  // clang-format on

  return "INVALID ENUM VALUE";
}

////////////////////////////////////////////////////////////////////////////////
/// Process signals.
////////////////////////////////////////////////////////////////////////////////
enum class Signals {
  // :CODEGEN_BEGIN:

  //! No signal
  kNone = 0,

  //! Interrupt (e.g. by pressing STRG + C)
  kInt = (1 << 0),

  //! Termination request
  kTerm = (1 << 1),

  //! User-defined signal 1
  kUsr1 = (1 << 24),

  //! User-defined signal 2
  kUsr2 = (1 << 25),

  //! User-defined signal 3
  kUsr3 = (1 << 26),

  //! User-defined signal 4
  kUsr4 = (1 << 27),

  //! User-defined signal 5
  kUsr5 = (1 << 28),

  //! User-defined signal 6
  kUsr6 = (1 << 29),

  //! User-defined signal 7
  kUsr7 = (1 << 30),

  //! User-defined signal 8
  kUsr8 = (1 << 31),

  //! All signals
  kAll = (kInt | kTerm | kUsr1 | kUsr2 | kUsr3 | kUsr4 | kUsr5 | kUsr6 | kUsr7 | kUsr8),
};

inline Signals operator~(Signals flags) {
  return static_cast<Signals>(~static_cast<int>(flags));
}

inline Signals operator|(Signals a, Signals b) {
  return static_cast<Signals>(static_cast<int>(a) | static_cast<int>(b));
}

inline Signals operator&(Signals a, Signals b) {
  return static_cast<Signals>(static_cast<int>(a) & static_cast<int>(b));
}

inline Signals& operator|=(Signals& a, Signals b) {
  return a = a | b;
}

inline Signals& operator&=(Signals& a, Signals b) {
  return a = a & b;
}

template <>
inline std::string to_string<Signals>(Signals flags) {
  std::string ss;

  if ((flags & Signals::kAll) == Signals::kAll) {
    flags = flags & ~Signals::kAll;
    ss    = std::string{"kAll | "} + ss;
  }

  if ((flags & Signals::kUsr8) == Signals::kUsr8) {
    flags = flags & ~Signals::kUsr8;
    ss    = std::string{"kUsr8 | "} + ss;
  }

  if ((flags & Signals::kUsr7) == Signals::kUsr7) {
    flags = flags & ~Signals::kUsr7;
    ss    = std::string{"kUsr7 | "} + ss;
  }

  if ((flags & Signals::kUsr6) == Signals::kUsr6) {
    flags = flags & ~Signals::kUsr6;
    ss    = std::string{"kUsr6 | "} + ss;
  }

  if ((flags & Signals::kUsr5) == Signals::kUsr5) {
    flags = flags & ~Signals::kUsr5;
    ss    = std::string{"kUsr5 | "} + ss;
  }

  if ((flags & Signals::kUsr4) == Signals::kUsr4) {
    flags = flags & ~Signals::kUsr4;
    ss    = std::string{"kUsr4 | "} + ss;
  }

  if ((flags & Signals::kUsr3) == Signals::kUsr3) {
    flags = flags & ~Signals::kUsr3;
    ss    = std::string{"kUsr3 | "} + ss;
  }

  if ((flags & Signals::kUsr2) == Signals::kUsr2) {
    flags = flags & ~Signals::kUsr2;
    ss    = std::string{"kUsr2 | "} + ss;
  }

  if ((flags & Signals::kUsr1) == Signals::kUsr1) {
    flags = flags & ~Signals::kUsr1;
    ss    = std::string{"kUsr1 | "} + ss;
  }

  if ((flags & Signals::kTerm) == Signals::kTerm) {
    flags = flags & ~Signals::kTerm;
    ss    = std::string{"kTerm | "} + ss;
  }

  if ((flags & Signals::kInt) == Signals::kInt) {
    flags = flags & ~Signals::kInt;
    ss    = std::string{"kInt | "} + ss;
  }

  return ss.empty() ? "kNone" : ss.substr(0, ss.size() - 3);
}

// :CODEGEN_END:

////////////////////////////////////////////////////////////////////////////////
/// Flags used to change a configuration object's behaviour.
////////////////////////////////////////////////////////////////////////////////
enum class ConfigurationFlags {
  // :CODEGEN_BEGIN:

  //! No flags
  kNone = 0,

  //! Disables support for variables in the configuration
  //!
  //! Variables are used to define common values (or parts of values) in a
  //! pre-defined  section. They can then be used in multiple other parts of the
  //! configuration.
  //!
  //! \note
  //!   Variables can *not* be used in keys.
  //!
  //! Variables are defined in the *variables* section of the configuration:
  //!
  //! \code
  //! {
  //!   "variables": {
  //!     "ROOT":         "/usr/share/my-app",
  //!     "DATA_DIR":     "${ROOT}/data"
  //!     "DURATION":     30,
  //!     "MAX_DURATION": "${DURATION}",
  //! ...
  //! \endcode
  //! These variables can then be used anywhere in the configuration, even in the
  //! *variables* section itself as shown above.
  //!
  //! \note
  //!   Even if the value of a variable is not a string, the placeholder *${name}*
  //!   always has to be a string to conform to the JSON format. When resolving a
  //!   placeholder for a non-string variable, the type of the target value will
  //!   be changed accordingly if and only if the placeholder is surrounded by
  //!   quotation marks as shown for the *MAX_DURATION* variable above. Otherwise,
  //!   the target value will remain a string and the placeholder will be replaced
  //!   with the stringified value of the variable.
  //!
  kDisableVariables = (1 << 0),

  //! Makes configuration options given directly on the command line overridable
  //!
  //! By default, configuration options given direclty on the command line are
  //! immutable, i.e. they will never be updated from another source.
  //!
  kMutableCmdLine = (1 << 1),
};

inline ConfigurationFlags operator~(ConfigurationFlags flags) {
  return static_cast<ConfigurationFlags>(~static_cast<int>(flags));
}

inline ConfigurationFlags operator|(ConfigurationFlags a, ConfigurationFlags b) {
  return static_cast<ConfigurationFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline ConfigurationFlags operator&(ConfigurationFlags a, ConfigurationFlags b) {
  return static_cast<ConfigurationFlags>(static_cast<int>(a) & static_cast<int>(b));
}

inline ConfigurationFlags& operator|=(ConfigurationFlags& a, ConfigurationFlags b) {
  return a = a | b;
}

inline ConfigurationFlags& operator&=(ConfigurationFlags& a, ConfigurationFlags b) {
  return a = a & b;
}

template <>
inline std::string to_string<ConfigurationFlags>(ConfigurationFlags flags) {
  std::string ss;

  if ((flags & ConfigurationFlags::kMutableCmdLine) == ConfigurationFlags::kMutableCmdLine) {
    flags = flags & ~ConfigurationFlags::kMutableCmdLine;
    ss    = std::string{"kMutableCmdLine | "} + ss;
  }

  if ((flags & ConfigurationFlags::kDisableVariables) == ConfigurationFlags::kDisableVariables) {
    flags = flags & ~ConfigurationFlags::kDisableVariables;
    ss    = std::string{"kDisableVariables | "} + ss;
  }

  return ss.empty() ? "kNone" : ss.substr(0, ss.size() - 3);
}

// :CODEGEN_END:

////////////////////////////////////////////////////////////////////////////////
/// Flags used to adjust how command line options are parsed.
////////////////////////////////////////////////////////////////////////////////
enum class CommandLineOptions {
  // :CODEGEN_BEGIN:

  //! No options
  kNone = 0,

  //! Include the --log... switches for configuring file logging
  kLogging = (1 << 0),

  //! Include the --name switch for setting the branch name
  kBranchName = (1 << 1),

  //! Include the --description switch for setting the branch description
  kBranchDescription = (1 << 2),

  //! Include the --network switch for setting the network name
  kBranchNetwork = (1 << 3),

  //! Include the --password switch for setting the network password
  kBranchPassword = (1 << 4),

  //! Include the --path switch for setting the branch path
  kBranchPath = (1 << 5),

  //! Include the --adv-interfaces switch for setting the branch advertising interfaces
  kBranchAdvIfs = (1 << 6),

  //! Include the --adv-addr switch for setting the branch advertising address
  kBranchAdvAddr = (1 << 7),

  //! Include the --adv-port switch for setting the branch advertising port
  kBranchAdvPort = (1 << 8),

  //! Include the --adv-int switch for setting the branch advertising interval
  kBranchAdvInt = (1 << 9),

  //! Include the --timeout switch for setting the branch timeout
  kBranchTimeout = (1 << 10),

  //! Include the --ghost_mode switch for enabling ghost mode for the branch
  kBranchGhostMode = (1 << 11),

  //! Parse configuration files given on the command line
  //!
  //! The files will be parsed from left to right, i.e. if the same value is set
  //! in two supplied configuration files, then the value from the rightmost file
  //! will be used. However, values given directly on the command line, i.e. not
  //! through files, have higher priority.
  //!
  kFiles = (1 << 12),

  //! At least one configuration file must be given
  kFilesRequired = (1 << 13),

  //! Include the --override switch for overriding arbitrary configuration sections
  //!
  //! This is useful for supplying arbitrary parameters on the command line
  //! without having to store them in a file.
  //!
  //! \note
  //!   Parameters supplied in this way override the same parameters in any
  //!   given configuration file. If the same parameter is set directly on the
  //!   command line multiple times, then the rightmost value is used.
  //!
  kOverrides = (1 << 14),

  //! Include the --var switch for setting variables
  kVariables = (1 << 15),

  //! Combination of all branch flags
  kBranchAll = (kBranchName | kBranchDescription | kBranchNetwork | kBranchPassword | kBranchPath | kBranchAdvIfs |
                kBranchAdvAddr | kBranchAdvPort | kBranchAdvInt | kBranchTimeout | kBranchGhostMode),

  //! Combination of all flags
  //!
  //! This is usually used with the application object.
  //!
  kAll = (kLogging | kBranchAll | kFiles | kFilesRequired | kOverrides | kVariables),
};

inline CommandLineOptions operator~(CommandLineOptions flags) {
  return static_cast<CommandLineOptions>(~static_cast<int>(flags));
}

inline CommandLineOptions operator|(CommandLineOptions a, CommandLineOptions b) {
  return static_cast<CommandLineOptions>(static_cast<int>(a) | static_cast<int>(b));
}

inline CommandLineOptions operator&(CommandLineOptions a, CommandLineOptions b) {
  return static_cast<CommandLineOptions>(static_cast<int>(a) & static_cast<int>(b));
}

inline CommandLineOptions& operator|=(CommandLineOptions& a, CommandLineOptions b) {
  return a = a | b;
}

inline CommandLineOptions& operator&=(CommandLineOptions& a, CommandLineOptions b) {
  return a = a & b;
}

template <>
inline std::string to_string<CommandLineOptions>(CommandLineOptions flags) {
  std::string ss;

  if ((flags & CommandLineOptions::kAll) == CommandLineOptions::kAll) {
    flags = flags & ~CommandLineOptions::kAll;
    ss    = std::string{"kAll | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchAll) == CommandLineOptions::kBranchAll) {
    flags = flags & ~CommandLineOptions::kBranchAll;
    ss    = std::string{"kBranchAll | "} + ss;
  }

  if ((flags & CommandLineOptions::kVariables) == CommandLineOptions::kVariables) {
    flags = flags & ~CommandLineOptions::kVariables;
    ss    = std::string{"kVariables | "} + ss;
  }

  if ((flags & CommandLineOptions::kOverrides) == CommandLineOptions::kOverrides) {
    flags = flags & ~CommandLineOptions::kOverrides;
    ss    = std::string{"kOverrides | "} + ss;
  }

  if ((flags & CommandLineOptions::kFilesRequired) == CommandLineOptions::kFilesRequired) {
    flags = flags & ~CommandLineOptions::kFilesRequired;
    ss    = std::string{"kFilesRequired | "} + ss;
  }

  if ((flags & CommandLineOptions::kFiles) == CommandLineOptions::kFiles) {
    flags = flags & ~CommandLineOptions::kFiles;
    ss    = std::string{"kFiles | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchGhostMode) == CommandLineOptions::kBranchGhostMode) {
    flags = flags & ~CommandLineOptions::kBranchGhostMode;
    ss    = std::string{"kBranchGhostMode | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchTimeout) == CommandLineOptions::kBranchTimeout) {
    flags = flags & ~CommandLineOptions::kBranchTimeout;
    ss    = std::string{"kBranchTimeout | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchAdvInt) == CommandLineOptions::kBranchAdvInt) {
    flags = flags & ~CommandLineOptions::kBranchAdvInt;
    ss    = std::string{"kBranchAdvInt | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchAdvPort) == CommandLineOptions::kBranchAdvPort) {
    flags = flags & ~CommandLineOptions::kBranchAdvPort;
    ss    = std::string{"kBranchAdvPort | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchAdvAddr) == CommandLineOptions::kBranchAdvAddr) {
    flags = flags & ~CommandLineOptions::kBranchAdvAddr;
    ss    = std::string{"kBranchAdvAddr | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchAdvIfs) == CommandLineOptions::kBranchAdvIfs) {
    flags = flags & ~CommandLineOptions::kBranchAdvIfs;
    ss    = std::string{"kBranchAdvIfs | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchPath) == CommandLineOptions::kBranchPath) {
    flags = flags & ~CommandLineOptions::kBranchPath;
    ss    = std::string{"kBranchPath | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchPassword) == CommandLineOptions::kBranchPassword) {
    flags = flags & ~CommandLineOptions::kBranchPassword;
    ss    = std::string{"kBranchPassword | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchNetwork) == CommandLineOptions::kBranchNetwork) {
    flags = flags & ~CommandLineOptions::kBranchNetwork;
    ss    = std::string{"kBranchNetwork | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchDescription) == CommandLineOptions::kBranchDescription) {
    flags = flags & ~CommandLineOptions::kBranchDescription;
    ss    = std::string{"kBranchDescription | "} + ss;
  }

  if ((flags & CommandLineOptions::kBranchName) == CommandLineOptions::kBranchName) {
    flags = flags & ~CommandLineOptions::kBranchName;
    ss    = std::string{"kBranchName | "} + ss;
  }

  if ((flags & CommandLineOptions::kLogging) == CommandLineOptions::kLogging) {
    flags = flags & ~CommandLineOptions::kLogging;
    ss    = std::string{"kLogging | "} + ss;
  }

  return ss.empty() ? "kNone" : ss.substr(0, ss.size() - 3);
}

// :CODEGEN_END:

////////////////////////////////////////////////////////////////////////////////
/// Events that can be observed on a branch.
////////////////////////////////////////////////////////////////////////////////
enum class BranchEvents {
  // :CODEGEN_BEGIN:

  //! No event (passed to event handler function if wait operation failed)
  kNone = 0,

  //! A new branch has been discovered
  //!
  //! A remote branch is considered to be a _new branch_ if it is neither already
  //! connected nor in the process of being connected to. This means if we
  //! discover a branch and connect to it but connecting to it fails and the
  //! remote branch itself has not connected to us, then the next time an
  //! we receive an advertisement message we consider the branch new again.
  //!
  //! Associated event information:
  //!
  //! \code
  //!   {
  //!     "uuid":               "123e4567-e89b-12d3-a456-426655440000",
  //!     "tcp_server_address": "fe80::f086:b106:2c1b:c45",
  //!     "tcp_server_port":    43384
  //!   }
  //! \endcode
  //!
  kBranchDiscovered = (1 << 0),

  //! Querying a new branch for information finished (successfully or not)
  //!
  //! Associated event information:
  //!
  //! \code
  //!   {
  //!     "uuid":                 "123e4567-e89b-12d3-a456-426655440000",
  //!     "name":                 "Pump Safety Logic",
  //!     "description":          "Monitors the pump for safety",
  //!     "network_name":         "Hardware Control",
  //!     "path":                 "/Cooling System/Pump/Safety",
  //!     "hostname":             "beaglebone",
  //!     "pid":                  3321,
  //!     "tcp_server_address":   "fe80::f086:b106:2c1b:c45",
  //!     "tcp_server_port":      43384,
  //!     "start_time":           "2018-04-23T18:25:43.511Z",
  //!     "timeout":              3.0,
  //!     "advertising_interval": 1.0,
  //!     "ghost_mode":           false
  //!   }
  //! \endcode
  //!
  kBranchQueried = (1 << 1),

  //! Connecting to a branch finished (successfully or not)
  //!
  //! Associated event information:
  //!
  //! \code
  //!   {
  //!     "uuid": "123e4567-e89b-12d3-a456-426655440000"
  //!   }
  //! \endcode
  //!
  kConnectFinished = (1 << 2),

  //! The connection to a branch was lost
  //!
  //! Associated event information:
  //!
  //! \code
  //!   {
  //!     "uuid": "123e4567-e89b-12d3-a456-426655440000"
  //!   }
  //! \endcode
  //!
  kConnectionLost = (1 << 3),

  //! All branch events
  kAll = (kBranchDiscovered | kBranchQueried | kConnectFinished | kConnectionLost),
};

inline BranchEvents operator~(BranchEvents flags) {
  return static_cast<BranchEvents>(~static_cast<int>(flags));
}

inline BranchEvents operator|(BranchEvents a, BranchEvents b) {
  return static_cast<BranchEvents>(static_cast<int>(a) | static_cast<int>(b));
}

inline BranchEvents operator&(BranchEvents a, BranchEvents b) {
  return static_cast<BranchEvents>(static_cast<int>(a) & static_cast<int>(b));
}

inline BranchEvents& operator|=(BranchEvents& a, BranchEvents b) {
  return a = a | b;
}

inline BranchEvents& operator&=(BranchEvents& a, BranchEvents b) {
  return a = a & b;
}

template <>
inline std::string to_string<BranchEvents>(BranchEvents flags) {
  std::string ss;

  if ((flags & BranchEvents::kAll) == BranchEvents::kAll) {
    flags = flags & ~BranchEvents::kAll;
    ss    = std::string{"kAll | "} + ss;
  }

  if ((flags & BranchEvents::kConnectionLost) == BranchEvents::kConnectionLost) {
    flags = flags & ~BranchEvents::kConnectionLost;
    ss    = std::string{"kConnectionLost | "} + ss;
  }

  if ((flags & BranchEvents::kConnectFinished) == BranchEvents::kConnectFinished) {
    flags = flags & ~BranchEvents::kConnectFinished;
    ss    = std::string{"kConnectFinished | "} + ss;
  }

  if ((flags & BranchEvents::kBranchQueried) == BranchEvents::kBranchQueried) {
    flags = flags & ~BranchEvents::kBranchQueried;
    ss    = std::string{"kBranchQueried | "} + ss;
  }

  if ((flags & BranchEvents::kBranchDiscovered) == BranchEvents::kBranchDiscovered) {
    flags = flags & ~BranchEvents::kBranchDiscovered;
    ss    = std::string{"kBranchDiscovered | "} + ss;
  }

  return ss.empty() ? "kNone" : ss.substr(0, ss.size() - 3);
}

// :CODEGEN_END:

////////////////////////////////////////////////////////////////////////////////
/// HTTP request methods.
////////////////////////////////////////////////////////////////////////////////
enum class HttpMethods {
  // :CODEGEN_BEGIN:

  //! No methods
  kNone = 0,

  //! GET method for retrieving data
  kGet = (1 << 0),

  //! HEAD method for retrieving the same header as with the GET request but
  //! without the response body
  //!
  kHead = (1 << 1),

  //! POST method for submitting data
  kPost = (1 << 2),

  //! PUT method for modifying data
  kPut = (1 << 3),

  //! DELETE method for deleting data
  kDelete = (1 << 4),

  //! PATCH method for partially modifying data
  kPatch = (1 << 5),
};

inline HttpMethods operator~(HttpMethods flags) {
  return static_cast<HttpMethods>(~static_cast<int>(flags));
}

inline HttpMethods operator|(HttpMethods a, HttpMethods b) {
  return static_cast<HttpMethods>(static_cast<int>(a) | static_cast<int>(b));
}

inline HttpMethods operator&(HttpMethods a, HttpMethods b) {
  return static_cast<HttpMethods>(static_cast<int>(a) & static_cast<int>(b));
}

inline HttpMethods& operator|=(HttpMethods& a, HttpMethods b) {
  return a = a | b;
}

inline HttpMethods& operator&=(HttpMethods& a, HttpMethods b) {
  return a = a & b;
}

template <>
inline std::string to_string<HttpMethods>(HttpMethods flags) {
  std::string ss;

  if ((flags & HttpMethods::kPatch) == HttpMethods::kPatch) {
    flags = flags & ~HttpMethods::kPatch;
    ss    = std::string{"kPatch | "} + ss;
  }

  if ((flags & HttpMethods::kDelete) == HttpMethods::kDelete) {
    flags = flags & ~HttpMethods::kDelete;
    ss    = std::string{"kDelete | "} + ss;
  }

  if ((flags & HttpMethods::kPut) == HttpMethods::kPut) {
    flags = flags & ~HttpMethods::kPut;
    ss    = std::string{"kPut | "} + ss;
  }

  if ((flags & HttpMethods::kPost) == HttpMethods::kPost) {
    flags = flags & ~HttpMethods::kPost;
    ss    = std::string{"kPost | "} + ss;
  }

  if ((flags & HttpMethods::kHead) == HttpMethods::kHead) {
    flags = flags & ~HttpMethods::kHead;
    ss    = std::string{"kHead | "} + ss;
  }

  if ((flags & HttpMethods::kGet) == HttpMethods::kGet) {
    flags = flags & ~HttpMethods::kGet;
    ss    = std::string{"kGet | "} + ss;
  }

  return ss.empty() ? "kNone" : ss.substr(0, ss.size() - 3);
}

// :CODEGEN_END:

/// @} enums

}  // namespace yogi

#endif  // _YOGI_ENUMS_h
