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

#include <src/api/constants.h>
#include <src/objects/logger.h>
#include <src/objects/logger/console_log_sink.h>
#include <src/objects/logger/file_log_sink.h>
#include <src/objects/logger/hook_log_sink.h>
#include <src/system/process.h>
#include <src/util/time.h>

#include <boost/algorithm/string.hpp>

#include <regex>
#include <stdexcept>

using namespace std::string_literals;

int Logger::string_to_verbosity(std::string_view str) {
  if (boost::iequals(str, "NONE")) return YOGI_VB_NONE;
  if (boost::iequals(str, "FATAL")) return YOGI_VB_FATAL;
  if (boost::iequals(str, "ERROR")) return YOGI_VB_ERROR;
  if (boost::iequals(str, "WARNING")) return YOGI_VB_WARNING;
  if (boost::iequals(str, "INFO")) return YOGI_VB_INFO;
  if (boost::iequals(str, "DEBUG")) return YOGI_VB_DEBUG;
  if (boost::iequals(str, "TRACE")) return YOGI_VB_TRACE;

  auto txt = "Invalid verbosity \""s;
  txt.append(str.begin(), str.end());
  txt += "\"";
  throw std::runtime_error(txt);
}

std::string Logger::configure_file_logging(int verbosity, const char* filename, const char* timefmt, const char* fmt) {
  std::lock_guard<std::mutex> lock{sinks_mutex_};

  file_verbosity_ = verbosity;
  file_sink_.reset();
  if (verbosity == YOGI_VB_NONE) return {};

  file_sink_ = std::make_unique<FileLogSink>(filename, timefmt, fmt);
  return file_sink_->generated_filename();
}

void Logger::configure_console_logging(int verbosity, int stream, int color, const char* timefmt, const char* fmt) {
  std::lock_guard<std::mutex> lock{sinks_mutex_};

  console_verbosity_ = verbosity;
  console_sink_.reset();
  if (verbosity == YOGI_VB_NONE) return;

  console_sink_ = std::make_unique<ConsoleLogSink>(stream, color, timefmt, fmt);
}

void Logger::configure_hook_logging(int verbosity, HookFn fn, void* userarg) {
  std::lock_guard<std::mutex> lock{sinks_mutex_};

  hook_verbosity_ = verbosity;
  hook_sink_.reset();
  if (verbosity == YOGI_VB_NONE) return;

  hook_sink_ = std::make_unique<HookLogSink>(fn, userarg);
}

int Logger::set_components_verbosity(const char* components_re, int verbosity) {
  std::regex re;

  try {
    re = std::regex(components_re);
  } catch (const std::regex_error& e) {
    throw DescriptiveError{YOGI_ERR_INVALID_REGEX} << e.what();
  }

  int count = 0;

  auto fn = [&](const LoggerPtr& log) {
    std::smatch m;
    if (std::regex_match(log->component(), m, re)) {
      log->set_verbosity(verbosity);
      ++count;
    }
  };

  // App logger
  fn(app_logger());

  // Loggers created by the user
  for (auto& log : ObjectRegister::get_all<Logger>()) {
    fn(log);
  }

  // Internal loggers
  for (auto& weak_log : internal_loggers()) {
    auto log = weak_log.lock();
    if (log) fn(log);
  }

  return count;
}

LoggerPtr Logger::make_static_internal_logger(const char* component) {
  auto logger = std::make_shared<Logger>(std::string("Yogi.") + component);
  internal_loggers().push_back(logger);
  return logger;
}

Logger::Logger(std::string component) : component_(component), verbosity_(constants::kDefaultLoggerVerbosity) {}

void Logger::log(int severity, const char* file, int line, const char* msg) {
  if (severity > verbosity_) {
    return;
  }

  auto timestamp = Timestamp::now();
  int tid        = get_thread_id();

  std::lock_guard<std::mutex> lock{sinks_mutex_};

  auto fn = [&](int sink_verbosity, auto& sink) {
    if (sink && sink_verbosity >= severity) {
      sink->publish(severity, timestamp, tid, file, line, component_, msg);
    }
  };

  fn(file_verbosity_, file_sink_);
  fn(console_verbosity_, console_sink_);
  fn(hook_verbosity_, hook_sink_);
}

// We use a function instead of a member here so we don't get static initialization order problems
Logger::LoggerVector& Logger::internal_loggers() {
  static LoggerVector vec;
  return vec;
}

LoggerPtr Logger::app_logger_ = Logger::create("App");
std::mutex Logger::sinks_mutex_;
FileLogSinkPtr Logger::file_sink_;
ConsoleLogSinkPtr Logger::console_sink_;
HookLogSinkPtr Logger::hook_sink_;
int Logger::file_verbosity_    = YOGI_VB_NONE;
int Logger::console_verbosity_ = YOGI_VB_NONE;
int Logger::hook_verbosity_    = YOGI_VB_NONE;
