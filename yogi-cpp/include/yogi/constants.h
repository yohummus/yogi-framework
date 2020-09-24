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

#ifndef _YOGI_CONSTANTS_H
#define _YOGI_CONSTANTS_H

//! \file
//!
//! Constants.

#include "detail/api.h"
#include "detail/error_helpers.h"
#include "duration.h"
#include "logging.h"

#include <string>

namespace yogi {

////////////////////////////////////////////////////////////////////////////////
/// Constants built into the Yogi Core library.
////////////////////////////////////////////////////////////////////////////////
struct constants {
  // clang-format off
  // :CODEGEN_BEGIN:
  static const std::string kVersion;                     ///< Whole version string of the core library
  static const int         kVersionMajor;                ///< Major version number of the core library
  static const int         kVersionMinor;                ///< Minor version number of the core library
  static const int         kVersionPatch;                ///< Patch version number of the core library
  static const std::string kVersionSuffix;               ///< Suffix of the core library version
  static const std::string kDefaultAdvInterfaces;        ///< Default network interfaces to use for advertising
  static const std::string kDefaultAdvAddress;           ///< Default multicast addresses to use for advertising
  static const int         kDefaultAdvPort;              ///< Default port to use for advertising via UDP IPv6 multicasts
  static const Duration    kDefaultAdvInterval;          ///< Default advertising interval in nanoseconds
  static const Duration    kDefaultConnectionTimeout;    ///< Default connection timeout in nanoseconds
  static const Verbosity   kDefaultLoggerVerbosity;      ///< Default logging verbosity
  static const std::string kDefaultLogTimeFormat;        ///< Default textual format for timestamps in log entries
  static const std::string kDefaultLogFormat;            ///< Default textual format for log entries
  static const int         kMaxMessagePayloadSize;       ///< Maximum size of the payload in a message
  static const std::string kDefaultTimeFormat;           ///< Default textual format for timestamps
  static const std::string kDefaultInfDurationString;    ///< Default string to denote an infinite duration
  static const std::string kDefaultDurationFormat;       ///< Default textual format for duration strings
  static const std::string kDefaultInvalidHandleString;  ///< Default string to denote an invalid object handle
  static const std::string kDefaultObjectFormat;         ///< Default textual format for strings describing an object
  static const int         kMinTxQueueSize;              ///< Minimum size of a send queue for a remote branch
  static const int         kMaxTxQueueSize;              ///< Maximum size of a send queue for a remote branch
  static const int         kDefaultTxQueueSize;          ///< Default size of a send queue for a remote branch
  static const int         kMinRxQueueSize;              ///< Minimum size of a receive queue for a remote branch
  static const int         kMaxRxQueueSize;              ///< Maximum size of a receive queue for a remote branch
  static const int         kDefaultRxQueueSize;          ///< Default size of a receive queue for a remote branch
  static const int         kDefaultWebPort;              ///< Default port for the web server to listen on for client connections
  static const std::string kDefaultWebInterfaces;        ///< Default network interfaces to use for the web server
  static const Duration    kDefaultWebTimeout;           ///< Default timeout for web server connections in nanoseconds
  static const int         kDefaultHttpHeaderLimit;      ///< Default maximum HTTP header size of incoming requests in bytes
  static const int         kDefaultHttpBodyLimit;        ///< Default maximum HTTP body size of incoming requests in bytes
  static const int         kDefaultWebCacheSize;         ///< Default size of the web server cache in bytes
  static const int         kMaxWebCacheSize;             ///< Maximum size of the web server cache in bytes
  static const std::string kDefaultAdminUser;            ///< Default user name for the administrator account
  static const std::string kDefaultAdminPassword;        ///< Default password for the administrator account
  static const std::string kDefaultSslPrivateKey;        ///< Default private key to use for SSL connections
  static const std::string kDefaultSslCertificateChain;  ///< Default certificate chain to use for SSL connections
  static const std::string kDefaultSslDhParams;          ///< Default DH parameters to use for SSL connections
  // :CODEGEN_END:
  // clang-format on
};

namespace detail {

template <typename ConstantType, typename ApiType>
inline ConstantType get_core_constant(int constant) {
  ApiType val;
  int res = YOGI_GetConstant(&val, constant);
  check_error_code(res);
  return {val};
}

template <>
inline Duration get_core_constant<Duration, long long>(int constant) {
  return Duration::from_nanoseconds(get_core_constant<long long, long long>(constant));
}

template <>
inline Verbosity get_core_constant<Verbosity, int>(int constant) {
  return static_cast<Verbosity>(get_core_constant<int, int>(constant));
}

}  // namespace detail

// clang-format off
// :CODEGEN_BEGIN:
_YOGI_WEAK_SYMBOL const std::string constants::kVersion                    = detail::get_core_constant<std::string, const char*>( 1);
_YOGI_WEAK_SYMBOL const int         constants::kVersionMajor               = detail::get_core_constant<int,         int        >( 2);
_YOGI_WEAK_SYMBOL const int         constants::kVersionMinor               = detail::get_core_constant<int,         int        >( 3);
_YOGI_WEAK_SYMBOL const int         constants::kVersionPatch               = detail::get_core_constant<int,         int        >( 4);
_YOGI_WEAK_SYMBOL const std::string constants::kVersionSuffix              = detail::get_core_constant<std::string, const char*>( 5);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultAdvInterfaces       = detail::get_core_constant<std::string, const char*>( 6);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultAdvAddress          = detail::get_core_constant<std::string, const char*>( 7);
_YOGI_WEAK_SYMBOL const int         constants::kDefaultAdvPort             = detail::get_core_constant<int,         int        >( 8);
_YOGI_WEAK_SYMBOL const Duration    constants::kDefaultAdvInterval         = detail::get_core_constant<Duration,    long long  >( 9);
_YOGI_WEAK_SYMBOL const Duration    constants::kDefaultConnectionTimeout   = detail::get_core_constant<Duration,    long long  >(10);
_YOGI_WEAK_SYMBOL const Verbosity   constants::kDefaultLoggerVerbosity     = detail::get_core_constant<Verbosity,   int        >(11);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultLogTimeFormat       = detail::get_core_constant<std::string, const char*>(12);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultLogFormat           = detail::get_core_constant<std::string, const char*>(13);
_YOGI_WEAK_SYMBOL const int         constants::kMaxMessagePayloadSize      = detail::get_core_constant<int,         int        >(14);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultTimeFormat          = detail::get_core_constant<std::string, const char*>(15);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultInfDurationString   = detail::get_core_constant<std::string, const char*>(16);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultDurationFormat      = detail::get_core_constant<std::string, const char*>(17);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultInvalidHandleString = detail::get_core_constant<std::string, const char*>(18);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultObjectFormat        = detail::get_core_constant<std::string, const char*>(19);
_YOGI_WEAK_SYMBOL const int         constants::kMinTxQueueSize             = detail::get_core_constant<int,         int        >(20);
_YOGI_WEAK_SYMBOL const int         constants::kMaxTxQueueSize             = detail::get_core_constant<int,         int        >(21);
_YOGI_WEAK_SYMBOL const int         constants::kDefaultTxQueueSize         = detail::get_core_constant<int,         int        >(22);
_YOGI_WEAK_SYMBOL const int         constants::kMinRxQueueSize             = detail::get_core_constant<int,         int        >(23);
_YOGI_WEAK_SYMBOL const int         constants::kMaxRxQueueSize             = detail::get_core_constant<int,         int        >(24);
_YOGI_WEAK_SYMBOL const int         constants::kDefaultRxQueueSize         = detail::get_core_constant<int,         int        >(25);
_YOGI_WEAK_SYMBOL const int         constants::kDefaultWebPort             = detail::get_core_constant<int,         int        >(26);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultWebInterfaces       = detail::get_core_constant<std::string, const char*>(27);
_YOGI_WEAK_SYMBOL const Duration    constants::kDefaultWebTimeout          = detail::get_core_constant<Duration,    long long  >(28);
_YOGI_WEAK_SYMBOL const int         constants::kDefaultHttpHeaderLimit     = detail::get_core_constant<int,         int        >(29);
_YOGI_WEAK_SYMBOL const int         constants::kDefaultHttpBodyLimit       = detail::get_core_constant<int,         int        >(30);
_YOGI_WEAK_SYMBOL const int         constants::kDefaultWebCacheSize        = detail::get_core_constant<int,         int        >(31);
_YOGI_WEAK_SYMBOL const int         constants::kMaxWebCacheSize            = detail::get_core_constant<int,         int        >(32);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultAdminUser           = detail::get_core_constant<std::string, const char*>(33);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultAdminPassword       = detail::get_core_constant<std::string, const char*>(34);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultSslPrivateKey       = detail::get_core_constant<std::string, const char*>(35);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultSslCertificateChain = detail::get_core_constant<std::string, const char*>(36);
_YOGI_WEAK_SYMBOL const std::string constants::kDefaultSslDhParams         = detail::get_core_constant<std::string, const char*>(37);
// :CODEGEN_END:
// clang-format on

}  // namespace yogi

#endif  // _YOGI_CONSTANTS_H
