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
#include <src/objects/branch.h>
#include <src/schemas/schemas.h>
#include <src/system/network_info.h>
#include <src/util/bind.h>
#include <src/util/json_helpers.h>

#include <chrono>
using namespace std::chrono_literals;
using namespace std::string_literals;

YOGI_DEFINE_INTERNAL_LOGGER("Branch")

Branch::Branch(ContextPtr context, const nlohmann::json& cfg) : context_(context) {
  validate_json(cfg, YOGI_SCM_BRANCH_CONFIG);

  con_man_ = std::make_shared<ConnectionManager>(context_, cfg);
  info_    = std::make_shared<LocalBranchInfo>(cfg, con_man_->get_advertising_interfaces(),
                                            static_cast<unsigned short>(con_man_->get_tcp_server_port()));
  bc_man_  = std::make_shared<BroadcastManager>(context, *con_man_);
}

void Branch::start() {
  set_logging_prefix(info_->logging_prefix());
  con_man_->start(info_, bind_weak(&Branch::on_connection_changed, this),
                  bind_weak(&Branch::on_message_received, this));
  bc_man_->start(info_);
}

ContextPtr Branch::get_context() const {
  return context_;
}

const boost::uuids::uuid& Branch::get_uuid() const {
  return info_->get_uuid();
}

std::string Branch::make_info_string() const {
  return info_->to_json().dump();
}

Branch::BranchInfoStringsList Branch::make_connected_branches_info_strings() const {
  return con_man_->make_connected_branches_info_strings();
}

void Branch::await_event_async(int branch_events, BranchEventHandler handler) {
  con_man_->await_event_async(branch_events, handler);
}

bool Branch::cancel_await_event() {
  return con_man_->cancel_await_event();
}

Branch::SendBroadcastOperationId Branch::send_broadcast_async(const Payload& payload, bool retry,
                                                              SendBroadcastHandler handler) {
  return bc_man_->send_broadcast_async(payload, retry, handler);
}

Result Branch::send_broadcast(const Payload& payload, bool block) {
  return bc_man_->send_broadcast(payload, block);
}

bool Branch::cancel_send_broadcast(SendBroadcastOperationId oid) {
  return bc_man_->cancel_send_broadcast(oid);
}

void Branch::receive_broadcast(int encoding, boost::asio::mutable_buffer data, ReceiveBroadcastHandler handler) {
  bc_man_->receive_broadcast(encoding, data, handler);
}

bool Branch::cancel_receive_broadcast() {
  return bc_man_->cancel_receive_broadcast();
}

void Branch::on_connection_changed(const Result& res, const BranchConnectionPtr& conn) {
  LOG_IFO("Connection to " << conn->get_remote_branch_info() << " changed: " << res);
  // TODO
}

void Branch::on_message_received(const IncomingMessage& msg, const BranchConnectionPtr& conn) {
  LOG_TRC("Message received: " << msg);

  switch (msg.get_type()) {
    case MessageType::kHeartbeat:
      break;

    case MessageType::kBroadcast:
      bc_man_->on_broadcast_received(static_cast<const messages::BroadcastIncoming&>(msg), conn);
      break;

    default:
      LOG_ERR("Message of unexpected type received: " << msg);
      YOGI_NEVER_REACHED;
      break;
  }
}
