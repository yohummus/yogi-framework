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

namespace network {
namespace internal {

std::size_t SerializeMsgSizeField(std::size_t msg_size, std::array<Byte, 5>* buffer);
bool DeserializeMsgSizeField(const std::array<Byte, 5>& buffer, std::size_t size, std::size_t* msg_size);

}  // namespace internal

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

  ContextPtr GetContext() const {
    return context_;
  }

  void Start();

  bool TrySend(const OutgoingMessage& msg);
  void SendAsync(OutgoingMessage* msg, OperationTag tag, SendHandler handler);
  void SendAsync(OutgoingMessage* msg, SendHandler handler);
  bool CancelSend(OperationTag tag);
  void ReceiveAsync(boost::asio::mutable_buffer msg, ReceiveHandler handler);

  void CancelReceive();
  void Close() {
    transport_->Close();
  }

 private:
  typedef std::array<Byte, 5> SizeFieldBuffer;

  struct PendingSend {
    OperationTag tag;  // 0 => operation cannot be canceled
    SharedSmallBuffer msg_bytes;
    SendHandler handler;
  };

  MessageTransportWeakPtr MakeWeakPtr() {
    return shared_from_this();
  }
  bool TrySendImpl(const SmallBuffer& msg_bytes);
  bool CanSend(std::size_t msg_size) const;
  void SendAsyncImpl(OutgoingMessage* msg, OperationTag tag, SendHandler handler);
  void SendSomeBytesToTransport();
  void RetrySendingPendingSends();
  bool TryGetReceivedSizeField(std::size_t* msg_size);
  void ResetReceivedSizeField();
  void ReceiveSomeBytesFromTransport();
  void TryDeliveringPendingReceive();
  void HandleSendError(const Error& err);
  void HandleReceiveError(const Error& err);
  void CheckOperationTagIsNotUsed(OperationTag tag);

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

}  // namespace network
