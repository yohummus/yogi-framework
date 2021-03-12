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

#include <src/network/tcp_listener.h>
#include <src/network/tcp_transport.h>
#include <src/objects/branch/advertising_receiver.h>
#include <src/objects/branch/advertising_sender.h>
#include <src/objects/branch/branch_connection.h>
#include <src/system/network_info.h>

#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <boost/functional/hash.hpp>
#include <mutex>
#include <nlohmann/json.hpp>
#include <set>
#include <unordered_map>
#include <unordered_set>

class ConnectionManager;
typedef std::shared_ptr<ConnectionManager> ConnectionManagerPtr;
typedef std::weak_ptr<ConnectionManager> ConnectionManagerWeakPtr;

class ConnectionManager : public std::enable_shared_from_this<ConnectionManager>, public LogUser {
 public:
  typedef std::function<void(const Result&, int branch_event, const Result&, const boost::uuids::uuid&,
                             const std::string& json)>
      BranchEventHandler;
  typedef std::function<void(const Result&, BranchConnectionPtr)> ConnectionChangedHandler;
  typedef std::function<void(const IncomingMessage&, const BranchConnectionPtr&)> MessageReceiveHandler;
  typedef std::vector<std::string> BranchInfoStringsList;
  using OperationTag = MessageTransport::OperationTag;

  ConnectionManager(ContextPtr context, const nlohmann::json& cfg);
  virtual ~ConnectionManager();

  void start(LocalBranchInfoPtr info, ConnectionChangedHandler connection_changed_handler,
             MessageReceiveHandler message_handler);

  const NetworkInterfaceInfosVector& get_advertising_interfaces() const {
    return listener_->get_interfaces();
  }

  const boost::asio::ip::udp::endpoint& get_advertising_endpoint() const {
    return adv_sender_->get_endpoint();
  }

  int get_tcp_server_port() {
    return listener_->get_port();
  }

  BranchInfoStringsList make_connected_branches_info_strings() const;

  bool await_event_async(int branch_events, BranchEventHandler handler);
  bool cancel_await_event();

  template <typename Fn>
  void foreach_running_session(Fn fn) {
    std::lock_guard<std::mutex> lock(connections_mutex_);
    for (auto& entry : connections_) {
      auto& conn = entry.second;
      if (conn->session_running()) {
        fn(conn);
      }
    }
  }

  OperationTag make_operation_id();

 private:
  typedef std::unordered_set<boost::uuids::uuid, boost::hash<boost::uuids::uuid>> UuidSet;

  typedef std::unordered_map<boost::uuids::uuid, BranchConnectionPtr, boost::hash<boost::uuids::uuid>> ConnectionsMap;

  typedef ConnectionsMap::value_type ConnectionsMapEntry;
  typedef std::set<TcpTransport::ConnectGuardPtr> ConnectGuardsSet;
  typedef std::set<BranchConnectionPtr> ConnectionsSet;

  ConnectionManagerWeakPtr make_weak_ptr() {
    return {shared_from_this()};
  }

  void create_adv_sender_and_receiver(const nlohmann::json& cfg);
  void create_listener(const nlohmann::json& cfg);
  void on_accepted(boost::asio::ip::tcp::socket socket);
  void on_advertisement_received(const boost::uuids::uuid& adv_uuid, const boost::asio::ip::tcp::endpoint& ep);
  void on_connect_finished(const Result& res, const boost::uuids::uuid& adv_uuid, TcpTransportPtr transport);
  void start_exchange_branch_info(TransportPtr transport, const boost::asio::ip::address& peer_address,
                                  const boost::uuids::uuid& adv_uuid);
  void on_exchange_branch_info_finished(const Result& res, BranchConnectionPtr conn,
                                        const boost::uuids::uuid& adv_uuid);
  bool check_exchange_branch_info_error(const Result& res, const BranchConnectionPtr& conn);
  bool verify_uuids_match(const boost::uuids::uuid& remote_uuid, const boost::uuids::uuid& adv_uuid);
  bool verify_uuid_not_blacklisted(const boost::uuids::uuid& uuid);
  bool verify_connection_has_higher_priority(bool conn_already_exists, const BranchConnectionPtr& conn);
  void publish_exchange_branch_info_error(const Error& err, BranchConnectionPtr conn, ConnectionsMapEntry* entry);
  Result check_remote_branch_info(const BranchInfoPtr& remote_info);
  void start_authenticate(BranchConnectionPtr conn);
  void on_authenticate_finished(const Result& res, BranchConnectionPtr conn);
  void start_session(BranchConnectionPtr conn);
  void on_session_terminated(const Error& err, BranchConnectionPtr conn);
  BranchConnectionPtr make_connection_and_keep_it_alive(const boost::asio::ip::address& peer_address,
                                                        TransportPtr transport);
  BranchConnectionPtr stop_keeping_connection_alive(const branch_connection_weak_ptr& weak_conn);

  template <typename Fn>
  void emit_branch_event(int branch_event, const Result& ev_res, const boost::uuids::uuid& uuid, Fn make_json_fn);

  void emit_branch_event(int branch_event, const Result& ev_res, const boost::uuids::uuid& uuid);

  template <typename Fn>
  void log_branch_event(int branch_event, const Result& ev_res, Fn make_json_fn);

  const ContextPtr context_;
  boost::asio::ip::udp::endpoint adv_ep_;
  SharedBuffer password_hash_;
  ConnectionChangedHandler connection_changed_handler_;
  MessageReceiveHandler message_handler_;
  AdvertisingSenderPtr adv_sender_;
  AdvertisingReceiverPtr adv_receiver_;
  TcpListenerPtr listener_;
  ConnectGuardsSet connect_guards_;
  ConnectionsSet connections_kept_alive_;
  LocalBranchInfoPtr info_;

  UuidSet blacklisted_uuids_;
  UuidSet pending_connects_;
  ConnectionsMap connections_;
  mutable std::mutex connections_mutex_;

  std::atomic<OperationTag> last_op_tag_;

  BranchEventHandler event_handler_;
  int observed_branch_events_;
  std::recursive_mutex event_mutex_;
};
