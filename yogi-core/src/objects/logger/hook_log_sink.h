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

#include <src/objects/logger.h>
#include <src/objects/logger/text_based_log_sink.h>
#include <src/util/time.h>

#include <string>

class HookLogSink {
 public:
  HookLogSink(Logger::HookFn fn, void* userarg) : fn_{fn}, userarg_{userarg} {}

  void publish(int severity, Timestamp timestamp, int tid, const char* file, int line, const std::string& component,
               const char* msg) {
    fn_(severity, timestamp.ns_since_epoch(), tid, file, line, component.c_str(), msg, userarg_);
  }

 private:
  const Logger::HookFn fn_;
  void* const userarg_;
};
