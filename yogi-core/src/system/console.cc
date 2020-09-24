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

#include <src/system/console.h>

#include <stdio.h>

#ifdef _WIN32
#  include <Windows.h>
#  include <cstdlib>
#else
#  include <unistd.h>
#endif

namespace {

#ifdef _WIN32

auto win32_original_stdout_colors = []() {
  HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hdl, &csbi);
  return csbi.wAttributes;
}();

auto win32_original_stderr_colors = []() {
  HANDLE hdl = GetStdHandle(STD_ERROR_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hdl, &csbi);
  return csbi.wAttributes;
}();

HANDLE get_std_handle(const FILE* file) {
  return GetStdHandle(file == stdout ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
}

#endif

}  // anonymous namespace

Console::Console(int stream) : file_{stream == YOGI_ST_STDOUT ? stdout : stderr} {
#ifdef _WIN32
  supports_color_ = true;
#else
  supports_color_ = isatty(stream == YOGI_ST_STDOUT ? STDOUT_FILENO : STDERR_FILENO);
#endif
}

void Console::set_color(ForegroundColour color) {
  if (!supports_color_) return;

#ifdef _WIN32
  HANDLE hdl = get_std_handle(file_);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hdl, &csbi);
  SetConsoleTextAttribute(hdl, (csbi.wAttributes & 0xFFF0) | static_cast<WORD>(color));
#else
  switch (color) {
    // clang-format off
    case ForegroundColour::kBlack:        write("\033[22;30m"); break;
    case ForegroundColour::kBlue:         write("\033[22;34m"); break;
    case ForegroundColour::kGreen:        write("\033[22;32m"); break;
    case ForegroundColour::kCyan:         write("\033[22;36m"); break;
    case ForegroundColour::kRed:          write("\033[22;31m"); break;
    case ForegroundColour::kMagenta:      write("\033[22;35m"); break;
    case ForegroundColour::kBrown:        write("\033[22;33m"); break;
    case ForegroundColour::kGrey:         write("\033[22;37m"); break;
    case ForegroundColour::kDarkGrey:     write("\033[01;30m"); break;
    case ForegroundColour::kLightBlue:    write("\033[01;34m"); break;
    case ForegroundColour::kLightGreen:   write("\033[01;32m"); break;
    case ForegroundColour::kLightCyan:    write("\033[01;36m"); break;
    case ForegroundColour::kLightRed:     write("\033[01;31m"); break;
    case ForegroundColour::kLightMagenta: write("\033[01;35m"); break;
    case ForegroundColour::kYellow:       write("\033[01;33m"); break;
    case ForegroundColour::kWhite:        write("\033[01;37m"); break;
      // clang-format on
  }
#endif
}

void Console::set_color(BackgroundColour color) {
  if (!supports_color_) return;

#ifdef _WIN32
  HANDLE hdl = get_std_handle(file_);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hdl, &csbi);
  SetConsoleTextAttribute(hdl, (csbi.wAttributes & 0xFF0F) | (static_cast<WORD>(color) << 4));
#else
  switch (color) {
    // clang-format off
    case BackgroundColour::kBlack:   write("\033[40m"); break;
    case BackgroundColour::kRed:     write("\033[41m"); break;
    case BackgroundColour::kGreen:   write("\033[42m"); break;
    case BackgroundColour::kYellow:  write("\033[43m"); break;
    case BackgroundColour::kBlue:    write("\033[44m"); break;
    case BackgroundColour::kMagenta: write("\033[45m"); break;
    case BackgroundColour::kCyan:    write("\033[46m"); break;
    case BackgroundColour::kWhite:   write("\033[47m"); break;
      // clang-format on
  }
#endif
}

void Console::reset_colors() {
  if (!supports_color_) return;

#ifdef _WIN32
  HANDLE hdl   = get_std_handle(file_);
  auto& colors = file_ == stdout ? win32_original_stdout_colors : win32_original_stderr_colors;
  SetConsoleTextAttribute(hdl, colors);
#else
  write("\033[0m");
#endif
}

void Console::flush() {
  fflush(file_);
}

void Console::write(const char* txt) {
  fputs(txt, file_);
}
