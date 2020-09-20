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

#include <cstdio>

class Console {
 public:
  enum class ForegroundColour {
    kBlack,
    kBlue,
    kGreen,
    kCyan,
    kRed,
    kMagenta,
    kBrown,
    kGrey,
    kDarkGrey,
    kLightBlue,
    kLightGreen,
    kLightCyan,
    kLightRed,
    kLightMagenta,
    kYellow,
    kWhite,
  };

  enum class BackgroundColour {
    kBlack   = static_cast<int>(ForegroundColour::kBlack),
    kRed     = static_cast<int>(ForegroundColour::kRed),
    kGreen   = static_cast<int>(ForegroundColour::kGreen),
    kYellow  = static_cast<int>(ForegroundColour::kYellow),
    kBlue    = static_cast<int>(ForegroundColour::kBlue),
    kMagenta = static_cast<int>(ForegroundColour::kMagenta),
    kCyan    = static_cast<int>(ForegroundColour::kCyan),
    kWhite   = static_cast<int>(ForegroundColour::kWhite),
  };

  Console(int stream);

  void set_color(ForegroundColour color);
  void set_color(BackgroundColour color);
  void reset_colors();
  void flush();
  void write(const char* txt);

 private:
  FILE* file_;
  bool supports_color_;
};
