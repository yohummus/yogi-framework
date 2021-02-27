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

#pragma once

#ifdef _MSC_VER
#  define _CRT_SECURE_NO_WARNINGS
#endif

#include <yogi.h>
#include <yogi_core.h>
using namespace yogi;

#include <gtest/gtest.h>

#include <initializer_list>

class Test : public testing::Test {
 public:
  Test();
  virtual ~Test();

 protected:
  static void* const kPointer;

  static yogi::ContextPtr create_context();
  static yogi::ConfigurationPtr create_configuration(ConfigurationFlags flags = ConfigurationFlags::kNone);
  static yogi::BranchPtr create_branch();

  static void (*MOCK_ResetMocks)();

  // clang-format off
  // :CODEGEN_BEGIN:
  static void (*MOCK_GetVersion)(const char* (*fn)());
  static void (*MOCK_CheckBindingsCompatibility)(int (*fn)(const char* bindver));
  static void (*MOCK_GetErrorString)(const char* (*fn)(int err));
  static void (*MOCK_GetLastErrorDetails)(const char* (*fn)());
  static void (*MOCK_GetConstant)(int (*fn)(void* dest, int constant));
  static void (*MOCK_GetSchema)(const char* (*fn)(int schema));
  static void (*MOCK_GetCurrentTime)(int (*fn)(long long* timestamp));
  static void (*MOCK_FormatTime)(int (*fn)(long long timestamp, const char** str, int* strsize, const char* timefmt));
  static void (*MOCK_ParseTime)(int (*fn)(long long* timestamp, const char* str, const char* timefmt));
  static void (*MOCK_FormatDuration)(int (*fn)(long long dur, int neg, const char** str, int* strsize, const char* durfmt, const char* inffmt));
  static void (*MOCK_FormatObject)(int (*fn)(void* obj, const char** str, int* strsize, const char* objfmt, const char* nullstr));
  static void (*MOCK_ConfigureConsoleLogging)(int (*fn)(int verbosity, int stream, int color, const char* timefmt, const char* fmt));
  static void (*MOCK_ConfigureHookLogging)(int (*fn)(int verbosity, void (*fn)(int severity, long long timestamp, int tid, const char* file, int line, const char* comp, const char* msg, void* userarg), void* userarg));
  static void (*MOCK_ConfigureFileLogging)(int (*fn)(int verbosity, const char* filename, const char** genfn, int* genfnsize, const char* timefmt, const char* fmt));
  static void (*MOCK_LoggerCreate)(int (*fn)(void** logger, const char* component));
  static void (*MOCK_LoggerGetVerbosity)(int (*fn)(void* logger, int* verbosity));
  static void (*MOCK_LoggerSetVerbosity)(int (*fn)(void* logger, int verbosity));
  static void (*MOCK_LoggerSetComponentsVerbosity)(int (*fn)(const char* components, int verbosity, int* count));
  static void (*MOCK_LoggerLog)(int (*fn)(void* logger, int severity, const char* file, int line, const char* msg));
  static void (*MOCK_ConfigurationCreate)(int (*fn)(void** config, int flags));
  static void (*MOCK_ConfigurationUpdateFromCommandLine)(int (*fn)(void* config, int argc, const char* const* argv, int options));
  static void (*MOCK_ConfigurationUpdateFromJson)(int (*fn)(void* config, const char* json));
  static void (*MOCK_ConfigurationUpdateFromFile)(int (*fn)(void* config, const char* filename));
  static void (*MOCK_ConfigurationDump)(int (*fn)(void* config, const char** json, int* jsonsize, int resvars, int indent));
  static void (*MOCK_ConfigurationWriteToFile)(int (*fn)(void* config, const char* filename, int resvars, int indent));
  static void (*MOCK_ConfigurationValidate)(int (*fn)(void* config, const char* section, void* schema));
  static void (*MOCK_ContextCreate)(int (*fn)(void** context));
  static void (*MOCK_ContextPoll)(int (*fn)(void* context, int* count));
  static void (*MOCK_ContextPollOne)(int (*fn)(void* context, int* count));
  static void (*MOCK_ContextRun)(int (*fn)(void* context, int* count, long long duration));
  static void (*MOCK_ContextRunOne)(int (*fn)(void* context, int* count, long long duration));
  static void (*MOCK_ContextRunInBackground)(int (*fn)(void* context));
  static void (*MOCK_ContextStop)(int (*fn)(void* context));
  static void (*MOCK_ContextWaitForRunning)(int (*fn)(void* context, long long duration));
  static void (*MOCK_ContextWaitForStopped)(int (*fn)(void* context, long long duration));
  static void (*MOCK_ContextPost)(int (*fn)(void* context, void (*fn)(void* userarg), void* userarg));
  static void (*MOCK_RaiseSignal)(int (*fn)(int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg), void* userarg));
  static void (*MOCK_SignalSetCreate)(int (*fn)(void** sigset, void* context, int signals));
  static void (*MOCK_SignalSetAwaitSignalAsync)(int (*fn)(void* sigset, void (*fn)(int res, int sig, void* sigarg, void* userarg), void* userarg));
  static void (*MOCK_SignalSetCancelAwaitSignal)(int (*fn)(void* sigset));
  static void (*MOCK_TimerCreate)(int (*fn)(void** timer, void* context));
  static void (*MOCK_TimerStartAsync)(int (*fn)(void* timer, long long duration, void (*fn)(int res, void* userarg), void* userarg));
  static void (*MOCK_TimerCancel)(int (*fn)(void* timer));
  static void (*MOCK_BranchCreate)(int (*fn)(void** branch, void* context, void* config, const char* section));
  static void (*MOCK_BranchGetInfo)(int (*fn)(void* branch, void* uuid, char* json, int jsonsize));
  static void (*MOCK_BranchGetConnectedBranches)(int (*fn)(void* branch, void* uuid, char* json, int jsonsize, void (*fn)(int res, void* userarg), void* userarg));
  static void (*MOCK_BranchAwaitEventAsync)(int (*fn)(void* branch, int events, void* uuid, char* json, int jsonsize, void (*fn)(int res, int ev, int evres, void* userarg), void* userarg));
  static void (*MOCK_BranchCancelAwaitEvent)(int (*fn)(void* branch));
  static void (*MOCK_BranchSendBroadcast)(int (*fn)(void* branch, int enc, const void* data, int datasize, int block));
  static void (*MOCK_BranchSendBroadcastAsync)(int (*fn)(void* branch, int enc, const void* data, int datasize, int retry, void (*fn)(int res, int oid, void* userarg), void* userarg));
  static void (*MOCK_BranchCancelSendBroadcast)(int (*fn)(void* branch, int oid));
  static void (*MOCK_BranchReceiveBroadcastAsync)(int (*fn)(void* branch, void* uuid, int enc, void* data, int datasize, void (*fn)(int res, int size, void* userarg), void* userarg));
  static void (*MOCK_BranchCancelReceiveBroadcast)(int (*fn)(void* branch));
  static void (*MOCK_Destroy)(int (*fn)(void* obj));
  static void (*MOCK_DestroyAll)(int (*fn)());
  static void (*MOCK_WebServerCreate)(int (*fn)(void** server, void* context, void* branch, void* config, const char* section));
  static void (*MOCK_WebServerAddWorker)(int (*fn)(void* server, void* context));
  static void (*MOCK_WebRouteCreate)(int (*fn)(void** route, void* server, const char* baseuri, void (*fn)(int res, int rid, const char* user, const char* owner, int method, const char* uri, const char* const* qparams, void* userarg), void* userarg));
  static void (*MOCK_WebRouteRespond)(int (*fn)(void* route, int rid, int finished, int status, const char* contype, const char* content));
  static void (*MOCK_WebProcessCreate)(int (*fn)(void** wproc, void* server, const char* name, void (*fn)(int res, int wpid, const char* user, int wpa, const void* data, const int* sizes, void* userarg), void* userarg));
  static void (*MOCK_WebProcessUpdate)(int (*fn)(void* wproc, int wpid, int wpu, int exitcode, const void* output, int outsize));
  // :CODEGEN_END:
  // clang-format on
};
