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

#ifndef YOGI_API
#ifdef _MSC_VER
#  define YOGI_API __declspec(dllexport)
#else
#  define YOGI_API __attribute__((visibility("default")))
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// :CODEGEN_BEGIN:

// YOGI_GetVersion
YOGI_API const char* YOGI_GetVersion();
YOGI_API void MOCK_GetVersion(decltype(YOGI_GetVersion) fn);

// YOGI_CheckBindingsCompatibility
YOGI_API int YOGI_CheckBindingsCompatibility(const char* bindver);
YOGI_API void MOCK_CheckBindingsCompatibility(decltype(YOGI_CheckBindingsCompatibility) fn);

// YOGI_GetErrorString
YOGI_API const char* YOGI_GetErrorString(int err);
YOGI_API void MOCK_GetErrorString(decltype(YOGI_GetErrorString) fn);

// YOGI_GetLastErrorDetails
YOGI_API const char* YOGI_GetLastErrorDetails();
YOGI_API void MOCK_GetLastErrorDetails(decltype(YOGI_GetLastErrorDetails) fn);

// YOGI_GetConstant
YOGI_API int YOGI_GetConstant(void* dest, int constant);
YOGI_API void MOCK_GetConstant(decltype(YOGI_GetConstant) fn);

// YOGI_GetSchema
YOGI_API const char* YOGI_GetSchema(int schema);
YOGI_API void MOCK_GetSchema(decltype(YOGI_GetSchema) fn);

// YOGI_GetCurrentTime
YOGI_API int YOGI_GetCurrentTime(long long* timestamp);
YOGI_API void MOCK_GetCurrentTime(decltype(YOGI_GetCurrentTime) fn);

// YOGI_FormatTime
YOGI_API int YOGI_FormatTime(long long timestamp, const char** str, int* strsize, const char* timefmt);
YOGI_API void MOCK_FormatTime(decltype(YOGI_FormatTime) fn);

// YOGI_ParseTime
YOGI_API int YOGI_ParseTime(long long* timestamp, const char* str, const char* timefmt);
YOGI_API void MOCK_ParseTime(decltype(YOGI_ParseTime) fn);

// YOGI_FormatDuration
YOGI_API int YOGI_FormatDuration(long long dur, int neg, const char** str, int* strsize, const char* durfmt,
                                 const char* inffmt);
YOGI_API void MOCK_FormatDuration(decltype(YOGI_FormatDuration) fn);

// YOGI_FormatObject
YOGI_API int YOGI_FormatObject(void* obj, const char** str, int* strsize, const char* objfmt, const char* nullstr);
YOGI_API void MOCK_FormatObject(decltype(YOGI_FormatObject) fn);

// YOGI_ConfigureConsoleLogging
YOGI_API int YOGI_ConfigureConsoleLogging(int verbosity, int stream, int color, const char* timefmt, const char* fmt);
YOGI_API void MOCK_ConfigureConsoleLogging(decltype(YOGI_ConfigureConsoleLogging) fn);

// YOGI_ConfigureHookLogging
YOGI_API int YOGI_ConfigureHookLogging(int verbosity,
                                       void (*fn)(int severity, long long timestamp, int tid, const char* file,
                                                  int line, const char* comp, const char* msg, void* userarg),
                                       void* userarg);
YOGI_API void MOCK_ConfigureHookLogging(decltype(YOGI_ConfigureHookLogging) fn);

// YOGI_ConfigureFileLogging
YOGI_API int YOGI_ConfigureFileLogging(int verbosity, const char* filename, const char** genfn, int* genfnsize,
                                       const char* timefmt, const char* fmt);
YOGI_API void MOCK_ConfigureFileLogging(decltype(YOGI_ConfigureFileLogging) fn);

// YOGI_LoggerCreate
YOGI_API int YOGI_LoggerCreate(void** logger, const char* component);
YOGI_API void MOCK_LoggerCreate(decltype(YOGI_LoggerCreate) fn);

// YOGI_LoggerGetVerbosity
YOGI_API int YOGI_LoggerGetVerbosity(void* logger, int* verbosity);
YOGI_API void MOCK_LoggerGetVerbosity(decltype(YOGI_LoggerGetVerbosity) fn);

// YOGI_LoggerSetVerbosity
YOGI_API int YOGI_LoggerSetVerbosity(void* logger, int verbosity);
YOGI_API void MOCK_LoggerSetVerbosity(decltype(YOGI_LoggerSetVerbosity) fn);

// YOGI_LoggerSetComponentsVerbosity
YOGI_API int YOGI_LoggerSetComponentsVerbosity(const char* components, int verbosity, int* count);
YOGI_API void MOCK_LoggerSetComponentsVerbosity(decltype(YOGI_LoggerSetComponentsVerbosity) fn);

// YOGI_LoggerLog
YOGI_API int YOGI_LoggerLog(void* logger, int severity, const char* file, int line, const char* msg);
YOGI_API void MOCK_LoggerLog(decltype(YOGI_LoggerLog) fn);

// YOGI_ConfigurationCreate
YOGI_API int YOGI_ConfigurationCreate(void** config, int flags);
YOGI_API void MOCK_ConfigurationCreate(decltype(YOGI_ConfigurationCreate) fn);

// YOGI_ConfigurationUpdateFromCommandLine
YOGI_API int YOGI_ConfigurationUpdateFromCommandLine(void* config, int argc, const char* const* argv, int options);
YOGI_API void MOCK_ConfigurationUpdateFromCommandLine(decltype(YOGI_ConfigurationUpdateFromCommandLine) fn);

// YOGI_ConfigurationUpdateFromJson
YOGI_API int YOGI_ConfigurationUpdateFromJson(void* config, const char* json);
YOGI_API void MOCK_ConfigurationUpdateFromJson(decltype(YOGI_ConfigurationUpdateFromJson) fn);

// YOGI_ConfigurationUpdateFromFile
YOGI_API int YOGI_ConfigurationUpdateFromFile(void* config, const char* filename);
YOGI_API void MOCK_ConfigurationUpdateFromFile(decltype(YOGI_ConfigurationUpdateFromFile) fn);

// YOGI_ConfigurationDump
YOGI_API int YOGI_ConfigurationDump(void* config, const char** json, int* jsonsize, int resvars, int indent);
YOGI_API void MOCK_ConfigurationDump(decltype(YOGI_ConfigurationDump) fn);

// YOGI_ConfigurationWriteToFile
YOGI_API int YOGI_ConfigurationWriteToFile(void* config, const char* filename, int resvars, int indent);
YOGI_API void MOCK_ConfigurationWriteToFile(decltype(YOGI_ConfigurationWriteToFile) fn);

// YOGI_ConfigurationValidate
YOGI_API int YOGI_ConfigurationValidate(void* config, const char* section, void* schema);
YOGI_API void MOCK_ConfigurationValidate(decltype(YOGI_ConfigurationValidate) fn);

// YOGI_ContextCreate
YOGI_API int YOGI_ContextCreate(void** context);
YOGI_API void MOCK_ContextCreate(decltype(YOGI_ContextCreate) fn);

// YOGI_ContextPoll
YOGI_API int YOGI_ContextPoll(void* context, int* count);
YOGI_API void MOCK_ContextPoll(decltype(YOGI_ContextPoll) fn);

// YOGI_ContextPollOne
YOGI_API int YOGI_ContextPollOne(void* context, int* count);
YOGI_API void MOCK_ContextPollOne(decltype(YOGI_ContextPollOne) fn);

// YOGI_ContextRun
YOGI_API int YOGI_ContextRun(void* context, int* count, long long duration);
YOGI_API void MOCK_ContextRun(decltype(YOGI_ContextRun) fn);

// YOGI_ContextRunOne
YOGI_API int YOGI_ContextRunOne(void* context, int* count, long long duration);
YOGI_API void MOCK_ContextRunOne(decltype(YOGI_ContextRunOne) fn);

// YOGI_ContextRunInBackground
YOGI_API int YOGI_ContextRunInBackground(void* context);
YOGI_API void MOCK_ContextRunInBackground(decltype(YOGI_ContextRunInBackground) fn);

// YOGI_ContextStop
YOGI_API int YOGI_ContextStop(void* context);
YOGI_API void MOCK_ContextStop(decltype(YOGI_ContextStop) fn);

// YOGI_ContextWaitForRunning
YOGI_API int YOGI_ContextWaitForRunning(void* context, long long duration);
YOGI_API void MOCK_ContextWaitForRunning(decltype(YOGI_ContextWaitForRunning) fn);

// YOGI_ContextWaitForStopped
YOGI_API int YOGI_ContextWaitForStopped(void* context, long long duration);
YOGI_API void MOCK_ContextWaitForStopped(decltype(YOGI_ContextWaitForStopped) fn);

// YOGI_ContextPost
YOGI_API int YOGI_ContextPost(void* context, void (*fn)(void* userarg), void* userarg);
YOGI_API void MOCK_ContextPost(decltype(YOGI_ContextPost) fn);

// YOGI_RaiseSignal
YOGI_API int YOGI_RaiseSignal(int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg), void* userarg);
YOGI_API void MOCK_RaiseSignal(decltype(YOGI_RaiseSignal) fn);

// YOGI_SignalSetCreate
YOGI_API int YOGI_SignalSetCreate(void** sigset, void* context, int signals);
YOGI_API void MOCK_SignalSetCreate(decltype(YOGI_SignalSetCreate) fn);

// YOGI_SignalSetAwaitSignalAsync
YOGI_API int YOGI_SignalSetAwaitSignalAsync(void* sigset, void (*fn)(int res, int sig, void* sigarg, void* userarg),
                                            void* userarg);
YOGI_API void MOCK_SignalSetAwaitSignalAsync(decltype(YOGI_SignalSetAwaitSignalAsync) fn);

// YOGI_SignalSetCancelAwaitSignal
YOGI_API int YOGI_SignalSetCancelAwaitSignal(void* sigset);
YOGI_API void MOCK_SignalSetCancelAwaitSignal(decltype(YOGI_SignalSetCancelAwaitSignal) fn);

// YOGI_TimerCreate
YOGI_API int YOGI_TimerCreate(void** timer, void* context);
YOGI_API void MOCK_TimerCreate(decltype(YOGI_TimerCreate) fn);

// YOGI_TimerStartAsync
YOGI_API int YOGI_TimerStartAsync(void* timer, long long duration, void (*fn)(int res, void* userarg), void* userarg);
YOGI_API void MOCK_TimerStartAsync(decltype(YOGI_TimerStartAsync) fn);

// YOGI_TimerCancel
YOGI_API int YOGI_TimerCancel(void* timer);
YOGI_API void MOCK_TimerCancel(decltype(YOGI_TimerCancel) fn);

// YOGI_BranchCreate
YOGI_API int YOGI_BranchCreate(void** branch, void* context, void* config, const char* section);
YOGI_API void MOCK_BranchCreate(decltype(YOGI_BranchCreate) fn);

// YOGI_BranchGetInfo
YOGI_API int YOGI_BranchGetInfo(void* branch, void* uuid, char* json, int jsonsize);
YOGI_API void MOCK_BranchGetInfo(decltype(YOGI_BranchGetInfo) fn);

// YOGI_BranchGetConnectedBranches
YOGI_API int YOGI_BranchGetConnectedBranches(void* branch, void* uuid, char* json, int jsonsize,
                                             void (*fn)(int res, void* userarg), void* userarg);
YOGI_API void MOCK_BranchGetConnectedBranches(decltype(YOGI_BranchGetConnectedBranches) fn);

// YOGI_BranchAwaitEventAsync
YOGI_API int YOGI_BranchAwaitEventAsync(void* branch, int events, void* uuid, char* json, int jsonsize,
                                        void (*fn)(int res, int ev, int evres, void* userarg), void* userarg);
YOGI_API void MOCK_BranchAwaitEventAsync(decltype(YOGI_BranchAwaitEventAsync) fn);

// YOGI_BranchCancelAwaitEvent
YOGI_API int YOGI_BranchCancelAwaitEvent(void* branch);
YOGI_API void MOCK_BranchCancelAwaitEvent(decltype(YOGI_BranchCancelAwaitEvent) fn);

// YOGI_BranchSendBroadcast
YOGI_API int YOGI_BranchSendBroadcast(void* branch, int enc, const void* data, int datasize, int block);
YOGI_API void MOCK_BranchSendBroadcast(decltype(YOGI_BranchSendBroadcast) fn);

// YOGI_BranchSendBroadcastAsync
YOGI_API int YOGI_BranchSendBroadcastAsync(void* branch, int enc, const void* data, int datasize, int retry,
                                           void (*fn)(int res, int oid, void* userarg), void* userarg);
YOGI_API void MOCK_BranchSendBroadcastAsync(decltype(YOGI_BranchSendBroadcastAsync) fn);

// YOGI_BranchCancelSendBroadcast
YOGI_API int YOGI_BranchCancelSendBroadcast(void* branch, int oid);
YOGI_API void MOCK_BranchCancelSendBroadcast(decltype(YOGI_BranchCancelSendBroadcast) fn);

// YOGI_BranchReceiveBroadcastAsync
YOGI_API int YOGI_BranchReceiveBroadcastAsync(void* branch, void* uuid, int enc, void* data, int datasize,
                                              void (*fn)(int res, int size, void* userarg), void* userarg);
YOGI_API void MOCK_BranchReceiveBroadcastAsync(decltype(YOGI_BranchReceiveBroadcastAsync) fn);

// YOGI_BranchCancelReceiveBroadcast
YOGI_API int YOGI_BranchCancelReceiveBroadcast(void* branch);
YOGI_API void MOCK_BranchCancelReceiveBroadcast(decltype(YOGI_BranchCancelReceiveBroadcast) fn);

// YOGI_Destroy
YOGI_API int YOGI_Destroy(void* obj);
YOGI_API void MOCK_Destroy(decltype(YOGI_Destroy) fn);

// YOGI_DestroyAll
YOGI_API int YOGI_DestroyAll();
YOGI_API void MOCK_DestroyAll(decltype(YOGI_DestroyAll) fn);

// YOGI_WebServerCreate
YOGI_API int YOGI_WebServerCreate(void** server, void* context, void* branch, void* config, const char* section);
YOGI_API void MOCK_WebServerCreate(decltype(YOGI_WebServerCreate) fn);

// YOGI_WebServerAddWorker
YOGI_API int YOGI_WebServerAddWorker(void* server, void* context);
YOGI_API void MOCK_WebServerAddWorker(decltype(YOGI_WebServerAddWorker) fn);

// YOGI_WebRouteCreate
YOGI_API int YOGI_WebRouteCreate(void** route, void* server, const char* baseuri,
                                 void (*fn)(int res, int rid, const char* user, const char* owner, int method,
                                            const char* uri, const char* const* qparams, void* userarg),
                                 void* userarg);
YOGI_API void MOCK_WebRouteCreate(decltype(YOGI_WebRouteCreate) fn);

// YOGI_WebRouteRespond
YOGI_API int YOGI_WebRouteRespond(void* route, int rid, int finished, int status, const char* contype,
                                  const char* content);
YOGI_API void MOCK_WebRouteRespond(decltype(YOGI_WebRouteRespond) fn);

// YOGI_WebProcessCreate
YOGI_API int YOGI_WebProcessCreate(void** wproc, void* server, const char* name,
                                   void (*fn)(int res, int wpid, const char* user, int wpa, const void* data,
                                              const int* sizes, void* userarg),
                                   void* userarg);
YOGI_API void MOCK_WebProcessCreate(decltype(YOGI_WebProcessCreate) fn);

// YOGI_WebProcessUpdate
YOGI_API int YOGI_WebProcessUpdate(void* wproc, int wpid, int wpu, int exitcode, const void* output, int outsize);
YOGI_API void MOCK_WebProcessUpdate(decltype(YOGI_WebProcessUpdate) fn);

// :CODEGEN_END:

YOGI_API void MOCK_ResetMocks();

#ifdef __cplusplus
}  // extern "C"
#endif
