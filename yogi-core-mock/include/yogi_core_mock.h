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

#ifndef YOGI_API
#  ifdef _MSC_VER
#    define YOGI_API __declspec(dllexport)
#  else
#    define YOGI_API __attribute__((visibility("default")))
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// :CODEGEN_BEGIN:
YOGI_API void MOCK_GetVersion(decltype(YOGI_GetVersion) fn);
YOGI_API void MOCK_CheckBindingsCompatibility(decltype(YOGI_CheckBindingsCompatibility) fn);
YOGI_API void MOCK_GetErrorString(decltype(YOGI_GetErrorString) fn);
YOGI_API void MOCK_GetLastErrorDetails(decltype(YOGI_GetLastErrorDetails) fn);
YOGI_API void MOCK_GetConstant(decltype(YOGI_GetConstant) fn);
YOGI_API void MOCK_GetSchema(decltype(YOGI_GetSchema) fn);
YOGI_API void MOCK_GetCurrentTime(decltype(YOGI_GetCurrentTime) fn);
YOGI_API void MOCK_FormatTime(decltype(YOGI_FormatTime) fn);
YOGI_API void MOCK_ParseTime(decltype(YOGI_ParseTime) fn);
YOGI_API void MOCK_FormatDuration(decltype(YOGI_FormatDuration) fn);
YOGI_API void MOCK_FormatObject(decltype(YOGI_FormatObject) fn);
YOGI_API void MOCK_ConfigureConsoleLogging(decltype(YOGI_ConfigureConsoleLogging) fn);
YOGI_API void MOCK_ConfigureHookLogging(decltype(YOGI_ConfigureHookLogging) fn);
YOGI_API void MOCK_ConfigureFileLogging(decltype(YOGI_ConfigureFileLogging) fn);
YOGI_API void MOCK_LoggerCreate(decltype(YOGI_LoggerCreate) fn);
YOGI_API void MOCK_LoggerGetVerbosity(decltype(YOGI_LoggerGetVerbosity) fn);
YOGI_API void MOCK_LoggerSetVerbosity(decltype(YOGI_LoggerSetVerbosity) fn);
YOGI_API void MOCK_LoggerSetComponentsVerbosity(decltype(YOGI_LoggerSetComponentsVerbosity) fn);
YOGI_API void MOCK_LoggerLog(decltype(YOGI_LoggerLog) fn);
YOGI_API void MOCK_ConfigurationCreate(decltype(YOGI_ConfigurationCreate) fn);
YOGI_API void MOCK_ConfigurationUpdateFromCommandLine(decltype(YOGI_ConfigurationUpdateFromCommandLine) fn);
YOGI_API void MOCK_ConfigurationUpdateFromJson(decltype(YOGI_ConfigurationUpdateFromJson) fn);
YOGI_API void MOCK_ConfigurationUpdateFromFile(decltype(YOGI_ConfigurationUpdateFromFile) fn);
YOGI_API void MOCK_ConfigurationDump(decltype(YOGI_ConfigurationDump) fn);
YOGI_API void MOCK_ConfigurationWriteToFile(decltype(YOGI_ConfigurationWriteToFile) fn);
YOGI_API void MOCK_ConfigurationValidate(decltype(YOGI_ConfigurationValidate) fn);
YOGI_API void MOCK_ContextCreate(decltype(YOGI_ContextCreate) fn);
YOGI_API void MOCK_ContextPoll(decltype(YOGI_ContextPoll) fn);
YOGI_API void MOCK_ContextPollOne(decltype(YOGI_ContextPollOne) fn);
YOGI_API void MOCK_ContextRun(decltype(YOGI_ContextRun) fn);
YOGI_API void MOCK_ContextRunOne(decltype(YOGI_ContextRunOne) fn);
YOGI_API void MOCK_ContextRunInBackground(decltype(YOGI_ContextRunInBackground) fn);
YOGI_API void MOCK_ContextStop(decltype(YOGI_ContextStop) fn);
YOGI_API void MOCK_ContextWaitForRunning(decltype(YOGI_ContextWaitForRunning) fn);
YOGI_API void MOCK_ContextWaitForStopped(decltype(YOGI_ContextWaitForStopped) fn);
YOGI_API void MOCK_ContextPost(decltype(YOGI_ContextPost) fn);
YOGI_API void MOCK_RaiseSignal(decltype(YOGI_RaiseSignal) fn);
YOGI_API void MOCK_SignalSetCreate(decltype(YOGI_SignalSetCreate) fn);
YOGI_API void MOCK_SignalSetAwaitSignalAsync(decltype(YOGI_SignalSetAwaitSignalAsync) fn);
YOGI_API void MOCK_SignalSetCancelAwaitSignal(decltype(YOGI_SignalSetCancelAwaitSignal) fn);
YOGI_API void MOCK_TimerCreate(decltype(YOGI_TimerCreate) fn);
YOGI_API void MOCK_TimerStartAsync(decltype(YOGI_TimerStartAsync) fn);
YOGI_API void MOCK_TimerCancel(decltype(YOGI_TimerCancel) fn);
YOGI_API void MOCK_BranchCreate(decltype(YOGI_BranchCreate) fn);
YOGI_API void MOCK_BranchGetInfo(decltype(YOGI_BranchGetInfo) fn);
YOGI_API void MOCK_BranchGetConnectedBranches(decltype(YOGI_BranchGetConnectedBranches) fn);
YOGI_API void MOCK_BranchAwaitEventAsync(decltype(YOGI_BranchAwaitEventAsync) fn);
YOGI_API void MOCK_BranchCancelAwaitEvent(decltype(YOGI_BranchCancelAwaitEvent) fn);
YOGI_API void MOCK_BranchSendBroadcast(decltype(YOGI_BranchSendBroadcast) fn);
YOGI_API void MOCK_BranchSendBroadcastAsync(decltype(YOGI_BranchSendBroadcastAsync) fn);
YOGI_API void MOCK_BranchCancelSendBroadcast(decltype(YOGI_BranchCancelSendBroadcast) fn);
YOGI_API void MOCK_BranchReceiveBroadcastAsync(decltype(YOGI_BranchReceiveBroadcastAsync) fn);
YOGI_API void MOCK_BranchCancelReceiveBroadcast(decltype(YOGI_BranchCancelReceiveBroadcast) fn);
YOGI_API void MOCK_Destroy(decltype(YOGI_Destroy) fn);
YOGI_API void MOCK_DestroyAll(decltype(YOGI_DestroyAll) fn);
YOGI_API void MOCK_WebServerCreate(decltype(YOGI_WebServerCreate) fn);
YOGI_API void MOCK_WebServerAddWorker(decltype(YOGI_WebServerAddWorker) fn);
YOGI_API void MOCK_WebRouteCreate(decltype(YOGI_WebRouteCreate) fn);
YOGI_API void MOCK_WebRouteRespond(decltype(YOGI_WebRouteRespond) fn);
YOGI_API void MOCK_WebProcessCreate(decltype(YOGI_WebProcessCreate) fn);
YOGI_API void MOCK_WebProcessUpdate(decltype(YOGI_WebProcessUpdate) fn);
// :CODEGEN_END:

YOGI_API void MOCK_ResetMocks();

#ifdef __cplusplus
}  // extern "C"
#endif
