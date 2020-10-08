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

#include <src/network/msg_transport.h>
#include <src/objects/branch/branch_info.h>
#include <src/objects/context.h>

#include <atomic>
#include <boost/asio.hpp>
#include <fstream>
#include <functional>
#include <memory>

class BranchConnection;
typedef std::shared_ptr<BranchConnection> BranchConnectionPtr;
typedef std::weak_ptr<BranchConnection> branch_connection_weak_ptr;

class BranchConnection : public std::enable_shared_from_this<BranchConnection> {
 public:
  typedef std::function<void(const Result&)> CompletionHandler;
  using MessageReceiveHandler = IncomingMessage::MessageHandler;
  using OperationTag          = MessageTransport::OperationTag;
  using SendHandler           = MessageTransport::SendHandler;

  BranchConnection(TransportPtr transport, const boost::asio::ip::address& peer_address, LocalBranchInfoPtr local_info);

  BranchInfoPtr get_remote_branch_info() const {
    return remote_info_;
  }
  std::string make_info_string() const;

  bool session_running() const {
    return session_running_;
  }

  bool created_from_incoming_connection_request() const {
    return transport_->created_from_incoming_connection_request();
  };

  const std::string& get_peer_description() const {
    return transport_->get_peer_description();
  }

  void exchange_branch_info(CompletionHandler handler);
  void authenticate(SharedBuffer password_hash, CompletionHandler handler);
  void run_session(MessageReceiveHandler rcv_handler, CompletionHandler session_handler);

  bool try_send(const OutgoingMessage& msg) {
    return msg_transport_->try_send(msg);
  }

  void send_async(OutgoingMessage* msg, OperationTag tag, SendHandler handler) {
    msg_transport_->send_async(msg, tag, handler);
  }

  void send_async(OutgoingMessage* msg, SendHandler handler) {
    msg_transport_->send_async(msg, handler);
  }

  bool cancel_send(OperationTag tag) {
    return msg_transport_->cancel_send(tag);
  }

 private:
  branch_connection_weak_ptr make_weak_ptr() {
    return {shared_from_this()};
  }
  void on_info_sent(CompletionHandler handler);
  void on_info_header_received(SharedBuffer buffer, CompletionHandler handler);
  void on_info_body_received(SharedBuffer info_msg, CompletionHandler handler);
  void on_info_ack_sent(CompletionHandler handler);
  void on_info_ack_received(const Result& res, SharedBuffer ack_msg, CompletionHandler handler);
  void on_challenge_sent(SharedBuffer my_challenge, SharedBuffer password_hash, CompletionHandler handler);
  void on_challenge_received(SharedBuffer remote_challenge, SharedBuffer my_challenge, SharedBuffer password_hash,
                             CompletionHandler handler);
  SharedBuffer solve_challenge(const Buffer& challenge, const Buffer& password_hash) const;
  void on_solution_sent(SharedBuffer my_solution, CompletionHandler handler);
  void on_solution_received(SharedBuffer received_solution, SharedBuffer my_solution, CompletionHandler handler);
  void on_solution_ack_sent(bool solutions_match, CompletionHandler handler);
  void on_solution_ack_received(const Result& res, bool solutions_match, SharedBuffer ack_msg,
                                CompletionHandler handler);
  void restart_heartbeat_timer();
  void on_heartbeat_timer_expired(boost::system::error_code ec);
  void start_receive(SharedBuffer buffer);
  void on_session_error(const Error& err);
  void check_ack_and_set_next_result(const Result& res, const Buffer& ack_msg);
  bool check_next_result(CompletionHandler handler);
  void on_message_received(const SharedBuffer& msg);

  const TransportPtr transport_;
  const ContextPtr context_;
  const LocalBranchInfoPtr local_info_;
  const boost::asio::ip::address peer_address_;
  const Timestamp connected_since_;
  messages::HeartbeatOutgoing heartbeat_msg_;
  messages::AcknowledgeOutgoing ack_msg_;
  RemoteBranchInfoPtr remote_info_;
  MessageTransportPtr msg_transport_;
  std::atomic<bool> session_running_;
  CompletionHandler session_handler_;
  MessageReceiveHandler rcv_handler_;
  boost::asio::steady_timer heartbeat_timer_;
  Result next_result_;
};

std::ostream& operator<<(std::ostream& os, const BranchConnection& conn);
