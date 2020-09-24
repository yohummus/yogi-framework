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

#ifndef _YOGI_CONFIGURATION_H
#define _YOGI_CONFIGURATION_H

//! \file
//!
//! Configuration objects.

#include "enums.h"
#include "io.h"
#include "json_view.h"
#include "object.h"
#include "string_view.h"

#include <memory>
#include <type_traits>

namespace yogi {

class Configuration;

/// Shared pointer to a configuration.
using ConfigurationPtr = std::shared_ptr<Configuration>;

////////////////////////////////////////////////////////////////////////////////
/// A configuration represents a set of parameters that usually remain constant
/// throughout the runtime of a program.
///
/// Parameters can come from different sources such as the command line or a
/// file. Configurations are used for other parts of the library such as
/// application objects, however, they are also intended to store user-defined
/// parameters.
////////////////////////////////////////////////////////////////////////////////
class Configuration : public ObjectT<Configuration> {
 public:
  /// Creates a configuration.
  ///
  /// \param flags Flags for behaviour adjustments.
  ///
  /// \returns The created configuration.
  static ConfigurationPtr create(ConfigurationFlags flags = ConfigurationFlags::kNone) {
    return ConfigurationPtr(new Configuration(flags));
  }

  /// Returns the flags set for the configuration.
  ///
  /// \return Flags set for the configuration.
  ConfigurationFlags flags() const {
    return flags_;
  }

  /// Updates the configuration from command line options.
  ///
  /// If parsing the command line, files or any given JSON string fails, or
  /// if help is requested (e.g. by using the --help switch) then a
  /// DetailedFailureException will be raised containing detailed
  /// information about the error or the help text.
  ///
  /// \param argc    Number of command line arguments in \p argv.
  /// \param argv    Command line arguments as passed to main().
  /// \param options Options to provide on the command line.
  void update_from_command_line(int argc, const char* const* argv,
                                CommandLineOptions options = CommandLineOptions::kNone) {
    int res = detail::YOGI_ConfigurationUpdateFromCommandLine(handle(), argc, argv, static_cast<int>(options));
    detail::check_error_code(res);
  }

  /// Updates the configuration from a serialized JSON object.
  ///
  /// If parsing fails then a DetailedFailureException will be raised
  /// containing detailed information about the error.
  ///
  /// \param json JSON object or serialized JSON.
  void update_from_json(const JsonView& json) {
    int res = detail::YOGI_ConfigurationUpdateFromJson(handle(), json);
    detail::check_error_code(res);
  }

  /// Updates the configuration from a JSON file.
  ///
  /// If parsing the file fails then a DetailedFailureException will be
  /// raised containing detailed information about the error.
  ///
  /// \param filename Path to the JSON file.
  void update_from_file(const StringView& filename) {
    int res = detail::YOGI_ConfigurationUpdateFromFile(this->handle(), filename);
    detail::check_error_code(res);
  }

  /// Retrieves the configuration as a JSON-formatted string.
  ///
  /// \param resolve_variables Resolve all configuration variables.
  /// \param indentation       Number of space characters to use for indentation; must be >= 0.
  ///
  /// \returns The configuration serialized to a string.
  std::string dump(bool resolve_variables, int indentation) const {
    if (indentation < 0) {
      throw FailureException(ErrorCode::kInvalidParam);
    }

    return dump_impl(resolve_variables, indentation);
  }

  /// Retrieves the configuration as a JSON-formatted string.
  ///
  /// No indentation and no newlines will be generated; i.e. the returned string
  /// will be as compact as possible.
  ///
  /// \param resolve_variables Resolve all configuration variables.
  ///
  /// \returns The configuration serialized to a string.
  std::string dump(bool resolve_variables) const {
    return dump_impl(resolve_variables, -1);
  }

  /// Retrieves the configuration as a JSON-formatted string.
  ///
  /// %Configuration variables get resolved if the configuration supports them.
  ///
  /// \param indentation Number of space characters to use for indentation; must be >= 0.
  ///
  /// \returns The configuration serialized to a string.
  std::string dump(int indentation) const {
    if (indentation < 0) {
      throw FailureException(ErrorCode::kInvalidParam);
    }

    bool resolve_variables = (flags_ & ConfigurationFlags::kDisableVariables) == ConfigurationFlags::kNone;
    return dump_impl(resolve_variables, indentation);
  }

  /// Retrieves the configuration as a JSON-formatted string.
  ///
  /// %Configuration variables get resolved if the configuration supports them.
  ///
  /// No indentation and no newlines will be generated; i.e. the returned string
  /// will be as compact as possible.
  ///
  /// \returns The configuration serialized to a string.
  std::string dump() const {
    bool resolve_variables = (flags_ & ConfigurationFlags::kDisableVariables) == ConfigurationFlags::kNone;
    return dump_impl(resolve_variables, -1);
  }

  /// Writes the configuration to a file in JSON format.
  ///
  /// \param filename          Path to the output file.
  /// \param resolve_variables Resolve all configuration variables.
  /// \param indentation       Number of space characters to use forindentation; must be >= 0.
  void write_to_file(const StringView& filename, bool resolve_variables, int indentation) const {
    if (indentation < 0) {
      throw FailureException(ErrorCode::kInvalidParam);
    }

    write_to_file_impl(filename, resolve_variables, indentation);
  }

  /// Writes the configuration to a file in JSON format.
  ///
  /// No indentation and no newlines will be generated; i.e. the returned string
  /// will be as compact as possible.
  ///
  /// \param filename          Path to the output file.
  /// \param resolve_variables Resolve all configuration variables.
  void write_to_file(const StringView& filename, bool resolve_variables) const {
    write_to_file_impl(filename, resolve_variables, -1);
  }

  /// Writes the configuration to a file in JSON format.
  ///
  /// %Configuration variables get resolved if the configuration supports them.
  ///
  /// \param filename    Path to the output file.
  /// \param indentation Number of space characters to use for indentation; must
  ///                    be >= 0.
  void write_to_file(const StringView& filename, int indentation) const {
    if (indentation < 0) {
      throw FailureException(ErrorCode::kInvalidParam);
    }

    bool resolve_variables = (flags_ & ConfigurationFlags::kDisableVariables) == ConfigurationFlags::kNone;
    write_to_file_impl(filename, resolve_variables, indentation);
  }

  /// Writes the configuration to a file in JSON format.
  ///
  /// %Configuration variables get resolved if the configuration supports them.
  ///
  /// No indentation and no newlines will be generated; i.e. the returned string
  /// will be as compact as possible.
  ///
  /// \param filename Path to the output file.
  void write_to_file(const StringView& filename) const {
    bool resolve_variables = (flags_ & ConfigurationFlags::kDisableVariables) == ConfigurationFlags::kNone;
    write_to_file_impl(filename, resolve_variables, -1);
  }

  /// Validates a section of the configuration against a JSON Schema.
  ///
  /// The validation is based on JSON Schema draft-07, see
  /// http://json-schema.org/. The schema to validate against has to be
  /// supplied in \p schema which needs to be a configuration object itself.
  ///
  /// If the validation fails, the kConfigurationValidationFailed error will be
  /// thrown, containing a human-readable description about the failure.
  ///
  /// \param section Section in the configuration to validate; syntax is JSON pointer (RFC 6901)
  /// \param schema  The schema to use
  void validate(const StringView& section, const Configuration& schema) {
    int res = detail::YOGI_ConfigurationValidate(handle(), section, get_foreign_handle(schema));
    detail::check_error_code(res);
  }

  /// Validates the configuration against a JSON Schema.
  ///
  /// The validation is based on JSON Schema draft-07, see
  /// http://json-schema.org/. The schema to validate against has to be
  /// supplied in \p schema which needs to be a configuration object itself.
  ///
  /// If the validation fails, the kConfigurationValidationFailed error will be
  /// thrown, containing a human-readable description about the failure.
  ///
  /// \param schema The schema to use
  void validate(const Configuration& schema) {
    validate({}, schema);
  }

 private:
  Configuration(ConfigurationFlags flags)
      : ObjectT(detail::call_api_create(detail::YOGI_ConfigurationCreate, static_cast<int>(flags)), {}), flags_(flags) {
  }

  std::string dump_impl(bool resolve_variables, int indentation) const {
    const char* json;
    int jsonsize;
    int res = detail::YOGI_ConfigurationDump(handle(), &json, &jsonsize, resolve_variables ? 1 : 0, indentation);
    detail::check_error_code(res);
    return std::string(json, static_cast<std::string::size_type>(jsonsize - 1));
  }

  void write_to_file_impl(const char* filename, bool resolve_variables, int indentation) const {
    int res = detail::YOGI_ConfigurationWriteToFile(handle(), filename, resolve_variables ? 1 : 0, indentation);
    detail::check_error_code(res);
  }

  const ConfigurationFlags flags_;
};

}  // namespace yogi

#endif  // _YOGI_CONFIGURATION_H
