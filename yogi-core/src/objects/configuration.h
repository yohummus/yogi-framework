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

#include <src/api/object.h>
#include <src/objects/logger/log_user.h>

#include <mutex>
#include <nlohmann/json.hpp>

class Configuration : public ExposedObjectT<Configuration, ObjectType::kConfiguration>, public LogUser {
 public:
  Configuration(int cfg_flags);

  void update_from_cmdline(int argc, const char* const* argv, int clo_flags);
  void update_from_string(const char* json_str);
  void update_from_file(const char* filename);
  nlohmann::json get_json(bool resolve_vars, const char* section = nullptr) const;
  nlohmann::json get_json(const char* section = nullptr) const;
  std::string dump(bool resolve_variables, int indentation_width) const;
  void write_to_file(const char* filename, bool resolve_vars, int indentation_width) const;

 private:
  void verify_and_merge(const nlohmann::json& json_to_merge, const nlohmann::json& immutable_json);

  const bool variables_supported_;
  const bool mutable_cmdline_;
  mutable std::mutex mutex_;

  nlohmann::json json_;
  nlohmann::json immutable_json_;
};

typedef std::shared_ptr<Configuration> ConfigurationPtr;
