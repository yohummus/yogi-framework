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

#include <src/api/errors.h>
#include <src/objects/logger/text_based_log_sink.h>
#include <src/system/console.h>

class ConsoleLogSink : public TextBasedLogSink {
 public:
  ConsoleLogSink(int stream, int color, const char* timefmt, const char* fmt)
      : TextBasedLogSink{timefmt, fmt, !!color}, console_{stream} {
  }

 protected:
  virtual void write_partial_output(const std::string& str) override {
    console_.write(str.c_str());
  }

  virtual void set_output_colors(int severity) override {
    switch (severity) {
      case YOGI_VB_FATAL:
        console_.set_color(Console::ForegroundColour::kWhite);
        console_.set_color(Console::BackgroundColour::kRed);
        break;

      case YOGI_VB_ERROR:
        console_.set_color(Console::ForegroundColour::kLightRed);
        break;

      case YOGI_VB_WARNING:
        console_.set_color(Console::ForegroundColour::kYellow);
        break;

      case YOGI_VB_INFO:
        console_.set_color(Console::ForegroundColour::kWhite);
        break;

      case YOGI_VB_DEBUG:
        console_.set_color(Console::ForegroundColour::kLightGreen);
        break;

      case YOGI_VB_TRACE:
        console_.set_color(Console::ForegroundColour::kBrown);
        break;

      default:
        YOGI_NEVER_REACHED;
        break;
    }
  }

  virtual void reset_output_colors() override {
    console_.reset_colors();
  }

  virtual void flush() override {
    console_.flush();
  }

 private:
  Console console_;
};
