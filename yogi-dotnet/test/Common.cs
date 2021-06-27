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

namespace test
{
    public class TestCase : IDisposable
    {
        public static string helloString = "hello";
        public static IntPtr helloBytes = Marshal.StringToHGlobalAnsi(helloString);
        public static int helloSize = helloString.Length + 1;
        public static string validJsonString = "{\"x\": 5}";
        public static IntPtr validJsonBytes = Marshal.StringToHGlobalAnsi(validJsonString);
        public static int validJsonSize = validJsonString.Length + 1;
        public static IntPtr pointer = new IntPtr(1234);

        public static Yogi.Context CreateContext()
        {
            MOCK_ContextCreate((ref IntPtr context) =>
            {
                context = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            return new Yogi.Context();
        }

        public static Yogi.Configuration CreateConfiguration(
            Yogi.ConfigurationFlags flags = Yogi.ConfigurationFlags.None)
        {
            MOCK_ConfigurationCreate((ref IntPtr config, int flags_) =>
            {
                config = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            return new Yogi.Configuration(flags);
        }

        public TestCase()
        {
            MOCK_Destroy((IntPtr obj) =>
            {
                return (int)Yogi.ErrorCode.Ok;
            });
        }

        public void Dispose()
        {
            MOCK_ResetMocks();

            MOCK_Destroy((IntPtr obj) =>
            {
                return (int)Yogi.ErrorCode.Ok;
            });

            GC.Collect();
        }

        // MOCK_ResetMocks
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ResetMocksDelegate();

        public static ResetMocksDelegate MOCK_ResetMocks
            = Yogi.Library.GetDelegateForFunction<ResetMocksDelegate>("MOCK_ResetMocks");

        // :CODEGEN_BEGIN:

        // MOCK_GetVersion
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr GetVersionDelegate();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void GetVersionMockDelegate(GetVersionDelegate fn);

        internal static GetVersionMockDelegate MOCK_GetVersion
            = Yogi.Library.GetDelegateForFunction<GetVersionMockDelegate>("MOCK_GetVersion");

        // MOCK_CheckBindingsCompatibility
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int CheckBindingsCompatibilityDelegate(string bindver);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void CheckBindingsCompatibilityMockDelegate(CheckBindingsCompatibilityDelegate fn);

        internal static CheckBindingsCompatibilityMockDelegate MOCK_CheckBindingsCompatibility
            = Yogi.Library.GetDelegateForFunction<CheckBindingsCompatibilityMockDelegate>("MOCK_CheckBindingsCompatibility");

        // MOCK_GetErrorString
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr GetErrorStringDelegate(int err);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void GetErrorStringMockDelegate(GetErrorStringDelegate fn);

        internal static GetErrorStringMockDelegate MOCK_GetErrorString
            = Yogi.Library.GetDelegateForFunction<GetErrorStringMockDelegate>("MOCK_GetErrorString");

        // MOCK_GetLastErrorDetails
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr GetLastErrorDetailsDelegate();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void GetLastErrorDetailsMockDelegate(GetLastErrorDetailsDelegate fn);

        internal static GetLastErrorDetailsMockDelegate MOCK_GetLastErrorDetails
            = Yogi.Library.GetDelegateForFunction<GetLastErrorDetailsMockDelegate>("MOCK_GetLastErrorDetails");

        // MOCK_GetConstant
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int GetConstantDelegate(ref IntPtr dest, int constant);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void GetConstantMockDelegate(GetConstantDelegate fn);

        internal static GetConstantMockDelegate MOCK_GetConstant
            = Yogi.Library.GetDelegateForFunction<GetConstantMockDelegate>("MOCK_GetConstant");

        // MOCK_GetSchema
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr GetSchemaDelegate(int schema);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void GetSchemaMockDelegate(GetSchemaDelegate fn);

        internal static GetSchemaMockDelegate MOCK_GetSchema
            = Yogi.Library.GetDelegateForFunction<GetSchemaMockDelegate>("MOCK_GetSchema");

        // MOCK_GetCurrentTime
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int GetCurrentTimeDelegate(ref long timestamp);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void GetCurrentTimeMockDelegate(GetCurrentTimeDelegate fn);

        internal static GetCurrentTimeMockDelegate MOCK_GetCurrentTime
            = Yogi.Library.GetDelegateForFunction<GetCurrentTimeMockDelegate>("MOCK_GetCurrentTime");

        // MOCK_FormatTime
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int FormatTimeDelegate(long timestamp, ref IntPtr str, ref int strsize, string timefmt);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void FormatTimeMockDelegate(FormatTimeDelegate fn);

        internal static FormatTimeMockDelegate MOCK_FormatTime
            = Yogi.Library.GetDelegateForFunction<FormatTimeMockDelegate>("MOCK_FormatTime");

        // MOCK_ParseTime
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ParseTimeDelegate(ref long timestamp, string str, string timefmt);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ParseTimeMockDelegate(ParseTimeDelegate fn);

        internal static ParseTimeMockDelegate MOCK_ParseTime
            = Yogi.Library.GetDelegateForFunction<ParseTimeMockDelegate>("MOCK_ParseTime");

        // MOCK_FormatDuration
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int FormatDurationDelegate(long dur, int neg, ref IntPtr str, ref int strsize, string durfmt, string inffmt);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void FormatDurationMockDelegate(FormatDurationDelegate fn);

        internal static FormatDurationMockDelegate MOCK_FormatDuration
            = Yogi.Library.GetDelegateForFunction<FormatDurationMockDelegate>("MOCK_FormatDuration");

        // MOCK_FormatObject
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int FormatObjectDelegate(IntPtr obj, ref IntPtr str, ref int strsize, string objfmt, string nullstr);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void FormatObjectMockDelegate(FormatObjectDelegate fn);

        internal static FormatObjectMockDelegate MOCK_FormatObject
            = Yogi.Library.GetDelegateForFunction<FormatObjectMockDelegate>("MOCK_FormatObject");

        // MOCK_ConfigureConsoleLogging
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigureConsoleLoggingDelegate(int verbosity, int stream, int color, string timefmt, string fmt);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigureConsoleLoggingMockDelegate(ConfigureConsoleLoggingDelegate fn);

        internal static ConfigureConsoleLoggingMockDelegate MOCK_ConfigureConsoleLogging
            = Yogi.Library.GetDelegateForFunction<ConfigureConsoleLoggingMockDelegate>("MOCK_ConfigureConsoleLogging");

        // MOCK_ConfigureHookLogging
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ConfigureHookLoggingFnDelegate(int severity, long timestamp, int tid, string file, int line, string comp, string msg, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigureHookLoggingDelegate(int verbosity, ConfigureHookLoggingFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigureHookLoggingMockDelegate(ConfigureHookLoggingDelegate fn);

        internal static ConfigureHookLoggingMockDelegate MOCK_ConfigureHookLogging
            = Yogi.Library.GetDelegateForFunction<ConfigureHookLoggingMockDelegate>("MOCK_ConfigureHookLogging");

        // MOCK_ConfigureFileLogging
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigureFileLoggingDelegate(int verbosity, string filename, ref IntPtr genfn, ref int genfnsize, string timefmt, string fmt);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigureFileLoggingMockDelegate(ConfigureFileLoggingDelegate fn);

        internal static ConfigureFileLoggingMockDelegate MOCK_ConfigureFileLogging
            = Yogi.Library.GetDelegateForFunction<ConfigureFileLoggingMockDelegate>("MOCK_ConfigureFileLogging");

        // MOCK_LoggerCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerCreateDelegate(ref IntPtr logger, string component);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void LoggerCreateMockDelegate(LoggerCreateDelegate fn);

        internal static LoggerCreateMockDelegate MOCK_LoggerCreate
            = Yogi.Library.GetDelegateForFunction<LoggerCreateMockDelegate>("MOCK_LoggerCreate");

        // MOCK_LoggerGetVerbosity
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerGetVerbosityDelegate(IntPtr logger, ref int verbosity);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void LoggerGetVerbosityMockDelegate(LoggerGetVerbosityDelegate fn);

        internal static LoggerGetVerbosityMockDelegate MOCK_LoggerGetVerbosity
            = Yogi.Library.GetDelegateForFunction<LoggerGetVerbosityMockDelegate>("MOCK_LoggerGetVerbosity");

        // MOCK_LoggerSetVerbosity
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerSetVerbosityDelegate(IntPtr logger, int verbosity);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void LoggerSetVerbosityMockDelegate(LoggerSetVerbosityDelegate fn);

        internal static LoggerSetVerbosityMockDelegate MOCK_LoggerSetVerbosity
            = Yogi.Library.GetDelegateForFunction<LoggerSetVerbosityMockDelegate>("MOCK_LoggerSetVerbosity");

        // MOCK_LoggerSetComponentsVerbosity
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerSetComponentsVerbosityDelegate(string components, int verbosity, ref int count);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void LoggerSetComponentsVerbosityMockDelegate(LoggerSetComponentsVerbosityDelegate fn);

        internal static LoggerSetComponentsVerbosityMockDelegate MOCK_LoggerSetComponentsVerbosity
            = Yogi.Library.GetDelegateForFunction<LoggerSetComponentsVerbosityMockDelegate>("MOCK_LoggerSetComponentsVerbosity");

        // MOCK_LoggerLog
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int LoggerLogDelegate(IntPtr logger, int severity, string file, int line, string msg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void LoggerLogMockDelegate(LoggerLogDelegate fn);

        internal static LoggerLogMockDelegate MOCK_LoggerLog
            = Yogi.Library.GetDelegateForFunction<LoggerLogMockDelegate>("MOCK_LoggerLog");

        // MOCK_ConfigurationCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationCreateDelegate(ref IntPtr config, int flags);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigurationCreateMockDelegate(ConfigurationCreateDelegate fn);

        internal static ConfigurationCreateMockDelegate MOCK_ConfigurationCreate
            = Yogi.Library.GetDelegateForFunction<ConfigurationCreateMockDelegate>("MOCK_ConfigurationCreate");

        // MOCK_ConfigurationUpdateFromCommandLine
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationUpdateFromCommandLineDelegate(IntPtr config, int argc, IntPtr argv, int options);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigurationUpdateFromCommandLineMockDelegate(ConfigurationUpdateFromCommandLineDelegate fn);

        internal static ConfigurationUpdateFromCommandLineMockDelegate MOCK_ConfigurationUpdateFromCommandLine
            = Yogi.Library.GetDelegateForFunction<ConfigurationUpdateFromCommandLineMockDelegate>("MOCK_ConfigurationUpdateFromCommandLine");

        // MOCK_ConfigurationUpdateFromJson
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationUpdateFromJsonDelegate(IntPtr config, IntPtr json);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigurationUpdateFromJsonMockDelegate(ConfigurationUpdateFromJsonDelegate fn);

        internal static ConfigurationUpdateFromJsonMockDelegate MOCK_ConfigurationUpdateFromJson
            = Yogi.Library.GetDelegateForFunction<ConfigurationUpdateFromJsonMockDelegate>("MOCK_ConfigurationUpdateFromJson");

        // MOCK_ConfigurationUpdateFromFile
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationUpdateFromFileDelegate(IntPtr config, string filename);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigurationUpdateFromFileMockDelegate(ConfigurationUpdateFromFileDelegate fn);

        internal static ConfigurationUpdateFromFileMockDelegate MOCK_ConfigurationUpdateFromFile
            = Yogi.Library.GetDelegateForFunction<ConfigurationUpdateFromFileMockDelegate>("MOCK_ConfigurationUpdateFromFile");

        // MOCK_ConfigurationDump
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationDumpDelegate(IntPtr config, ref IntPtr json, ref int jsonsize, int resvars, int indent);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigurationDumpMockDelegate(ConfigurationDumpDelegate fn);

        internal static ConfigurationDumpMockDelegate MOCK_ConfigurationDump
            = Yogi.Library.GetDelegateForFunction<ConfigurationDumpMockDelegate>("MOCK_ConfigurationDump");

        // MOCK_ConfigurationWriteToFile
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationWriteToFileDelegate(IntPtr config, string filename, int resvars, int indent);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigurationWriteToFileMockDelegate(ConfigurationWriteToFileDelegate fn);

        internal static ConfigurationWriteToFileMockDelegate MOCK_ConfigurationWriteToFile
            = Yogi.Library.GetDelegateForFunction<ConfigurationWriteToFileMockDelegate>("MOCK_ConfigurationWriteToFile");

        // MOCK_ConfigurationValidate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ConfigurationValidateDelegate(IntPtr config, string section, IntPtr schema);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ConfigurationValidateMockDelegate(ConfigurationValidateDelegate fn);

        internal static ConfigurationValidateMockDelegate MOCK_ConfigurationValidate
            = Yogi.Library.GetDelegateForFunction<ConfigurationValidateMockDelegate>("MOCK_ConfigurationValidate");

        // MOCK_ContextCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextCreateDelegate(ref IntPtr context);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextCreateMockDelegate(ContextCreateDelegate fn);

        internal static ContextCreateMockDelegate MOCK_ContextCreate
            = Yogi.Library.GetDelegateForFunction<ContextCreateMockDelegate>("MOCK_ContextCreate");

        // MOCK_ContextPoll
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextPollDelegate(IntPtr context, ref int count);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextPollMockDelegate(ContextPollDelegate fn);

        internal static ContextPollMockDelegate MOCK_ContextPoll
            = Yogi.Library.GetDelegateForFunction<ContextPollMockDelegate>("MOCK_ContextPoll");

        // MOCK_ContextPollOne
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextPollOneDelegate(IntPtr context, ref int count);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextPollOneMockDelegate(ContextPollOneDelegate fn);

        internal static ContextPollOneMockDelegate MOCK_ContextPollOne
            = Yogi.Library.GetDelegateForFunction<ContextPollOneMockDelegate>("MOCK_ContextPollOne");

        // MOCK_ContextRun
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextRunDelegate(IntPtr context, ref int count, long duration);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextRunMockDelegate(ContextRunDelegate fn);

        internal static ContextRunMockDelegate MOCK_ContextRun
            = Yogi.Library.GetDelegateForFunction<ContextRunMockDelegate>("MOCK_ContextRun");

        // MOCK_ContextRunOne
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextRunOneDelegate(IntPtr context, ref int count, long duration);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextRunOneMockDelegate(ContextRunOneDelegate fn);

        internal static ContextRunOneMockDelegate MOCK_ContextRunOne
            = Yogi.Library.GetDelegateForFunction<ContextRunOneMockDelegate>("MOCK_ContextRunOne");

        // MOCK_ContextRunInBackground
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextRunInBackgroundDelegate(IntPtr context);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextRunInBackgroundMockDelegate(ContextRunInBackgroundDelegate fn);

        internal static ContextRunInBackgroundMockDelegate MOCK_ContextRunInBackground
            = Yogi.Library.GetDelegateForFunction<ContextRunInBackgroundMockDelegate>("MOCK_ContextRunInBackground");

        // MOCK_ContextStop
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextStopDelegate(IntPtr context);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextStopMockDelegate(ContextStopDelegate fn);

        internal static ContextStopMockDelegate MOCK_ContextStop
            = Yogi.Library.GetDelegateForFunction<ContextStopMockDelegate>("MOCK_ContextStop");

        // MOCK_ContextWaitForRunning
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextWaitForRunningDelegate(IntPtr context, long duration);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextWaitForRunningMockDelegate(ContextWaitForRunningDelegate fn);

        internal static ContextWaitForRunningMockDelegate MOCK_ContextWaitForRunning
            = Yogi.Library.GetDelegateForFunction<ContextWaitForRunningMockDelegate>("MOCK_ContextWaitForRunning");

        // MOCK_ContextWaitForStopped
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextWaitForStoppedDelegate(IntPtr context, long duration);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextWaitForStoppedMockDelegate(ContextWaitForStoppedDelegate fn);

        internal static ContextWaitForStoppedMockDelegate MOCK_ContextWaitForStopped
            = Yogi.Library.GetDelegateForFunction<ContextWaitForStoppedMockDelegate>("MOCK_ContextWaitForStopped");

        // MOCK_ContextPost
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ContextPostFnDelegate(IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ContextPostDelegate(IntPtr context, ContextPostFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void ContextPostMockDelegate(ContextPostDelegate fn);

        internal static ContextPostMockDelegate MOCK_ContextPost
            = Yogi.Library.GetDelegateForFunction<ContextPostMockDelegate>("MOCK_ContextPost");

        // MOCK_RaiseSignal
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void RaiseSignalFnDelegate(IntPtr sigarg, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int RaiseSignalDelegate(int signal, IntPtr sigarg, RaiseSignalFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void RaiseSignalMockDelegate(RaiseSignalDelegate fn);

        internal static RaiseSignalMockDelegate MOCK_RaiseSignal
            = Yogi.Library.GetDelegateForFunction<RaiseSignalMockDelegate>("MOCK_RaiseSignal");

        // MOCK_SignalSetCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int SignalSetCreateDelegate(ref IntPtr sigset, IntPtr context, int signals);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void SignalSetCreateMockDelegate(SignalSetCreateDelegate fn);

        internal static SignalSetCreateMockDelegate MOCK_SignalSetCreate
            = Yogi.Library.GetDelegateForFunction<SignalSetCreateMockDelegate>("MOCK_SignalSetCreate");

        // MOCK_SignalSetAwaitSignalAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void SignalSetAwaitSignalAsyncFnDelegate(int res, int sig, IntPtr sigarg, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int SignalSetAwaitSignalAsyncDelegate(IntPtr sigset, SignalSetAwaitSignalAsyncFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void SignalSetAwaitSignalAsyncMockDelegate(SignalSetAwaitSignalAsyncDelegate fn);

        internal static SignalSetAwaitSignalAsyncMockDelegate MOCK_SignalSetAwaitSignalAsync
            = Yogi.Library.GetDelegateForFunction<SignalSetAwaitSignalAsyncMockDelegate>("MOCK_SignalSetAwaitSignalAsync");

        // MOCK_SignalSetCancelAwaitSignal
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int SignalSetCancelAwaitSignalDelegate(IntPtr sigset);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void SignalSetCancelAwaitSignalMockDelegate(SignalSetCancelAwaitSignalDelegate fn);

        internal static SignalSetCancelAwaitSignalMockDelegate MOCK_SignalSetCancelAwaitSignal
            = Yogi.Library.GetDelegateForFunction<SignalSetCancelAwaitSignalMockDelegate>("MOCK_SignalSetCancelAwaitSignal");

        // MOCK_TimerCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int TimerCreateDelegate(ref IntPtr timer, IntPtr context);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void TimerCreateMockDelegate(TimerCreateDelegate fn);

        internal static TimerCreateMockDelegate MOCK_TimerCreate
            = Yogi.Library.GetDelegateForFunction<TimerCreateMockDelegate>("MOCK_TimerCreate");

        // MOCK_TimerStartAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void TimerStartAsyncFnDelegate(int res, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int TimerStartAsyncDelegate(IntPtr timer, long duration, TimerStartAsyncFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void TimerStartAsyncMockDelegate(TimerStartAsyncDelegate fn);

        internal static TimerStartAsyncMockDelegate MOCK_TimerStartAsync
            = Yogi.Library.GetDelegateForFunction<TimerStartAsyncMockDelegate>("MOCK_TimerStartAsync");

        // MOCK_TimerCancel
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int TimerCancelDelegate(IntPtr timer);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void TimerCancelMockDelegate(TimerCancelDelegate fn);

        internal static TimerCancelMockDelegate MOCK_TimerCancel
            = Yogi.Library.GetDelegateForFunction<TimerCancelMockDelegate>("MOCK_TimerCancel");

        // MOCK_BranchCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchCreateDelegate(ref IntPtr branch, IntPtr context, IntPtr config, string section);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchCreateMockDelegate(BranchCreateDelegate fn);

        internal static BranchCreateMockDelegate MOCK_BranchCreate
            = Yogi.Library.GetDelegateForFunction<BranchCreateMockDelegate>("MOCK_BranchCreate");

        // MOCK_BranchGetInfo
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchGetInfoDelegate(IntPtr branch, ref IntPtr uuid, ref IntPtr json, ref int jsonsize);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchGetInfoMockDelegate(BranchGetInfoDelegate fn);

        internal static BranchGetInfoMockDelegate MOCK_BranchGetInfo
            = Yogi.Library.GetDelegateForFunction<BranchGetInfoMockDelegate>("MOCK_BranchGetInfo");

        // MOCK_BranchGetConnectedBranches
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchGetConnectedBranchesDelegate(IntPtr branch, ref IntPtr uuids, ref int numuuids, ref IntPtr json, ref int jsonsize);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchGetConnectedBranchesMockDelegate(BranchGetConnectedBranchesDelegate fn);

        internal static BranchGetConnectedBranchesMockDelegate MOCK_BranchGetConnectedBranches
            = Yogi.Library.GetDelegateForFunction<BranchGetConnectedBranchesMockDelegate>("MOCK_BranchGetConnectedBranches");

        // MOCK_BranchAwaitEventAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BranchAwaitEventAsyncFnDelegate(int res, int ev, int evres, IntPtr uuid, string json, int jsonsize, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchAwaitEventAsyncDelegate(IntPtr branch, int events, BranchAwaitEventAsyncFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchAwaitEventAsyncMockDelegate(BranchAwaitEventAsyncDelegate fn);

        internal static BranchAwaitEventAsyncMockDelegate MOCK_BranchAwaitEventAsync
            = Yogi.Library.GetDelegateForFunction<BranchAwaitEventAsyncMockDelegate>("MOCK_BranchAwaitEventAsync");

        // MOCK_BranchCancelAwaitEvent
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchCancelAwaitEventDelegate(IntPtr branch);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchCancelAwaitEventMockDelegate(BranchCancelAwaitEventDelegate fn);

        internal static BranchCancelAwaitEventMockDelegate MOCK_BranchCancelAwaitEvent
            = Yogi.Library.GetDelegateForFunction<BranchCancelAwaitEventMockDelegate>("MOCK_BranchCancelAwaitEvent");

        // MOCK_BranchSendBroadcast
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchSendBroadcastDelegate(IntPtr branch, int enc, IntPtr data, int datasize, int block);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchSendBroadcastMockDelegate(BranchSendBroadcastDelegate fn);

        internal static BranchSendBroadcastMockDelegate MOCK_BranchSendBroadcast
            = Yogi.Library.GetDelegateForFunction<BranchSendBroadcastMockDelegate>("MOCK_BranchSendBroadcast");

        // MOCK_BranchSendBroadcastAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BranchSendBroadcastAsyncFnDelegate(int res, int oid, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchSendBroadcastAsyncDelegate(IntPtr branch, int enc, IntPtr data, int datasize, int retry, BranchSendBroadcastAsyncFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchSendBroadcastAsyncMockDelegate(BranchSendBroadcastAsyncDelegate fn);

        internal static BranchSendBroadcastAsyncMockDelegate MOCK_BranchSendBroadcastAsync
            = Yogi.Library.GetDelegateForFunction<BranchSendBroadcastAsyncMockDelegate>("MOCK_BranchSendBroadcastAsync");

        // MOCK_BranchCancelSendBroadcast
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchCancelSendBroadcastDelegate(IntPtr branch, int oid);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchCancelSendBroadcastMockDelegate(BranchCancelSendBroadcastDelegate fn);

        internal static BranchCancelSendBroadcastMockDelegate MOCK_BranchCancelSendBroadcast
            = Yogi.Library.GetDelegateForFunction<BranchCancelSendBroadcastMockDelegate>("MOCK_BranchCancelSendBroadcast");

        // MOCK_BranchReceiveBroadcastAsync
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BranchReceiveBroadcastAsyncFnDelegate(int res, int size, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchReceiveBroadcastAsyncDelegate(IntPtr branch, IntPtr uuid, int enc, IntPtr data, int datasize, BranchReceiveBroadcastAsyncFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchReceiveBroadcastAsyncMockDelegate(BranchReceiveBroadcastAsyncDelegate fn);

        internal static BranchReceiveBroadcastAsyncMockDelegate MOCK_BranchReceiveBroadcastAsync
            = Yogi.Library.GetDelegateForFunction<BranchReceiveBroadcastAsyncMockDelegate>("MOCK_BranchReceiveBroadcastAsync");

        // MOCK_BranchCancelReceiveBroadcast
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int BranchCancelReceiveBroadcastDelegate(IntPtr branch);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void BranchCancelReceiveBroadcastMockDelegate(BranchCancelReceiveBroadcastDelegate fn);

        internal static BranchCancelReceiveBroadcastMockDelegate MOCK_BranchCancelReceiveBroadcast
            = Yogi.Library.GetDelegateForFunction<BranchCancelReceiveBroadcastMockDelegate>("MOCK_BranchCancelReceiveBroadcast");

        // MOCK_Destroy
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int DestroyDelegate(IntPtr obj);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void DestroyMockDelegate(DestroyDelegate fn);

        internal static DestroyMockDelegate MOCK_Destroy
            = Yogi.Library.GetDelegateForFunction<DestroyMockDelegate>("MOCK_Destroy");

        // MOCK_DestroyAll
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int DestroyAllDelegate();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void DestroyAllMockDelegate(DestroyAllDelegate fn);

        internal static DestroyAllMockDelegate MOCK_DestroyAll
            = Yogi.Library.GetDelegateForFunction<DestroyAllMockDelegate>("MOCK_DestroyAll");

        // MOCK_WebServerCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebServerCreateDelegate(ref IntPtr server, IntPtr context, IntPtr branch, IntPtr config, string section);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void WebServerCreateMockDelegate(WebServerCreateDelegate fn);

        internal static WebServerCreateMockDelegate MOCK_WebServerCreate
            = Yogi.Library.GetDelegateForFunction<WebServerCreateMockDelegate>("MOCK_WebServerCreate");

        // MOCK_WebServerAddWorker
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebServerAddWorkerDelegate(IntPtr server, IntPtr context);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void WebServerAddWorkerMockDelegate(WebServerAddWorkerDelegate fn);

        internal static WebServerAddWorkerMockDelegate MOCK_WebServerAddWorker
            = Yogi.Library.GetDelegateForFunction<WebServerAddWorkerMockDelegate>("MOCK_WebServerAddWorker");

        // MOCK_WebRouteCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void WebRouteCreateFnDelegate(int res, int rid, string user, string owner, int method, string uri, IntPtr qparams, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebRouteCreateDelegate(ref IntPtr route, IntPtr server, string baseuri, WebRouteCreateFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void WebRouteCreateMockDelegate(WebRouteCreateDelegate fn);

        internal static WebRouteCreateMockDelegate MOCK_WebRouteCreate
            = Yogi.Library.GetDelegateForFunction<WebRouteCreateMockDelegate>("MOCK_WebRouteCreate");

        // MOCK_WebRouteRespond
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebRouteRespondDelegate(IntPtr route, int rid, int finished, int status, string contype, string content);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void WebRouteRespondMockDelegate(WebRouteRespondDelegate fn);

        internal static WebRouteRespondMockDelegate MOCK_WebRouteRespond
            = Yogi.Library.GetDelegateForFunction<WebRouteRespondMockDelegate>("MOCK_WebRouteRespond");

        // MOCK_WebProcessCreate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void WebProcessCreateFnDelegate(int res, int wpid, string user, int wpa, IntPtr data, ref int sizes, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebProcessCreateDelegate(ref IntPtr wproc, IntPtr server, string name, WebProcessCreateFnDelegate fn, IntPtr userarg);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void WebProcessCreateMockDelegate(WebProcessCreateDelegate fn);

        internal static WebProcessCreateMockDelegate MOCK_WebProcessCreate
            = Yogi.Library.GetDelegateForFunction<WebProcessCreateMockDelegate>("MOCK_WebProcessCreate");

        // MOCK_WebProcessUpdate
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int WebProcessUpdateDelegate(IntPtr wproc, int wpid, int wpu, int exitcode, IntPtr output, int outsize);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void WebProcessUpdateMockDelegate(WebProcessUpdateDelegate fn);

        internal static WebProcessUpdateMockDelegate MOCK_WebProcessUpdate
            = Yogi.Library.GetDelegateForFunction<WebProcessUpdateMockDelegate>("MOCK_WebProcessUpdate");

        // :CODEGEN_END:
    }
}
