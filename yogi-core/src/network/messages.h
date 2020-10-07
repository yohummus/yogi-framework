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

#include <boost/asio/buffer.hpp>
#include <msgpack.hpp>

#include <array>
#include <fstream>
#include <functional>

class MsgPackBufferStream {
 public:
  MsgPackBufferStream(SmallBuffer* buffer) : buffer_(*buffer) {
  }

  void write(const char* data, std::size_t size) {
    buffer_.insert(buffer_.end(), data, data + size);
  }

 private:
  SmallBuffer& buffer_;
};

enum MessageType : Byte {
  kHeartbeat,
  kAcknowledge,
  kBroadcast,
};

class Message {
 public:
  virtual ~Message() {
  }

  virtual MessageType get_type() const  = 0;
  virtual std::string to_string() const = 0;
};

class IncomingMessage : virtual public Message {
 public:
  typedef std::function<void(const IncomingMessage&)> MessageHandler;

  static void deserialize(const Buffer& serialized_msg, const MessageHandler& fn);

 protected:
  IncomingMessage() = default;
};

class Payload {
 public:
  Payload(boost::asio::const_buffer data, int encoding) : data_(data), encoding_(encoding) {
  }

  void serialize_to(SmallBuffer* buffer) const;
  Result serialize_to_user_buffer(boost::asio::mutable_buffer buffer, int encoding, std::size_t* bytes_written) const;

 private:
  boost::asio::const_buffer data_;
  int encoding_;
};

template <MessageType MsgType>
class MessageT : virtual public Message {
 public:
  static constexpr MessageType kMessageType = MsgType;

  virtual MessageType get_type() const override final {
    return kMessageType;
  }

 protected:
  static SmallBuffer make_msg_bytes() {
    return SmallBuffer{kMessageType};
  }

  static SmallBuffer make_msg_bytes(const Payload& payload) {
    SmallBuffer bytes{kMessageType};
    payload.serialize_to(&bytes);
    return bytes;
  }

  template <typename... Fields>
  static SmallBuffer make_msg_bytes(const std::tuple<Fields...>& fields) {
    SmallBuffer bytes{kMessageType};
    MsgPackBufferStream stream(&bytes);
    msgpack::pack(stream, fields);
    return bytes;
  }

  template <typename... Fields>
  static SmallBuffer make_msg_bytes(const std::tuple<Fields...>& fields, const Payload& payload) {
    SmallBuffer bytes{kMessageType};
    MsgPackBufferStream stream(&bytes);
    msgpack::pack(stream, fields);
    payload.serialize_to(&bytes);
    return bytes;
  }
};

class OutgoingMessage : virtual public Message {
 public:
  std::size_t get_size() const;
  const SmallBuffer& serialize() const;
  SharedSmallBuffer serialize_shared();

 protected:
  OutgoingMessage(SmallBuffer serialized_msg);

 private:
  SmallBuffer serialized_msg_;
  SharedSmallBuffer shared_serialized_msg_;
};

namespace messages {

// This is a message whose length is zero (msg type is omitted)
class Heartbeat : public MessageT<MessageType::kHeartbeat> {
  virtual std::string to_string() const override final {
    return "Heartbeat";
  }
};

class HeartbeatIncoming : public IncomingMessage, public Heartbeat {};

class HeartbeatOutgoing : public OutgoingMessage, public Heartbeat {
 public:
  HeartbeatOutgoing() : OutgoingMessage({}) {
  }
};

class Acknowledge : public MessageT<MessageType::kAcknowledge> {
  virtual std::string to_string() const override final {
    return "Acknowledge";
  }
};

class AcknowledgeIncoming : public IncomingMessage, public Acknowledge {};

class AcknowledgeOutgoing : public OutgoingMessage, public Acknowledge {
 public:
  AcknowledgeOutgoing() : OutgoingMessage(make_msg_bytes()) {
  }
};

class Broadcast : public MessageT<MessageType::kBroadcast> {};

class BroadcastIncoming : public IncomingMessage, public Broadcast {
 public:
  BroadcastIncoming(const Buffer& serialized_msg);

  virtual std::string to_string() const override final;

  const Payload& get_payload() const {
    return payload_;
  }

 private:
  const Payload payload_;
};

class BroadcastOutgoing : public OutgoingMessage, public Broadcast {
 public:
  BroadcastOutgoing(const Payload& payload);

  virtual std::string to_string() const override final;
};

}  // namespace messages

std::ostream& operator<<(std::ostream& os, const Message& msg);
