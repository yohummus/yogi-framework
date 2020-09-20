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

#pragma once

#include <src/config.h>

#include <src/api/object.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

// Forward declarations
class Logger;
typedef std::shared_ptr<Logger> LoggerPtr;
typedef std::weak_ptr<Logger> LoggerWeakPtr;

class FileLogSink;
typedef std::unique_ptr<FileLogSink> FileLogSinkPtr;

class ConsoleLogSink;
typedef std::unique_ptr<ConsoleLogSink> ConsoleLogSinkPtr;

class HookLogSink;
typedef std::unique_ptr<HookLogSink> HookLogSinkPtr;

// Class for representing a Yogi Logger
class Logger : public ExposedObjectT<Logger, ObjectType::kLogger> {
 public:
  using HookFn = void (*)(int severity, long long timestamp, int tid, const char* file, int line, const char* comp,
                          const char* msg, void* userarg);

  static int string_to_verbosity(std::string_view str);
  static LoggerPtr app_logger() { return app_logger_; }
  static LoggerPtr make_static_internal_logger(const char* component);
  static int set_components_verbosity(const char* components_re, int verbosity);

  static std::string configure_file_logging(int verbosity, const char* filename, const char* timefmt, const char* fmt);
  static void configure_console_logging(int verbosity, int stream, int color, const char* timefmt, const char* fmt);
  static void configure_hook_logging(int verbosity, HookFn fn, void* userarg);

  Logger(std::string component);

  const std::string& component() const { return component_; }
  int verbosity() const { return verbosity_; }
  void set_verbosity(int verbosity) { verbosity_ = verbosity; }
  void log(int severity, const char* file, int line, const char* msg);

 private:
  using LoggerVector = std::vector<LoggerWeakPtr>;

  static LoggerVector& internal_loggers();

  static LoggerPtr app_logger_;
  static std::mutex sinks_mutex_;
  static FileLogSinkPtr file_sink_;
  static ConsoleLogSinkPtr console_sink_;
  static HookLogSinkPtr hook_sink_;
  static int file_verbosity_;
  static int console_verbosity_;
  static int hook_verbosity_;

  const std::string component_;
  std::atomic<int> verbosity_;
};
