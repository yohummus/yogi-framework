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
#include <src/data/crypto.h>
#include <src/objects/branch/connection_manager.h>
#include <src/system/network_info.h>
#include <src/util/bind.h>
#include <src/util/json_helpers.h>

#include <algorithm>
#include <boost/uuid/uuid_io.hpp>

YOGI_DEFINE_INTERNAL_LOGGER("Branch.ConnectionManager")

ConnectionManager::ConnectionManager(ContextPtr context, const nlohmann::json& cfg)
    : context_(context), last_op_tag_(0), observed_branch_events_(YOGI_BEV_NONE) {
  password_hash_ = make_shared_buffer(make_sha256(cfg.value("network_password", std::string{})));

  create_adv_sender_and_receiver(cfg);
  create_listener(cfg);

  YOGI_ASSERT(adv_ep_.port() != 0);
}

ConnectionManager::~ConnectionManager() {
  cancel_await_event();
}

void ConnectionManager::start(LocalBranchInfoPtr info, ConnectionChangedHandler connection_changed_handler,
                              MessageReceiveHandler message_handler) {
  info_                       = info;
  connection_changed_handler_ = connection_changed_handler;
  message_handler_            = message_handler;

  set_logging_prefix(info->logging_prefix());

  listener_->start(bind_weak(&ConnectionManager::on_accepted, this));
  adv_receiver_->start(info, bind_weak(&ConnectionManager::on_advertisement_received, this));
  adv_sender_->start(info);

  LOG_DBG("Started ConnectionManager with TCP server port " << info_->get_tcp_server_port()
                                                            << (info_->get_ghost_mode() ? " in ghost mode" : ""));
}

ConnectionManager::BranchInfoStringsList ConnectionManager::make_connected_branches_info_strings() const {
  BranchInfoStringsList branches;

  std::lock_guard<std::mutex> lock(connections_mutex_);
  for (auto& entry : connections_) {
    YOGI_ASSERT(entry.second);
    if (entry.second->session_running()) {
      branches.push_back(std::make_pair(entry.first, entry.second->make_info_string()));
    }
  }

  return branches;
}

bool ConnectionManager::await_event_async(int branch_events, BranchEventHandler handler) {
  std::lock_guard<std::recursive_mutex> lock(event_mutex_);

  bool canceled = false;
  if (event_handler_) {
    canceled         = true;
    auto old_handler = event_handler_;
    context_->post([old_handler] { old_handler(Error(YOGI_ERR_CANCELED), YOGI_BEV_NONE, Success(), {}, {}); });
  }

  observed_branch_events_ = branch_events;
  event_handler_          = handler;

  return canceled;
}

bool ConnectionManager::cancel_await_event() {
  return await_event_async(YOGI_BEV_NONE, {});
}

ConnectionManager::OperationTag ConnectionManager::make_operation_id() {
  OperationTag tag;
  do {
    tag = ++last_op_tag_;
  } while (tag == 0);

  return tag;
}

void ConnectionManager::create_adv_sender_and_receiver(const nlohmann::json& cfg) {
  adv_ep_ = extract_udp_endpoint(cfg, "advertising_address", constants::kDefaultAdvAddress, "advertising_port",
                                 constants::kDefaultAdvPort);

  adv_sender_   = std::make_shared<AdvertisingSender>(context_, adv_ep_);
  adv_receiver_ = std::make_shared<AdvertisingReceiver>(context_, adv_ep_);
}

void ConnectionManager::create_listener(const nlohmann::json&) {
  IpVersion ip_version = adv_ep_.address().is_v4() ? IpVersion::k4 : IpVersion::k6;

  listener_ = std::make_shared<TcpListener>(context_, std::vector<std::string>{"all"}, ip_version, "branch");
}

void ConnectionManager::on_accepted(boost::asio::ip::tcp::socket socket) {
  auto transport = std::make_shared<TcpTransport>(context_, std::move(socket), info_->get_timeout(),
                                                  info_->get_transceive_byte_limit(), true);

  LOG_DBG("Accepted incoming TCP connection from " << make_ip_address_string(transport->get_peer_endpoint()));

  start_exchange_branch_info(transport, transport->get_peer_endpoint().address(), {});
}

void ConnectionManager::on_advertisement_received(const boost::uuids::uuid& adv_uuid,
                                                  const boost::asio::ip::tcp::endpoint& ep) {
  std::lock_guard<std::mutex> lock(connections_mutex_);
  if (connections_.count(adv_uuid)) return;
  if (blacklisted_uuids_.count(adv_uuid)) return;
  if (pending_connects_.count(adv_uuid)) return;

  LOG_DBG("Attempting to connect to [" << adv_uuid << "] on " << make_ip_address_string(ep) << " port " << ep.port());

  auto weak_self = make_weak_ptr();
  auto guard     = TcpTransport::connect_async(context_, ep, info_->get_timeout(), info_->get_transceive_byte_limit(),
                                           [=](auto& res, auto transport, auto guard) {
                                             auto self = weak_self.lock();
                                             if (!self) return;

                                             self->connect_guards_.erase(guard);
                                             self->on_connect_finished(res, adv_uuid, transport);
                                           });

  pending_connects_.insert(adv_uuid);
  connect_guards_.insert(guard);

  emit_branch_event(YOGI_BEV_BRANCH_DISCOVERED, Success(), adv_uuid, [&] {
    return nlohmann::json{{"uuid", boost::uuids::to_string(adv_uuid)},
                          {"tcp_server_address", make_ip_address_string(ep)},
                          {"tcp_server_port", ep.port()}};
  });
}

void ConnectionManager::on_connect_finished(const Result& res, const boost::uuids::uuid& adv_uuid,
                                            TcpTransportPtr transport) {
  if (res.is_error()) {
    emit_branch_event(YOGI_BEV_BRANCH_QUERIED, res, adv_uuid);
    pending_connects_.erase(adv_uuid);
    return;
  }

  LOG_DBG("TCP connection to " << *transport << " established successfully");

  start_exchange_branch_info(transport, transport->get_peer_endpoint().address(), adv_uuid);
}

void ConnectionManager::start_exchange_branch_info(TransportPtr transport, const boost::asio::ip::address& peer_address,
                                                   const boost::uuids::uuid& adv_uuid) {
  auto conn      = make_connection_and_keep_it_alive(peer_address, transport);
  auto weak_conn = branch_connection_weak_ptr(conn);
  conn->exchange_branch_info([this, weak_conn, adv_uuid](auto& res) {
    YOGI_ASSERT(weak_conn.lock());
    this->on_exchange_branch_info_finished(res, weak_conn.lock(), adv_uuid);
    this->stop_keeping_connection_alive(weak_conn);
    this->pending_connects_.erase(adv_uuid);
  });
}

void ConnectionManager::on_exchange_branch_info_finished(const Result& res, BranchConnectionPtr conn,
                                                         const boost::uuids::uuid& adv_uuid) {
  if (!check_exchange_branch_info_error(res, conn)) {
    return;
  }

  auto remote_info  = conn->get_remote_branch_info();
  auto& remote_uuid = remote_info->get_uuid();

  if (!conn->created_from_incoming_connection_request() && !verify_uuids_match(remote_uuid, adv_uuid)) {
    return;
  }

  if (!verify_uuid_not_blacklisted(remote_uuid)) {
    return;
  }

  LOG_DBG("Successfully exchanged branch info with "
          << remote_info << " (source: " << (conn->created_from_incoming_connection_request() ? "server" : "client")
          << ")");

  std::lock_guard<std::mutex> lock(connections_mutex_);
  auto con_res             = connections_.insert(std::make_pair(remote_uuid, conn));
  bool conn_already_exists = !con_res.second;

  if (!verify_connection_has_higher_priority(conn_already_exists, conn)) {
    return;
  }

  con_res.first->second = conn;

  if (!conn_already_exists) {
    emit_branch_event(YOGI_BEV_BRANCH_QUERIED, Success(), remote_uuid, [&] { return remote_info->to_json(); });

    // If a connection already exists, the following check has already been
    // performed successfully, so we don't do it again
    auto chk_res = check_remote_branch_info(remote_info);
    if (chk_res.is_error()) {
      emit_branch_event(YOGI_BEV_CONNECT_FINISHED, chk_res, remote_uuid);
      return;
    }
  }

  if (info_->get_ghost_mode()) {
    blacklisted_uuids_.insert(remote_uuid);
  } else {
    start_authenticate(conn);
  }
}

bool ConnectionManager::check_exchange_branch_info_error(const Result& res, const BranchConnectionPtr& conn) {
  if (res.is_success()) {
    return true;
  }

  if (conn->created_from_incoming_connection_request()) {
    LOG_ERR("Exchanging branch info with server connection from " << conn->get_peer_description()
                                                                  << " failed: " << res);
  } else {
    LOG_ERR("Exchanging branch info with client connection to " << conn->get_peer_description() << " failed: " << res);
  }

  return false;
}

bool ConnectionManager::verify_uuids_match(const boost::uuids::uuid& remote_uuid, const boost::uuids::uuid& adv_uuid) {
  if (remote_uuid == adv_uuid) {
    return true;
  }

  LOG_WRN("Dropping connection since branch info UUID ["
          << remote_uuid << "] does not match advertised UUID [" << adv_uuid
          << "]. This will likely be fixed with the next  connection attempt.");
  return false;
}

bool ConnectionManager::verify_uuid_not_blacklisted(const boost::uuids::uuid& uuid) {
  if (!blacklisted_uuids_.count(uuid)) {
    return true;
  }

  LOG_DBG("Dropping connection to [" << uuid << "] since it is blacklisted");

  return false;
}

bool ConnectionManager::verify_connection_has_higher_priority(bool conn_already_exists,
                                                              const BranchConnectionPtr& conn) {
  if (!conn_already_exists) {
    return true;
  }

  auto& remote_uuid = conn->get_remote_branch_info()->get_uuid();
  YOGI_ASSERT(connections_[remote_uuid]);

  if ((remote_uuid < info_->get_uuid()) == conn->created_from_incoming_connection_request()) {
    return true;
  }

  LOG_DBG("Dropping TCP " << (conn->created_from_incoming_connection_request() ? "server" : "client")
                          << " connection to " << conn << " since a connection with a higher priority already exists")

  return false;
}

Result ConnectionManager::check_remote_branch_info(const BranchInfoPtr& remote_info) {
  if (remote_info->get_network_name() != info_->get_network_name()) {
    blacklisted_uuids_.insert(remote_info->get_uuid());
    return Error(YOGI_ERR_NET_NAME_MISMATCH);
  }

  if (remote_info->get_name() == info_->get_name()) {
    blacklisted_uuids_.insert(remote_info->get_uuid());
    return Error(YOGI_ERR_DUPLICATE_BRANCH_NAME);
  }

  if (remote_info->get_path() == info_->get_path()) {
    blacklisted_uuids_.insert(remote_info->get_uuid());
    return Error(YOGI_ERR_DUPLICATE_BRANCH_PATH);
  }

  for (auto& entry : connections_) {
    auto branch_info = entry.second->get_remote_branch_info();
    if (branch_info == remote_info) continue;

    YOGI_ASSERT(remote_info->get_uuid() != branch_info->get_uuid());

    if (remote_info->get_name() == branch_info->get_name()) {
      return Error(YOGI_ERR_DUPLICATE_BRANCH_NAME);
    }

    if (remote_info->get_path() == branch_info->get_path()) {
      return Error(YOGI_ERR_DUPLICATE_BRANCH_PATH);
    }
  }

  return Success();
}

void ConnectionManager::start_authenticate(BranchConnectionPtr conn) {
  auto weak_conn = branch_connection_weak_ptr(conn);
  conn->authenticate(password_hash_, [this, weak_conn](auto& res) {
    YOGI_ASSERT(weak_conn.lock());
    this->on_authenticate_finished(res, weak_conn.lock());
  });
}

void ConnectionManager::on_authenticate_finished(const Result& res, BranchConnectionPtr conn) {
  auto& uuid = conn->get_remote_branch_info()->get_uuid();

  if (res.is_error()) {
    if (res == Error(YOGI_ERR_PASSWORD_MISMATCH)) {
      blacklisted_uuids_.insert(uuid);
    }

    std::lock_guard<std::mutex> lock(connections_mutex_);
    connections_.erase(uuid);

    emit_branch_event(YOGI_BEV_CONNECT_FINISHED, res, uuid);
  } else {
    LOG_DBG("Successfully authenticated with " << conn->get_remote_branch_info());
    start_session(conn);
  }
}

void ConnectionManager::start_session(BranchConnectionPtr conn) {
  auto weak_conn = branch_connection_weak_ptr(conn);
  conn->run_session(
      [this, weak_conn](auto& msg) {
        YOGI_ASSERT(weak_conn.lock());
        this->message_handler_(msg, weak_conn.lock());
      },
      [this, weak_conn](auto& res) {
        YOGI_ASSERT(weak_conn.lock());
        this->on_session_terminated(res.to_error(), weak_conn.lock());
      });

  emit_branch_event(YOGI_BEV_CONNECT_FINISHED, Success(), conn->get_remote_branch_info()->get_uuid());

  LOG_DBG("Successfully started session for " << conn->get_remote_branch_info());

  connection_changed_handler_(Success(), conn);
}

void ConnectionManager::on_session_terminated(const Error& err, BranchConnectionPtr conn) {
  LOG_DBG("Session for " << conn->get_remote_branch_info() << " terminated: " << err);

  emit_branch_event(YOGI_BEV_CONNECTION_LOST, err, conn->get_remote_branch_info()->get_uuid());

  std::lock_guard<std::mutex> lock(connections_mutex_);
  connections_.erase(conn->get_remote_branch_info()->get_uuid());

  connection_changed_handler_(err, conn);
}

BranchConnectionPtr ConnectionManager::make_connection_and_keep_it_alive(const boost::asio::ip::address& peer_address,
                                                                         TransportPtr transport) {
  auto conn = std::make_shared<BranchConnection>(transport, peer_address, info_);
  connections_kept_alive_.insert(conn);
  return conn;
}

BranchConnectionPtr ConnectionManager::stop_keeping_connection_alive(const branch_connection_weak_ptr& weak_conn) {
  auto conn = weak_conn.lock();
  YOGI_ASSERT(conn);
  connections_kept_alive_.erase(conn);
  return conn;
}

template <typename Fn>
void ConnectionManager::emit_branch_event(int branch_event, const Result& ev_res, const boost::uuids::uuid& uuid,
                                          Fn make_json_fn) {
  log_branch_event(branch_event, ev_res, make_json_fn);

  std::lock_guard<std::recursive_mutex> lock(event_mutex_);

  if (!event_handler_) return;
  if (!(observed_branch_events_ & branch_event)) return;

  auto handler   = event_handler_;
  event_handler_ = {};
  handler(Success(), branch_event, ev_res, uuid, make_json_fn().dump());
}

void ConnectionManager::emit_branch_event(int branch_event, const Result& ev_res, const boost::uuids::uuid& uuid) {
  return emit_branch_event(branch_event, ev_res, uuid, [&] {
    return nlohmann::json{{"uuid", boost::uuids::to_string(uuid)}};
  });
}

template <typename Fn>
void ConnectionManager::log_branch_event(int branch_event, const Result& ev_res, Fn make_json_fn) {
  switch (branch_event) {
    case YOGI_BEV_BRANCH_DISCOVERED:
      LOG_DBG("Event: YOGI_BEV_BRANCH_DISCOVERED; ev_res=\"" << ev_res << "; json=\"" << make_json_fn() << "\"");
      break;

    case YOGI_BEV_BRANCH_QUERIED:
      LOG_IFO("Event: YOGI_BEV_BRANCH_QUERIED; ev_res=\"" << ev_res << "; json=\"" << make_json_fn() << "\"");
      break;

    case YOGI_BEV_CONNECT_FINISHED:
      LOG_IFO("Event: YOGI_BEV_CONNECT_FINISHED; ev_res=\"" << ev_res << "; json=\"" << make_json_fn() << "\"");
      break;

    case YOGI_BEV_CONNECTION_LOST:
      LOG_WRN("Event: YOGI_BEV_CONNECTION_LOST; ev_res=\"" << ev_res << "; json=\"" << make_json_fn() << "\"");
      break;
  }
}
