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

#include <src/lib/lib_helpers.h>
#include <src/objects/configuration.h>

#include <nlohmann/json-schema.hpp>

YOGI_API int YOGI_ConfigurationCreate(void** config, int flags) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(config != nullptr);
  CHECK_FLAGS(flags, YOGI_CFG_DISABLE_VARIABLES | YOGI_CFG_MUTABLE_CMD_LINE);

  auto cfg = Configuration::create(flags);
  *config  = ObjectRegister::register_object(cfg);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ConfigurationUpdateFromCommandLine(void* config, int argc, const char* const* argv, int options) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(argc > 0);
  CHECK_PARAM(argv != nullptr);
  CHECK_FLAGS(options, YOGI_CLO_ALL);

  auto cfg = ObjectRegister::get<Configuration>(config);
  cfg->update_from_cmdline(argc, argv, options);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ConfigurationUpdateFromJson(void* config, const char* json) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(json != nullptr);

  auto cfg = ObjectRegister::get<Configuration>(config);
  cfg->update_from_string(json);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ConfigurationUpdateFromFile(void* config, const char* filename) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(filename != nullptr);

  auto cfg = ObjectRegister::get<Configuration>(config);
  cfg->update_from_file(filename);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ConfigurationDump(void* config, const char** json, int* jsonsize, int resvars, int indent) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(resvars == YOGI_TRUE || resvars == YOGI_FALSE);
  CHECK_PARAM(indent >= -1);

  auto cfg = ObjectRegister::get<Configuration>(config);
  auto s   = cfg->dump(resvars == YOGI_TRUE, indent);
  set_api_buffer(std::move(s), json, jsonsize);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ConfigurationWriteToFile(void* config, const char* filename, int resvars, int indent) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(filename != nullptr);
  CHECK_PARAM(resvars == YOGI_TRUE || resvars == YOGI_FALSE);
  CHECK_PARAM(indent >= -1);

  auto cfg = ObjectRegister::get<Configuration>(config);
  cfg->write_to_file(filename, resvars == YOGI_TRUE, indent);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_ConfigurationValidate(void* config, const char* section, void* schema) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(config != nullptr);
  CHECK_PARAM(schema != nullptr);

  auto cfg = ObjectRegister::get<Configuration>(config);
  auto jsn = cfg->get_json(section);
  auto sma = ObjectRegister::get<Configuration>(schema);

  try {
    nlohmann::json_schema::json_validator validator;
    validator.set_root_schema(sma->get_json());
    validator.validate(jsn);
  } catch (const std::exception& e) {
    throw DescriptiveError{YOGI_ERR_CONFIGURATION_VALIDATION_FAILED} << e.what();
  }

  END_CHECKED_API_FUNCTION
}
