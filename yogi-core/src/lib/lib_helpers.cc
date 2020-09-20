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

#include <src/api/errors.h>
#include <src/lib/lib_helpers.h>

#include <string_view>

thread_local std::string last_error_details;

int update_last_error_details(const DescriptiveError& e) {
  auto details = e.details();
  last_error_details.resize(details.size());
  last_error_details.assign(details.begin(), details.end());
  return e.error_code();
}

int update_last_error_details(const Result& res) {
  last_error_details.clear();
  return res.error_code();
}

const char* get_last_error_details() { return last_error_details.c_str(); }

thread_local std::string api_buffer;

void set_api_buffer(std::string&& buffer, const char** data_ptr, int* size_ptr) {
  if (size_ptr) {
    *size_ptr = static_cast<int>(buffer.size() + 1);
  }

  if (data_ptr) {
    api_buffer = std::move(buffer);
    *data_ptr  = api_buffer.c_str();
  }
}
