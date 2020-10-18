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

#include <src/network/messages.h>
#include <src/objects/branch/connection_manager.h>
#include <src/objects/context.h>
#include <src/objects/logger/log_user.h>

#include <boost/asio/buffer.hpp>
#include <mutex>
#include <vector>

class BroadcastManager final : public std::enable_shared_from_this<BroadcastManager>, public LogUser {
 public:
  typedef MessageTransport::OperationTag SendBroadcastOperationId;
  typedef std::function<void(const Result& res, SendBroadcastOperationId oid)> SendBroadcastHandler;
  typedef std::function<void(const Result& res, const boost::uuids::uuid& src_uuid, std::size_t size)>
      ReceiveBroadcastHandler;

  BroadcastManager(ContextPtr context, ConnectionManager& conn_manager);
  virtual ~BroadcastManager();

  void start(LocalBranchInfoPtr info);
  Result send_broadcast(const Payload& payload, bool retry);
  SendBroadcastOperationId send_broadcast_async(const Payload& payload, bool retry, SendBroadcastHandler handler);
  bool cancel_send_broadcast(SendBroadcastOperationId oid);
  void receive_broadcast(int encoding, boost::asio::mutable_buffer data, ReceiveBroadcastHandler handler);
  bool cancel_receive_broadcast();
  void on_broadcast_received(const messages::BroadcastIncoming& msg, const BranchConnectionPtr& conn);

 private:
  typedef std::shared_ptr<int> SharedCounter;

  void send_now_or_later(SharedCounter* pending_handlers, OutgoingMessage* msg, BranchConnectionPtr conn,
                         SendBroadcastHandler handler, SendBroadcastOperationId oid);

  void store_oid_for_later_or_call_handler_now(SharedCounter pending_handlers, SendBroadcastHandler handler,
                                               SendBroadcastOperationId oid);

  void create_and_increment_counter(SharedCounter* counter);
  bool remove_active_oid(SendBroadcastOperationId oid);

  const ContextPtr context_;
  ConnectionManager& conn_manager_;
  std::mutex tx_oids_mutex_;
  std::vector<SendBroadcastOperationId> tx_active_oids_;
  std::mutex tx_sync_mutex_;
  std::condition_variable tx_sync_cv_;
  std::recursive_mutex rx_mutex_;
  int rx_encoding_;
  boost::asio::mutable_buffer rx_data_;
  ReceiveBroadcastHandler rx_handler_;
};

typedef std::shared_ptr<BroadcastManager> BroadcastManagerPtr;
