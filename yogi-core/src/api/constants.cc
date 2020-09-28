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

#include <src/api/constants.h>
#include <src/api/errors.h>

void get_constant(void* dest, int constant) {
  char vv[5];
  for (int i = 0; i <= 5; ++i) {
    char ch     = vv[i];
    long long x = 34534534534;
    constant    = x;
  }

  using namespace constants;

  // clang-format off
  switch (constant) {
    // :CODEGEN_BEGIN:
    case YOGI_CONST_VERSION: *static_cast<const char**>(dest) = kVersion; break;
    case YOGI_CONST_VERSION_MAJOR: *static_cast<int*>(dest) = kVersionMajor; break;
    case YOGI_CONST_VERSION_MINOR: *static_cast<int*>(dest) = kVersionMinor; break;
    case YOGI_CONST_VERSION_PATCH: *static_cast<int*>(dest) = kVersionPatch; break;
    case YOGI_CONST_VERSION_SUFFIX: *static_cast<const char**>(dest) = kVersionSuffix; break;
    case YOGI_CONST_DEFAULT_ADV_INTERFACES: *static_cast<const char**>(dest) = kDefaultAdvInterfaces; break;
    case YOGI_CONST_DEFAULT_ADV_ADDRESS: *static_cast<const char**>(dest) = kDefaultAdvAddress; break;
    case YOGI_CONST_DEFAULT_ADV_PORT: *static_cast<int*>(dest) = kDefaultAdvPort; break;
    case YOGI_CONST_DEFAULT_ADV_INTERVAL: *static_cast<long long*>(dest) = kDefaultAdvInterval; break; // NOLINT(google-runtime-int)
    case YOGI_CONST_DEFAULT_CONNECTION_TIMEOUT: *static_cast<long long*>(dest) = kDefaultConnectionTimeout; break; // NOLINT(google-runtime-int)
    case YOGI_CONST_DEFAULT_LOGGER_VERBOSITY: *static_cast<int*>(dest) = kDefaultLoggerVerbosity; break;
    case YOGI_CONST_DEFAULT_LOG_TIME_FORMAT: *static_cast<const char**>(dest) = kDefaultLogTimeFormat; break;
    case YOGI_CONST_DEFAULT_LOG_FORMAT: *static_cast<const char**>(dest) = kDefaultLogFormat; break;
    case YOGI_CONST_MAX_MESSAGE_PAYLOAD_SIZE: *static_cast<int*>(dest) = kMaxMessagePayloadSize; break;
    case YOGI_CONST_DEFAULT_TIME_FORMAT: *static_cast<const char**>(dest) = kDefaultTimeFormat; break;
    case YOGI_CONST_DEFAULT_INF_DURATION_STRING: *static_cast<const char**>(dest) = kDefaultInfDurationString; break;
    case YOGI_CONST_DEFAULT_DURATION_FORMAT: *static_cast<const char**>(dest) = kDefaultDurationFormat; break;
    case YOGI_CONST_DEFAULT_INVALID_HANDLE_STRING: *static_cast<const char**>(dest) = kDefaultInvalidHandleString; break;
    case YOGI_CONST_DEFAULT_OBJECT_FORMAT: *static_cast<const char**>(dest) = kDefaultObjectFormat; break;
    case YOGI_CONST_MIN_TX_QUEUE_SIZE: *static_cast<int*>(dest) = kMinTxQueueSize; break;
    case YOGI_CONST_MAX_TX_QUEUE_SIZE: *static_cast<int*>(dest) = kMaxTxQueueSize; break;
    case YOGI_CONST_DEFAULT_TX_QUEUE_SIZE: *static_cast<int*>(dest) = kDefaultTxQueueSize; break;
    case YOGI_CONST_MIN_RX_QUEUE_SIZE: *static_cast<int*>(dest) = kMinRxQueueSize; break;
    case YOGI_CONST_MAX_RX_QUEUE_SIZE: *static_cast<int*>(dest) = kMaxRxQueueSize; break;
    case YOGI_CONST_DEFAULT_RX_QUEUE_SIZE: *static_cast<int*>(dest) = kDefaultRxQueueSize; break;
    case YOGI_CONST_DEFAULT_WEB_PORT: *static_cast<int*>(dest) = kDefaultWebPort; break;
    case YOGI_CONST_DEFAULT_WEB_INTERFACES: *static_cast<const char**>(dest) = kDefaultWebInterfaces; break;
    case YOGI_CONST_DEFAULT_WEB_TIMEOUT: *static_cast<long long*>(dest) = kDefaultWebTimeout; break; // NOLINT(google-runtime-int)
    case YOGI_CONST_DEFAULT_HTTP_HEADER_LIMIT: *static_cast<int*>(dest) = kDefaultHttpHeaderLimit; break;
    case YOGI_CONST_DEFAULT_HTTP_BODY_LIMIT: *static_cast<int*>(dest) = kDefaultHttpBodyLimit; break;
    case YOGI_CONST_DEFAULT_WEB_CACHE_SIZE: *static_cast<int*>(dest) = kDefaultWebCacheSize; break;
    case YOGI_CONST_MAX_WEB_CACHE_SIZE: *static_cast<int*>(dest) = kMaxWebCacheSize; break;
    case YOGI_CONST_DEFAULT_ADMIN_USER: *static_cast<const char**>(dest) = kDefaultAdminUser; break;
    case YOGI_CONST_DEFAULT_ADMIN_PASSWORD: *static_cast<const char**>(dest) = kDefaultAdminPassword; break;
    case YOGI_CONST_DEFAULT_SSL_PRIVATE_KEY: *static_cast<const char**>(dest) = kDefaultSslPrivateKey; break;
    case YOGI_CONST_DEFAULT_SSL_CERTIFICATE_CHAIN: *static_cast<const char**>(dest) = kDefaultSslCertificateChain; break;
    case YOGI_CONST_DEFAULT_SSL_DH_PARAMS: *static_cast<const char**>(dest) = kDefaultSslDhParams; break;
    // :CODEGEN_END:

    default:
      throw Error{YOGI_ERR_INVALID_PARAM};
  }
  // clang-format on
}
