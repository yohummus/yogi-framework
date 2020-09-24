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

#include <yogi_core.h>

// The default_...() function definitions get picked up by codegen and used as default functions in yogi_core_mock.cc

const char* default_GetVersion() {
  // :CODEGEN_BEGIN:
  return "0.0.1-alpha";
  // :CODEGEN_END:
}

int default_CheckBindingsCompatibility(const char* bindver) {
  return YOGI_OK;
}

int default_GetConstant(void* dest, int constant) {
  // clang-format off
  switch (constant) {
    // :CODEGEN_BEGIN:
    case YOGI_CONST_VERSION: *static_cast<const char**>(dest) = "0.0.1-alpha"; break;
    case YOGI_CONST_VERSION_MAJOR: *static_cast<int*>(dest) = 0; break;
    case YOGI_CONST_VERSION_MINOR: *static_cast<int*>(dest) = 0; break;
    case YOGI_CONST_VERSION_PATCH: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_VERSION_SUFFIX: *static_cast<const char**>(dest) = "-alpha"; break;
    case YOGI_CONST_DEFAULT_ADV_INTERFACES: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_ADV_ADDRESS: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_ADV_PORT: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_ADV_INTERVAL: *static_cast<long long*>(dest) = 123456789; break;
    case YOGI_CONST_DEFAULT_CONNECTION_TIMEOUT: *static_cast<long long*>(dest) = 123456789; break;
    case YOGI_CONST_DEFAULT_LOGGER_VERBOSITY: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_LOG_TIME_FORMAT: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_LOG_FORMAT: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_MAX_MESSAGE_PAYLOAD_SIZE: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_TIME_FORMAT: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_INF_DURATION_STRING: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_DURATION_FORMAT: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_INVALID_HANDLE_STRING: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_OBJECT_FORMAT: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_MIN_TX_QUEUE_SIZE: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_MAX_TX_QUEUE_SIZE: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_TX_QUEUE_SIZE: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_MIN_RX_QUEUE_SIZE: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_MAX_RX_QUEUE_SIZE: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_RX_QUEUE_SIZE: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_WEB_PORT: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_WEB_INTERFACES: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_WEB_TIMEOUT: *static_cast<long long*>(dest) = 123456789; break;
    case YOGI_CONST_DEFAULT_HTTP_HEADER_LIMIT: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_HTTP_BODY_LIMIT: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_WEB_CACHE_SIZE: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_MAX_WEB_CACHE_SIZE: *static_cast<int*>(dest) = 1; break;
    case YOGI_CONST_DEFAULT_ADMIN_USER: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_ADMIN_PASSWORD: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_SSL_PRIVATE_KEY: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_SSL_CERTIFICATE_CHAIN: *static_cast<const char**>(dest) = "foobar"; break;
    case YOGI_CONST_DEFAULT_SSL_DH_PARAMS: *static_cast<const char**>(dest) = "foobar"; break;
    // :CODEGEN_END:
  }
  // clang-format on

  return YOGI_OK;
}
