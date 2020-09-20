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
#include <src/schemas/schemas.h>

const char* get_schema(int schema) {
  // clang-format off
  switch (schema) {
    // :CODEGEN_BEGIN:
    case YOGI_SCM_BRANCH_CONFIG: return schemas::kBranchConfig;
    case YOGI_SCM_BRANCH_PROPERTIES: return schemas::kBranchProperties;
    case YOGI_SCM_REMOTE_BRANCH_INFO: return schemas::kRemoteBranchInfo;
    case YOGI_SCM_LOCAL_BRANCH_INFO: return schemas::kLocalBranchInfo;
    case YOGI_SCM_BRANCH_EVENT: return schemas::kBranchEvent;
    // :CODEGEN_END:
  }
  // clang-format on

  static const char dummy[] = "";
  return dummy;
}

namespace schemas {

// :CODEGEN_BEGIN:

// YOGI_SCM_BRANCH_CONFIG
const char kBranchConfig[] = R"raw({
  "$schema": "http://json-schema.org/draft-07/schema#",
  "$id": "file:///branch_config.schema.json",
  "title": "Branch configuration",
  "type": "object",
  "additionalProperties": false,
  "properties": {
    "name":                   { "$ref": "branch_properties.schema.json#/name" },
    "description":            { "$ref": "branch_properties.schema.json#/description" },
    "path":                   { "$ref": "branch_properties.schema.json#/path" },
    "network_name":           { "$ref": "branch_properties.schema.json#/network_name" },
    "network_password":       { "$ref": "branch_properties.schema.json#/network_password" },
    "advertising_interfaces": { "$ref": "branch_properties.schema.json#/advertising_interfaces" },
    "advertising_address":    { "$ref": "branch_properties.schema.json#/advertising_address" },
    "advertising_port":       { "$ref": "branch_properties.schema.json#/advertising_port" },
    "advertising_interval":   { "$ref": "branch_properties.schema.json#/advertising_interval" },
    "timeout":                { "$ref": "branch_properties.schema.json#/timeout" },
    "ghost_mode":             { "$ref": "branch_properties.schema.json#/ghost_mode" },
    "tx_queue_size":          { "$ref": "branch_properties.schema.json#/tx_queue_size" },
    "rx_queue_size":          { "$ref": "branch_properties.schema.json#/rx_queue_size" }
  }
}
)raw";

// YOGI_SCM_BRANCH_PROPERTIES
const char kBranchProperties[] = R"raw({
  "$schema": "http://json-schema.org/draft-07/schema#",
  "$id": "file:///branch_properties.schema.json",
  "properties": {
    "uuid": {
      "title": "Unique universal ID of the branch.",
      "description": "The UUID is randomly generated and uniquely identifies a branch in the network.",
      "examples": ["6ba7b810-9dad-11d1-80b4-00c04fd430c8"]
    },
    "name": {
      "title": "Branch name",
      "description": "Name of the branch as it appears in the network. By default the format is pid@hostname where pid is the process ID (PID) of the process.",
      "examples": ["12345@beaglebone", "MyBranch"],
      "type": "string"
    },
    "description": {
      "title": "Branch description",
      "description": "Arbitrary description of the branch intended to be read by humans.",
      "type": "string"
    },
    "path": {
      "title": "Branch path",
      "description": "Path of the branch in the network. Must start with a slash. Default is /name where name is the name of the branch.",
      "examples": ["/Controller", "/Storage/Backup/Database"],
      "type": "string",
      "pattern": "^/.+$"
    },
    "network_name": {
      "title": "Network",
      "description": "Name of the network to join. Only branches on the same network connect to each other. Default is /hostname.",
      "type": "string"
    },
    "network_password": {
      "title": "Network password",
      "description": "Password associated with the network. Branches only connect to each other if the passwords match. Default is no password.",
      "default": null,
      "type": ["string", "null"]
    },
    "hostname": {
      "title": "Hostname of the branch's OS.",
      "type": "string",
      "examples": ["ubuntu", "pc1234"]
    },
    "pid": {
      "title": "Process ID",
      "description": "The ID of the process running the branch, retrieved from the OS.",
      "type": "integer",
      "examples": [4124]
    },
    "start_time": {
      "title": "Start time",
      "description": "Date and time when the branch was started, in ISO8601 format.",
      "type": "string",
      "format": "date-time",
      "examples": ["2018-04-23T18:25:43.511Z"]
    },
    "advertising_interfaces": {
      "title": "Advertising interfaces",
      "description": "Network interfaces to use for advertising. Valid strings are Unix device names (\"eth0\", \"en5\", \"wlan0\"), adapter names on Windows (\"Ethernet\", \"VMware Network Adapter WMnet1\") or MAC addresses (\"11:22:33:44:55:66\"). Furthermore, the special strings \"localhost\" and \"all\" can be used to denote loopback and all available interfaces respectively.",
      "type": "array",
      "items": { "type": "string", "minLength": 1 },
      "uniqueItems": true,
      "default": ["localhost"],
      "examples": [
        ["eth0", "wlan0"],
        ["Ethernet"],
        ["all"],
        ["localhost", "1A:44:E8:BB:21:01"]
      ]
    },
    "advertising_address": {
      "title": "Advertising address",
      "description": "IPv4/IPv6 Multicast address to use for advertising",
      "type": "string",
      "minLength": 2,
      "default": "ff02::8000:2439",
      "examples": ["239.255.0.1", "ff02::8000:2439"]
    },
    "advertising_port": {
      "title": "Advertising port",
      "type": "integer",
      "minimum": 1,
      "maximum": 65535,
      "default": 13531,
      "examples": [10000]
    },
    "advertising_interval": {
      "title": "Advertising interval",
      "description": "Time between advertising messages; null disables advertising.",
      "type": ["number", "null"],
      "anyOf": [{ "const": "null" }, { "minimum": 0.001 }],
      "default": 1.0
    },
    "tcp_server_port": {
      "title": "TCP port for branch connections",
      "description": "TCP port that the branch listens on for connections from other branches",
      "type": "integer",
      "minimum": 1,
      "maximum": 65535
    },
    "timeout": {
      "title": "Timeout",
      "description": "Amount of time of inactivity before a connection is considered to be broken; null disables the timeout.",
      "type": "number",
      "anyOf": [{ "const": "null" }, { "minimum": 0.001 }],
      "default": 3.0
    },
    "ghost_mode": {
      "title": "Ghost mode",
      "description": "Ghost mode enabled, i.e. the branch does not connect to any other branches and just listens for their advertising messages.",
      "type": "boolean",
      "default": false
    },
    "tx_queue_size": {
      "title": "Send queue size",
      "description": "Size of the send queues for remote branches.",
      "type": "integer",
      "minimum": 35000,
      "maximum": 10000000,
      "default": 35000
    },
    "rx_queue_size": {
      "title": "Receive queue size",
      "description": "Size of the receive queues for remote branches.",
      "type": "integer",
      "minimum": 35000,
      "maximum": 10000000,
      "default": 35000
    }
  }
}
)raw";

// YOGI_SCM_REMOTE_BRANCH_INFO
const char kRemoteBranchInfo[] = R"raw({
  "$schema": "http://json-schema.org/draft-07/schema#",
  "$id": "file:///remote_branch_info.schema.json",
  "additionalProperties": false,
  "properties": {
    "uuid":                   { "$ref": "branch_properties.schema.json#/uuid" },
    "name":                   { "$ref": "branch_properties.schema.json#/name" },
    "description":            { "$ref": "branch_properties.schema.json#/description" },
    "network_name":           { "$ref": "branch_properties.schema.json#/network_name" },
    "path":                   { "$ref": "branch_properties.schema.json#/path" },
    "hostname":               { "$ref": "branch_properties.schema.json#/hostname" },
    "pid":                    { "$ref": "branch_properties.schema.json#/pid" },
    "tcp_server_address":     { "$ref": "branch_properties.schema.json#/tcp_server_address" },
    "tcp_server_port":        { "$ref": "branch_properties.schema.json#/tcp_server_port" },
    "start_time":             { "$ref": "branch_properties.schema.json#/start_time" },
    "timeout":                { "$ref": "branch_properties.schema.json#/timeout" },
    "advertising_interval":   { "$ref": "branch_properties.schema.json#/advertising_interval" },
    "ghost_mode":             { "$ref": "branch_properties.schema.json#/ghost_mode" }
  }
}
)raw";

// YOGI_SCM_LOCAL_BRANCH_INFO
const char kLocalBranchInfo[] = R"raw({
  "$schema": "http://json-schema.org/draft-07/schema#",
  "$id": "file:///local_branch_info.schema.json",
  "additionalProperties": false,
  "properties": {
    "uuid":                   { "$ref": "branch_properties.schema.json#/uuid" },
    "name":                   { "$ref": "branch_properties.schema.json#/name" },
    "description":            { "$ref": "branch_properties.schema.json#/description" },
    "network_name":           { "$ref": "branch_properties.schema.json#/network_name" },
    "path":                   { "$ref": "branch_properties.schema.json#/path" },
    "hostname":               { "$ref": "branch_properties.schema.json#/hostname" },
    "pid":                    { "$ref": "branch_properties.schema.json#/pid" },
    "advertising_interfaces": { "$ref": "branch_properties.schema.json#/advertising_interfaces" },
    "advertising_address":    { "$ref": "branch_properties.schema.json#/advertising_address" },
    "advertising_port":       { "$ref": "branch_properties.schema.json#/advertising_port" },
    "advertising_interval":   { "$ref": "branch_properties.schema.json#/advertising_interval" },
    "tcp_server_port":        { "$ref": "branch_properties.schema.json#/tcp_server_port" },
    "timeout":                { "$ref": "branch_properties.schema.json#/timeout" },
    "start_time":             { "$ref": "branch_properties.schema.json#/start_time" },
    "ghost_mode":             { "$ref": "branch_properties.schema.json#/ghost_mode" },
    "tx_queue_size":          { "$ref": "branch_properties.schema.json#/tx_queue_size" },
    "rx_queue_size":          { "$ref": "branch_properties.schema.json#/rx_queue_size" }
  }
}
)raw";

// YOGI_SCM_BRANCH_EVENT
const char kBranchEvent[] = R"raw({
  "$schema": "http://json-schema.org/draft-07/schema#",
  "$id": "file:///branch_event.schema.json",
  "additionalProperties": false,
  "oneOf": [{
    "properties": {
      "uuid":               { "$ref": "branch_properties.schema.json#/uuid" }
    }
  }, {
    "properties": {
      "uuid":               { "$ref": "branch_properties.schema.json#/uuid" },
      "tcp_server_address": { "$ref": "branch_properties.schema.json#/tcp_server_address" },
      "tcp_server_port":    { "$ref": "branch_properties.schema.json#/tcp_server_port" }
    }
  }, {
    "$ref": "remote_branch_info.schema.json"
  }]
}
)raw";
// :CODEGEN_END:

}  // namespace schemas
