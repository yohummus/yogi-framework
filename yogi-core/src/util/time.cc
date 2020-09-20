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
#include <src/api/errors.h>
#include <src/util/time.h>

#include <boost/algorithm/string.hpp>

#include <stdio.h>
#include <time.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <ratio>
#include <sstream>

namespace {

template <int Width>
int parse_number(std::string_view str, std::string_view::const_iterator* it, int min, int max) {
  if (std::distance(*it, str.end()) < Width) {
    throw Error{YOGI_ERR_PARSING_TIME_FAILED};
  }

  char buf[static_cast<std::size_t>(Width + 1)];
  std::copy_n(*it, Width, buf);
  buf[Width] = '\0';

  int multiplier = 1;
  int number     = 0;
  for (int i = Width - 1; i >= 0; --i) {
    int digit = buf[i] - '0';
    if (digit < 0 || digit > 9) {
      throw Error{YOGI_ERR_PARSING_TIME_FAILED};
    }

    number += digit * multiplier;
    multiplier *= 10;
  }

  if (number < min || number > max) {
    throw Error{YOGI_ERR_PARSING_TIME_FAILED};
  }

  *it += Width;

  return number;
}

void match_char(std::string_view str, std::string_view::const_iterator* it, char ch) {
  if (*it == str.end() || **it != ch) {
    throw Error{YOGI_ERR_PARSING_TIME_FAILED};
  }

  ++*it;
}

}  // anonymous namespace

Timestamp Timestamp::now() {
  using namespace std::chrono;
  auto tp = time_point_cast<duration<long long, std::nano>>(system_clock::now());
  return Timestamp(tp.time_since_epoch().count());
}

Timestamp Timestamp::parse(std::string_view str, const char* fmt) {
  std::tm tm  = {};
  tm.tm_year  = 70;
  tm.tm_mon   = 1;
  tm.tm_mday  = 1;
  tm.tm_isdst = 0;

  int msec = 0;
  int usec = 0;
  int nsec = 0;

  auto strit   = str.begin();
  bool special = false;
  for (auto fmtch : std::string_view(fmt ? fmt : constants::kDefaultTimeFormat)) {
    if (special) {
      switch (fmtch) {
        case 'F':
          tm.tm_year = parse_number<4>(str, &strit, 0070, 9999) - 1900;
          match_char(str, &strit, '-');
          tm.tm_mon = parse_number<2>(str, &strit, 01, 12) - 1;
          match_char(str, &strit, '-');
          tm.tm_mday = parse_number<2>(str, &strit, 01, 31);
          break;

        case 'T':
          tm.tm_hour = parse_number<2>(str, &strit, 00, 23);
          match_char(str, &strit, ':');
          tm.tm_min = parse_number<2>(str, &strit, 00, 59);
          match_char(str, &strit, ':');
          tm.tm_sec = parse_number<2>(str, &strit, 00, 59);
          break;

          // clang-format off
        case 'Y': tm.tm_year = parse_number<4>(str, &strit, 0070, 9999) - 1900; break;
        case 'm': tm.tm_mon  = parse_number<2>(str, &strit,   01,   12) - 1;    break;
        case 'd': tm.tm_mday = parse_number<2>(str, &strit,   01,   31);        break;
        case 'H': tm.tm_hour = parse_number<2>(str, &strit,   00,   23);        break;
        case 'M': tm.tm_min  = parse_number<2>(str, &strit,   00,   59);        break;
        case 'S': tm.tm_sec  = parse_number<2>(str, &strit,   00,   59);        break;
        case '3': msec       = parse_number<3>(str, &strit,  000,  999);        break;
        case '6': usec       = parse_number<3>(str, &strit,  000,  999);        break;
        case '9': nsec       = parse_number<3>(str, &strit,  000,  999);        break;
          // clang-format on

        default:
          throw Error{YOGI_ERR_INVALID_TIME_FORMAT};
      }

      special = false;
    } else {
      if (fmtch == '%') {
        special = true;
      } else {
        match_char(str, &strit, fmtch);
      }
    }
  }

  if (strit != str.end()) {
    throw Error{YOGI_ERR_PARSING_TIME_FAILED};
  }

#ifdef _WIN32
  auto time = _mkgmtime(&tm);
#else
  auto time = timegm(&tm);
#endif

  if (time == -1) {
    throw Error{YOGI_ERR_PARSING_TIME_FAILED};
  }

  long long ns = time * 1000000000ll;
  ns += msec * 1000000ll;
  ns += usec * 1000ll;
  ns += nsec;

  return Timestamp(ns);
}

std::string Timestamp::format(const char* fmt) const {
  using namespace std::chrono;

  std::string s = fmt ? fmt : constants::kDefaultTimeFormat;
  auto dur      = duration_cast<system_clock::duration>(nanoseconds{ns_since_epoch_});
  auto tp       = system_clock::time_point{dur};
  auto time     = system_clock::to_time_t(tp);

  std::tm tm = {};
#ifdef _WIN32
  gmtime_s(&tm, &time);
#else
  gmtime_r(&time, &tm);
#endif

  char tmp[4];
  sprintf(tmp, "%03lld", (ns_since_epoch_ / 1000000) % 1000);
  boost::replace_all(s, "%3", tmp);
  sprintf(tmp, "%03lld", (ns_since_epoch_ / 1000) % 1000);
  boost::replace_all(s, "%6", tmp);
  sprintf(tmp, "%03lld", ns_since_epoch_ % 1000);
  boost::replace_all(s, "%9", tmp);

  std::ostringstream ss;
  ss << std::put_time(&tm, s.c_str());
  return ss.str();
}

std::string Timestamp::to_javascript_string() const { return format("%FT%T.%3Z"); }

std::chrono::nanoseconds Duration::to_chrono_duration() const {
  using std::chrono::nanoseconds;
  if (is_inf()) {
    return is_neg() ? nanoseconds::min() : nanoseconds::max();
  } else {
    return std::chrono::nanoseconds{ns_};
  }
}

std::string Duration::format(const char* durfmt, const char* infstr) const {
  std::string s;
  if (ns_ < 0) {
    s = infstr ? infstr : constants::kDefaultInfDurationString;

    boost::replace_all(s, "%-", is_negative_ ? "-" : "");
    boost::replace_all(s, "%+", is_negative_ ? "-" : "+");
  } else {
    s = durfmt ? durfmt : constants::kDefaultDurationFormat;

    boost::replace_all(s, "%-", is_negative_ ? "-" : "");
    boost::replace_all(s, "%+", is_negative_ ? "-" : "+");

    char tmp[32];
    auto days = ns_ / 86'400'000'000'000ll;
    sprintf(tmp, "%lld", days);
    boost::replace_all(s, "%d", tmp);
    boost::replace_all(s, "%D", days > 0 ? tmp : "");
    boost::replace_all(s, "%T", "%H:%M:%S");
    sprintf(tmp, "%02lld", (ns_ / 3'600'000'000'000ll) % 24);
    boost::replace_all(s, "%H", tmp);
    sprintf(tmp, "%02lld", (ns_ / 60'000'000'000ll) % 60);
    boost::replace_all(s, "%M", tmp);
    sprintf(tmp, "%02lld", (ns_ / 1'000'000'000ll) % 60);
    boost::replace_all(s, "%S", tmp);
    sprintf(tmp, "%03lld", (ns_ / 1'000'000ll) % 1000);
    boost::replace_all(s, "%3", tmp);
    sprintf(tmp, "%03lld", (ns_ / 1'000ll) % 1000);
    boost::replace_all(s, "%6", tmp);
    sprintf(tmp, "%03lld", ns_ % 1000ll);
    boost::replace_all(s, "%9", tmp);
  }

  return s;
}
