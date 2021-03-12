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
#include <src/objects/branch.h>
#include <src/objects/configuration.h>

namespace {

void copy_uuid_to_user_buffer(const boost::uuids::uuid& uuid, void* buffer) {
  if (buffer == nullptr) return;
  std::memcpy(buffer, &uuid, uuid.size());
}

bool copy_string_to_user_buffer(const std::string& str, char* buffer, int buffer_size) {
  if (buffer == nullptr) return true;

  auto n = std::min(str.size() + 1, static_cast<std::size_t>(buffer_size));
  std::strncpy(buffer, str.c_str(), n);
  if (str.size() + 1 > n) {
    buffer[buffer_size - 1] = '\0';
    return false;
  }

  return true;
}

nlohmann::json user_supplied_config_to_json(void* config, const char* section) {
  auto json = nlohmann::json::object();
  if (config) {
    auto cfg = ObjectRegister::get<Configuration>(config);
    json     = cfg->get_json(section);
    if (!json.is_object()) {
      throw DescriptiveError(YOGI_ERR_CONFIGURATION_SECTION_NOT_FOUND)
          << "Could not find section \"" << section << "\" in the configuration.";
    }
  }

  return json;
}

}  // anonymous namespace

YOGI_API int YOGI_BranchCreate(void** branch, void* context, void* config, const char* section) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(branch != nullptr);
  CHECK_PARAM(context != nullptr);

  auto ctx = ObjectRegister::get<Context>(context);
  auto cfg = user_supplied_config_to_json(config, section);

  auto brn = Branch::create(ctx, cfg);
  *branch  = ObjectRegister::register_object(brn);

  brn->start();

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_BranchGetInfo(void* branch, void* uuid, const char** json, int* jsonsize) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(branch != nullptr);

  auto brn = ObjectRegister::get<Branch>(branch);
  copy_uuid_to_user_buffer(brn->get_uuid(), uuid);

  auto info_string = brn->make_info_string();
  set_api_buffer(std::move(info_string), json, jsonsize);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_BranchGetConnectedBranches(void* branch, const char** json, int* jsonsize) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(branch != nullptr);

  auto brn = ObjectRegister::get<Branch>(branch);

  std::ostringstream ss;
  ss << "[";

  for (auto& entry : brn->make_connected_branches_info_strings()) {
    ss << entry << ",";
  }

  auto info_string_array = ss.str();
  if (info_string_array.size() == 1) {
    info_string_array += ']';
  } else {
    info_string_array.back() = ']';
  }

  set_api_buffer(std::move(info_string_array), json, jsonsize);

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_BranchAwaitEventAsync(void* branch, int events, void* uuid, char* json, int jsonsize,
                                        void (*fn)(int res, int event, int evres, void* userarg), void* userarg) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(branch != nullptr);
  CHECK_FLAGS(events, YOGI_BEV_ALL);
  CHECK_PARAM(json == nullptr || jsonsize > 0);
  CHECK_PARAM(fn != nullptr);

  auto brn = ObjectRegister::get<Branch>(branch);

  brn->await_event_async(events, [=](auto& res, auto event, auto& evres, auto& tmp_uuid, auto& tmp_json) {
    if (res.is_error()) {
      fn(res.value(), event, evres.value(), userarg);
      return;
    }

    copy_uuid_to_user_buffer(tmp_uuid, uuid);
    if (copy_string_to_user_buffer(tmp_json, json, jsonsize)) {
      fn(res.value(), event, evres.value(), userarg);
    } else {
      fn(YOGI_ERR_BUFFER_TOO_SMALL, event, evres.value(), userarg);
    }
  });

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_BranchCancelAwaitEvent(void* branch) {
  BEGIN_CHECKED_API_FUNCTION

  auto brn = ObjectRegister::get<Branch>(branch);
  if (!brn->cancel_await_event()) {
    throw Error{YOGI_ERR_OPERATION_NOT_RUNNING};
  }

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_BranchSendBroadcast(void* branch, int enc, const void* data, int datasize, int block) {
  BEGIN_CHECKED_API_FUNCTION_RETURN_INT

  CHECK_PARAM(branch != nullptr);
  CHECK_PARAM(enc == YOGI_ENC_JSON || enc == YOGI_ENC_MSGPACK);
  CHECK_PARAM(data != nullptr);
  CHECK_PARAM(datasize > 0);
  CHECK_PARAM(block == YOGI_TRUE || block == YOGI_FALSE);

  auto brn    = ObjectRegister::get<Branch>(branch);
  auto buffer = boost::asio::buffer(data, static_cast<std::size_t>(datasize));

  return brn->send_broadcast(Payload(buffer, enc), block == YOGI_TRUE).error_code();

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_BranchSendBroadcastAsync(void* branch, int enc, const void* data, int datasize, int retry,
                                           void (*fn)(int res, int oid, void* userarg), void* userarg) {
  BEGIN_CHECKED_API_FUNCTION_RETURN_INT

  CHECK_PARAM(branch != nullptr);
  CHECK_PARAM(enc == YOGI_ENC_JSON || enc == YOGI_ENC_MSGPACK);
  CHECK_PARAM(data != nullptr);
  CHECK_PARAM(datasize > 0);
  CHECK_PARAM(retry == YOGI_TRUE || retry == YOGI_FALSE);
  CHECK_PARAM(fn != nullptr);

  auto brn    = ObjectRegister::get<Branch>(branch);
  auto buffer = boost::asio::buffer(data, static_cast<std::size_t>(datasize));

  return brn->send_broadcast_async(Payload(buffer, enc), retry == YOGI_TRUE,
                                   [=](auto& res, auto oid) { fn(res.error_code(), oid, userarg); });

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_BranchCancelSendBroadcast(void* branch, int oid) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(branch != nullptr);
  CHECK_PARAM(oid > 0);

  auto brn = ObjectRegister::get<Branch>(branch);
  if (!brn->cancel_send_broadcast(oid)) {
    throw Error{YOGI_ERR_INVALID_OPERATION_ID};
  }

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_BranchReceiveBroadcastAsync(void* branch, void* uuid, int enc, void* data, int datasize,
                                              void (*fn)(int res, int size, void* userarg), void* userarg) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(branch != nullptr);
  CHECK_PARAM(enc == YOGI_ENC_JSON || enc == YOGI_ENC_MSGPACK);
  CHECK_PARAM(data != nullptr || datasize == 0);
  CHECK_PARAM(fn != nullptr);

  auto brn = ObjectRegister::get<Branch>(branch);
  brn->receive_broadcast(enc, boost::asio::buffer(data, static_cast<std::size_t>(datasize)),
                         [=](auto& res, auto& src_uuid, auto size) {
                           if (uuid) {
                             copy_uuid_to_user_buffer(src_uuid, uuid);
                           }

                           fn(res.value(), static_cast<int>(size), userarg);
                         });

  END_CHECKED_API_FUNCTION
}

YOGI_API int YOGI_BranchCancelReceiveBroadcast(void* branch) {
  BEGIN_CHECKED_API_FUNCTION

  CHECK_PARAM(branch != nullptr);

  auto brn = ObjectRegister::get<Branch>(branch);
  if (!brn->cancel_receive_broadcast()) {
    throw Error{YOGI_ERR_OPERATION_NOT_RUNNING};
  }

  END_CHECKED_API_FUNCTION
}
