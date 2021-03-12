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

using System;
using System.Text;
using System.Runtime.InteropServices;

public static partial class Yogi
{
    internal class YogiCore
    {
        // :CODEGEN_BEGIN:

        // YOGI_GetVersion
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr GetVersionDelegate();

        public static GetVersionDelegate YOGI_GetVersion
            = Library.GetDelegateForFunction<GetVersionDelegate>("YOGI_GetVersion");

        // YOGI_CheckBindingsCompatibility
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int CheckBindingsCompatibilityDelegate(string bindver);

        public static CheckBindingsCompatibilityDelegate YOGI_CheckBindingsCompatibility
            = Library.GetDelegateForFunction<CheckBindingsCompatibilityDelegate>("YOGI_CheckBindingsCompatibility");

        // YOGI_GetErrorString
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr GetErrorStringDelegate(int err);

        public static GetErrorStringDelegate YOGI_GetErrorString
            = Library.GetDelegateForFunction<GetErrorStringDelegate>("YOGI_GetErrorString");

        // YOGI_GetLastErrorDetails
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr GetLastErrorDetailsDelegate();

        public static GetLastErrorDetailsDelegate YOGI_GetLastErrorDetails
            = Library.GetDelegateForFunction<GetLastErrorDetailsDelegate>("YOGI_GetLastErrorDetails");

        // YOGI_GetConstant
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int GetConstantDelegate(ref IntPtr dest, int constant);

        public static GetConstantDelegate YOGI_GetConstant
            = Library.GetDelegateForFunction<GetConstantDelegate>("YOGI_GetConstant");

        // YOGI_GetSchema
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr GetSchemaDelegate(int schema);

        public static GetSchemaDelegate YOGI_GetSchema
            = Library.GetDelegateForFunction<GetSchemaDelegate>("YOGI_GetSchema");

        // YOGI_GetCurrentTime
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int GetCurrentTimeDelegate(ref long timestamp);

        public static GetCurrentTimeDelegate YOGI_GetCurrentTime
            = Library.GetDelegateForFunction<GetCurrentTimeDelegate>("YOGI_GetCurrentTime");

        // YOGI_FormatTime
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int FormatTimeDelegate(long timestamp, ref IntPtr str, ref int strsize, string timefmt);

        public static FormatTimeDelegate YOGI_FormatTime
            = Library.GetDelegateForFunction<FormatTimeDelegate>("YOGI_FormatTime");

        // YOGI_ParseTime
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ParseTimeDelegate(ref long timestamp, string str, string timefmt);

        public static ParseTimeDelegate YOGI_ParseTime
            = Library.GetDelegateForFunction<ParseTimeDelegate>("YOGI_ParseTime");

        // YOGI_FormatDuration
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int FormatDurationDelegate(long dur, int neg, ref IntPtr str, ref int strsize, string durfmt, string inffmt);

        public static FormatDurationDelegate YOGI_FormatDuration
            = Library.GetDelegateForFunction<FormatDurationDelegate>("YOGI_FormatDuration");

        // YOGI_FormatObject
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int FormatObjectDelegate(SafeHandle obj, ref IntPtr str, ref int strsize, string objfmt, string nullstr);

        public static FormatObjectDelegate YOGI_FormatObject
            = Library.GetDelegateForFunction<FormatObjectDelegate>("YOGI_FormatObject");

        // YOGI_ConfigureConsoleLogging
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigureConsoleLoggingDelegate(int verbosity, int stream, int color, string timefmt, string fmt);

        public static ConfigureConsoleLoggingDelegate YOGI_ConfigureConsoleLogging
            = Library.GetDelegateForFunction<ConfigureConsoleLoggingDelegate>("YOGI_ConfigureConsoleLogging");

        // YOGI_ConfigureHookLogging
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ConfigureHookLoggingFnDelegate(int severity, long timestamp, int tid, string file, int line, string comp, string msg, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigureHookLoggingDelegate(int verbosity, ConfigureHookLoggingFnDelegate fn, IntPtr userarg);

        public static ConfigureHookLoggingDelegate YOGI_ConfigureHookLogging
            = Library.GetDelegateForFunction<ConfigureHookLoggingDelegate>("YOGI_ConfigureHookLogging");

        // YOGI_ConfigureFileLogging
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigureFileLoggingDelegate(int verbosity, string filename, ref IntPtr genfn, ref int genfnsize, string timefmt, string fmt);

        public static ConfigureFileLoggingDelegate YOGI_ConfigureFileLogging
            = Library.GetDelegateForFunction<ConfigureFileLoggingDelegate>("YOGI_ConfigureFileLogging");

        // YOGI_LoggerCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerCreateDelegate(ref IntPtr logger, string component);

        public static LoggerCreateDelegate YOGI_LoggerCreate
            = Library.GetDelegateForFunction<LoggerCreateDelegate>("YOGI_LoggerCreate");

        // YOGI_LoggerGetVerbosity
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerGetVerbosityDelegate(SafeHandle logger, ref int verbosity);

        public static LoggerGetVerbosityDelegate YOGI_LoggerGetVerbosity
            = Library.GetDelegateForFunction<LoggerGetVerbosityDelegate>("YOGI_LoggerGetVerbosity");

        // YOGI_LoggerSetVerbosity
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerSetVerbosityDelegate(SafeHandle logger, int verbosity);

        public static LoggerSetVerbosityDelegate YOGI_LoggerSetVerbosity
            = Library.GetDelegateForFunction<LoggerSetVerbosityDelegate>("YOGI_LoggerSetVerbosity");

        // YOGI_LoggerSetComponentsVerbosity
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerSetComponentsVerbosityDelegate(string components, int verbosity, ref int count);

        public static LoggerSetComponentsVerbosityDelegate YOGI_LoggerSetComponentsVerbosity
            = Library.GetDelegateForFunction<LoggerSetComponentsVerbosityDelegate>("YOGI_LoggerSetComponentsVerbosity");

        // YOGI_LoggerLog
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerLogDelegate(SafeHandle logger, int severity, string file, int line, string msg);

        public static LoggerLogDelegate YOGI_LoggerLog
            = Library.GetDelegateForFunction<LoggerLogDelegate>("YOGI_LoggerLog");

        // YOGI_ConfigurationCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationCreateDelegate(ref IntPtr config, int flags);

        public static ConfigurationCreateDelegate YOGI_ConfigurationCreate
            = Library.GetDelegateForFunction<ConfigurationCreateDelegate>("YOGI_ConfigurationCreate");

        // YOGI_ConfigurationUpdateFromCommandLine
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationUpdateFromCommandLineDelegate(SafeHandle config, int argc, string[] argv, int options);

        public static ConfigurationUpdateFromCommandLineDelegate YOGI_ConfigurationUpdateFromCommandLine
            = Library.GetDelegateForFunction<ConfigurationUpdateFromCommandLineDelegate>("YOGI_ConfigurationUpdateFromCommandLine");

        // YOGI_ConfigurationUpdateFromJson
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationUpdateFromJsonDelegate(SafeHandle config, byte[] json);

        public static ConfigurationUpdateFromJsonDelegate YOGI_ConfigurationUpdateFromJson
            = Library.GetDelegateForFunction<ConfigurationUpdateFromJsonDelegate>("YOGI_ConfigurationUpdateFromJson");

        // YOGI_ConfigurationUpdateFromFile
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationUpdateFromFileDelegate(SafeHandle config, string filename);

        public static ConfigurationUpdateFromFileDelegate YOGI_ConfigurationUpdateFromFile
            = Library.GetDelegateForFunction<ConfigurationUpdateFromFileDelegate>("YOGI_ConfigurationUpdateFromFile");

        // YOGI_ConfigurationDump
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationDumpDelegate(SafeHandle config, ref IntPtr json, ref int jsonsize, int resvars, int indent);

        public static ConfigurationDumpDelegate YOGI_ConfigurationDump
            = Library.GetDelegateForFunction<ConfigurationDumpDelegate>("YOGI_ConfigurationDump");

        // YOGI_ConfigurationWriteToFile
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationWriteToFileDelegate(SafeHandle config, string filename, int resvars, int indent);

        public static ConfigurationWriteToFileDelegate YOGI_ConfigurationWriteToFile
            = Library.GetDelegateForFunction<ConfigurationWriteToFileDelegate>("YOGI_ConfigurationWriteToFile");

        // YOGI_ConfigurationValidate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationValidateDelegate(SafeHandle config, string section, SafeHandle schema);

        public static ConfigurationValidateDelegate YOGI_ConfigurationValidate
            = Library.GetDelegateForFunction<ConfigurationValidateDelegate>("YOGI_ConfigurationValidate");

        // YOGI_ContextCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextCreateDelegate(ref IntPtr context);

        public static ContextCreateDelegate YOGI_ContextCreate
            = Library.GetDelegateForFunction<ContextCreateDelegate>("YOGI_ContextCreate");

        // YOGI_ContextPoll
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextPollDelegate(SafeHandle context, ref int count);

        public static ContextPollDelegate YOGI_ContextPoll
            = Library.GetDelegateForFunction<ContextPollDelegate>("YOGI_ContextPoll");

        // YOGI_ContextPollOne
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextPollOneDelegate(SafeHandle context, ref int count);

        public static ContextPollOneDelegate YOGI_ContextPollOne
            = Library.GetDelegateForFunction<ContextPollOneDelegate>("YOGI_ContextPollOne");

        // YOGI_ContextRun
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextRunDelegate(SafeHandle context, ref int count, long duration);

        public static ContextRunDelegate YOGI_ContextRun
            = Library.GetDelegateForFunction<ContextRunDelegate>("YOGI_ContextRun");

        // YOGI_ContextRunOne
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextRunOneDelegate(SafeHandle context, ref int count, long duration);

        public static ContextRunOneDelegate YOGI_ContextRunOne
            = Library.GetDelegateForFunction<ContextRunOneDelegate>("YOGI_ContextRunOne");

        // YOGI_ContextRunInBackground
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextRunInBackgroundDelegate(SafeHandle context);

        public static ContextRunInBackgroundDelegate YOGI_ContextRunInBackground
            = Library.GetDelegateForFunction<ContextRunInBackgroundDelegate>("YOGI_ContextRunInBackground");

        // YOGI_ContextStop
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextStopDelegate(SafeHandle context);

        public static ContextStopDelegate YOGI_ContextStop
            = Library.GetDelegateForFunction<ContextStopDelegate>("YOGI_ContextStop");

        // YOGI_ContextWaitForRunning
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextWaitForRunningDelegate(SafeHandle context, long duration);

        public static ContextWaitForRunningDelegate YOGI_ContextWaitForRunning
            = Library.GetDelegateForFunction<ContextWaitForRunningDelegate>("YOGI_ContextWaitForRunning");

        // YOGI_ContextWaitForStopped
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextWaitForStoppedDelegate(SafeHandle context, long duration);

        public static ContextWaitForStoppedDelegate YOGI_ContextWaitForStopped
            = Library.GetDelegateForFunction<ContextWaitForStoppedDelegate>("YOGI_ContextWaitForStopped");

        // YOGI_ContextPost
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ContextPostFnDelegate(IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextPostDelegate(SafeHandle context, ContextPostFnDelegate fn, IntPtr userarg);

        public static ContextPostDelegate YOGI_ContextPost
            = Library.GetDelegateForFunction<ContextPostDelegate>("YOGI_ContextPost");

        // YOGI_RaiseSignal
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void RaiseSignalFnDelegate(IntPtr sigarg, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int RaiseSignalDelegate(int signal, IntPtr sigarg, RaiseSignalFnDelegate fn, IntPtr userarg);

        public static RaiseSignalDelegate YOGI_RaiseSignal
            = Library.GetDelegateForFunction<RaiseSignalDelegate>("YOGI_RaiseSignal");

        // YOGI_SignalSetCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int SignalSetCreateDelegate(ref IntPtr sigset, SafeHandle context, int signals);

        public static SignalSetCreateDelegate YOGI_SignalSetCreate
            = Library.GetDelegateForFunction<SignalSetCreateDelegate>("YOGI_SignalSetCreate");

        // YOGI_SignalSetAwaitSignalAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void SignalSetAwaitSignalAsyncFnDelegate(int res, int sig, IntPtr sigarg, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int SignalSetAwaitSignalAsyncDelegate(SafeHandle sigset, SignalSetAwaitSignalAsyncFnDelegate fn, IntPtr userarg);

        public static SignalSetAwaitSignalAsyncDelegate YOGI_SignalSetAwaitSignalAsync
            = Library.GetDelegateForFunction<SignalSetAwaitSignalAsyncDelegate>("YOGI_SignalSetAwaitSignalAsync");

        // YOGI_SignalSetCancelAwaitSignal
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int SignalSetCancelAwaitSignalDelegate(SafeHandle sigset);

        public static SignalSetCancelAwaitSignalDelegate YOGI_SignalSetCancelAwaitSignal
            = Library.GetDelegateForFunction<SignalSetCancelAwaitSignalDelegate>("YOGI_SignalSetCancelAwaitSignal");

        // YOGI_TimerCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int TimerCreateDelegate(ref IntPtr timer, SafeHandle context);

        public static TimerCreateDelegate YOGI_TimerCreate
            = Library.GetDelegateForFunction<TimerCreateDelegate>("YOGI_TimerCreate");

        // YOGI_TimerStartAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void TimerStartAsyncFnDelegate(int res, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int TimerStartAsyncDelegate(SafeHandle timer, long duration, TimerStartAsyncFnDelegate fn, IntPtr userarg);

        public static TimerStartAsyncDelegate YOGI_TimerStartAsync
            = Library.GetDelegateForFunction<TimerStartAsyncDelegate>("YOGI_TimerStartAsync");

        // YOGI_TimerCancel
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int TimerCancelDelegate(SafeHandle timer);

        public static TimerCancelDelegate YOGI_TimerCancel
            = Library.GetDelegateForFunction<TimerCancelDelegate>("YOGI_TimerCancel");

        // YOGI_BranchCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchCreateDelegate(ref IntPtr branch, SafeHandle context, SafeHandle config, string section);

        public static BranchCreateDelegate YOGI_BranchCreate
            = Library.GetDelegateForFunction<BranchCreateDelegate>("YOGI_BranchCreate");

        // YOGI_BranchGetInfo
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchGetInfoDelegate(SafeHandle branch, ref IntPtr uuid, ref IntPtr json, ref int jsonsize);

        public static BranchGetInfoDelegate YOGI_BranchGetInfo
            = Library.GetDelegateForFunction<BranchGetInfoDelegate>("YOGI_BranchGetInfo");

        // YOGI_BranchGetConnectedBranches
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchGetConnectedBranchesDelegate(SafeHandle branch, ref IntPtr uuids, ref int numuuids, ref IntPtr json, ref int jsonsize);

        public static BranchGetConnectedBranchesDelegate YOGI_BranchGetConnectedBranches
            = Library.GetDelegateForFunction<BranchGetConnectedBranchesDelegate>("YOGI_BranchGetConnectedBranches");

        // YOGI_BranchAwaitEventAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BranchAwaitEventAsyncFnDelegate(int res, int ev, int evres, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchAwaitEventAsyncDelegate(SafeHandle branch, int events, IntPtr uuid, StringBuilder json, int jsonsize, BranchAwaitEventAsyncFnDelegate fn, IntPtr userarg);

        public static BranchAwaitEventAsyncDelegate YOGI_BranchAwaitEventAsync
            = Library.GetDelegateForFunction<BranchAwaitEventAsyncDelegate>("YOGI_BranchAwaitEventAsync");

        // YOGI_BranchCancelAwaitEvent
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchCancelAwaitEventDelegate(SafeHandle branch);

        public static BranchCancelAwaitEventDelegate YOGI_BranchCancelAwaitEvent
            = Library.GetDelegateForFunction<BranchCancelAwaitEventDelegate>("YOGI_BranchCancelAwaitEvent");

        // YOGI_BranchSendBroadcast
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchSendBroadcastDelegate(SafeHandle branch, int enc, IntPtr data, int datasize, int block);

        public static BranchSendBroadcastDelegate YOGI_BranchSendBroadcast
            = Library.GetDelegateForFunction<BranchSendBroadcastDelegate>("YOGI_BranchSendBroadcast");

        // YOGI_BranchSendBroadcastAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BranchSendBroadcastAsyncFnDelegate(int res, int oid, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchSendBroadcastAsyncDelegate(SafeHandle branch, int enc, IntPtr data, int datasize, int retry, BranchSendBroadcastAsyncFnDelegate fn, IntPtr userarg);

        public static BranchSendBroadcastAsyncDelegate YOGI_BranchSendBroadcastAsync
            = Library.GetDelegateForFunction<BranchSendBroadcastAsyncDelegate>("YOGI_BranchSendBroadcastAsync");

        // YOGI_BranchCancelSendBroadcast
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchCancelSendBroadcastDelegate(SafeHandle branch, int oid);

        public static BranchCancelSendBroadcastDelegate YOGI_BranchCancelSendBroadcast
            = Library.GetDelegateForFunction<BranchCancelSendBroadcastDelegate>("YOGI_BranchCancelSendBroadcast");

        // YOGI_BranchReceiveBroadcastAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BranchReceiveBroadcastAsyncFnDelegate(int res, int size, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchReceiveBroadcastAsyncDelegate(SafeHandle branch, IntPtr uuid, int enc, IntPtr data, int datasize, BranchReceiveBroadcastAsyncFnDelegate fn, IntPtr userarg);

        public static BranchReceiveBroadcastAsyncDelegate YOGI_BranchReceiveBroadcastAsync
            = Library.GetDelegateForFunction<BranchReceiveBroadcastAsyncDelegate>("YOGI_BranchReceiveBroadcastAsync");

        // YOGI_BranchCancelReceiveBroadcast
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchCancelReceiveBroadcastDelegate(SafeHandle branch);

        public static BranchCancelReceiveBroadcastDelegate YOGI_BranchCancelReceiveBroadcast
            = Library.GetDelegateForFunction<BranchCancelReceiveBroadcastDelegate>("YOGI_BranchCancelReceiveBroadcast");

        // YOGI_Destroy
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int DestroyDelegate(IntPtr obj);

        public static DestroyDelegate YOGI_Destroy
            = Library.GetDelegateForFunction<DestroyDelegate>("YOGI_Destroy");

        // YOGI_DestroyAll
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int DestroyAllDelegate();

        public static DestroyAllDelegate YOGI_DestroyAll
            = Library.GetDelegateForFunction<DestroyAllDelegate>("YOGI_DestroyAll");

        // YOGI_WebServerCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebServerCreateDelegate(ref IntPtr server, SafeHandle context, SafeHandle branch, SafeHandle config, string section);

        public static WebServerCreateDelegate YOGI_WebServerCreate
            = Library.GetDelegateForFunction<WebServerCreateDelegate>("YOGI_WebServerCreate");

        // YOGI_WebServerAddWorker
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebServerAddWorkerDelegate(SafeHandle server, SafeHandle context);

        public static WebServerAddWorkerDelegate YOGI_WebServerAddWorker
            = Library.GetDelegateForFunction<WebServerAddWorkerDelegate>("YOGI_WebServerAddWorker");

        // YOGI_WebRouteCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void WebRouteCreateFnDelegate(int res, int rid, string user, string owner, int method, string uri, string[] qparams, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebRouteCreateDelegate(ref IntPtr route, SafeHandle server, string baseuri, WebRouteCreateFnDelegate fn, IntPtr userarg);

        public static WebRouteCreateDelegate YOGI_WebRouteCreate
            = Library.GetDelegateForFunction<WebRouteCreateDelegate>("YOGI_WebRouteCreate");

        // YOGI_WebRouteRespond
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebRouteRespondDelegate(SafeHandle route, int rid, int finished, int status, string contype, string content);

        public static WebRouteRespondDelegate YOGI_WebRouteRespond
            = Library.GetDelegateForFunction<WebRouteRespondDelegate>("YOGI_WebRouteRespond");

        // YOGI_WebProcessCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void WebProcessCreateFnDelegate(int res, int wpid, string user, int wpa, IntPtr data, ref int sizes, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebProcessCreateDelegate(ref IntPtr wproc, SafeHandle server, string name, WebProcessCreateFnDelegate fn, IntPtr userarg);

        public static WebProcessCreateDelegate YOGI_WebProcessCreate
            = Library.GetDelegateForFunction<WebProcessCreateDelegate>("YOGI_WebProcessCreate");

        // YOGI_WebProcessUpdate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebProcessUpdateDelegate(SafeHandle wproc, int wpid, int wpu, int exitcode, IntPtr output, int outsize);

        public static WebProcessUpdateDelegate YOGI_WebProcessUpdate
            = Library.GetDelegateForFunction<WebProcessUpdateDelegate>("YOGI_WebProcessUpdate");

        // :CODEGEN_END:
    }
}
