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
#include <src/network/serialize.h>
#include <src/objects/branch/branch_info.h>
#include <src/system/network_info.h>
#include <src/system/process.h>
#include <src/util/json_helpers.h>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <string>
using namespace std::string_literals;

namespace {

template <typename Field>
void deserialize_field(Field* field, const Buffer& msg, Buffer::const_iterator* it) {
  if (!deserialize<Field>(field, msg, it)) {
    throw Error(YOGI_ERR_DESERIALIZE_MSG_FAILED);
  }
}

}  // anonymous namespace

void BranchInfo::populate_json() {
  float timeout = -1;
  if (timeout_ != timeout_.max()) {
    timeout = static_cast<float>(timeout_.count()) / 1e9f;
  }

  float adv_interval = -1;
  if (adv_interval_ != adv_interval_.max()) {
    adv_interval = static_cast<float>(adv_interval_.count()) / 1e9f;
  }

  auto uuid_str = boost::uuids::to_string(uuid_);
  set_logging_prefix("["s + uuid_str + ']');

  json_ = {
      {"uuid", uuid_str},
      {"name", name_},
      {"description", description_},
      {"network_name", net_name_},
      {"path", path_},
      {"hostname", hostname_},
      {"pid", pid_},
      {"tcp_server_port", tcp_server_port_},
      {"start_time", start_time_.to_javascript_string()},
      {"timeout", timeout},
      {"advertising_interval", adv_interval},
      {"ghost_mode", ghost_mode_},
  };
}

LocalBranchInfo::LocalBranchInfo(const nlohmann::json& cfg, const NetworkInterfaceInfosVector& adv_ifs,
                                 unsigned short tcp_server_port) {
  // clang-format off
  uuid_            = boost::uuids::random_generator()();
  name_            = cfg.value("name", std::to_string(get_process_id()) + '@' + get_hostname());
  description_     = cfg.value("description", std::string{});
  net_name_        = cfg.value("network_name", get_hostname());
  path_            = cfg.value("path", "/"s + name_);
  hostname_        = get_hostname();
  pid_             = get_process_id();
  adv_ifs_         = adv_ifs;
  tcp_server_port_ = tcp_server_port;
  start_time_      = Timestamp::now();
  timeout_         = extract_duration(cfg, "timeout", constants::kDefaultConnectionTimeout);
  adv_interval_    = extract_duration(cfg, "advertising_interval", constants::kDefaultAdvInterval);
  ghost_mode_      = cfg.value("ghost_mode", false);
  adv_ep_          = extract_udp_endpoint(cfg, "advertising_address", constants::kDefaultAdvAddress, "advertising_port", constants::kDefaultAdvPort);
  tx_queue_size_   = extract_size(cfg, "tx_queue_size", constants::kDefaultTxQueueSize);
  rx_queue_size_   = extract_size(cfg, "rx_queue_size", constants::kDefaultRxQueueSize);
  txrx_byte_limit_ = extract_size_with_inf_support(cfg, "_transceive_byte_limit", -1);
  // clang-format on

  populate_messages();
  populate_json();
  populate_json_with_local_info();
}

void LocalBranchInfo::populate_messages() {
  Buffer buffer{'Y', 'O', 'G', 'I', 0};
  buffer.push_back(constants::kVersionMajor);
  buffer.push_back(constants::kVersionMinor);
  serialize(&buffer, uuid_);
  serialize(&buffer, tcp_server_port_);

  YOGI_ASSERT(buffer.size() == kAdvertisingMessageSize);
  adv_msg_ = make_shared_buffer(buffer);

  info_msg_ = make_shared_buffer(buffer);

  buffer.clear();
  serialize(&buffer, name_);
  serialize(&buffer, description_);
  serialize(&buffer, net_name_);
  serialize(&buffer, path_);
  serialize(&buffer, hostname_);
  serialize(&buffer, pid_);
  serialize(&buffer, start_time_);
  serialize(&buffer, timeout_);
  serialize(&buffer, adv_interval_);
  serialize(&buffer, ghost_mode_);

  serialize(&*info_msg_, buffer.size());
  YOGI_ASSERT(info_msg_->size() == kInfoMessageHeaderSize);
  info_msg_->insert(info_msg_->end(), buffer.begin(), buffer.end());
}

void LocalBranchInfo::populate_json_with_local_info() {
  nlohmann::json::array_t ifs;
  for (auto& info : adv_ifs_) {
    nlohmann::json::array_t addrs;
    for (auto& addr : info.addresses) {
      addrs.push_back(addr.to_string());
    }

    auto entry = nlohmann::json{{"name", info.name},
                                {"identifier", info.identifier},
                                {"mac", info.mac},
                                {"addresses", addrs},
                                {"is_loopback", info.is_loopback}};
    ifs.push_back(entry);
  }

  json_["advertising_interfaces"] = ifs;
  json_["advertising_address"]    = make_ip_address_string(adv_ep_);
  json_["advertising_port"]       = adv_ep_.port();
  json_["tx_queue_size"]          = tx_queue_size_;
  json_["rx_queue_size"]          = rx_queue_size_;
}

RemoteBranchInfo::RemoteBranchInfo(const Buffer& info_msg, const boost::asio::ip::address& addr) {
  auto res = deserialize_advertising_message(&uuid_, &tcp_server_port_, info_msg);
  if (res.is_error()) {
    throw res.to_error();
  }

  auto it = info_msg.cbegin() + kInfoMessageHeaderSize;
  deserialize_field(&name_, info_msg, &it);
  deserialize_field(&description_, info_msg, &it);
  deserialize_field(&net_name_, info_msg, &it);
  deserialize_field(&path_, info_msg, &it);
  deserialize_field(&hostname_, info_msg, &it);
  deserialize_field(&pid_, info_msg, &it);
  deserialize_field(&start_time_, info_msg, &it);
  deserialize_field(&timeout_, info_msg, &it);
  deserialize_field(&adv_interval_, info_msg, &it);
  deserialize_field(&ghost_mode_, info_msg, &it);

  populate_json();

  auto addr_str               = addr.to_string();
  addr_str                    = addr_str.substr(0, addr_str.find('%'));
  json_["tcp_server_address"] = addr_str;
}

Result RemoteBranchInfo::deserialize_advertising_message(boost::uuids::uuid* uuid, unsigned short* tcp_port,
                                                         const Buffer& adv_msg) {
  YOGI_ASSERT(adv_msg.size() >= kAdvertisingMessageSize);

  auto res = check_magic_prefix_and_version(adv_msg);
  if (res.is_error()) {
    return res;
  }

  auto it = adv_msg.cbegin() + 7;
  deserialize(uuid, adv_msg, &it);
  deserialize(tcp_port, adv_msg, &it);

  return Success();
}

Result RemoteBranchInfo::deserialize_info_message_body_size(std::size_t* body_size, const Buffer& info_msg_hdr) {
  YOGI_ASSERT(info_msg_hdr.size() >= kInfoMessageHeaderSize);

  auto res = check_magic_prefix_and_version(info_msg_hdr);
  if (res.is_error()) {
    return res;
  }

  auto it = info_msg_hdr.cbegin() + kAdvertisingMessageSize;
  deserialize(body_size, info_msg_hdr, &it);

  return Success();
}

Result RemoteBranchInfo::check_magic_prefix_and_version(const Buffer& adv_msg) {
  YOGI_ASSERT(adv_msg.size() >= kAdvertisingMessageSize);
  if (std::memcmp(adv_msg.data(), "YOGI", 5)) {
    return Error(YOGI_ERR_INVALID_MAGIC_PREFIX);
  }

  if (adv_msg[5] != constants::kVersionMajor) {
    return Error(YOGI_ERR_INCOMPATIBLE_VERSION);
  }

  return Success();
}

std::ostream& operator<<(std::ostream& os, const BranchInfoPtr& info) {
  if (info) {
    os << '[' << info->get_uuid() << ']';
  } else {
    os << "[########-####-####-####-############]";
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const LocalBranchInfoPtr& info) {
  return os << std::static_pointer_cast<BranchInfo>(info);
}

std::ostream& operator<<(std::ostream& os, const RemoteBranchInfoPtr& info) {
  return os << std::static_pointer_cast<BranchInfo>(info);
}
