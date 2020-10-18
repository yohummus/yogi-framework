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

#include <src/api/object.h>
#include <src/objects/context.h>
#include <src/objects/logger/log_user.h>
#include "src/objects/branch/broadcast_manager.h"
#include "src/objects/branch/connection_manager.h"

#include <nlohmann/json.hpp>

class Branch : public ExposedObjectT<Branch, ObjectType::kBranch>, public LogUser {
 public:
  using BranchEventHandler       = ConnectionManager::BranchEventHandler;
  using SendBroadcastHandler     = BroadcastManager::SendBroadcastHandler;
  using ReceiveBroadcastHandler  = BroadcastManager::ReceiveBroadcastHandler;
  using BranchInfoStringsList    = ConnectionManager::BranchInfoStringsList;
  using SendBroadcastOperationId = BroadcastManager::SendBroadcastOperationId;

  Branch(ContextPtr context, const nlohmann::json& cfg);

  void start();

  ContextPtr get_context() const;
  const boost::uuids::uuid& get_uuid() const;
  std::string make_info_string() const;
  BranchInfoStringsList make_connected_branches_info_strings() const;
  void await_event_async(int branch_events, BranchEventHandler handler);
  bool cancel_await_event();
  SendBroadcastOperationId send_broadcast_async(const Payload& payload, bool retry, SendBroadcastHandler handler);
  Result send_broadcast(const Payload& payload, bool block);
  bool cancel_send_broadcast(SendBroadcastOperationId oid);
  void receive_broadcast(int encoding, boost::asio::mutable_buffer data, ReceiveBroadcastHandler handler);
  bool cancel_receive_broadcast();

 private:
  void on_connection_changed(const Result& res, const BranchConnectionPtr& conn);
  void on_message_received(const IncomingMessage& msg, const BranchConnectionPtr& conn);

  const ContextPtr context_;
  ConnectionManagerPtr con_man_;
  LocalBranchInfoPtr info_;
  BroadcastManagerPtr bc_man_;
};

typedef std::shared_ptr<Branch> BranchPtr;
