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

#include <src/api/constants.h>
#include <src/system/process.h>
#include <src/util/time.h>

#include <sstream>
#include <string>

class TextBasedLogSink {
 public:
  TextBasedLogSink(const char* timefmt, const char* fmt, bool use_color)
      : time_fmt_{timefmt ? timefmt : constants::kDefaultTimeFormat},
        fmt_{fmt ? fmt : constants::kDefaultLogFormat},
        use_color_{use_color} {}

  virtual ~TextBasedLogSink() {}

  void publish(int severity, Timestamp timestamp, int tid, const char* file, int line, const std::string& component,
               const char* msg) {
    std::stringstream ss;
    bool color_cleared = true;

    std::string::size_type old_pos = 0;
    std::string::size_type pos     = fmt_.find('$');
    while (pos != std::string::npos) {
      if (pos > old_pos) {
        ss.write(fmt_.c_str() + old_pos, static_cast<std::streamsize>(pos - old_pos));
      }

      switch (fmt_[pos + 1]) {
          // clang-format off
      case 't': ss << timestamp.format(time_fmt_.c_str());  break;
      case 'P': ss << get_process_id();                     break;
      case 'T': ss << tid;                                  break;
      case 'm': ss << msg;                                  break;
      case 'f': ss << file;                                 break;
      case 'l': ss << line;                                 break;
      case 'c': ss << component;                            break;
      case '$': ss << '$';                                  break;
      case 's': ss << severity_to_string(severity);         break;
          // clang-format on

        case '<':
          if (use_color_ && color_cleared) {
            write_partial_output(ss.str());
            ss.str(std::string());

            set_output_colors(severity);
            color_cleared = false;
          }
          break;

        case '>':
          if (use_color_ && !color_cleared) {
            write_partial_output(ss.str());
            ss.str(std::string());

            reset_output_colors();
            color_cleared = true;
          }
          break;
      }

      old_pos = pos + 2;  // skip placeholder
      pos     = fmt_.find('$', old_pos);
    }

    ss << fmt_.substr(old_pos);
    ss << std::endl;
    write_partial_output(ss.str());
    if (!color_cleared) {
      reset_output_colors();
    }

    flush();
  }

 protected:
  virtual void write_partial_output(const std::string& str) = 0;
  virtual void set_output_colors(int verbosity) { YOGI_UNUSED(verbosity); }
  virtual void reset_output_colors() {}
  virtual void flush() {}

 private:
  static const char* severity_to_string(int severity) {
    // clang-format off
    switch (severity) {
      case YOGI_VB_FATAL:   return "FAT";
      case YOGI_VB_ERROR:   return "ERR";
      case YOGI_VB_WARNING: return "WRN";
      case YOGI_VB_INFO:    return "IFO";
      case YOGI_VB_DEBUG:   return "DBG";
      case YOGI_VB_TRACE:   return "TRC";

      default:
        YOGI_NEVER_REACHED;
        return "";
    }
    // clang-format on
  }

  const std::string time_fmt_;
  const std::string fmt_;
  const bool use_color_;
};
