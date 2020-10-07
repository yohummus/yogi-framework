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

#include <src/data/ringbuffer.h>
#include <src/network/messages.h>
#include <src/network/transport.h>
#include <src/objects/logger/log_user.h>

#include <array>
#include <boost/asio/buffer.hpp>
#include <functional>
#include <memory>
#include <vector>

std::size_t serialize_msg_size_field(std::size_t msg_size, std::array<Byte, 5>* buffer);
bool deserialize_msg_size_field(const std::array<Byte, 5>& buffer, std::size_t size, std::size_t* msg_size);

class MessageTransport;
typedef std::shared_ptr<MessageTransport> MessageTransportPtr;
typedef std::weak_ptr<MessageTransport> MessageTransportWeakPtr;

class MessageTransport : public std::enable_shared_from_this<MessageTransport>, public LogUser {
 public:
  typedef int OperationTag;
  typedef std::function<void(const Result&)> SendHandler;
  typedef std::function<void(const Result&, std::size_t msg_size)> ReceiveHandler;
  typedef ReceiveHandler SizeFieldReceiveHandler;

  MessageTransport(TransportPtr transport, std::size_t tx_queue_size, std::size_t rx_queue_size);

  ContextPtr get_context() const {
    return context_;
  }

  void start();

  bool try_send(const OutgoingMessage& msg);
  void send_async(OutgoingMessage* msg, OperationTag tag, SendHandler handler);
  void send_async(OutgoingMessage* msg, SendHandler handler);
  bool cancel_send(OperationTag tag);
  void receive_async(boost::asio::mutable_buffer msg, ReceiveHandler handler);
  void cancel_receive();

  void close() {
    transport_->close();
  }

 private:
  typedef std::array<Byte, 5> SizeFieldBuffer;

  struct PendingSend {
    OperationTag tag;  // 0 => operation cannot be canceled
    SharedSmallBuffer msg_bytes;
    SendHandler handler;
  };

  MessageTransportWeakPtr make_weak_ptr() {
    return shared_from_this();
  }

  bool try_send_impl(const SmallBuffer& msg_bytes);
  bool can_send(std::size_t msg_size) const;
  void send_async_impl(OutgoingMessage* msg, OperationTag tag, SendHandler handler);
  void send_some_bytes_to_transport();
  void retry_sending_pending_sends();
  bool try_get_received_size_field(std::size_t* msg_size);
  void reset_received_size_field();
  void receive_some_bytes_from_transport();
  void try_deliver_pending_receive();
  void handle_send_error(const Error& err);
  void handle_receive_error(const Error& err);
  void check_operation_tag_not_used(OperationTag tag);

  const ContextPtr context_;
  const TransportPtr transport_;
  LockFreeRingBuffer tx_rb_;
  LockFreeRingBuffer rx_rb_;
  std::mutex tx_mutex_;
  Result last_tx_error_;
  bool send_to_transport_running_;
  std::vector<PendingSend> pending_sends_;
  SizeFieldBuffer size_field_buffer_;
  std::size_t size_field_buffer_size_;
  std::size_t size_field_;
  bool size_field_valid_;
  boost::asio::mutable_buffer pending_receive_buffer_;
  ReceiveHandler pending_receive_handler_;
  bool receive_from_transport_running_;
  Result last_rx_error_;
};
