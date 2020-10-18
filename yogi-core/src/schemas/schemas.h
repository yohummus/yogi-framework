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

#include <nlohmann/json.hpp>

const char* get_schema(int schema);
void validate_json(const nlohmann::json& json, int schema, const std::string& error_location = {});

namespace schemas {

// :CODEGEN_BEGIN:
extern const char kBranchConfig[];      // YOGI_SCM_BRANCH_CONFIG
extern const char kBranchProperties[];  // YOGI_SCM_BRANCH_PROPERTIES
extern const char kRemoteBranchInfo[];  // YOGI_SCM_REMOTE_BRANCH_INFO
extern const char kLocalBranchInfo[];   // YOGI_SCM_LOCAL_BRANCH_INFO
extern const char kBranchEvent[];       // YOGI_SCM_BRANCH_EVENT
// :CODEGEN_END:

}  // namespace schemas
