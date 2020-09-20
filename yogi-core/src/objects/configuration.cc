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

#include <src/objects/configuration.h>
#include <src/objects/configuration/cmdline_parser.h>

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include <string>
using namespace std::string_literals;

YOGI_DEFINE_INTERNAL_LOGGER("Configuration")

namespace {

void check_circular_variable_dependency(const std::string& var_ref, const nlohmann::json& var_val) {
  if (var_val.is_string()) {
    auto str = var_val.get<std::string>();
    if (str.find(var_ref) != std::string::npos) {
      throw DescriptiveError{YOGI_ERR_UNDEFINED_VARIABLES} << "Circular dependency in variable \""
                                                           << var_ref.substr(2, var_ref.size() - 3) << '"';
    }
  }
}

template <typename Fn>
void walk_all_elements(nlohmann::json* json, Fn fn) {
  for (auto it = json->begin(); it != json->end(); ++it) {
    if (it.value().is_structured()) {
      walk_all_elements(&it.value(), fn);
    }

    if (json->is_object()) {
      fn(it.key(), &it.value());
    } else {
      static const std::string dummy;
      fn(dummy, &it.value());
    }
  }
}

void resolve_single_variable(nlohmann::json* elem, const std::string& var_ref, const nlohmann::json& var_val) {
  if (!elem->is_string()) {
    return;
  }

  auto val = elem->get<std::string>();
  if (val == var_ref) {
    *elem = var_val;
  } else {
    auto replacement = var_val.is_string() ? var_val.get<std::string>() : var_val.dump();
    boost::replace_all(val, var_ref, replacement);
    *elem = val;
  }
}

void resolve_variables_section(nlohmann::json* vars) {
  for (auto it = vars->begin(); it != vars->end(); ++it) {
    auto var_ref = "${"s + it.key() + '}';
    auto var_val = it.value();
    check_circular_variable_dependency(var_ref, var_val);

    for (auto& elem : *vars) {
      resolve_single_variable(&elem, var_ref, var_val);
    }
  }
}

nlohmann::json resolve_variables(const nlohmann::json& unresolved_json) {
  auto json = unresolved_json;
  if (!json.count("variables")) {
    return json;
  }

  auto& vars = json["variables"];

  resolve_variables_section(&vars);

  walk_all_elements(&json, [&](const auto&, auto* elem) {
    for (auto it = vars.cbegin(); it != vars.cend(); ++it) {
      auto var_ref = "${"s + it.key() + '}';
      resolve_single_variable(elem, var_ref, it.value());
    }
  });

  return json;
}

void check_variables_only_used_in_values(nlohmann::json* json) {
  walk_all_elements(json, [=](const auto& key, auto) {
    if (key.find("${") != std::string::npos) {
      throw DescriptiveError{YOGI_ERR_VARIABLE_USED_IN_KEY} << "Found syntax for variable in key: " << key;
    }
  });
}

void check_all_variables_are_resolved(nlohmann::json* json) {
  walk_all_elements(json, [=](const auto&, const auto* elem) {
    if (!elem->is_string()) return;
    auto val = elem->template get<std::string>();
    auto pos = val.find("${");
    if (pos != std::string::npos) {
      throw DescriptiveError{YOGI_ERR_UNDEFINED_VARIABLES}
          << "Variable \"" << val.substr(pos + 2, val.find('}', pos + 2) - pos - 2) << "\" could not be resolved";
    }
  });
}

}  // anonymous namespace

Configuration::Configuration(int cfg_flags)
    : variables_supported_{!(cfg_flags & YOGI_CFG_DISABLE_VARIABLES)},
      mutable_cmdline_{!!(cfg_flags & YOGI_CFG_MUTABLE_CMD_LINE)},
      json_({}),
      immutable_json_({}) {
  set_logging_prefix(*this);
}

void Configuration::update_from_cmdline(int argc, const char* const* argv, int clo_flags) {
  CmdlineParser parser(argc, argv, clo_flags);
  parser.parse();

  verify_and_merge(parser.get_files_configuration(), parser.get_direct_configuration());

  if (!mutable_cmdline_) {
    immutable_json_ = parser.get_direct_configuration();
  }
}

void Configuration::update_from_string(const char* json_str) {
  nlohmann::json json;

  try {
    json = nlohmann::json::parse(json_str);
  } catch (const nlohmann::json::exception& e) {
    throw DescriptiveError{YOGI_ERR_PARSING_JSON_FAILED} << "Could not parse JSON string: " << e.what();
  }

  verify_and_merge(json, immutable_json_);
}

void Configuration::update_from_file(const char* filename) {
  std::ifstream f(filename);
  if (!f.is_open() || f.fail()) {
    throw DescriptiveError{YOGI_ERR_PARSING_FILE_FAILED} << "Could not open " << filename;
  }

  nlohmann::json json;
  try {
    f >> json;
  } catch (const std::exception& e) {
    throw DescriptiveError{YOGI_ERR_PARSING_FILE_FAILED} << "Could not parse " << filename << ": " << e.what();
  }

  verify_and_merge(json, immutable_json_);
}

nlohmann::json Configuration::get_json(bool resolve_vars, const char* section) const {
  nlohmann::json json;
  nlohmann::json::json_pointer jp;

  if (section != nullptr) {
    try {
      jp = nlohmann::json::json_pointer(section);
    } catch (const nlohmann::json::exception& e) {
      throw DescriptiveError{YOGI_ERR_INVALID_PARAM} << "Could not parse JSON pointer: " << e.what();
    }
  }

  if (resolve_vars) {
    if (!variables_supported_) {
      throw Error{YOGI_ERR_NO_VARIABLE_SUPPORT};
    }

    json = resolve_variables(json_)[jp];
  } else {
    json = json_[jp];
  }

  if (!json.is_object()) {
    throw DescriptiveError{YOGI_ERR_CONFIGURATION_SECTION_NOT_FOUND} << "Could not find section \"" << section
                                                                     << "\" in the configuration.";
  }

  return json;
}

nlohmann::json Configuration::get_json(const char* section) const { return get_json(variables_supported_, section); }

std::string Configuration::dump(bool resolve_variables, int indentation_width) const {
  return get_json(resolve_variables).dump(indentation_width);
}

void Configuration::write_to_file(const char* filename, bool resolve_vars, int indentation_width) const {
  if (!variables_supported_ && resolve_vars) {
    throw Error{YOGI_ERR_NO_VARIABLE_SUPPORT};
  }

  std::ofstream f(filename);
  if (!f.is_open() || f.fail()) {
    throw Error{YOGI_ERR_READ_FILE_FAILED};
  }

  try {
    if (resolve_vars) {
      f << resolve_variables(json_).dump(indentation_width);
    } else {
      f << json_.dump(indentation_width);
    }

    if (indentation_width != -1) {
      f << std::endl;
    }
  } catch (const std::exception& e) {
    LOG_ERR("Could not write configuration to " << filename << ": " << e.what());
    throw Error{YOGI_ERR_WRITE_FILE_FAILED};
  }
}

void Configuration::verify_and_merge(const nlohmann::json& json_to_merge, const nlohmann::json& immutable_json) {
  auto new_json = json_;
  new_json.merge_patch(json_to_merge);
  new_json.merge_patch(immutable_json);

  if (variables_supported_) {
    check_variables_only_used_in_values(&new_json);
    auto resolved_json = resolve_variables(new_json);
    check_all_variables_are_resolved(&resolved_json);
  }

  json_ = new_json;
}
