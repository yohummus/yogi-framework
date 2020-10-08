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

#include <boost/asio/ip/udp.hpp>
#include <nlohmann/json.hpp>

nlohmann::json read_json_file(const std::string& filename);
std::chrono::nanoseconds extract_duration(const nlohmann::json& json, const char* key, long long defaultValue);
std::vector<std::string> extract_array_of_strings(const nlohmann::json& json, const char* key, const char* default_val);
std::size_t extract_size(const nlohmann::json& json, const char* key, int default_val);
boost::asio::ip::udp::endpoint extract_udp_endpoint(const nlohmann::json& json, const char* addr_key,
                                                    const std::string& default_addr, const char* port_key,
                                                    int default_port);

template <typename T>
T extract_number_with_inf_support(const nlohmann::json& json, const char* key, int default_val) {
  auto val = json.value(key, default_val);
  if (val == -1) {
    return std::numeric_limits<T>::max();
  } else {
    return static_cast<T>(val);
  }
}

inline std::size_t extract_size_with_inf_support(const nlohmann::json& json, const char* key, int default_val) {
  return extract_number_with_inf_support<std::size_t>(json, key, default_val);
}

template <typename T>
void copy_json_property(const nlohmann::json& from_section, const char* key, T&& default_value,
                        nlohmann::json* to_section) {
  (*to_section)[key] = from_section.value(key, std::forward<T>(default_value));
}
