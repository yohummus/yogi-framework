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

#include <src/default_functions.h>
#include <yogi_core_mock.h>

#include <functional>
#include <iostream>
#include <mutex>
#include <string>

std::mutex global_mock_mutex;

// :CODEGEN_BEGIN:

// Mock implementation for YOGI_GetVersion
static std::function<decltype(YOGI_GetVersion)> mock_GetVersion_fn = default_GetVersion;

YOGI_API const char* YOGI_GetVersion() {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_GetVersion_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_GetVersion()" << std::endl;
    return "";
  }

  return mock_GetVersion_fn();
}

YOGI_API void MOCK_GetVersion(decltype(YOGI_GetVersion) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_GetVersion_fn = fn ? fn : decltype(mock_GetVersion_fn){};
}

// Mock implementation for YOGI_CheckBindingsCompatibility
static std::function<decltype(YOGI_CheckBindingsCompatibility)> mock_CheckBindingsCompatibility_fn =
    default_CheckBindingsCompatibility;

YOGI_API int YOGI_CheckBindingsCompatibility(const char* bindver) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_CheckBindingsCompatibility_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_CheckBindingsCompatibility()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_CheckBindingsCompatibility_fn(bindver);
}

YOGI_API void MOCK_CheckBindingsCompatibility(decltype(YOGI_CheckBindingsCompatibility) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_CheckBindingsCompatibility_fn = fn ? fn : decltype(mock_CheckBindingsCompatibility_fn){};
}

// Mock implementation for YOGI_GetErrorString
static std::function<decltype(YOGI_GetErrorString)> mock_GetErrorString_fn = {};

YOGI_API const char* YOGI_GetErrorString(int err) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_GetErrorString_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_GetErrorString()" << std::endl;
    return "";
  }

  return mock_GetErrorString_fn(err);
}

YOGI_API void MOCK_GetErrorString(decltype(YOGI_GetErrorString) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_GetErrorString_fn = fn ? fn : decltype(mock_GetErrorString_fn){};
}

// Mock implementation for YOGI_GetLastErrorDetails
static std::function<decltype(YOGI_GetLastErrorDetails)> mock_GetLastErrorDetails_fn = {};

YOGI_API const char* YOGI_GetLastErrorDetails() {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_GetLastErrorDetails_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_GetLastErrorDetails()" << std::endl;
    return "";
  }

  return mock_GetLastErrorDetails_fn();
}

YOGI_API void MOCK_GetLastErrorDetails(decltype(YOGI_GetLastErrorDetails) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_GetLastErrorDetails_fn = fn ? fn : decltype(mock_GetLastErrorDetails_fn){};
}

// Mock implementation for YOGI_GetConstant
static std::function<decltype(YOGI_GetConstant)> mock_GetConstant_fn = default_GetConstant;

YOGI_API int YOGI_GetConstant(void* dest, int constant) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_GetConstant_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_GetConstant()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_GetConstant_fn(dest, constant);
}

YOGI_API void MOCK_GetConstant(decltype(YOGI_GetConstant) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_GetConstant_fn = fn ? fn : decltype(mock_GetConstant_fn){};
}

// Mock implementation for YOGI_GetSchema
static std::function<decltype(YOGI_GetSchema)> mock_GetSchema_fn = {};

YOGI_API const char* YOGI_GetSchema(int schema) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_GetSchema_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_GetSchema()" << std::endl;
    return "";
  }

  return mock_GetSchema_fn(schema);
}

YOGI_API void MOCK_GetSchema(decltype(YOGI_GetSchema) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_GetSchema_fn = fn ? fn : decltype(mock_GetSchema_fn){};
}

// Mock implementation for YOGI_GetCurrentTime
static std::function<decltype(YOGI_GetCurrentTime)> mock_GetCurrentTime_fn = {};

YOGI_API int YOGI_GetCurrentTime(long long* timestamp) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_GetCurrentTime_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_GetCurrentTime()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_GetCurrentTime_fn(timestamp);
}

YOGI_API void MOCK_GetCurrentTime(decltype(YOGI_GetCurrentTime) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_GetCurrentTime_fn = fn ? fn : decltype(mock_GetCurrentTime_fn){};
}

// Mock implementation for YOGI_FormatTime
static std::function<decltype(YOGI_FormatTime)> mock_FormatTime_fn = {};

YOGI_API int YOGI_FormatTime(long long timestamp, const char** str, int* strsize, const char* timefmt) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_FormatTime_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_FormatTime()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_FormatTime_fn(timestamp, str, strsize, timefmt);
}

YOGI_API void MOCK_FormatTime(decltype(YOGI_FormatTime) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_FormatTime_fn = fn ? fn : decltype(mock_FormatTime_fn){};
}

// Mock implementation for YOGI_ParseTime
static std::function<decltype(YOGI_ParseTime)> mock_ParseTime_fn = {};

YOGI_API int YOGI_ParseTime(long long* timestamp, const char* str, const char* timefmt) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ParseTime_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ParseTime()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ParseTime_fn(timestamp, str, timefmt);
}

YOGI_API void MOCK_ParseTime(decltype(YOGI_ParseTime) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ParseTime_fn = fn ? fn : decltype(mock_ParseTime_fn){};
}

// Mock implementation for YOGI_FormatDuration
static std::function<decltype(YOGI_FormatDuration)> mock_FormatDuration_fn = {};

YOGI_API int YOGI_FormatDuration(long long dur, int neg, const char** str, int* strsize, const char* durfmt,
                                 const char* inffmt) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_FormatDuration_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_FormatDuration()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_FormatDuration_fn(dur, neg, str, strsize, durfmt, inffmt);
}

YOGI_API void MOCK_FormatDuration(decltype(YOGI_FormatDuration) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_FormatDuration_fn = fn ? fn : decltype(mock_FormatDuration_fn){};
}

// Mock implementation for YOGI_FormatObject
static std::function<decltype(YOGI_FormatObject)> mock_FormatObject_fn = {};

YOGI_API int YOGI_FormatObject(void* obj, const char** str, int* strsize, const char* objfmt, const char* nullstr) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_FormatObject_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_FormatObject()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_FormatObject_fn(obj, str, strsize, objfmt, nullstr);
}

YOGI_API void MOCK_FormatObject(decltype(YOGI_FormatObject) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_FormatObject_fn = fn ? fn : decltype(mock_FormatObject_fn){};
}

// Mock implementation for YOGI_ConfigureConsoleLogging
static std::function<decltype(YOGI_ConfigureConsoleLogging)> mock_ConfigureConsoleLogging_fn = {};

YOGI_API int YOGI_ConfigureConsoleLogging(int verbosity, int stream, int color, const char* timefmt, const char* fmt) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigureConsoleLogging_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigureConsoleLogging()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigureConsoleLogging_fn(verbosity, stream, color, timefmt, fmt);
}

YOGI_API void MOCK_ConfigureConsoleLogging(decltype(YOGI_ConfigureConsoleLogging) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigureConsoleLogging_fn = fn ? fn : decltype(mock_ConfigureConsoleLogging_fn){};
}

// Mock implementation for YOGI_ConfigureHookLogging
static std::function<decltype(YOGI_ConfigureHookLogging)> mock_ConfigureHookLogging_fn = {};

YOGI_API int YOGI_ConfigureHookLogging(int verbosity,
                                       void (*fn)(int severity, long long timestamp, int tid, const char* file,
                                                  int line, const char* comp, const char* msg, void* userarg),
                                       void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigureHookLogging_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigureHookLogging()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigureHookLogging_fn(verbosity, fn, userarg);
}

YOGI_API void MOCK_ConfigureHookLogging(decltype(YOGI_ConfigureHookLogging) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigureHookLogging_fn = fn ? fn : decltype(mock_ConfigureHookLogging_fn){};
}

// Mock implementation for YOGI_ConfigureFileLogging
static std::function<decltype(YOGI_ConfigureFileLogging)> mock_ConfigureFileLogging_fn = {};

YOGI_API int YOGI_ConfigureFileLogging(int verbosity, const char* filename, const char** genfn, int* genfnsize,
                                       const char* timefmt, const char* fmt) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigureFileLogging_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigureFileLogging()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigureFileLogging_fn(verbosity, filename, genfn, genfnsize, timefmt, fmt);
}

YOGI_API void MOCK_ConfigureFileLogging(decltype(YOGI_ConfigureFileLogging) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigureFileLogging_fn = fn ? fn : decltype(mock_ConfigureFileLogging_fn){};
}

// Mock implementation for YOGI_LoggerCreate
static std::function<decltype(YOGI_LoggerCreate)> mock_LoggerCreate_fn = {};

YOGI_API int YOGI_LoggerCreate(void** logger, const char* component) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_LoggerCreate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_LoggerCreate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_LoggerCreate_fn(logger, component);
}

YOGI_API void MOCK_LoggerCreate(decltype(YOGI_LoggerCreate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_LoggerCreate_fn = fn ? fn : decltype(mock_LoggerCreate_fn){};
}

// Mock implementation for YOGI_LoggerGetVerbosity
static std::function<decltype(YOGI_LoggerGetVerbosity)> mock_LoggerGetVerbosity_fn = {};

YOGI_API int YOGI_LoggerGetVerbosity(void* logger, int* verbosity) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_LoggerGetVerbosity_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_LoggerGetVerbosity()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_LoggerGetVerbosity_fn(logger, verbosity);
}

YOGI_API void MOCK_LoggerGetVerbosity(decltype(YOGI_LoggerGetVerbosity) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_LoggerGetVerbosity_fn = fn ? fn : decltype(mock_LoggerGetVerbosity_fn){};
}

// Mock implementation for YOGI_LoggerSetVerbosity
static std::function<decltype(YOGI_LoggerSetVerbosity)> mock_LoggerSetVerbosity_fn = {};

YOGI_API int YOGI_LoggerSetVerbosity(void* logger, int verbosity) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_LoggerSetVerbosity_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_LoggerSetVerbosity()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_LoggerSetVerbosity_fn(logger, verbosity);
}

YOGI_API void MOCK_LoggerSetVerbosity(decltype(YOGI_LoggerSetVerbosity) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_LoggerSetVerbosity_fn = fn ? fn : decltype(mock_LoggerSetVerbosity_fn){};
}

// Mock implementation for YOGI_LoggerSetComponentsVerbosity
static std::function<decltype(YOGI_LoggerSetComponentsVerbosity)> mock_LoggerSetComponentsVerbosity_fn = {};

YOGI_API int YOGI_LoggerSetComponentsVerbosity(const char* components, int verbosity, int* count) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_LoggerSetComponentsVerbosity_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_LoggerSetComponentsVerbosity()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_LoggerSetComponentsVerbosity_fn(components, verbosity, count);
}

YOGI_API void MOCK_LoggerSetComponentsVerbosity(decltype(YOGI_LoggerSetComponentsVerbosity) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_LoggerSetComponentsVerbosity_fn = fn ? fn : decltype(mock_LoggerSetComponentsVerbosity_fn){};
}

// Mock implementation for YOGI_LoggerLog
static std::function<decltype(YOGI_LoggerLog)> mock_LoggerLog_fn = {};

YOGI_API int YOGI_LoggerLog(void* logger, int severity, const char* file, int line, const char* msg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_LoggerLog_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_LoggerLog()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_LoggerLog_fn(logger, severity, file, line, msg);
}

YOGI_API void MOCK_LoggerLog(decltype(YOGI_LoggerLog) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_LoggerLog_fn = fn ? fn : decltype(mock_LoggerLog_fn){};
}

// Mock implementation for YOGI_ConfigurationCreate
static std::function<decltype(YOGI_ConfigurationCreate)> mock_ConfigurationCreate_fn = {};

YOGI_API int YOGI_ConfigurationCreate(void** config, int flags) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigurationCreate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigurationCreate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigurationCreate_fn(config, flags);
}

YOGI_API void MOCK_ConfigurationCreate(decltype(YOGI_ConfigurationCreate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigurationCreate_fn = fn ? fn : decltype(mock_ConfigurationCreate_fn){};
}

// Mock implementation for YOGI_ConfigurationUpdateFromCommandLine
static std::function<decltype(YOGI_ConfigurationUpdateFromCommandLine)> mock_ConfigurationUpdateFromCommandLine_fn = {};

YOGI_API int YOGI_ConfigurationUpdateFromCommandLine(void* config, int argc, const char* const* argv, int options) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigurationUpdateFromCommandLine_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigurationUpdateFromCommandLine()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigurationUpdateFromCommandLine_fn(config, argc, argv, options);
}

YOGI_API void MOCK_ConfigurationUpdateFromCommandLine(decltype(YOGI_ConfigurationUpdateFromCommandLine) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigurationUpdateFromCommandLine_fn = fn ? fn : decltype(mock_ConfigurationUpdateFromCommandLine_fn){};
}

// Mock implementation for YOGI_ConfigurationUpdateFromJson
static std::function<decltype(YOGI_ConfigurationUpdateFromJson)> mock_ConfigurationUpdateFromJson_fn = {};

YOGI_API int YOGI_ConfigurationUpdateFromJson(void* config, const char* json) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigurationUpdateFromJson_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigurationUpdateFromJson()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigurationUpdateFromJson_fn(config, json);
}

YOGI_API void MOCK_ConfigurationUpdateFromJson(decltype(YOGI_ConfigurationUpdateFromJson) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigurationUpdateFromJson_fn = fn ? fn : decltype(mock_ConfigurationUpdateFromJson_fn){};
}

// Mock implementation for YOGI_ConfigurationUpdateFromFile
static std::function<decltype(YOGI_ConfigurationUpdateFromFile)> mock_ConfigurationUpdateFromFile_fn = {};

YOGI_API int YOGI_ConfigurationUpdateFromFile(void* config, const char* filename) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigurationUpdateFromFile_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigurationUpdateFromFile()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigurationUpdateFromFile_fn(config, filename);
}

YOGI_API void MOCK_ConfigurationUpdateFromFile(decltype(YOGI_ConfigurationUpdateFromFile) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigurationUpdateFromFile_fn = fn ? fn : decltype(mock_ConfigurationUpdateFromFile_fn){};
}

// Mock implementation for YOGI_ConfigurationDump
static std::function<decltype(YOGI_ConfigurationDump)> mock_ConfigurationDump_fn = {};

YOGI_API int YOGI_ConfigurationDump(void* config, const char** json, int* jsonsize, int resvars, int indent) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigurationDump_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigurationDump()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigurationDump_fn(config, json, jsonsize, resvars, indent);
}

YOGI_API void MOCK_ConfigurationDump(decltype(YOGI_ConfigurationDump) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigurationDump_fn = fn ? fn : decltype(mock_ConfigurationDump_fn){};
}

// Mock implementation for YOGI_ConfigurationWriteToFile
static std::function<decltype(YOGI_ConfigurationWriteToFile)> mock_ConfigurationWriteToFile_fn = {};

YOGI_API int YOGI_ConfigurationWriteToFile(void* config, const char* filename, int resvars, int indent) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigurationWriteToFile_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigurationWriteToFile()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigurationWriteToFile_fn(config, filename, resvars, indent);
}

YOGI_API void MOCK_ConfigurationWriteToFile(decltype(YOGI_ConfigurationWriteToFile) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigurationWriteToFile_fn = fn ? fn : decltype(mock_ConfigurationWriteToFile_fn){};
}

// Mock implementation for YOGI_ConfigurationValidate
static std::function<decltype(YOGI_ConfigurationValidate)> mock_ConfigurationValidate_fn = {};

YOGI_API int YOGI_ConfigurationValidate(void* config, const char* section, void* schema) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ConfigurationValidate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ConfigurationValidate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ConfigurationValidate_fn(config, section, schema);
}

YOGI_API void MOCK_ConfigurationValidate(decltype(YOGI_ConfigurationValidate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ConfigurationValidate_fn = fn ? fn : decltype(mock_ConfigurationValidate_fn){};
}

// Mock implementation for YOGI_ContextCreate
static std::function<decltype(YOGI_ContextCreate)> mock_ContextCreate_fn = {};

YOGI_API int YOGI_ContextCreate(void** context) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextCreate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextCreate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextCreate_fn(context);
}

YOGI_API void MOCK_ContextCreate(decltype(YOGI_ContextCreate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextCreate_fn = fn ? fn : decltype(mock_ContextCreate_fn){};
}

// Mock implementation for YOGI_ContextPoll
static std::function<decltype(YOGI_ContextPoll)> mock_ContextPoll_fn = {};

YOGI_API int YOGI_ContextPoll(void* context, int* count) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextPoll_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextPoll()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextPoll_fn(context, count);
}

YOGI_API void MOCK_ContextPoll(decltype(YOGI_ContextPoll) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextPoll_fn = fn ? fn : decltype(mock_ContextPoll_fn){};
}

// Mock implementation for YOGI_ContextPollOne
static std::function<decltype(YOGI_ContextPollOne)> mock_ContextPollOne_fn = {};

YOGI_API int YOGI_ContextPollOne(void* context, int* count) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextPollOne_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextPollOne()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextPollOne_fn(context, count);
}

YOGI_API void MOCK_ContextPollOne(decltype(YOGI_ContextPollOne) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextPollOne_fn = fn ? fn : decltype(mock_ContextPollOne_fn){};
}

// Mock implementation for YOGI_ContextRun
static std::function<decltype(YOGI_ContextRun)> mock_ContextRun_fn = {};

YOGI_API int YOGI_ContextRun(void* context, int* count, long long duration) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextRun_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextRun()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextRun_fn(context, count, duration);
}

YOGI_API void MOCK_ContextRun(decltype(YOGI_ContextRun) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextRun_fn = fn ? fn : decltype(mock_ContextRun_fn){};
}

// Mock implementation for YOGI_ContextRunOne
static std::function<decltype(YOGI_ContextRunOne)> mock_ContextRunOne_fn = {};

YOGI_API int YOGI_ContextRunOne(void* context, int* count, long long duration) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextRunOne_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextRunOne()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextRunOne_fn(context, count, duration);
}

YOGI_API void MOCK_ContextRunOne(decltype(YOGI_ContextRunOne) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextRunOne_fn = fn ? fn : decltype(mock_ContextRunOne_fn){};
}

// Mock implementation for YOGI_ContextRunInBackground
static std::function<decltype(YOGI_ContextRunInBackground)> mock_ContextRunInBackground_fn = {};

YOGI_API int YOGI_ContextRunInBackground(void* context) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextRunInBackground_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextRunInBackground()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextRunInBackground_fn(context);
}

YOGI_API void MOCK_ContextRunInBackground(decltype(YOGI_ContextRunInBackground) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextRunInBackground_fn = fn ? fn : decltype(mock_ContextRunInBackground_fn){};
}

// Mock implementation for YOGI_ContextStop
static std::function<decltype(YOGI_ContextStop)> mock_ContextStop_fn = {};

YOGI_API int YOGI_ContextStop(void* context) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextStop_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextStop()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextStop_fn(context);
}

YOGI_API void MOCK_ContextStop(decltype(YOGI_ContextStop) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextStop_fn = fn ? fn : decltype(mock_ContextStop_fn){};
}

// Mock implementation for YOGI_ContextWaitForRunning
static std::function<decltype(YOGI_ContextWaitForRunning)> mock_ContextWaitForRunning_fn = {};

YOGI_API int YOGI_ContextWaitForRunning(void* context, long long duration) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextWaitForRunning_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextWaitForRunning()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextWaitForRunning_fn(context, duration);
}

YOGI_API void MOCK_ContextWaitForRunning(decltype(YOGI_ContextWaitForRunning) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextWaitForRunning_fn = fn ? fn : decltype(mock_ContextWaitForRunning_fn){};
}

// Mock implementation for YOGI_ContextWaitForStopped
static std::function<decltype(YOGI_ContextWaitForStopped)> mock_ContextWaitForStopped_fn = {};

YOGI_API int YOGI_ContextWaitForStopped(void* context, long long duration) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextWaitForStopped_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextWaitForStopped()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextWaitForStopped_fn(context, duration);
}

YOGI_API void MOCK_ContextWaitForStopped(decltype(YOGI_ContextWaitForStopped) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextWaitForStopped_fn = fn ? fn : decltype(mock_ContextWaitForStopped_fn){};
}

// Mock implementation for YOGI_ContextPost
static std::function<decltype(YOGI_ContextPost)> mock_ContextPost_fn = {};

YOGI_API int YOGI_ContextPost(void* context, void (*fn)(void* userarg), void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_ContextPost_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_ContextPost()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_ContextPost_fn(context, fn, userarg);
}

YOGI_API void MOCK_ContextPost(decltype(YOGI_ContextPost) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_ContextPost_fn = fn ? fn : decltype(mock_ContextPost_fn){};
}

// Mock implementation for YOGI_RaiseSignal
static std::function<decltype(YOGI_RaiseSignal)> mock_RaiseSignal_fn = {};

YOGI_API int YOGI_RaiseSignal(int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg), void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_RaiseSignal_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_RaiseSignal()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_RaiseSignal_fn(signal, sigarg, fn, userarg);
}

YOGI_API void MOCK_RaiseSignal(decltype(YOGI_RaiseSignal) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_RaiseSignal_fn = fn ? fn : decltype(mock_RaiseSignal_fn){};
}

// Mock implementation for YOGI_SignalSetCreate
static std::function<decltype(YOGI_SignalSetCreate)> mock_SignalSetCreate_fn = {};

YOGI_API int YOGI_SignalSetCreate(void** sigset, void* context, int signals) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_SignalSetCreate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_SignalSetCreate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_SignalSetCreate_fn(sigset, context, signals);
}

YOGI_API void MOCK_SignalSetCreate(decltype(YOGI_SignalSetCreate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_SignalSetCreate_fn = fn ? fn : decltype(mock_SignalSetCreate_fn){};
}

// Mock implementation for YOGI_SignalSetAwaitSignalAsync
static std::function<decltype(YOGI_SignalSetAwaitSignalAsync)> mock_SignalSetAwaitSignalAsync_fn = {};

YOGI_API int YOGI_SignalSetAwaitSignalAsync(void* sigset, void (*fn)(int res, int sig, void* sigarg, void* userarg),
                                            void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_SignalSetAwaitSignalAsync_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_SignalSetAwaitSignalAsync()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_SignalSetAwaitSignalAsync_fn(sigset, fn, userarg);
}

YOGI_API void MOCK_SignalSetAwaitSignalAsync(decltype(YOGI_SignalSetAwaitSignalAsync) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_SignalSetAwaitSignalAsync_fn = fn ? fn : decltype(mock_SignalSetAwaitSignalAsync_fn){};
}

// Mock implementation for YOGI_SignalSetCancelAwaitSignal
static std::function<decltype(YOGI_SignalSetCancelAwaitSignal)> mock_SignalSetCancelAwaitSignal_fn = {};

YOGI_API int YOGI_SignalSetCancelAwaitSignal(void* sigset) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_SignalSetCancelAwaitSignal_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_SignalSetCancelAwaitSignal()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_SignalSetCancelAwaitSignal_fn(sigset);
}

YOGI_API void MOCK_SignalSetCancelAwaitSignal(decltype(YOGI_SignalSetCancelAwaitSignal) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_SignalSetCancelAwaitSignal_fn = fn ? fn : decltype(mock_SignalSetCancelAwaitSignal_fn){};
}

// Mock implementation for YOGI_TimerCreate
static std::function<decltype(YOGI_TimerCreate)> mock_TimerCreate_fn = {};

YOGI_API int YOGI_TimerCreate(void** timer, void* context) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_TimerCreate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_TimerCreate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_TimerCreate_fn(timer, context);
}

YOGI_API void MOCK_TimerCreate(decltype(YOGI_TimerCreate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_TimerCreate_fn = fn ? fn : decltype(mock_TimerCreate_fn){};
}

// Mock implementation for YOGI_TimerStartAsync
static std::function<decltype(YOGI_TimerStartAsync)> mock_TimerStartAsync_fn = {};

YOGI_API int YOGI_TimerStartAsync(void* timer, long long duration, void (*fn)(int res, void* userarg), void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_TimerStartAsync_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_TimerStartAsync()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_TimerStartAsync_fn(timer, duration, fn, userarg);
}

YOGI_API void MOCK_TimerStartAsync(decltype(YOGI_TimerStartAsync) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_TimerStartAsync_fn = fn ? fn : decltype(mock_TimerStartAsync_fn){};
}

// Mock implementation for YOGI_TimerCancel
static std::function<decltype(YOGI_TimerCancel)> mock_TimerCancel_fn = {};

YOGI_API int YOGI_TimerCancel(void* timer) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_TimerCancel_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_TimerCancel()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_TimerCancel_fn(timer);
}

YOGI_API void MOCK_TimerCancel(decltype(YOGI_TimerCancel) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_TimerCancel_fn = fn ? fn : decltype(mock_TimerCancel_fn){};
}

// Mock implementation for YOGI_BranchCreate
static std::function<decltype(YOGI_BranchCreate)> mock_BranchCreate_fn = {};

YOGI_API int YOGI_BranchCreate(void** branch, void* context, void* config, const char* section) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchCreate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchCreate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchCreate_fn(branch, context, config, section);
}

YOGI_API void MOCK_BranchCreate(decltype(YOGI_BranchCreate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchCreate_fn = fn ? fn : decltype(mock_BranchCreate_fn){};
}

// Mock implementation for YOGI_BranchGetInfo
static std::function<decltype(YOGI_BranchGetInfo)> mock_BranchGetInfo_fn = {};

YOGI_API int YOGI_BranchGetInfo(void* branch, void* uuid, char* json, int jsonsize) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchGetInfo_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchGetInfo()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchGetInfo_fn(branch, uuid, json, jsonsize);
}

YOGI_API void MOCK_BranchGetInfo(decltype(YOGI_BranchGetInfo) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchGetInfo_fn = fn ? fn : decltype(mock_BranchGetInfo_fn){};
}

// Mock implementation for YOGI_BranchGetConnectedBranches
static std::function<decltype(YOGI_BranchGetConnectedBranches)> mock_BranchGetConnectedBranches_fn = {};

YOGI_API int YOGI_BranchGetConnectedBranches(void* branch, void* uuid, char* json, int jsonsize,
                                             void (*fn)(int res, void* userarg), void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchGetConnectedBranches_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchGetConnectedBranches()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchGetConnectedBranches_fn(branch, uuid, json, jsonsize, fn, userarg);
}

YOGI_API void MOCK_BranchGetConnectedBranches(decltype(YOGI_BranchGetConnectedBranches) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchGetConnectedBranches_fn = fn ? fn : decltype(mock_BranchGetConnectedBranches_fn){};
}

// Mock implementation for YOGI_BranchAwaitEventAsync
static std::function<decltype(YOGI_BranchAwaitEventAsync)> mock_BranchAwaitEventAsync_fn = {};

YOGI_API int YOGI_BranchAwaitEventAsync(void* branch, int events, void* uuid, char* json, int jsonsize,
                                        void (*fn)(int res, int ev, int evres, void* userarg), void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchAwaitEventAsync_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchAwaitEventAsync()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchAwaitEventAsync_fn(branch, events, uuid, json, jsonsize, fn, userarg);
}

YOGI_API void MOCK_BranchAwaitEventAsync(decltype(YOGI_BranchAwaitEventAsync) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchAwaitEventAsync_fn = fn ? fn : decltype(mock_BranchAwaitEventAsync_fn){};
}

// Mock implementation for YOGI_BranchCancelAwaitEvent
static std::function<decltype(YOGI_BranchCancelAwaitEvent)> mock_BranchCancelAwaitEvent_fn = {};

YOGI_API int YOGI_BranchCancelAwaitEvent(void* branch) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchCancelAwaitEvent_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchCancelAwaitEvent()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchCancelAwaitEvent_fn(branch);
}

YOGI_API void MOCK_BranchCancelAwaitEvent(decltype(YOGI_BranchCancelAwaitEvent) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchCancelAwaitEvent_fn = fn ? fn : decltype(mock_BranchCancelAwaitEvent_fn){};
}

// Mock implementation for YOGI_BranchSendBroadcast
static std::function<decltype(YOGI_BranchSendBroadcast)> mock_BranchSendBroadcast_fn = {};

YOGI_API int YOGI_BranchSendBroadcast(void* branch, int enc, const void* data, int datasize, int block) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchSendBroadcast_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchSendBroadcast()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchSendBroadcast_fn(branch, enc, data, datasize, block);
}

YOGI_API void MOCK_BranchSendBroadcast(decltype(YOGI_BranchSendBroadcast) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchSendBroadcast_fn = fn ? fn : decltype(mock_BranchSendBroadcast_fn){};
}

// Mock implementation for YOGI_BranchSendBroadcastAsync
static std::function<decltype(YOGI_BranchSendBroadcastAsync)> mock_BranchSendBroadcastAsync_fn = {};

YOGI_API int YOGI_BranchSendBroadcastAsync(void* branch, int enc, const void* data, int datasize, int retry,
                                           void (*fn)(int res, int oid, void* userarg), void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchSendBroadcastAsync_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchSendBroadcastAsync()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchSendBroadcastAsync_fn(branch, enc, data, datasize, retry, fn, userarg);
}

YOGI_API void MOCK_BranchSendBroadcastAsync(decltype(YOGI_BranchSendBroadcastAsync) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchSendBroadcastAsync_fn = fn ? fn : decltype(mock_BranchSendBroadcastAsync_fn){};
}

// Mock implementation for YOGI_BranchCancelSendBroadcast
static std::function<decltype(YOGI_BranchCancelSendBroadcast)> mock_BranchCancelSendBroadcast_fn = {};

YOGI_API int YOGI_BranchCancelSendBroadcast(void* branch, int oid) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchCancelSendBroadcast_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchCancelSendBroadcast()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchCancelSendBroadcast_fn(branch, oid);
}

YOGI_API void MOCK_BranchCancelSendBroadcast(decltype(YOGI_BranchCancelSendBroadcast) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchCancelSendBroadcast_fn = fn ? fn : decltype(mock_BranchCancelSendBroadcast_fn){};
}

// Mock implementation for YOGI_BranchReceiveBroadcastAsync
static std::function<decltype(YOGI_BranchReceiveBroadcastAsync)> mock_BranchReceiveBroadcastAsync_fn = {};

YOGI_API int YOGI_BranchReceiveBroadcastAsync(void* branch, void* uuid, int enc, void* data, int datasize,
                                              void (*fn)(int res, int size, void* userarg), void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchReceiveBroadcastAsync_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchReceiveBroadcastAsync()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchReceiveBroadcastAsync_fn(branch, uuid, enc, data, datasize, fn, userarg);
}

YOGI_API void MOCK_BranchReceiveBroadcastAsync(decltype(YOGI_BranchReceiveBroadcastAsync) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchReceiveBroadcastAsync_fn = fn ? fn : decltype(mock_BranchReceiveBroadcastAsync_fn){};
}

// Mock implementation for YOGI_BranchCancelReceiveBroadcast
static std::function<decltype(YOGI_BranchCancelReceiveBroadcast)> mock_BranchCancelReceiveBroadcast_fn = {};

YOGI_API int YOGI_BranchCancelReceiveBroadcast(void* branch) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_BranchCancelReceiveBroadcast_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_BranchCancelReceiveBroadcast()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_BranchCancelReceiveBroadcast_fn(branch);
}

YOGI_API void MOCK_BranchCancelReceiveBroadcast(decltype(YOGI_BranchCancelReceiveBroadcast) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_BranchCancelReceiveBroadcast_fn = fn ? fn : decltype(mock_BranchCancelReceiveBroadcast_fn){};
}

// Mock implementation for YOGI_Destroy
static std::function<decltype(YOGI_Destroy)> mock_Destroy_fn = {};

YOGI_API int YOGI_Destroy(void* obj) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_Destroy_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_Destroy()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_Destroy_fn(obj);
}

YOGI_API void MOCK_Destroy(decltype(YOGI_Destroy) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_Destroy_fn = fn ? fn : decltype(mock_Destroy_fn){};
}

// Mock implementation for YOGI_DestroyAll
static std::function<decltype(YOGI_DestroyAll)> mock_DestroyAll_fn = {};

YOGI_API int YOGI_DestroyAll() {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_DestroyAll_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_DestroyAll()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_DestroyAll_fn();
}

YOGI_API void MOCK_DestroyAll(decltype(YOGI_DestroyAll) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_DestroyAll_fn = fn ? fn : decltype(mock_DestroyAll_fn){};
}

// Mock implementation for YOGI_WebServerCreate
static std::function<decltype(YOGI_WebServerCreate)> mock_WebServerCreate_fn = {};

YOGI_API int YOGI_WebServerCreate(void** server, void* context, void* branch, void* config, const char* section) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_WebServerCreate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_WebServerCreate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_WebServerCreate_fn(server, context, branch, config, section);
}

YOGI_API void MOCK_WebServerCreate(decltype(YOGI_WebServerCreate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_WebServerCreate_fn = fn ? fn : decltype(mock_WebServerCreate_fn){};
}

// Mock implementation for YOGI_WebServerAddWorker
static std::function<decltype(YOGI_WebServerAddWorker)> mock_WebServerAddWorker_fn = {};

YOGI_API int YOGI_WebServerAddWorker(void* server, void* context) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_WebServerAddWorker_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_WebServerAddWorker()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_WebServerAddWorker_fn(server, context);
}

YOGI_API void MOCK_WebServerAddWorker(decltype(YOGI_WebServerAddWorker) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_WebServerAddWorker_fn = fn ? fn : decltype(mock_WebServerAddWorker_fn){};
}

// Mock implementation for YOGI_WebRouteCreate
static std::function<decltype(YOGI_WebRouteCreate)> mock_WebRouteCreate_fn = {};

YOGI_API int YOGI_WebRouteCreate(void** route, void* server, const char* baseuri,
                                 void (*fn)(int res, int rid, const char* user, const char* owner, int method,
                                            const char* uri, const char* const* qparams, void* userarg),
                                 void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_WebRouteCreate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_WebRouteCreate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_WebRouteCreate_fn(route, server, baseuri, fn, userarg);
}

YOGI_API void MOCK_WebRouteCreate(decltype(YOGI_WebRouteCreate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_WebRouteCreate_fn = fn ? fn : decltype(mock_WebRouteCreate_fn){};
}

// Mock implementation for YOGI_WebRouteRespond
static std::function<decltype(YOGI_WebRouteRespond)> mock_WebRouteRespond_fn = {};

YOGI_API int YOGI_WebRouteRespond(void* route, int rid, int finished, int status, const char* contype,
                                  const char* content) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_WebRouteRespond_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_WebRouteRespond()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_WebRouteRespond_fn(route, rid, finished, status, contype, content);
}

YOGI_API void MOCK_WebRouteRespond(decltype(YOGI_WebRouteRespond) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_WebRouteRespond_fn = fn ? fn : decltype(mock_WebRouteRespond_fn){};
}

// Mock implementation for YOGI_WebProcessCreate
static std::function<decltype(YOGI_WebProcessCreate)> mock_WebProcessCreate_fn = {};

YOGI_API int YOGI_WebProcessCreate(void** wproc, void* server, const char* name,
                                   void (*fn)(int res, int wpid, const char* user, int wpa, const void* data,
                                              const int* sizes, void* userarg),
                                   void* userarg) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_WebProcessCreate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_WebProcessCreate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_WebProcessCreate_fn(wproc, server, name, fn, userarg);
}

YOGI_API void MOCK_WebProcessCreate(decltype(YOGI_WebProcessCreate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_WebProcessCreate_fn = fn ? fn : decltype(mock_WebProcessCreate_fn){};
}

// Mock implementation for YOGI_WebProcessUpdate
static std::function<decltype(YOGI_WebProcessUpdate)> mock_WebProcessUpdate_fn = {};

YOGI_API int YOGI_WebProcessUpdate(void* wproc, int wpid, int wpu, int exitcode, const void* output, int outsize) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  if (!mock_WebProcessUpdate_fn) {
    std::cout << "WARNING: Unmonitored mock function call: YOGI_WebProcessUpdate()" << std::endl;
    return YOGI_ERR_UNKNOWN;
  }

  return mock_WebProcessUpdate_fn(wproc, wpid, wpu, exitcode, output, outsize);
}

YOGI_API void MOCK_WebProcessUpdate(decltype(YOGI_WebProcessUpdate) fn) {
  std::lock_guard<std::mutex> lock(global_mock_mutex);
  mock_WebProcessUpdate_fn = fn ? fn : decltype(mock_WebProcessUpdate_fn){};
}

// :CODEGEN_END:

YOGI_API void MOCK_ResetMocks() {
  std::lock_guard<std::mutex> lock(global_mock_mutex);

  // :CODEGEN_BEGIN:
  mock_GetVersion_fn                         = default_GetVersion;
  mock_CheckBindingsCompatibility_fn         = default_CheckBindingsCompatibility;
  mock_GetErrorString_fn                     = {};
  mock_GetLastErrorDetails_fn                = {};
  mock_GetConstant_fn                        = default_GetConstant;
  mock_GetSchema_fn                          = {};
  mock_GetCurrentTime_fn                     = {};
  mock_FormatTime_fn                         = {};
  mock_ParseTime_fn                          = {};
  mock_FormatDuration_fn                     = {};
  mock_FormatObject_fn                       = {};
  mock_ConfigureConsoleLogging_fn            = {};
  mock_ConfigureHookLogging_fn               = {};
  mock_ConfigureFileLogging_fn               = {};
  mock_LoggerCreate_fn                       = {};
  mock_LoggerGetVerbosity_fn                 = {};
  mock_LoggerSetVerbosity_fn                 = {};
  mock_LoggerSetComponentsVerbosity_fn       = {};
  mock_LoggerLog_fn                          = {};
  mock_ConfigurationCreate_fn                = {};
  mock_ConfigurationUpdateFromCommandLine_fn = {};
  mock_ConfigurationUpdateFromJson_fn        = {};
  mock_ConfigurationUpdateFromFile_fn        = {};
  mock_ConfigurationDump_fn                  = {};
  mock_ConfigurationWriteToFile_fn           = {};
  mock_ConfigurationValidate_fn              = {};
  mock_ContextCreate_fn                      = {};
  mock_ContextPoll_fn                        = {};
  mock_ContextPollOne_fn                     = {};
  mock_ContextRun_fn                         = {};
  mock_ContextRunOne_fn                      = {};
  mock_ContextRunInBackground_fn             = {};
  mock_ContextStop_fn                        = {};
  mock_ContextWaitForRunning_fn              = {};
  mock_ContextWaitForStopped_fn              = {};
  mock_ContextPost_fn                        = {};
  mock_RaiseSignal_fn                        = {};
  mock_SignalSetCreate_fn                    = {};
  mock_SignalSetAwaitSignalAsync_fn          = {};
  mock_SignalSetCancelAwaitSignal_fn         = {};
  mock_TimerCreate_fn                        = {};
  mock_TimerStartAsync_fn                    = {};
  mock_TimerCancel_fn                        = {};
  mock_BranchCreate_fn                       = {};
  mock_BranchGetInfo_fn                      = {};
  mock_BranchGetConnectedBranches_fn         = {};
  mock_BranchAwaitEventAsync_fn              = {};
  mock_BranchCancelAwaitEvent_fn             = {};
  mock_BranchSendBroadcast_fn                = {};
  mock_BranchSendBroadcastAsync_fn           = {};
  mock_BranchCancelSendBroadcast_fn          = {};
  mock_BranchReceiveBroadcastAsync_fn        = {};
  mock_BranchCancelReceiveBroadcast_fn       = {};
  mock_Destroy_fn                            = {};
  mock_DestroyAll_fn                         = {};
  mock_WebServerCreate_fn                    = {};
  mock_WebServerAddWorker_fn                 = {};
  mock_WebRouteCreate_fn                     = {};
  mock_WebRouteRespond_fn                    = {};
  mock_WebProcessCreate_fn                   = {};
  mock_WebProcessUpdate_fn                   = {};
  // :CODEGEN_END:
}
