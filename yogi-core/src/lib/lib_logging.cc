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

#include <src/lib/lib_helpers.h>
#include <src/objects/logger.h>

#include <boost/algorithm/string.hpp>

#include <regex>

namespace {

bool is_time_format_valid(const char* timefmt) {
  if (timefmt == nullptr) return true;
  if (timefmt[0] == '\0') return false;

  static std::regex re{"%([^YmdFHMST369]|$)"};
  return !std::regex_search(timefmt, re);
}

bool is_log_format_valid(const char* fmt) {
  if (fmt == nullptr) return true;
  if (fmt[0] == '\0') return false;

  std::string s = fmt;
  boost::replace_all(s, "$$", "");
  static std::regex re{"\\$([^tPTsmflc<>]|$)"};
  return !std::regex_search(s, re);
}

}  // anonymous namespace

YOGI_API int YOGI_ConfigureConsoleLogging(int verbosity, int stream, int color, const char* timefmt, const char* fmt) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(YOGI_VB_NONE <= verbosity && verbosity <= YOGI_VB_TRACE);
  CHECK_PARAM(stream == YOGI_ST_STDOUT || stream == YOGI_ST_STDERR);
  CHECK_PARAM(color == YOGI_TRUE || color == YOGI_FALSE);
  CHECK_PARAM(is_time_format_valid(timefmt));
  CHECK_PARAM(is_log_format_valid(fmt));

  Logger::configure_console_logging(verbosity, stream, color, timefmt, fmt);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ConfigureHookLogging(int verbosity,
                                       void (*fn)(int severity, long long timestamp, int tid, const char* file,
                                                  int line, const char* comp, const char* msg, void* userarg),
                                       void* userarg) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(YOGI_VB_NONE <= verbosity && verbosity <= YOGI_VB_TRACE);

  if (fn == nullptr) fn = [](int, long long, int, const char*, int, const char*, const char*, void*) {};
  Logger::configure_hook_logging(verbosity, fn, userarg);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ConfigureFileLogging(int verbosity, const char* filename, const char** genfn, int* genfnsize,
                                       const char* timefmt, const char* fmt) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(YOGI_VB_NONE <= verbosity && verbosity <= YOGI_VB_TRACE);
  CHECK_PARAM(verbosity == YOGI_VB_NONE || filename != nullptr);
  CHECK_PARAM(is_time_format_valid(filename));
  CHECK_PARAM(is_time_format_valid(timefmt));
  CHECK_PARAM(is_log_format_valid(fmt));

  auto gen_filename = Logger::configure_file_logging(verbosity, filename, timefmt, fmt);
  set_api_buffer(std::move(gen_filename), genfn, genfnsize);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_LoggerCreate(void** logger, const char* component) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(logger != nullptr);
  CHECK_PARAM(component != nullptr && *component != '\0');

  auto log = Logger::create(component);
  *logger  = ObjectRegister::register_object(log);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_LoggerGetVerbosity(void* logger, int* verbosity) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(verbosity != nullptr);

  auto log   = logger ? ObjectRegister::get<Logger>(logger) : Logger::app_logger();
  *verbosity = log->verbosity();

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_LoggerSetVerbosity(void* logger, int verbosity) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(YOGI_VB_NONE <= verbosity && verbosity <= YOGI_VB_TRACE);

  auto log = logger ? ObjectRegister::get<Logger>(logger) : Logger::app_logger();
  log->set_verbosity(verbosity);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_LoggerSetComponentsVerbosity(const char* components, int verbosity, int* count) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(components != nullptr && *components != '\0');
  CHECK_PARAM(YOGI_VB_NONE <= verbosity && verbosity <= YOGI_VB_TRACE);

  auto n = Logger::set_components_verbosity(components, verbosity);
  if (count) *count = n;

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_LoggerLog(void* logger, int severity, const char* file, int line, const char* msg) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(YOGI_VB_NONE <= severity && severity <= YOGI_VB_TRACE);
  CHECK_PARAM(file == nullptr || *file != '\0');
  CHECK_PARAM(line >= 0);
  CHECK_PARAM(msg != nullptr && *msg != '\0');

  auto log = logger ? ObjectRegister::get<Logger>(logger) : Logger::app_logger();
  log->log(severity, file, line, msg);

  END_CHECKED_API_FUNCTION
}
