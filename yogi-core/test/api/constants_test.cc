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

#include <test/common.h>

#include <src/api/constants.h>

void check(int constant, int value) {
  int x;
  YOGI_GetConstant(&x, constant);
  EXPECT_EQ(x, value) << "Constant (int): " << constant;
}

void check(int constant, long long value) {
  long long x;
  YOGI_GetConstant(&x, constant);
  EXPECT_EQ(x, value) << "Constant (long long): " << constant;
}

void check(int constant, const char* value) {
  const char* x;
  YOGI_GetConstant(&x, constant);
  EXPECT_STREQ(x, value) << "Constant (const char*): " << constant;
}

TEST(ConstantsTest, GetConstant) {
  int num = -1;
  YOGI_GetConstant(&num, YOGI_CONST_VERSION_MAJOR);
  EXPECT_NE(num, -1);

  int res = YOGI_GetConstant(nullptr, YOGI_CONST_VERSION_MAJOR);
  EXPECT_ERR(res, YOGI_ERR_INVALID_PARAM);

  EXPECT_EQ(YOGI_GetConstant(&num, -9999), YOGI_ERR_INVALID_PARAM);
}

TEST(ConstantsTest, GetEachConstant) {
  using namespace constants;

  // :CODEGEN_BEGIN:
  check(YOGI_CONST_VERSION, kVersion);
  check(YOGI_CONST_VERSION_MAJOR, kVersionMajor);
  check(YOGI_CONST_VERSION_MINOR, kVersionMinor);
  check(YOGI_CONST_VERSION_PATCH, kVersionPatch);
  check(YOGI_CONST_VERSION_SUFFIX, kVersionSuffix);
  check(YOGI_CONST_DEFAULT_ADV_INTERFACES, kDefaultAdvInterfaces);
  check(YOGI_CONST_DEFAULT_ADV_ADDRESS, kDefaultAdvAddress);
  check(YOGI_CONST_DEFAULT_ADV_PORT, kDefaultAdvPort);
  check(YOGI_CONST_DEFAULT_ADV_INTERVAL, kDefaultAdvInterval);
  check(YOGI_CONST_DEFAULT_CONNECTION_TIMEOUT, kDefaultConnectionTimeout);
  check(YOGI_CONST_DEFAULT_LOGGER_VERBOSITY, kDefaultLoggerVerbosity);
  check(YOGI_CONST_DEFAULT_LOG_TIME_FORMAT, kDefaultLogTimeFormat);
  check(YOGI_CONST_DEFAULT_LOG_FORMAT, kDefaultLogFormat);
  check(YOGI_CONST_MAX_MESSAGE_PAYLOAD_SIZE, kMaxMessagePayloadSize);
  check(YOGI_CONST_DEFAULT_TIME_FORMAT, kDefaultTimeFormat);
  check(YOGI_CONST_DEFAULT_INF_DURATION_STRING, kDefaultInfDurationString);
  check(YOGI_CONST_DEFAULT_DURATION_FORMAT, kDefaultDurationFormat);
  check(YOGI_CONST_DEFAULT_INVALID_HANDLE_STRING, kDefaultInvalidHandleString);
  check(YOGI_CONST_DEFAULT_OBJECT_FORMAT, kDefaultObjectFormat);
  check(YOGI_CONST_MIN_TX_QUEUE_SIZE, kMinTxQueueSize);
  check(YOGI_CONST_MAX_TX_QUEUE_SIZE, kMaxTxQueueSize);
  check(YOGI_CONST_DEFAULT_TX_QUEUE_SIZE, kDefaultTxQueueSize);
  check(YOGI_CONST_MIN_RX_QUEUE_SIZE, kMinRxQueueSize);
  check(YOGI_CONST_MAX_RX_QUEUE_SIZE, kMaxRxQueueSize);
  check(YOGI_CONST_DEFAULT_RX_QUEUE_SIZE, kDefaultRxQueueSize);
  check(YOGI_CONST_DEFAULT_WEB_PORT, kDefaultWebPort);
  check(YOGI_CONST_DEFAULT_WEB_INTERFACES, kDefaultWebInterfaces);
  check(YOGI_CONST_DEFAULT_WEB_TIMEOUT, kDefaultWebTimeout);
  check(YOGI_CONST_DEFAULT_HTTP_HEADER_LIMIT, kDefaultHttpHeaderLimit);
  check(YOGI_CONST_DEFAULT_HTTP_BODY_LIMIT, kDefaultHttpBodyLimit);
  check(YOGI_CONST_DEFAULT_WEB_CACHE_SIZE, kDefaultWebCacheSize);
  check(YOGI_CONST_MAX_WEB_CACHE_SIZE, kMaxWebCacheSize);
  check(YOGI_CONST_DEFAULT_ADMIN_USER, kDefaultAdminUser);
  check(YOGI_CONST_DEFAULT_ADMIN_PASSWORD, kDefaultAdminPassword);
  check(YOGI_CONST_DEFAULT_SSL_PRIVATE_KEY, kDefaultSslPrivateKey);
  check(YOGI_CONST_DEFAULT_SSL_CERTIFICATE_CHAIN, kDefaultSslCertificateChain);
  check(YOGI_CONST_DEFAULT_SSL_DH_PARAMS, kDefaultSslDhParams);
  // :CODEGEN_END:
}
