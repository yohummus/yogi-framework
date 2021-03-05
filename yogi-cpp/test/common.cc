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

#include <chrono>
using namespace std::chrono_literals;

Test::Test() {
  MOCK_ResetMocks();
  MOCK_Destroy([](void*) { return YOGI_OK; });
}

Test::~Test() {
}

void* const Test::kPointer = reinterpret_cast<void*>(0xDEADBEAFll);

yogi::ContextPtr Test::create_context() {
  MOCK_ContextCreate([](void** context) {
    *context = kPointer;
    return YOGI_OK;
  });

  return yogi::Context::create();
}

yogi::ConfigurationPtr Test::create_configuration(ConfigurationFlags flags) {
  MOCK_ConfigurationCreate([](void** config, int) {
    *config = kPointer;
    return YOGI_OK;
  });

  return yogi::Configuration::create(flags);
}

yogi::BranchPtr Test::create_branch() {
  auto context = create_context();
  auto config  = create_configuration();

  MOCK_BranchCreate([](void** branch, void* context, void* config, const char* section) {
    *branch = kPointer;
    return YOGI_OK;
  });

  MOCK_BranchGetInfo([](void* branch, void* uuid, const char** json, int* jsonsize) {
    *json = "{}";
    *jsonsize = 3;
    return YOGI_OK;
  });

  return yogi::Branch::create(context, config);
}

// clang-format off

void (*Test::MOCK_ResetMocks)()
  = detail::Library::get_function_address<void (*)()>("MOCK_ResetMocks");

// :CODEGEN_BEGIN:
void (*Test::MOCK_GetVersion)(const char* (*fn)())
 = detail::Library::get_function_address<void (*)(const char* (*fn)())>("MOCK_GetVersion");

void (*Test::MOCK_CheckBindingsCompatibility)(int (*fn)(const char* bindver))
 = detail::Library::get_function_address<void (*)(int (*fn)(const char* bindver))>("MOCK_CheckBindingsCompatibility");

void (*Test::MOCK_GetErrorString)(const char* (*fn)(int err))
 = detail::Library::get_function_address<void (*)(const char* (*fn)(int err))>("MOCK_GetErrorString");

void (*Test::MOCK_GetLastErrorDetails)(const char* (*fn)())
 = detail::Library::get_function_address<void (*)(const char* (*fn)())>("MOCK_GetLastErrorDetails");

void (*Test::MOCK_GetConstant)(int (*fn)(void* dest, int constant))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* dest, int constant))>("MOCK_GetConstant");

void (*Test::MOCK_GetSchema)(const char* (*fn)(int schema))
 = detail::Library::get_function_address<void (*)(const char* (*fn)(int schema))>("MOCK_GetSchema");

void (*Test::MOCK_GetCurrentTime)(int (*fn)(long long* timestamp))
 = detail::Library::get_function_address<void (*)(int (*fn)(long long* timestamp))>("MOCK_GetCurrentTime");

void (*Test::MOCK_FormatTime)(int (*fn)(long long timestamp, const char** str, int* strsize, const char* timefmt))
 = detail::Library::get_function_address<void (*)(int (*fn)(long long timestamp, const char** str, int* strsize, const char* timefmt))>("MOCK_FormatTime");

void (*Test::MOCK_ParseTime)(int (*fn)(long long* timestamp, const char* str, const char* timefmt))
 = detail::Library::get_function_address<void (*)(int (*fn)(long long* timestamp, const char* str, const char* timefmt))>("MOCK_ParseTime");

void (*Test::MOCK_FormatDuration)(int (*fn)(long long dur, int neg, const char** str, int* strsize, const char* durfmt, const char* inffmt))
 = detail::Library::get_function_address<void (*)(int (*fn)(long long dur, int neg, const char** str, int* strsize, const char* durfmt, const char* inffmt))>("MOCK_FormatDuration");

void (*Test::MOCK_FormatObject)(int (*fn)(void* obj, const char** str, int* strsize, const char* objfmt, const char* nullstr))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* obj, const char** str, int* strsize, const char* objfmt, const char* nullstr))>("MOCK_FormatObject");

void (*Test::MOCK_ConfigureConsoleLogging)(int (*fn)(int verbosity, int stream, int color, const char* timefmt, const char* fmt))
 = detail::Library::get_function_address<void (*)(int (*fn)(int verbosity, int stream, int color, const char* timefmt, const char* fmt))>("MOCK_ConfigureConsoleLogging");

void (*Test::MOCK_ConfigureHookLogging)(int (*fn)(int verbosity, void (*fn)(int severity, long long timestamp, int tid, const char* file, int line, const char* comp, const char* msg, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(int verbosity, void (*fn)(int severity, long long timestamp, int tid, const char* file, int line, const char* comp, const char* msg, void* userarg), void* userarg))>("MOCK_ConfigureHookLogging");

void (*Test::MOCK_ConfigureFileLogging)(int (*fn)(int verbosity, const char* filename, const char** genfn, int* genfnsize, const char* timefmt, const char* fmt))
 = detail::Library::get_function_address<void (*)(int (*fn)(int verbosity, const char* filename, const char** genfn, int* genfnsize, const char* timefmt, const char* fmt))>("MOCK_ConfigureFileLogging");

void (*Test::MOCK_LoggerCreate)(int (*fn)(void** logger, const char* component))
 = detail::Library::get_function_address<void (*)(int (*fn)(void** logger, const char* component))>("MOCK_LoggerCreate");

void (*Test::MOCK_LoggerGetVerbosity)(int (*fn)(void* logger, int* verbosity))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* logger, int* verbosity))>("MOCK_LoggerGetVerbosity");

void (*Test::MOCK_LoggerSetVerbosity)(int (*fn)(void* logger, int verbosity))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* logger, int verbosity))>("MOCK_LoggerSetVerbosity");

void (*Test::MOCK_LoggerSetComponentsVerbosity)(int (*fn)(const char* components, int verbosity, int* count))
 = detail::Library::get_function_address<void (*)(int (*fn)(const char* components, int verbosity, int* count))>("MOCK_LoggerSetComponentsVerbosity");

void (*Test::MOCK_LoggerLog)(int (*fn)(void* logger, int severity, const char* file, int line, const char* msg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* logger, int severity, const char* file, int line, const char* msg))>("MOCK_LoggerLog");

void (*Test::MOCK_ConfigurationCreate)(int (*fn)(void** config, int flags))
 = detail::Library::get_function_address<void (*)(int (*fn)(void** config, int flags))>("MOCK_ConfigurationCreate");

void (*Test::MOCK_ConfigurationUpdateFromCommandLine)(int (*fn)(void* config, int argc, const char* const* argv, int options))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* config, int argc, const char* const* argv, int options))>("MOCK_ConfigurationUpdateFromCommandLine");

void (*Test::MOCK_ConfigurationUpdateFromJson)(int (*fn)(void* config, const char* json))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* config, const char* json))>("MOCK_ConfigurationUpdateFromJson");

void (*Test::MOCK_ConfigurationUpdateFromFile)(int (*fn)(void* config, const char* filename))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* config, const char* filename))>("MOCK_ConfigurationUpdateFromFile");

void (*Test::MOCK_ConfigurationDump)(int (*fn)(void* config, const char** json, int* jsonsize, int resvars, int indent))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* config, const char** json, int* jsonsize, int resvars, int indent))>("MOCK_ConfigurationDump");

void (*Test::MOCK_ConfigurationWriteToFile)(int (*fn)(void* config, const char* filename, int resvars, int indent))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* config, const char* filename, int resvars, int indent))>("MOCK_ConfigurationWriteToFile");

void (*Test::MOCK_ConfigurationValidate)(int (*fn)(void* config, const char* section, void* schema))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* config, const char* section, void* schema))>("MOCK_ConfigurationValidate");

void (*Test::MOCK_ContextCreate)(int (*fn)(void** context))
 = detail::Library::get_function_address<void (*)(int (*fn)(void** context))>("MOCK_ContextCreate");

void (*Test::MOCK_ContextPoll)(int (*fn)(void* context, int* count))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* context, int* count))>("MOCK_ContextPoll");

void (*Test::MOCK_ContextPollOne)(int (*fn)(void* context, int* count))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* context, int* count))>("MOCK_ContextPollOne");

void (*Test::MOCK_ContextRun)(int (*fn)(void* context, int* count, long long duration))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* context, int* count, long long duration))>("MOCK_ContextRun");

void (*Test::MOCK_ContextRunOne)(int (*fn)(void* context, int* count, long long duration))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* context, int* count, long long duration))>("MOCK_ContextRunOne");

void (*Test::MOCK_ContextRunInBackground)(int (*fn)(void* context))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* context))>("MOCK_ContextRunInBackground");

void (*Test::MOCK_ContextStop)(int (*fn)(void* context))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* context))>("MOCK_ContextStop");

void (*Test::MOCK_ContextWaitForRunning)(int (*fn)(void* context, long long duration))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* context, long long duration))>("MOCK_ContextWaitForRunning");

void (*Test::MOCK_ContextWaitForStopped)(int (*fn)(void* context, long long duration))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* context, long long duration))>("MOCK_ContextWaitForStopped");

void (*Test::MOCK_ContextPost)(int (*fn)(void* context, void (*fn)(void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* context, void (*fn)(void* userarg), void* userarg))>("MOCK_ContextPost");

void (*Test::MOCK_RaiseSignal)(int (*fn)(int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg), void* userarg))>("MOCK_RaiseSignal");

void (*Test::MOCK_SignalSetCreate)(int (*fn)(void** sigset, void* context, int signals))
 = detail::Library::get_function_address<void (*)(int (*fn)(void** sigset, void* context, int signals))>("MOCK_SignalSetCreate");

void (*Test::MOCK_SignalSetAwaitSignalAsync)(int (*fn)(void* sigset, void (*fn)(int res, int sig, void* sigarg, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* sigset, void (*fn)(int res, int sig, void* sigarg, void* userarg), void* userarg))>("MOCK_SignalSetAwaitSignalAsync");

void (*Test::MOCK_SignalSetCancelAwaitSignal)(int (*fn)(void* sigset))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* sigset))>("MOCK_SignalSetCancelAwaitSignal");

void (*Test::MOCK_TimerCreate)(int (*fn)(void** timer, void* context))
 = detail::Library::get_function_address<void (*)(int (*fn)(void** timer, void* context))>("MOCK_TimerCreate");

void (*Test::MOCK_TimerStartAsync)(int (*fn)(void* timer, long long duration, void (*fn)(int res, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* timer, long long duration, void (*fn)(int res, void* userarg), void* userarg))>("MOCK_TimerStartAsync");

void (*Test::MOCK_TimerCancel)(int (*fn)(void* timer))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* timer))>("MOCK_TimerCancel");

void (*Test::MOCK_BranchCreate)(int (*fn)(void** branch, void* context, void* config, const char* section))
 = detail::Library::get_function_address<void (*)(int (*fn)(void** branch, void* context, void* config, const char* section))>("MOCK_BranchCreate");

void (*Test::MOCK_BranchGetInfo)(int (*fn)(void* branch, void* uuid, const char** json, int* jsonsize))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* branch, void* uuid, const char** json, int* jsonsize))>("MOCK_BranchGetInfo");

void (*Test::MOCK_BranchGetConnectedBranches)(int (*fn)(void* branch, void* uuid, char* json, int jsonsize, void (*fn)(int res, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* branch, void* uuid, char* json, int jsonsize, void (*fn)(int res, void* userarg), void* userarg))>("MOCK_BranchGetConnectedBranches");

void (*Test::MOCK_BranchAwaitEventAsync)(int (*fn)(void* branch, int events, void* uuid, char* json, int jsonsize, void (*fn)(int res, int ev, int evres, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* branch, int events, void* uuid, char* json, int jsonsize, void (*fn)(int res, int ev, int evres, void* userarg), void* userarg))>("MOCK_BranchAwaitEventAsync");

void (*Test::MOCK_BranchCancelAwaitEvent)(int (*fn)(void* branch))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* branch))>("MOCK_BranchCancelAwaitEvent");

void (*Test::MOCK_BranchSendBroadcast)(int (*fn)(void* branch, int enc, const void* data, int datasize, int block))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* branch, int enc, const void* data, int datasize, int block))>("MOCK_BranchSendBroadcast");

void (*Test::MOCK_BranchSendBroadcastAsync)(int (*fn)(void* branch, int enc, const void* data, int datasize, int retry, void (*fn)(int res, int oid, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* branch, int enc, const void* data, int datasize, int retry, void (*fn)(int res, int oid, void* userarg), void* userarg))>("MOCK_BranchSendBroadcastAsync");

void (*Test::MOCK_BranchCancelSendBroadcast)(int (*fn)(void* branch, int oid))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* branch, int oid))>("MOCK_BranchCancelSendBroadcast");

void (*Test::MOCK_BranchReceiveBroadcastAsync)(int (*fn)(void* branch, void* uuid, int enc, void* data, int datasize, void (*fn)(int res, int size, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* branch, void* uuid, int enc, void* data, int datasize, void (*fn)(int res, int size, void* userarg), void* userarg))>("MOCK_BranchReceiveBroadcastAsync");

void (*Test::MOCK_BranchCancelReceiveBroadcast)(int (*fn)(void* branch))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* branch))>("MOCK_BranchCancelReceiveBroadcast");

void (*Test::MOCK_Destroy)(int (*fn)(void* obj))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* obj))>("MOCK_Destroy");

void (*Test::MOCK_DestroyAll)(int (*fn)())
 = detail::Library::get_function_address<void (*)(int (*fn)())>("MOCK_DestroyAll");

void (*Test::MOCK_WebServerCreate)(int (*fn)(void** server, void* context, void* branch, void* config, const char* section))
 = detail::Library::get_function_address<void (*)(int (*fn)(void** server, void* context, void* branch, void* config, const char* section))>("MOCK_WebServerCreate");

void (*Test::MOCK_WebServerAddWorker)(int (*fn)(void* server, void* context))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* server, void* context))>("MOCK_WebServerAddWorker");

void (*Test::MOCK_WebRouteCreate)(int (*fn)(void** route, void* server, const char* baseuri, void (*fn)(int res, int rid, const char* user, const char* owner, int method, const char* uri, const char* const* qparams, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void** route, void* server, const char* baseuri, void (*fn)(int res, int rid, const char* user, const char* owner, int method, const char* uri, const char* const* qparams, void* userarg), void* userarg))>("MOCK_WebRouteCreate");

void (*Test::MOCK_WebRouteRespond)(int (*fn)(void* route, int rid, int finished, int status, const char* contype, const char* content))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* route, int rid, int finished, int status, const char* contype, const char* content))>("MOCK_WebRouteRespond");

void (*Test::MOCK_WebProcessCreate)(int (*fn)(void** wproc, void* server, const char* name, void (*fn)(int res, int wpid, const char* user, int wpa, const void* data, const int* sizes, void* userarg), void* userarg))
 = detail::Library::get_function_address<void (*)(int (*fn)(void** wproc, void* server, const char* name, void (*fn)(int res, int wpid, const char* user, int wpa, const void* data, const int* sizes, void* userarg), void* userarg))>("MOCK_WebProcessCreate");

void (*Test::MOCK_WebProcessUpdate)(int (*fn)(void* wproc, int wpid, int wpu, int exitcode, const void* output, int outsize))
 = detail::Library::get_function_address<void (*)(int (*fn)(void* wproc, int wpid, int wpu, int exitcode, const void* output, int outsize))>("MOCK_WebProcessUpdate");

// :CODEGEN_END:
// clang-format on
