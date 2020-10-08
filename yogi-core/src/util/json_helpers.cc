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
#include <src/util/json_helpers.h>

#include <string.h>
#include <fstream>
#include <limits>

nlohmann::json read_json_file(const std::string& filename) {
  std::ifstream ifs(filename);
  if (!ifs.is_open() || ifs.fail()) {
    throw DescriptiveError(YOGI_ERR_READ_FILE_FAILED)
        << "The file " << filename << " does not exist or is not readable.";
  }

  nlohmann::json json;
  try {
    ifs >> json;
  } catch (const std::exception& e) {
    throw DescriptiveError(YOGI_ERR_PARSING_FILE_FAILED) << "Could not parse " << filename << ": " << e.what();
  }

  return json;
}

std::chrono::nanoseconds extract_duration(const nlohmann::json& json, const char* key, long long defaultValue) {
  float seconds = json.value(key, static_cast<float>(defaultValue) / 1e9f);
  if (seconds == -1) {
    return (std::chrono::nanoseconds::max)();
  }

  YOGI_ASSERT(seconds >= 0);

  auto ns = static_cast<long long>(seconds * 1e9f);
  return std::chrono::nanoseconds(ns);
}

std::vector<std::string> extract_array_of_strings(const nlohmann::json& json, const char* key,
                                                  const char* default_val) {
  std::vector<std::string> v;
  nlohmann::json json_vec;

  if (json.count(key)) {
    json_vec = json[key];
  } else {
    json_vec = nlohmann::json::parse(default_val);
  }

  YOGI_ASSERT(json_vec.is_array());

  for (auto& elem : json_vec) {
    YOGI_ASSERT(elem.is_string());
    v.push_back(elem.get<std::string>());
  }

  return v;
}

std::size_t extract_size(const nlohmann::json& json, const char* key, int default_val) {
  auto val = json.value(key, default_val);
  return static_cast<std::size_t>(val);
}

boost::asio::ip::udp::endpoint extract_udp_endpoint(const nlohmann::json& json, const char* addr_key,
                                                    const std::string& default_addr, const char* port_key,
                                                    int default_port) {
  auto adv_addr = json.value<std::string>(addr_key, default_addr);
  YOGI_ASSERT(!adv_addr.empty());

  auto adv_port = json.value(port_key, default_port);
  YOGI_ASSERT(0 < adv_port && adv_port < 65536);

  boost::system::error_code ec;
  auto adv_ep = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(adv_addr, ec),
                                               static_cast<unsigned short>(adv_port));
  if (ec) {
    throw DescriptiveError(YOGI_ERR_INVALID_PARAM) << "Could not parse address in property \"" << addr_key << "\".";
  }

  return adv_ep;
}
