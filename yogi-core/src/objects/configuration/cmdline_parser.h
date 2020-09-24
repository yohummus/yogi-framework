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

#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class CmdlineParser {
 public:
  CmdlineParser(int argc, const char* const* argv, int clo_flags);

  void parse();

  const nlohmann::json& get_files_configuration() const {
    return files_json_;
  }

  const nlohmann::json& get_direct_configuration() const {
    return direct_json_;
  }

 private:
  struct Override {
    bool json_pointer_syntax;
    nlohmann::json::json_pointer path;
    nlohmann::json value;
  };

  void add_help_options();
  void add_logging_options();
  void add_branch_options();
  void add_file_options();
  void add_override_options();
  void add_variable_options();

  void populate_variables_map();
  void handle_help_options();
  void extract_options();
  void apply_overrides();
  void load_config_files();

  void log_file_notifier(const std::string& val);
  void log_console_notifier(const std::string& val);
  void log_verbosity_notifier(const std::vector<std::string>& val);
  void file_notifier(const std::vector<std::string>& val);
  void override_notifier(const std::vector<std::string>& val);
  void variable_notifier(const std::vector<std::string>& val);

  const int argc_;
  const char* const* const argv_;
  const int clo_flags_;

  boost::program_options::options_description visible_options_;
  boost::program_options::options_description hidden_options_;
  boost::program_options::positional_options_description positional_options_;
  boost::program_options::variables_map vm_;
  nlohmann::json files_json_;
  nlohmann::json direct_json_;

  std::vector<std::string> config_files_;
  std::vector<Override> overrides_;
};
