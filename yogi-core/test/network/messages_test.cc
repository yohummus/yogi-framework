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

#include <test/common.h>

#include <src/network/messages.h>

#include <nlohmann/json.hpp>

class FakeOutgoingMessage : public OutgoingMessage, public MessageT<MessageType::kBroadcast> {
 public:
  using MessageT::make_msg_bytes;

  FakeOutgoingMessage(SmallBuffer serialized_msg) : OutgoingMessage(serialized_msg) {
  }

  virtual std::string to_string() const override {
    return {};
  }
};

TEST(MessagesTest, UserDataJson) {
  auto payload = Payload(boost::asio::buffer("{\"x\": 456}"), YOGI_ENC_JSON);

  SmallBuffer buffer;
  EXPECT_NO_THROW(payload.serialize_to(&buffer));

  auto json = nlohmann::json::from_msgpack(buffer.begin(), buffer.end());
  EXPECT_EQ(json.value("x", -1), 456);

  const Byte invalid_data[] = {200};
  payload                   = Payload(boost::asio::buffer(invalid_data), YOGI_ENC_JSON);
  EXPECT_THROW_ERROR(payload.serialize_to(&buffer), YOGI_ERR_PARSING_JSON_FAILED);
}

TEST(MessageTest, UserDataMsgPack) {
  auto data    = SmallBuffer{0x93, 0x1, 0x2, 0x3};
  auto payload = Payload(boost::asio::buffer(data.data(), data.size()), YOGI_ENC_MSGPACK);

  SmallBuffer buffer;
  EXPECT_NO_THROW(payload.serialize_to(&buffer));
  EXPECT_EQ(buffer, data);

  const Byte invalid_data[] = {200};
  payload                   = Payload(boost::asio::buffer(invalid_data), YOGI_ENC_MSGPACK);
  EXPECT_THROW_ERROR(payload.serialize_to(&buffer), YOGI_ERR_INVALID_USER_MSGPACK);
}

TEST(MessagesTest, UserDataSerializeToUserBuffer) {
  auto json    = Buffer{'[', '1', ',', '2', ',', '3', ']', '\0'};
  auto msgpack = Buffer{0x93, 0x1, 0x2, 0x3};

  auto fn = [&](const Buffer& bytes, int encoding) {
    Payload payload(boost::asio::buffer(bytes), encoding);
    Buffer data;
    std::size_t n = 0;

    // To JSON: Buffer too small
    data.resize(3);
    auto res = payload.serialize_to_user_buffer(boost::asio::buffer(data), YOGI_ENC_JSON, &n);
    EXPECT_EQ(res, Error(YOGI_ERR_BUFFER_TOO_SMALL));
    EXPECT_EQ(n, data.size());
    EXPECT_EQ(data[0], json[0]);
    EXPECT_EQ(data[1], json[1]);
    EXPECT_EQ(data[2], '\0');

    // To JSON: Success
    data.resize(json.size());
    res = payload.serialize_to_user_buffer(boost::asio::buffer(data), YOGI_ENC_JSON, &n);
    EXPECT_EQ(res, Success());
    EXPECT_EQ(n, json.size());
    EXPECT_EQ(data, json);

    // To MsgPack: Buffer too small
    data.resize(3);
    res = payload.serialize_to_user_buffer(boost::asio::buffer(data), YOGI_ENC_MSGPACK, &n);
    EXPECT_EQ(res, Error(YOGI_ERR_BUFFER_TOO_SMALL));
    EXPECT_EQ(n, data.size());
    EXPECT_EQ(data[0], msgpack[0]);
    EXPECT_EQ(data[1], msgpack[1]);
    EXPECT_EQ(data[2], msgpack[2]);

    // To MsgPack: Success
    data.resize(msgpack.size());
    res = payload.serialize_to_user_buffer(boost::asio::buffer(data), YOGI_ENC_MSGPACK, &n);
    EXPECT_EQ(res, Success());
    EXPECT_EQ(n, msgpack.size());
    EXPECT_EQ(data, msgpack);
  };

  fn(json, YOGI_ENC_JSON);
  fn(msgpack, YOGI_ENC_MSGPACK);
}

TEST(MessagesTest, GetType) {
  auto fakeType = FakeOutgoingMessage::kMessageType;
  EXPECT_EQ(fakeType, MessageType::kBroadcast);
  EXPECT_EQ(fakeType, FakeOutgoingMessage({}).get_type());
}

TEST(MessagesTest, MakeMsgBytes) {
  auto type    = FakeOutgoingMessage::kMessageType;
  auto payload = Payload(boost::asio::buffer("x", 1), YOGI_ENC_MSGPACK);

  auto bytes = FakeOutgoingMessage::make_msg_bytes();
  EXPECT_EQ(bytes, SmallBuffer{type});

  bytes = FakeOutgoingMessage::make_msg_bytes(payload);
  EXPECT_EQ(bytes, (SmallBuffer{type, 'x'}));

  bytes = FakeOutgoingMessage::make_msg_bytes(std::make_tuple(true, false, 123));
  EXPECT_EQ(bytes, (SmallBuffer{type, 0x93, 0xc3, 0xc2, 0x7b}));

  bytes = FakeOutgoingMessage::make_msg_bytes(std::make_tuple(true, false, 123), payload);
  EXPECT_EQ(bytes, (SmallBuffer{type, 0x93, 0xc3, 0xc2, 0x7b, 'x'}));
}

TEST(MessagesTest, get_size) {
  auto msg = FakeOutgoingMessage({1, 2, 3});
  EXPECT_EQ(msg.get_size(), 3);
}

TEST(MessagesTest, Serialize) {
  auto msg   = FakeOutgoingMessage({1, 2, 3});
  auto bytes = msg.serialize();
  EXPECT_EQ(bytes, (SmallBuffer{1, 2, 3}));

  msg.serialize_shared();
  EXPECT_EQ(msg.serialize(), bytes);
}

TEST(MessagesTest, serialize_shared) {
  auto msg   = FakeOutgoingMessage({1, 2, 3});
  auto bytes = msg.serialize_shared();
  EXPECT_EQ(*bytes, (SmallBuffer{1, 2, 3}));
}

TEST(MessagesTest, Deserialize) {
  Buffer bytes = {MessageType::kBroadcast, 0x93, 0x01, 0x02, 0x03};

  bool called = false;
  IncomingMessage::deserialize(bytes, [&](const IncomingMessage& msg) {
    EXPECT_EQ(msg.get_type(), MessageType::kBroadcast);

    auto bcm = dynamic_cast<const messages::BroadcastIncoming*>(&msg);
    ASSERT_NE(bcm, nullptr);

    SmallBuffer data;
    bcm->get_payload().serialize_to(&data);
    EXPECT_EQ(data, SmallBuffer(bytes.begin() + 1, bytes.end()));

    called = true;
  });

  EXPECT_TRUE(called);
}
