# This file is part of the Yogi Framework
# https://github.com/yohummus/yogi-framework.
#
# Copyright (c) 2020 Johannes Bergmann.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

import os
import platform
from ctypes import c_void_p, c_char, c_char_p, c_int, c_longlong, CFUNCTYPE, POINTER
from ctypes import cdll, create_string_buffer, sizeof, py_object

from ._errors import api_result_handler

# Determine Yogi Core library path and filename
lib_filename = os.getenv('YOGI_CORE_LIBRARY')
if lib_filename is None:
    lib_filename = {
        'Windows': 'yogi-core.dll',
        'Darwin': 'libyogi-core.dylib',
        'Linux': 'libyogi-core.so',
        'FreeBSD': 'libyogi-core.so',
    }.get(platform.system(), '')

    if not lib_filename:
        raise Exception(platform.system() + ' is not supported')

# Load the library
try:
    yogi_core = cdll.LoadLibrary(lib_filename)
except OSError as e:
    raise ImportError(
        'ERROR: Could not load {}. Make sure the library is in your library search path.'.format(lib_filename)) from e

# Check if the bindings version is compatible with the core version
yogi_core.YOGI_CheckBindingsCompatibility.restype = c_int
yogi_core.YOGI_CheckBindingsCompatibility.argtypes = [c_char_p]

yogi_core.YOGI_GetLastErrorDetails.restype = c_char_p
yogi_core.YOGI_GetLastErrorDetails.argtypes = []

# :CODEGEN_BEGIN:
bindings_version = '0.0.1-alpha'
# :CODEGEN_END:

if yogi_core.YOGI_CheckBindingsCompatibility(bindings_version.encode()) != 0:
    raise ImportError(yogi_core.YOGI_GetLastErrorDetails().decode())

# Setup Yogi Core API function signatures
# :CODEGEN_BEGIN:
yogi_core.YOGI_GetVersion.restype = c_char_p
yogi_core.YOGI_GetVersion.argtypes = []

yogi_core.YOGI_CheckBindingsCompatibility.restype = c_int
yogi_core.YOGI_CheckBindingsCompatibility.argtypes = [c_char_p]

yogi_core.YOGI_GetErrorString.restype = c_char_p
yogi_core.YOGI_GetErrorString.argtypes = [c_int]

yogi_core.YOGI_GetLastErrorDetails.restype = c_char_p
yogi_core.YOGI_GetLastErrorDetails.argtypes = []

yogi_core.YOGI_GetConstant.restype = api_result_handler
yogi_core.YOGI_GetConstant.argtypes = [c_void_p, c_int]

yogi_core.YOGI_GetSchema.restype = c_char_p
yogi_core.YOGI_GetSchema.argtypes = [c_int]

yogi_core.YOGI_GetCurrentTime.restype = api_result_handler
yogi_core.YOGI_GetCurrentTime.argtypes = [POINTER(c_longlong)]

yogi_core.YOGI_FormatTime.restype = api_result_handler
yogi_core.YOGI_FormatTime.argtypes = [c_longlong, POINTER(c_char_p), POINTER(c_int), c_char_p]

yogi_core.YOGI_ParseTime.restype = api_result_handler
yogi_core.YOGI_ParseTime.argtypes = [POINTER(c_longlong), c_char_p, c_char_p]

yogi_core.YOGI_FormatDuration.restype = api_result_handler
yogi_core.YOGI_FormatDuration.argtypes = [c_longlong, c_int, POINTER(c_char_p), POINTER(c_int), c_char_p, c_char_p]

yogi_core.YOGI_FormatObject.restype = api_result_handler
yogi_core.YOGI_FormatObject.argtypes = [c_void_p, POINTER(c_char_p), POINTER(c_int), c_char_p, c_char_p]

yogi_core.YOGI_ConfigureConsoleLogging.restype = api_result_handler
yogi_core.YOGI_ConfigureConsoleLogging.argtypes = [c_int, c_int, c_int, c_char_p, c_char_p]

yogi_core.YOGI_ConfigureHookLogging.restype = api_result_handler
yogi_core.YOGI_ConfigureHookLogging.argtypes = [c_int, CFUNCTYPE(
    None, c_int, c_longlong, c_int, c_char_p, c_int, c_char_p, c_char_p, c_void_p), c_void_p]

yogi_core.YOGI_ConfigureFileLogging.restype = api_result_handler
yogi_core.YOGI_ConfigureFileLogging.argtypes = [c_int, c_char_p, POINTER(c_char_p), POINTER(c_int), c_char_p, c_char_p]

yogi_core.YOGI_LoggerCreate.restype = api_result_handler
yogi_core.YOGI_LoggerCreate.argtypes = [POINTER(c_void_p), c_char_p]

yogi_core.YOGI_LoggerGetVerbosity.restype = api_result_handler
yogi_core.YOGI_LoggerGetVerbosity.argtypes = [c_void_p, POINTER(c_int)]

yogi_core.YOGI_LoggerSetVerbosity.restype = api_result_handler
yogi_core.YOGI_LoggerSetVerbosity.argtypes = [c_void_p, c_int]

yogi_core.YOGI_LoggerSetComponentsVerbosity.restype = api_result_handler
yogi_core.YOGI_LoggerSetComponentsVerbosity.argtypes = [c_char_p, c_int, POINTER(c_int)]

yogi_core.YOGI_LoggerLog.restype = api_result_handler
yogi_core.YOGI_LoggerLog.argtypes = [c_void_p, c_int, c_char_p, c_int, c_char_p]

yogi_core.YOGI_ConfigurationCreate.restype = api_result_handler
yogi_core.YOGI_ConfigurationCreate.argtypes = [POINTER(c_void_p), c_int]

yogi_core.YOGI_ConfigurationUpdateFromCommandLine.restype = api_result_handler
yogi_core.YOGI_ConfigurationUpdateFromCommandLine.argtypes = [c_void_p, c_int, POINTER(POINTER(c_char)), c_int]

yogi_core.YOGI_ConfigurationUpdateFromJson.restype = api_result_handler
yogi_core.YOGI_ConfigurationUpdateFromJson.argtypes = [c_void_p, c_char_p]

yogi_core.YOGI_ConfigurationUpdateFromFile.restype = api_result_handler
yogi_core.YOGI_ConfigurationUpdateFromFile.argtypes = [c_void_p, c_char_p]

yogi_core.YOGI_ConfigurationDump.restype = api_result_handler
yogi_core.YOGI_ConfigurationDump.argtypes = [c_void_p, POINTER(c_char_p), POINTER(c_int), c_int, c_int]

yogi_core.YOGI_ConfigurationWriteToFile.restype = api_result_handler
yogi_core.YOGI_ConfigurationWriteToFile.argtypes = [c_void_p, c_char_p, c_int, c_int]

yogi_core.YOGI_ConfigurationValidate.restype = api_result_handler
yogi_core.YOGI_ConfigurationValidate.argtypes = [c_void_p, c_char_p, c_void_p]

yogi_core.YOGI_ContextCreate.restype = api_result_handler
yogi_core.YOGI_ContextCreate.argtypes = [POINTER(c_void_p)]

yogi_core.YOGI_ContextPoll.restype = api_result_handler
yogi_core.YOGI_ContextPoll.argtypes = [c_void_p, POINTER(c_int)]

yogi_core.YOGI_ContextPollOne.restype = api_result_handler
yogi_core.YOGI_ContextPollOne.argtypes = [c_void_p, POINTER(c_int)]

yogi_core.YOGI_ContextRun.restype = api_result_handler
yogi_core.YOGI_ContextRun.argtypes = [c_void_p, POINTER(c_int), c_longlong]

yogi_core.YOGI_ContextRunOne.restype = api_result_handler
yogi_core.YOGI_ContextRunOne.argtypes = [c_void_p, POINTER(c_int), c_longlong]

yogi_core.YOGI_ContextRunInBackground.restype = api_result_handler
yogi_core.YOGI_ContextRunInBackground.argtypes = [c_void_p]

yogi_core.YOGI_ContextStop.restype = api_result_handler
yogi_core.YOGI_ContextStop.argtypes = [c_void_p]

yogi_core.YOGI_ContextWaitForRunning.restype = api_result_handler
yogi_core.YOGI_ContextWaitForRunning.argtypes = [c_void_p, c_longlong]

yogi_core.YOGI_ContextWaitForStopped.restype = api_result_handler
yogi_core.YOGI_ContextWaitForStopped.argtypes = [c_void_p, c_longlong]

yogi_core.YOGI_ContextPost.restype = api_result_handler
yogi_core.YOGI_ContextPost.argtypes = [c_void_p, CFUNCTYPE(None, c_void_p), c_void_p]

yogi_core.YOGI_RaiseSignal.restype = api_result_handler
yogi_core.YOGI_RaiseSignal.argtypes = [c_int, py_object, CFUNCTYPE(None, py_object, c_void_p), c_void_p]

yogi_core.YOGI_SignalSetCreate.restype = api_result_handler
yogi_core.YOGI_SignalSetCreate.argtypes = [POINTER(c_void_p), c_void_p, c_int]

yogi_core.YOGI_SignalSetAwaitSignalAsync.restype = api_result_handler
yogi_core.YOGI_SignalSetAwaitSignalAsync.argtypes = [
    c_void_p, CFUNCTYPE(None, c_int, c_int, py_object, c_void_p), c_void_p]

yogi_core.YOGI_SignalSetCancelAwaitSignal.restype = c_int
yogi_core.YOGI_SignalSetCancelAwaitSignal.argtypes = [c_void_p]

yogi_core.YOGI_TimerCreate.restype = api_result_handler
yogi_core.YOGI_TimerCreate.argtypes = [POINTER(c_void_p), c_void_p]

yogi_core.YOGI_TimerStartAsync.restype = api_result_handler
yogi_core.YOGI_TimerStartAsync.argtypes = [c_void_p, c_longlong, CFUNCTYPE(None, c_int, c_void_p), c_void_p]

yogi_core.YOGI_TimerCancel.restype = c_int
yogi_core.YOGI_TimerCancel.argtypes = [c_void_p]

yogi_core.YOGI_BranchCreate.restype = api_result_handler
yogi_core.YOGI_BranchCreate.argtypes = [POINTER(c_void_p), c_void_p, c_void_p, c_char_p]

yogi_core.YOGI_BranchGetInfo.restype = api_result_handler
yogi_core.YOGI_BranchGetInfo.argtypes = [c_void_p, POINTER(c_void_p), POINTER(c_char_p), POINTER(c_int)]

yogi_core.YOGI_BranchGetConnectedBranches.restype = api_result_handler
yogi_core.YOGI_BranchGetConnectedBranches.argtypes = [c_void_p, POINTER(
    c_void_p), POINTER(c_int), POINTER(c_char_p), POINTER(c_int)]

yogi_core.YOGI_BranchAwaitEventAsync.restype = api_result_handler
yogi_core.YOGI_BranchAwaitEventAsync.argtypes = [c_void_p, c_int, c_void_p, POINTER(
    c_char), c_int, CFUNCTYPE(None, c_int, c_int, c_int, c_void_p), c_void_p]

yogi_core.YOGI_BranchCancelAwaitEvent.restype = c_int
yogi_core.YOGI_BranchCancelAwaitEvent.argtypes = [c_void_p]

yogi_core.YOGI_BranchSendBroadcast.restype = c_int
yogi_core.YOGI_BranchSendBroadcast.argtypes = [c_void_p, c_int, c_void_p, c_int, c_int]

yogi_core.YOGI_BranchSendBroadcastAsync.restype = api_result_handler
yogi_core.YOGI_BranchSendBroadcastAsync.argtypes = [
    c_void_p, c_int, c_void_p, c_int, c_int, CFUNCTYPE(None, c_int, c_int, c_void_p), c_void_p]

yogi_core.YOGI_BranchCancelSendBroadcast.restype = c_int
yogi_core.YOGI_BranchCancelSendBroadcast.argtypes = [c_void_p, c_int]

yogi_core.YOGI_BranchReceiveBroadcastAsync.restype = api_result_handler
yogi_core.YOGI_BranchReceiveBroadcastAsync.argtypes = [
    c_void_p, c_void_p, c_int, c_void_p, c_int, CFUNCTYPE(None, c_int, c_int, c_void_p), c_void_p]

yogi_core.YOGI_BranchCancelReceiveBroadcast.restype = c_int
yogi_core.YOGI_BranchCancelReceiveBroadcast.argtypes = [c_void_p]

yogi_core.YOGI_Destroy.restype = api_result_handler
yogi_core.YOGI_Destroy.argtypes = [c_void_p]

yogi_core.YOGI_DestroyAll.restype = api_result_handler
yogi_core.YOGI_DestroyAll.argtypes = []

yogi_core.YOGI_WebServerCreate.restype = api_result_handler
yogi_core.YOGI_WebServerCreate.argtypes = [POINTER(c_void_p), c_void_p, c_void_p, c_void_p, c_char_p]

yogi_core.YOGI_WebServerAddWorker.restype = api_result_handler
yogi_core.YOGI_WebServerAddWorker.argtypes = [c_void_p, c_void_p]

yogi_core.YOGI_WebRouteCreate.restype = api_result_handler
yogi_core.YOGI_WebRouteCreate.argtypes = [POINTER(c_void_p), c_void_p, c_char_p, CFUNCTYPE(
    None, c_int, c_int, c_char_p, c_char_p, c_int, c_char_p, POINTER(POINTER(c_char)), c_void_p), c_void_p]

yogi_core.YOGI_WebRouteRespond.restype = api_result_handler
yogi_core.YOGI_WebRouteRespond.argtypes = [c_void_p, c_int, c_int, c_int, c_char_p, c_char_p]

yogi_core.YOGI_WebProcessCreate.restype = api_result_handler
yogi_core.YOGI_WebProcessCreate.argtypes = [POINTER(c_void_p), c_void_p, c_char_p, CFUNCTYPE(
    None, c_int, c_int, c_char_p, c_int, c_void_p, POINTER(c_int), c_void_p), c_void_p]

yogi_core.YOGI_WebProcessUpdate.restype = api_result_handler
yogi_core.YOGI_WebProcessUpdate.argtypes = [c_void_p, c_int, c_int, c_int, c_void_p, c_int]

# :CODEGEN_END:
