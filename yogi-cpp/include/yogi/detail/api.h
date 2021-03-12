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

#ifndef _YOGI_DETAIL_API_H
#define _YOGI_DETAIL_API_H

//! \file
//!
//! Symbols from the Yogi Core library.

#include "library.h"
#include "macros.h"

namespace yogi {
namespace detail {

// :CODEGEN_BEGIN:

// YOGI_GetVersion
_YOGI_WEAK_SYMBOL const char* (*YOGI_GetVersion)() =
    Library::get_function_address<const char* (*)()>("YOGI_GetVersion");

// YOGI_CheckBindingsCompatibility
_YOGI_WEAK_SYMBOL int (*YOGI_CheckBindingsCompatibility)(const char* bindver) =
    Library::get_function_address<int (*)(const char* bindver)>("YOGI_CheckBindingsCompatibility");

// YOGI_GetErrorString
_YOGI_WEAK_SYMBOL const char* (*YOGI_GetErrorString)(int err) =
    Library::get_function_address<const char* (*)(int err)>("YOGI_GetErrorString");

// YOGI_GetLastErrorDetails
_YOGI_WEAK_SYMBOL const char* (*YOGI_GetLastErrorDetails)() =
    Library::get_function_address<const char* (*)()>("YOGI_GetLastErrorDetails");

// YOGI_GetConstant
_YOGI_WEAK_SYMBOL int (*YOGI_GetConstant)(void* dest, int constant) =
    Library::get_function_address<int (*)(void* dest, int constant)>("YOGI_GetConstant");

// YOGI_GetSchema
_YOGI_WEAK_SYMBOL const char* (*YOGI_GetSchema)(int schema) =
    Library::get_function_address<const char* (*)(int schema)>("YOGI_GetSchema");

// YOGI_GetCurrentTime
_YOGI_WEAK_SYMBOL int (*YOGI_GetCurrentTime)(long long* timestamp) =
    Library::get_function_address<int (*)(long long* timestamp)>("YOGI_GetCurrentTime");

// YOGI_FormatTime
_YOGI_WEAK_SYMBOL int (*YOGI_FormatTime)(long long timestamp, const char** str, int* strsize, const char* timefmt) =
    Library::get_function_address<int (*)(long long timestamp, const char** str, int* strsize, const char* timefmt)>(
        "YOGI_FormatTime");

// YOGI_ParseTime
_YOGI_WEAK_SYMBOL int (*YOGI_ParseTime)(long long* timestamp, const char* str, const char* timefmt) =
    Library::get_function_address<int (*)(long long* timestamp, const char* str, const char* timefmt)>(
        "YOGI_ParseTime");

// YOGI_FormatDuration
_YOGI_WEAK_SYMBOL int (*YOGI_FormatDuration)(long long dur, int neg, const char** str, int* strsize, const char* durfmt,
                                             const char* inffmt) =
    Library::get_function_address<int (*)(long long dur, int neg, const char** str, int* strsize, const char* durfmt,
                                          const char* inffmt)>("YOGI_FormatDuration");

// YOGI_FormatObject
_YOGI_WEAK_SYMBOL int (*YOGI_FormatObject)(void* obj, const char** str, int* strsize, const char* objfmt,
                                           const char* nullstr) =
    Library::get_function_address<int (*)(void* obj, const char** str, int* strsize, const char* objfmt,
                                          const char* nullstr)>("YOGI_FormatObject");

// YOGI_ConfigureConsoleLogging
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigureConsoleLogging)(int verbosity, int stream, int color, const char* timefmt,
                                                      const char* fmt) =
    Library::get_function_address<int (*)(int verbosity, int stream, int color, const char* timefmt, const char* fmt)>(
        "YOGI_ConfigureConsoleLogging");

// YOGI_ConfigureHookLogging
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigureHookLogging)(int verbosity,
                                                   void (*fn)(int severity, long long timestamp, int tid,
                                                              const char* file, int line, const char* comp,
                                                              const char* msg, void* userarg),
                                                   void* userarg) =
    Library::get_function_address<int (*)(int verbosity,
                                          void (*fn)(int severity, long long timestamp, int tid, const char* file,
                                                     int line, const char* comp, const char* msg, void* userarg),
                                          void* userarg)>("YOGI_ConfigureHookLogging");

// YOGI_ConfigureFileLogging
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigureFileLogging)(int verbosity, const char* filename, const char** genfn,
                                                   int* genfnsize, const char* timefmt, const char* fmt) =
    Library::get_function_address<int (*)(int verbosity, const char* filename, const char** genfn, int* genfnsize,
                                          const char* timefmt, const char* fmt)>("YOGI_ConfigureFileLogging");

// YOGI_LoggerCreate
_YOGI_WEAK_SYMBOL int (*YOGI_LoggerCreate)(void** logger, const char* component) =
    Library::get_function_address<int (*)(void** logger, const char* component)>("YOGI_LoggerCreate");

// YOGI_LoggerGetVerbosity
_YOGI_WEAK_SYMBOL int (*YOGI_LoggerGetVerbosity)(void* logger, int* verbosity) =
    Library::get_function_address<int (*)(void* logger, int* verbosity)>("YOGI_LoggerGetVerbosity");

// YOGI_LoggerSetVerbosity
_YOGI_WEAK_SYMBOL int (*YOGI_LoggerSetVerbosity)(void* logger, int verbosity) =
    Library::get_function_address<int (*)(void* logger, int verbosity)>("YOGI_LoggerSetVerbosity");

// YOGI_LoggerSetComponentsVerbosity
_YOGI_WEAK_SYMBOL int (*YOGI_LoggerSetComponentsVerbosity)(const char* components, int verbosity, int* count) =
    Library::get_function_address<int (*)(const char* components, int verbosity, int* count)>(
        "YOGI_LoggerSetComponentsVerbosity");

// YOGI_LoggerLog
_YOGI_WEAK_SYMBOL int (*YOGI_LoggerLog)(void* logger, int severity, const char* file, int line, const char* msg) =
    Library::get_function_address<int (*)(void* logger, int severity, const char* file, int line, const char* msg)>(
        "YOGI_LoggerLog");

// YOGI_ConfigurationCreate
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigurationCreate)(void** config, int flags) =
    Library::get_function_address<int (*)(void** config, int flags)>("YOGI_ConfigurationCreate");

// YOGI_ConfigurationUpdateFromCommandLine
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigurationUpdateFromCommandLine)(void* config, int argc, const char* const* argv,
                                                                 int options) =
    Library::get_function_address<int (*)(void* config, int argc, const char* const* argv, int options)>(
        "YOGI_ConfigurationUpdateFromCommandLine");

// YOGI_ConfigurationUpdateFromJson
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigurationUpdateFromJson)(void* config, const char* json) =
    Library::get_function_address<int (*)(void* config, const char* json)>("YOGI_ConfigurationUpdateFromJson");

// YOGI_ConfigurationUpdateFromFile
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigurationUpdateFromFile)(void* config, const char* filename) =
    Library::get_function_address<int (*)(void* config, const char* filename)>("YOGI_ConfigurationUpdateFromFile");

// YOGI_ConfigurationDump
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigurationDump)(void* config, const char** json, int* jsonsize, int resvars,
                                                int indent) =
    Library::get_function_address<int (*)(void* config, const char** json, int* jsonsize, int resvars, int indent)>(
        "YOGI_ConfigurationDump");

// YOGI_ConfigurationWriteToFile
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigurationWriteToFile)(void* config, const char* filename, int resvars, int indent) =
    Library::get_function_address<int (*)(void* config, const char* filename, int resvars, int indent)>(
        "YOGI_ConfigurationWriteToFile");

// YOGI_ConfigurationValidate
_YOGI_WEAK_SYMBOL int (*YOGI_ConfigurationValidate)(void* config, const char* section, void* schema) =
    Library::get_function_address<int (*)(void* config, const char* section, void* schema)>(
        "YOGI_ConfigurationValidate");

// YOGI_ContextCreate
_YOGI_WEAK_SYMBOL int (*YOGI_ContextCreate)(void** context) =
    Library::get_function_address<int (*)(void** context)>("YOGI_ContextCreate");

// YOGI_ContextPoll
_YOGI_WEAK_SYMBOL int (*YOGI_ContextPoll)(void* context, int* count) =
    Library::get_function_address<int (*)(void* context, int* count)>("YOGI_ContextPoll");

// YOGI_ContextPollOne
_YOGI_WEAK_SYMBOL int (*YOGI_ContextPollOne)(void* context, int* count) =
    Library::get_function_address<int (*)(void* context, int* count)>("YOGI_ContextPollOne");

// YOGI_ContextRun
_YOGI_WEAK_SYMBOL int (*YOGI_ContextRun)(void* context, int* count, long long duration) =
    Library::get_function_address<int (*)(void* context, int* count, long long duration)>("YOGI_ContextRun");

// YOGI_ContextRunOne
_YOGI_WEAK_SYMBOL int (*YOGI_ContextRunOne)(void* context, int* count, long long duration) =
    Library::get_function_address<int (*)(void* context, int* count, long long duration)>("YOGI_ContextRunOne");

// YOGI_ContextRunInBackground
_YOGI_WEAK_SYMBOL int (*YOGI_ContextRunInBackground)(void* context) =
    Library::get_function_address<int (*)(void* context)>("YOGI_ContextRunInBackground");

// YOGI_ContextStop
_YOGI_WEAK_SYMBOL int (*YOGI_ContextStop)(void* context) =
    Library::get_function_address<int (*)(void* context)>("YOGI_ContextStop");

// YOGI_ContextWaitForRunning
_YOGI_WEAK_SYMBOL int (*YOGI_ContextWaitForRunning)(void* context, long long duration) =
    Library::get_function_address<int (*)(void* context, long long duration)>("YOGI_ContextWaitForRunning");

// YOGI_ContextWaitForStopped
_YOGI_WEAK_SYMBOL int (*YOGI_ContextWaitForStopped)(void* context, long long duration) =
    Library::get_function_address<int (*)(void* context, long long duration)>("YOGI_ContextWaitForStopped");

// YOGI_ContextPost
_YOGI_WEAK_SYMBOL int (*YOGI_ContextPost)(void* context, void (*fn)(void* userarg), void* userarg) =
    Library::get_function_address<int (*)(void* context, void (*fn)(void* userarg), void* userarg)>("YOGI_ContextPost");

// YOGI_RaiseSignal
_YOGI_WEAK_SYMBOL int (*YOGI_RaiseSignal)(int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg),
                                          void* userarg) =
    Library::get_function_address<int (*)(int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg),
                                          void* userarg)>("YOGI_RaiseSignal");

// YOGI_SignalSetCreate
_YOGI_WEAK_SYMBOL int (*YOGI_SignalSetCreate)(void** sigset, void* context, int signals) =
    Library::get_function_address<int (*)(void** sigset, void* context, int signals)>("YOGI_SignalSetCreate");

// YOGI_SignalSetAwaitSignalAsync
_YOGI_WEAK_SYMBOL int (*YOGI_SignalSetAwaitSignalAsync)(void* sigset,
                                                        void (*fn)(int res, int sig, void* sigarg, void* userarg),
                                                        void* userarg) =
    Library::get_function_address<int (*)(void* sigset, void (*fn)(int res, int sig, void* sigarg, void* userarg),
                                          void* userarg)>("YOGI_SignalSetAwaitSignalAsync");

// YOGI_SignalSetCancelAwaitSignal
_YOGI_WEAK_SYMBOL int (*YOGI_SignalSetCancelAwaitSignal)(void* sigset) =
    Library::get_function_address<int (*)(void* sigset)>("YOGI_SignalSetCancelAwaitSignal");

// YOGI_TimerCreate
_YOGI_WEAK_SYMBOL int (*YOGI_TimerCreate)(void** timer, void* context) =
    Library::get_function_address<int (*)(void** timer, void* context)>("YOGI_TimerCreate");

// YOGI_TimerStartAsync
_YOGI_WEAK_SYMBOL int (*YOGI_TimerStartAsync)(void* timer, long long duration, void (*fn)(int res, void* userarg),
                                              void* userarg) =
    Library::get_function_address<int (*)(void* timer, long long duration, void (*fn)(int res, void* userarg),
                                          void* userarg)>("YOGI_TimerStartAsync");

// YOGI_TimerCancel
_YOGI_WEAK_SYMBOL int (*YOGI_TimerCancel)(void* timer) =
    Library::get_function_address<int (*)(void* timer)>("YOGI_TimerCancel");

// YOGI_BranchCreate
_YOGI_WEAK_SYMBOL int (*YOGI_BranchCreate)(void** branch, void* context, void* config, const char* section) =
    Library::get_function_address<int (*)(void** branch, void* context, void* config, const char* section)>(
        "YOGI_BranchCreate");

// YOGI_BranchGetInfo
_YOGI_WEAK_SYMBOL int (*YOGI_BranchGetInfo)(void* branch, void* uuid, const char** json, int* jsonsize) =
    Library::get_function_address<int (*)(void* branch, void* uuid, const char** json, int* jsonsize)>(
        "YOGI_BranchGetInfo");

// YOGI_BranchGetConnectedBranches
_YOGI_WEAK_SYMBOL int (*YOGI_BranchGetConnectedBranches)(void* branch, const void** uuids, int* numuuids,
                                                         const char** json, int* jsonsize) =
    Library::get_function_address<int (*)(void* branch, const void** uuids, int* numuuids, const char** json,
                                          int* jsonsize)>("YOGI_BranchGetConnectedBranches");

// YOGI_BranchAwaitEventAsync
_YOGI_WEAK_SYMBOL int (*YOGI_BranchAwaitEventAsync)(void* branch, int events, void* uuid, char* json, int jsonsize,
                                                    void (*fn)(int res, int ev, int evres, void* userarg),
                                                    void* userarg) =
    Library::get_function_address<int (*)(void* branch, int events, void* uuid, char* json, int jsonsize,
                                          void (*fn)(int res, int ev, int evres, void* userarg), void* userarg)>(
        "YOGI_BranchAwaitEventAsync");

// YOGI_BranchCancelAwaitEvent
_YOGI_WEAK_SYMBOL int (*YOGI_BranchCancelAwaitEvent)(void* branch) =
    Library::get_function_address<int (*)(void* branch)>("YOGI_BranchCancelAwaitEvent");

// YOGI_BranchSendBroadcast
_YOGI_WEAK_SYMBOL int (*YOGI_BranchSendBroadcast)(void* branch, int enc, const void* data, int datasize, int block) =
    Library::get_function_address<int (*)(void* branch, int enc, const void* data, int datasize, int block)>(
        "YOGI_BranchSendBroadcast");

// YOGI_BranchSendBroadcastAsync
_YOGI_WEAK_SYMBOL int (*YOGI_BranchSendBroadcastAsync)(void* branch, int enc, const void* data, int datasize, int retry,
                                                       void (*fn)(int res, int oid, void* userarg), void* userarg) =
    Library::get_function_address<int (*)(void* branch, int enc, const void* data, int datasize, int retry,
                                          void (*fn)(int res, int oid, void* userarg), void* userarg)>(
        "YOGI_BranchSendBroadcastAsync");

// YOGI_BranchCancelSendBroadcast
_YOGI_WEAK_SYMBOL int (*YOGI_BranchCancelSendBroadcast)(void* branch, int oid) =
    Library::get_function_address<int (*)(void* branch, int oid)>("YOGI_BranchCancelSendBroadcast");

// YOGI_BranchReceiveBroadcastAsync
_YOGI_WEAK_SYMBOL int (*YOGI_BranchReceiveBroadcastAsync)(void* branch, void* uuid, int enc, void* data, int datasize,
                                                          void (*fn)(int res, int size, void* userarg), void* userarg) =
    Library::get_function_address<int (*)(void* branch, void* uuid, int enc, void* data, int datasize,
                                          void (*fn)(int res, int size, void* userarg), void* userarg)>(
        "YOGI_BranchReceiveBroadcastAsync");

// YOGI_BranchCancelReceiveBroadcast
_YOGI_WEAK_SYMBOL int (*YOGI_BranchCancelReceiveBroadcast)(void* branch) =
    Library::get_function_address<int (*)(void* branch)>("YOGI_BranchCancelReceiveBroadcast");

// YOGI_Destroy
_YOGI_WEAK_SYMBOL int (*YOGI_Destroy)(void* obj) = Library::get_function_address<int (*)(void* obj)>("YOGI_Destroy");

// YOGI_DestroyAll
_YOGI_WEAK_SYMBOL int (*YOGI_DestroyAll)() = Library::get_function_address<int (*)()>("YOGI_DestroyAll");

// YOGI_WebServerCreate
_YOGI_WEAK_SYMBOL int (*YOGI_WebServerCreate)(void** server, void* context, void* branch, void* config,
                                              const char* section) =
    Library::get_function_address<int (*)(void** server, void* context, void* branch, void* config,
                                          const char* section)>("YOGI_WebServerCreate");

// YOGI_WebServerAddWorker
_YOGI_WEAK_SYMBOL int (*YOGI_WebServerAddWorker)(void* server, void* context) =
    Library::get_function_address<int (*)(void* server, void* context)>("YOGI_WebServerAddWorker");

// YOGI_WebRouteCreate
_YOGI_WEAK_SYMBOL int (*YOGI_WebRouteCreate)(void** route, void* server, const char* baseuri,
                                             void (*fn)(int res, int rid, const char* user, const char* owner,
                                                        int method, const char* uri, const char* const* qparams,
                                                        void* userarg),
                                             void* userarg) =
    Library::get_function_address<int (*)(void** route, void* server, const char* baseuri,
                                          void (*fn)(int res, int rid, const char* user, const char* owner, int method,
                                                     const char* uri, const char* const* qparams, void* userarg),
                                          void* userarg)>("YOGI_WebRouteCreate");

// YOGI_WebRouteRespond
_YOGI_WEAK_SYMBOL int (*YOGI_WebRouteRespond)(void* route, int rid, int finished, int status, const char* contype,
                                              const char* content) =
    Library::get_function_address<int (*)(void* route, int rid, int finished, int status, const char* contype,
                                          const char* content)>("YOGI_WebRouteRespond");

// YOGI_WebProcessCreate
_YOGI_WEAK_SYMBOL int (*YOGI_WebProcessCreate)(void** wproc, void* server, const char* name,
                                               void (*fn)(int res, int wpid, const char* user, int wpa,
                                                          const void* data, const int* sizes, void* userarg),
                                               void* userarg) =
    Library::get_function_address<int (*)(void** wproc, void* server, const char* name,
                                          void (*fn)(int res, int wpid, const char* user, int wpa, const void* data,
                                                     const int* sizes, void* userarg),
                                          void* userarg)>("YOGI_WebProcessCreate");

// YOGI_WebProcessUpdate
_YOGI_WEAK_SYMBOL int (*YOGI_WebProcessUpdate)(void* wproc, int wpid, int wpu, int exitcode, const void* output,
                                               int outsize) =
    Library::get_function_address<int (*)(void* wproc, int wpid, int wpu, int exitcode, const void* output,
                                          int outsize)>("YOGI_WebProcessUpdate");

// :CODEGEN_END:

}  // namespace detail
}  // namespace yogi

#endif  // _YOGI_DETAIL_API_H
