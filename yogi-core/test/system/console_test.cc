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

#include <test/common.h>

#include <src/system/console.h>

std::vector<std::pair<Console::ForegroundColour, const char*>> fg_colors = {
    {Console::ForegroundColour::kBlack, "Black"},
    {Console::ForegroundColour::kBlue, "Blue"},
    {Console::ForegroundColour::kGreen, "Green"},
    {Console::ForegroundColour::kCyan, "Cyan"},
    {Console::ForegroundColour::kRed, "Red"},
    {Console::ForegroundColour::kMagenta, "Magenta"},
    {Console::ForegroundColour::kBrown, "Brown"},
    {Console::ForegroundColour::kGrey, "Grey"},
    {Console::ForegroundColour::kDarkGrey, "DarkGrey"},
    {Console::ForegroundColour::kLightBlue, "LightBlue"},
    {Console::ForegroundColour::kLightGreen, "LightGreen"},
    {Console::ForegroundColour::kLightCyan, "LightCyan"},
    {Console::ForegroundColour::kLightRed, "LightRed"},
    {Console::ForegroundColour::kLightMagenta, "LightMagenta"},
    {Console::ForegroundColour::kYellow, "Yellow"},
    {Console::ForegroundColour::kWhite, "White"},
};

std::vector<Console::BackgroundColour> bg_colors = {
    Console::BackgroundColour::kBlack,  Console::BackgroundColour::kRed,   Console::BackgroundColour::kGreen,
    Console::BackgroundColour::kYellow, Console::BackgroundColour::kBlue,  Console::BackgroundColour::kMagenta,
    Console::BackgroundColour::kCyan,   Console::BackgroundColour::kWhite,
};

void print_all_colors(int stream) {
  Console cons{stream};
  for (auto fg : fg_colors) {
    for (auto bg : bg_colors) {
      cons.set_color(fg.first);
      cons.set_color(bg);
      cons.write(fg.second);
    }

    cons.write("\n");
    cons.flush();
  }

  cons.reset_colors();
}

TEST(ConsoleTest, PrintStdoutColors) { print_all_colors(YOGI_ST_STDOUT); }

TEST(ConsoleTest, PrintStderrColors) { print_all_colors(YOGI_ST_STDERR); }
