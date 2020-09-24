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

#include <yogi_core.h>

#define CHECK(enum_type, enum_member, core_macro)                  \
  EXPECT_EQ(static_cast<int>(enum_type::enum_member), core_macro); \
  EXPECT_EQ(to_string(enum_type::enum_member), #enum_member);

class EnumsTest : public Test {};

TEST_F(EnumsTest, ErrorCode) {
  CHECK(ErrorCode, kOk, YOGI_OK);
  CHECK(ErrorCode, kUnknown, YOGI_ERR_UNKNOWN);
}

TEST_F(EnumsTest, Verbosity) {
  CHECK(Verbosity, kError, YOGI_VB_ERROR);
  CHECK(Verbosity, kDebug, YOGI_VB_DEBUG);
}

TEST_F(EnumsTest, Stream) {
  CHECK(Stream, kStdout, YOGI_ST_STDOUT);
  CHECK(Stream, kStderr, YOGI_ST_STDERR);
}

TEST_F(EnumsTest, Schema) {
  CHECK(Schema, kBranchConfig, YOGI_SCM_BRANCH_CONFIG);
  CHECK(Schema, kBranchEvent, YOGI_SCM_BRANCH_EVENT);
}

TEST_F(EnumsTest, Encoding) {
  CHECK(Encoding, kJson, YOGI_ENC_JSON);
  CHECK(Encoding, kMsgpack, YOGI_ENC_MSGPACK);
}

TEST_F(EnumsTest, HttpStatus) {
  CHECK(HttpStatus, k400, YOGI_HTTP_400);
  CHECK(HttpStatus, k503, YOGI_HTTP_503);
}

TEST_F(EnumsTest, WebProcessAction) {
  CHECK(WebProcessAction, kStart, YOGI_WPA_START);
  CHECK(WebProcessAction, kCanceled, YOGI_WPA_CANCELED);
}

TEST_F(EnumsTest, WebProcessUpdate) {
  CHECK(WebProcessUpdate, kStarted, YOGI_WPU_STARTED);
  CHECK(WebProcessUpdate, kCanceled, YOGI_WPU_CANCELED);
}

TEST_F(EnumsTest, HttpMethods) {
  CHECK(HttpMethods, kGet, YOGI_MET_GET);
  CHECK(HttpMethods, kDelete, YOGI_MET_DELETE);
}

TEST_F(EnumsTest, Signals) {
  CHECK(Signals, kTerm, YOGI_SIG_TERM);
  CHECK(Signals, kUsr1, YOGI_SIG_USR1);
}

TEST_F(EnumsTest, ConfigurationFlags) {
  CHECK(ConfigurationFlags, kDisableVariables, YOGI_CFG_DISABLE_VARIABLES);
  CHECK(ConfigurationFlags, kMutableCmdLine, YOGI_CFG_MUTABLE_CMD_LINE);
}

TEST_F(EnumsTest, CommandLineOptions) {
  CHECK(CommandLineOptions, kBranchName, YOGI_CLO_BRANCH_NAME);
  CHECK(CommandLineOptions, kBranchAll, YOGI_CLO_BRANCH_ALL);
}

TEST_F(EnumsTest, BranchEvents) {
  CHECK(BranchEvents, kBranchQueried, YOGI_BEV_BRANCH_QUERIED);
  CHECK(BranchEvents, kConnectFinished, YOGI_BEV_CONNECT_FINISHED);
}

TEST_F(EnumsTest, InvalidEnumValue) {
  EXPECT_EQ(to_string(static_cast<Verbosity>(9999999)), "INVALID ENUM VALUE");
}

TEST_F(EnumsTest, FlagsToString) {
  CommandLineOptions clo = CommandLineOptions::kBranchAdvInt;
  EXPECT_EQ(yogi::to_string(clo), "kBranchAdvInt");

  clo |= CommandLineOptions::kBranchAdvPort;
  EXPECT_EQ(yogi::to_string(clo), "kBranchAdvPort | kBranchAdvInt");

  clo |= CommandLineOptions::kBranchAll;
  EXPECT_EQ(yogi::to_string(clo), "kBranchAll");
}
