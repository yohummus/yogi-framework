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
#include <src/util/time.h>

#include <fstream>

class FileLogSink : public TextBasedLogSink {
 public:
  FileLogSink(const char* filename, const char* timefmt, const char* fmt) : TextBasedLogSink{timefmt, fmt, false} {
    filename_ = Timestamp::now().format(filename);
    file_.open(filename_, std::ios::out | std::ios::trunc);
    if (!file_.is_open()) {
      throw Error{YOGI_ERR_OPEN_FILE_FAILED};
    }
  }

  const std::string& generated_filename() const { return filename_; }

 protected:
  virtual void write_partial_output(const std::string& str) override { file_ << str; }
  virtual void flush() override { file_ << std::flush; }

 private:
  std::string filename_;
  std::ofstream file_;
};
