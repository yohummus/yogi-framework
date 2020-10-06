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

#include <src/api/errors.h>
#include <src/network/messages.h>

#include <nlohmann/json.hpp>

struct MsgPackCheckVisitor : public msgpack::null_visitor {
  void parse_error(std::size_t parsed_offset, std::size_t error_offset) {
    throw DescriptiveError(YOGI_ERR_INVALID_USER_MSGPACK)
        << "Parse errror. Parsed offset: " << parsed_offset << "; Error offset: " << error_offset;
  }

  void insufficient_bytes(std::size_t parsed_offset, std::size_t error_offset) {
    throw DescriptiveError(YOGI_ERR_INVALID_USER_MSGPACK)
        << "Insufficient bytes. Parsed offset: " << parsed_offset << "; Error offset: " << error_offset;
  }
};

void check_payload_is_valid_msgpack(const char* data, std::size_t size) {
  MsgPackCheckVisitor visitor;
  bool ok = msgpack::parse(data, size, visitor);
  if (!ok) {
    throw DescriptiveError(YOGI_ERR_INVALID_USER_MSGPACK) << "msgpack::parse() returned false";
  }
}

Buffer check_and_convert_payload_from_json_to_msgpack(const char* data, std::size_t size) {
  YOGI_ASSERT(size > 0);
  if (data[size - 1] != '\0') {
    throw DescriptiveError(YOGI_ERR_PARSING_JSON_FAILED) << "Unterminated string";
  }

  try {
    auto json = nlohmann::json::parse(data);
    return nlohmann::json::to_msgpack(json);
  } catch (const std::exception& e) {
    throw DescriptiveError(YOGI_ERR_PARSING_JSON_FAILED) << e.what();
  }
}

void IncomingMessage::deserialize(const Buffer& serialized_msg, const MessageHandler& fn) {
  if (serialized_msg.empty()) {
    fn(messages::HeartbeatIncoming());
    return;
  }

  switch (serialized_msg[0]) {
    case MessageType::kAcknowledge: fn(messages::AcknowledgeIncoming()); break;

    case MessageType::kBroadcast: fn(messages::BroadcastIncoming(serialized_msg)); break;

    default: throw DescriptiveError(YOGI_ERR_DESERIALIZE_MSG_FAILED) << "Unknown message type " << serialized_msg[0];
  }
}

void Payload::serialize_to(SmallBuffer* buffer) const {
  if (data_.size() == 0) return;

  auto raw = static_cast<const char*>(data_.data());

  switch (encoding_) {
    case YOGI_ENC_JSON: {
      auto data = check_and_convert_payload_from_json_to_msgpack(raw, data_.size());
      buffer->insert(buffer->end(), data.begin(), data.end());
      break;
    }

    case YOGI_ENC_MSGPACK: {
      check_payload_is_valid_msgpack(raw, data_.size());
      buffer->insert(buffer->end(), raw, raw + data_.size());

      break;
    }

    default: YOGI_NEVER_REACHED;
  }
}

Result Payload::serialize_to_user_buffer(boost::asio::mutable_buffer buffer, int encoding,
                                         std::size_t* bytes_written) const {
  Buffer tmp_buf;
  std::string tmp_str;
  boost::asio::const_buffer src;

  if (encoding == encoding_) {
    src = data_;
  } else {
    switch (encoding) {
      case YOGI_ENC_JSON: {
        auto raw  = static_cast<const unsigned char*>(data_.data());
        auto json = nlohmann::json::from_msgpack(raw, raw + data_.size());
        tmp_str   = json.dump();
        src       = boost::asio::buffer(tmp_str.data(), tmp_str.size() + 1);
        break;
      }

      case YOGI_ENC_MSGPACK: {
        auto raw  = static_cast<const char*>(data_.data());
        auto json = nlohmann::json::parse(raw);
        tmp_buf   = nlohmann::json::to_msgpack(json);
        src       = boost::asio::buffer(tmp_buf);
        break;
      }

      default: YOGI_NEVER_REACHED;
    }
  }

  auto n = boost::asio::buffer_copy(buffer, src);
  YOGI_ASSERT(bytes_written != nullptr);
  *bytes_written = n;

  if (n < src.size()) {
    if (encoding == YOGI_ENC_JSON) {
      static_cast<char*>(buffer.data())[buffer.size() - 1] = '\0';
    }

    return Error(YOGI_ERR_BUFFER_TOO_SMALL);
  }

  return Success();
}

std::size_t OutgoingMessage::get_size() const {
  return serialize().size();
}

const SmallBuffer& OutgoingMessage::serialize() const {
  if (shared_serialized_msg_) {
    return *shared_serialized_msg_;
  } else {
    return serialized_msg_;
  }
}

SharedSmallBuffer OutgoingMessage::serialize_shared() {
  if (!shared_serialized_msg_) {
    shared_serialized_msg_ = make_shared_small_buffer(std::move(serialized_msg_));
  }

  return shared_serialized_msg_;
}

OutgoingMessage::OutgoingMessage(SmallBuffer serialized_msg) : serialized_msg_(serialized_msg) {
}

namespace messages {

BroadcastIncoming::BroadcastIncoming(const Buffer& serialized_msg)
    : payload_(boost::asio::buffer(serialized_msg) + 1, YOGI_ENC_MSGPACK) {
}

std::string BroadcastIncoming::to_string() const {
  std::stringstream ss;
  ss << "Broadcast, " << BroadcastOutgoing(payload_).serialize().size() - 1 << " bytes user data";
  return ss.str();
}

BroadcastOutgoing::BroadcastOutgoing(const Payload& payload) : OutgoingMessage(make_msg_bytes(payload)) {
}

std::string BroadcastOutgoing::to_string() const {
  std::stringstream ss;
  ss << "Broadcast, " << get_size() - 1 << " bytes user data";
  return ss.str();
}

}  // namespace messages

std::ostream& operator<<(std::ostream& os, const Message& msg) {
  os << msg.to_string();
  return os;
}
