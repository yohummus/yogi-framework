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
#include <src/api/version.h>

#include <regex>

void check_bindings_compatibility(const std::string& bindings_version) {
  static const std::regex re("([0-9]+)\\.([0-9]+)\\.[0-9]+[^ \\s]*");

  std::smatch m;
  if (!std::regex_match(bindings_version, m, re) || m.size() != 3) {
    throw DescriptiveError{YOGI_ERR_INVALID_PARAM} << "The supplied version number \"" << bindings_version
                                                   << "\" does not have a valid format. It must have the format "
                                                      "<major>.<minor>.<patch>[<suffix>].";
  }

  int major = std::stoi(m[1]);
  int minor = std::stoi(m[2]);
  if (major != constants::kVersionMajor || minor < constants::kVersionMinor) {
    throw DescriptiveError{YOGI_ERR_INCOMPATIBLE_VERSION}
        << "The bindings version \"" << bindings_version << "\" is incompatible with the Yogi Core library version \""
        << constants::kVersion << "\". A core library with a major version of " << major
        << " and a minor version of at least " << minor << " is required.";
  }
}
