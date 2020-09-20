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
#include <src/objects/logger.h>

#include <regex>
#include <sstream>
#include <string>

// To be used only within class that inherit LoggerUser and in files that have
// defined a global internal logger using YOGI_DEFINE_INTERNAL_LOGGER().
#define LOG_FAT(...) YOGI_INTERNAL_LOG(YOGI_VB_FATAL, __VA_ARGS__)
#define LOG_ERR(...) YOGI_INTERNAL_LOG(YOGI_VB_ERROR, __VA_ARGS__)
#define LOG_WRN(...) YOGI_INTERNAL_LOG(YOGI_VB_WARNING, __VA_ARGS__)
#define LOG_IFO(...) YOGI_INTERNAL_LOG(YOGI_VB_INFO, __VA_ARGS__)
#define LOG_DBG(...) YOGI_INTERNAL_LOG(YOGI_VB_DEBUG, __VA_ARGS__)
#define LOG_TRC(...) YOGI_INTERNAL_LOG(YOGI_VB_TRACE, __VA_ARGS__)

#define YOGI_INTERNAL_LOG(severity, stream)                                      \
  {                                                                              \
    auto& logger = file_global_internal_logger;                                  \
    if (severity <= (logger)->verbosity()) {                                     \
      std::stringstream ss;                                                      \
      if (!this->logging_prefix().empty()) ss << this->logging_prefix() << ": "; \
      ss << stream;                                                              \
      (logger)->log(severity, __FILE__, __LINE__, ss.str().c_str());             \
    }                                                                            \
  }

#define YOGI_DEFINE_INTERNAL_LOGGER(component)                                                  \
  namespace {                                                                                   \
  const LoggerPtr file_global_internal_logger = Logger::make_static_internal_logger(component); \
  }  // anonymous namespace

// Class for logging internal messages
class LogUser {
 public:
  const std::string& logging_prefix() const { return prefix_; }

 protected:
  void set_logging_prefix(const ExposedObject& obj) {
    YOGI_ASSERT(prefix_.empty());
    prefix_ = obj.format("[$x]");
  }

  void set_logging_prefix(std::string prefix) { prefix_ = prefix; }
  void clear_logging_prefix() { prefix_ = {}; }

 private:
  std::string prefix_;
};
