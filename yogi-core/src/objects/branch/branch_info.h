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

#include <src/api/errors.h>
#include <src/data/buffer.h>
#include <src/objects/logger/log_user.h>
#include <src/system/network_info.h>
#include <src/util/time.h>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

class BranchInfo : public LogUser {
 public:
  enum {
    kAdvertisingMessageSize = 25,
    kInfoMessageHeaderSize  = kAdvertisingMessageSize + 4,
  };

  virtual ~BranchInfo() = default;

  const boost::uuids::uuid& get_uuid() const {
    return uuid_;
  }

  const std::string& get_name() const {
    return name_;
  }

  const std::string& get_description() const {
    return description_;
  }

  const std::string& get_network_name() const {
    return net_name_;
  }

  const std::string& get_path() const {
    return path_;
  }

  const std::string& get_hostname() const {
    return hostname_;
  }

  int get_pid() const {
    return pid_;
  }

  unsigned short get_tcp_server_port() const {
    return tcp_server_port_;
  }

  const Timestamp& get_start_time() const {
    return start_time_;
  }

  const std::chrono::nanoseconds& get_timeout() const {
    return timeout_;
  }

  const std::chrono::nanoseconds& get_advertising_interval() const {
    return adv_interval_;
  }

  bool get_ghost_mode() const {
    return ghost_mode_;
  }

  const nlohmann::json& to_json() const {
    return json_;
  }

 protected:
  void populate_json();

  boost::uuids::uuid uuid_;
  std::string name_;
  std::string description_;
  std::string net_name_;
  std::string path_;
  std::string hostname_;
  int pid_;
  unsigned short tcp_server_port_;
  Timestamp start_time_;
  std::chrono::nanoseconds timeout_;
  std::chrono::nanoseconds adv_interval_;
  bool ghost_mode_;
  nlohmann::json json_;
};

typedef std::shared_ptr<BranchInfo> BranchInfoPtr;

class LocalBranchInfo;
typedef std::shared_ptr<LocalBranchInfo> LocalBranchInfoPtr;

class LocalBranchInfo : public BranchInfo {
 public:
  LocalBranchInfo(const nlohmann::json& cfg, const NetworkInterfaceInfosVector& adv_ifs,
                  unsigned short tcp_server_port);

  const NetworkInterfaceInfosVector& get_advertising_interfaces() const {
    return adv_ifs_;
  }

  const boost::asio::ip::udp::endpoint& get_advertising_endpoint() const {
    return adv_ep_;
  }

  std::size_t get_tx_queue_size() const {
    return tx_queue_size_;
  }

  std::size_t get_rx_queue_size() const {
    return rx_queue_size_;
  }

  std::size_t get_transceive_byte_limit() const {
    return txrx_byte_limit_;
  }

  SharedBuffer make_advertising_message() const {
    YOGI_ASSERT(adv_msg_);
    return adv_msg_;
  }

  SharedBuffer make_info_message() const {
    YOGI_ASSERT(info_msg_);
    return info_msg_;
  };

 private:
  void populate_messages();
  void populate_json_with_local_info();

  NetworkInterfaceInfosVector adv_ifs_;
  boost::asio::ip::udp::endpoint adv_ep_;
  std::size_t tx_queue_size_;
  std::size_t rx_queue_size_;
  std::size_t txrx_byte_limit_;
  SharedBuffer adv_msg_;
  SharedBuffer info_msg_;
};

class RemoteBranchInfo;
typedef std::shared_ptr<RemoteBranchInfo> RemoteBranchInfoPtr;

class RemoteBranchInfo : public BranchInfo {
 public:
  RemoteBranchInfo(const Buffer& info_msg, const boost::asio::ip::address& addr);

  static Result deserialize_advertising_message(boost::uuids::uuid* uuid, unsigned short* tcp_port,
                                                const Buffer& adv_msg);

  static Result deserialize_info_message_body_size(std::size_t* body_size, const Buffer& info_msg_hdr);

 private:
  static Result check_magic_prefix_and_version(const Buffer& adv_msg);
};

// Format like this: [6ba7b810-9dad-11d1-80b4-00c04fd430c8]
std::ostream& operator<<(std::ostream& os, const BranchInfoPtr& info);
std::ostream& operator<<(std::ostream& os, const LocalBranchInfoPtr& info);
std::ostream& operator<<(std::ostream& os, const RemoteBranchInfoPtr& info);
