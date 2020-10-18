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
#include <src/network/serialize.h>
#include <src/objects/branch/branch_connection.h>
#include <src/util/bind.h>

BranchConnection::BranchConnection(TransportPtr transport, const boost::asio::ip::address& peer_address,
                                   LocalBranchInfoPtr local_info)
    : transport_(transport),
      context_(transport->get_context()),
      local_info_(local_info),
      peer_address_(peer_address),
      connected_since_(Timestamp::now()),
      session_running_(false),
      heartbeat_timer_(context_->io_context()),
      next_result_(Success()) {
}

std::string BranchConnection::make_info_string() const {
  auto json                = remote_info_->to_json();
  json["connected_since_"] = connected_since_.to_javascript_string();
  return json.dump();
}

void BranchConnection::exchange_branch_info(CompletionHandler handler) {
  YOGI_ASSERT(!remote_info_);

  auto weak_self = make_weak_ptr();
  transport_->send_all_async(local_info_->make_info_message(), [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      handler(res);
    } else {
      self->on_info_sent(handler);
    }
  });
}

void BranchConnection::authenticate(SharedBuffer password_hash, CompletionHandler handler) {
  YOGI_ASSERT(remote_info_);

  if (!check_next_result(handler)) return;

  auto my_challenge = make_shared_buffer(generate_random_bytes(8));

  auto weak_self = make_weak_ptr();
  transport_->send_all_async(my_challenge, [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      handler(res);
    } else {
      self->on_challenge_sent(my_challenge, password_hash, handler);
    }
  });
}

void BranchConnection::run_session(MessageReceiveHandler rcv_handler, CompletionHandler session_handler) {
  YOGI_ASSERT(remote_info_);
  YOGI_ASSERT(!session_running());

  if (!check_next_result(session_handler)) return;

  msg_transport_ = std::make_shared<MessageTransport>(transport_, local_info_->get_tx_queue_size(),
                                                      local_info_->get_rx_queue_size());
  msg_transport_->start();

  restart_heartbeat_timer();
  start_receive(make_shared_buffer());
  session_running_ = true;
  session_handler_ = session_handler;
  rcv_handler_     = rcv_handler;
}

void BranchConnection::on_info_sent(CompletionHandler handler) {
  auto weak_self = make_weak_ptr();
  auto buffer    = make_shared_buffer(BranchInfo::kInfoMessageHeaderSize);
  transport_->receive_all_async(buffer, [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      handler(res);
    } else {
      self->on_info_header_received(buffer, handler);
    }
  });
}

void BranchConnection::on_info_header_received(SharedBuffer buffer, CompletionHandler handler) {
  std::size_t body_size;
  auto res = RemoteBranchInfo::deserialize_info_message_body_size(&body_size, *buffer);
  if (res.is_error()) {
    handler(res);
    return;
  }

  if (body_size > constants::kMaxMessagePayloadSize) {
    handler(Error(YOGI_ERR_PAYLOAD_TOO_LARGE));
    return;
  }

  auto weak_self = make_weak_ptr();
  buffer->resize(BranchInfo::kInfoMessageHeaderSize + body_size);
  transport_->receive_all_async(boost::asio::buffer(*buffer) + BranchInfo::kInfoMessageHeaderSize, [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      handler(res);
    } else {
      self->on_info_body_received(buffer, handler);
    }
  });
}

void BranchConnection::on_info_body_received(SharedBuffer info_msg, CompletionHandler handler) {
  try {
    remote_info_ = std::make_shared<RemoteBranchInfo>(*info_msg, peer_address_);

    if (remote_info_->get_uuid() == local_info_->get_uuid()) {
      throw Error(YOGI_ERR_LOOPBACK_CONNECTION);
    }
  } catch (const Error& err) {
    handler(err);
    return;
  }

  auto weak_self = make_weak_ptr();
  transport_->send_all_async(ack_msg_.serialize_shared(), [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      handler(res);
    } else {
      self->on_info_ack_sent(handler);
    }
  });
}

void BranchConnection::on_info_ack_sent(CompletionHandler handler) {
  auto weak_self = make_weak_ptr();
  auto ack_msg   = make_shared_buffer(ack_msg_.get_size());
  transport_->receive_all_async(ack_msg, [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    self->on_info_ack_received(res, ack_msg, handler);
  });
}

void BranchConnection::on_info_ack_received(const Result& res, SharedBuffer ack_msg, CompletionHandler handler) {
  check_ack_and_set_next_result(res, *ack_msg);
  handler(Success());
}

void BranchConnection::on_challenge_sent(SharedBuffer my_challenge, SharedBuffer password_hash,
                                         CompletionHandler handler) {
  auto weak_self        = make_weak_ptr();
  auto remote_challenge = make_shared_buffer(my_challenge->size());
  transport_->receive_all_async(remote_challenge, [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      handler(res);
    } else {
      this->on_challenge_received(remote_challenge, my_challenge, password_hash, handler);
    }
  });
}

void BranchConnection::on_challenge_received(SharedBuffer remote_challenge, SharedBuffer my_challenge,
                                             SharedBuffer password_hash, CompletionHandler handler) {
  auto weak_self       = make_weak_ptr();
  auto my_solution     = solve_challenge(*my_challenge, *password_hash);
  auto remote_solution = solve_challenge(*remote_challenge, *password_hash);
  transport_->send_all_async(remote_solution, [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      handler(res);
    } else {
      self->on_solution_sent(my_solution, handler);
    }
  });
}

SharedBuffer BranchConnection::solve_challenge(const Buffer& challenge, const Buffer& password_hash) const {
  auto data = challenge;
  data.insert(data.end(), password_hash.begin(), password_hash.end());
  return make_shared_buffer(make_sha256(data));
}

void BranchConnection::on_solution_sent(SharedBuffer my_solution, CompletionHandler handler) {
  auto weak_self         = make_weak_ptr();
  auto received_solution = make_shared_buffer(my_solution->size());
  transport_->receive_all_async(received_solution, [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      handler(res);
    } else {
      self->on_solution_received(received_solution, my_solution, handler);
    }
  });
}

void BranchConnection::on_solution_received(SharedBuffer received_solution, SharedBuffer my_solution,
                                            CompletionHandler handler) {
  auto weak_self       = make_weak_ptr();
  bool solutions_match = *received_solution == *my_solution;
  transport_->send_all_async(ack_msg_.serialize_shared(), [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      handler(res);
    } else {
      self->on_solution_ack_sent(solutions_match, handler);
    }
  });
}

void BranchConnection::on_solution_ack_sent(bool solutions_match, CompletionHandler handler) {
  auto weak_self = make_weak_ptr();
  auto ack_msg   = make_shared_buffer(ack_msg_.get_size());
  transport_->receive_all_async(ack_msg, [=](auto& res) {
    auto self = weak_self.lock();
    if (!self) return;

    self->on_solution_ack_received(res, solutions_match, ack_msg, handler);
  });
}

void BranchConnection::on_solution_ack_received(const Result& res, bool solutions_match, SharedBuffer ack_msg,
                                                CompletionHandler handler) {
  check_ack_and_set_next_result(res, *ack_msg);

  if (!solutions_match) {
    handler(Error(YOGI_ERR_PASSWORD_MISMATCH));
  } else {
    handler(Success());
  }
}

void BranchConnection::restart_heartbeat_timer() {
  YOGI_ASSERT((remote_info_->get_timeout() / 2).count() > 0);
  heartbeat_timer_.expires_from_now(remote_info_->get_timeout() / 2);

  auto weak_self = make_weak_ptr();
  heartbeat_timer_.async_wait(bind_weak(&BranchConnection::on_heartbeat_timer_expired, this));
}

void BranchConnection::on_heartbeat_timer_expired(boost::system::error_code ec) {
  if (ec == boost::asio::error::operation_aborted) return;

  try_send(heartbeat_msg_);
  restart_heartbeat_timer();
}

void BranchConnection::start_receive(SharedBuffer buffer) {
  auto weak_self = std::weak_ptr<BranchConnection>(shared_from_this());
  buffer->resize(constants::kMinRxQueueSize);  // This will fit an entire message
  msg_transport_->receive_async(boost::asio::buffer(*buffer), [=](auto& res, auto msg_size) {
    auto self = weak_self.lock();
    if (!self) return;

    if (res.is_error()) {
      self->on_session_error(res.to_error());
    } else {
      buffer->resize(msg_size);
      self->on_message_received(buffer);
      self->start_receive(buffer);
    }
  });
}

void BranchConnection::on_session_error(const Error& err) {
  heartbeat_timer_.cancel();
  session_handler_(err);
}

void BranchConnection::check_ack_and_set_next_result(const Result& res, const Buffer& ack_msg) {
  if (res.is_error()) {
    next_result_ = res;
  } else if (ack_msg.size() != ack_msg_.get_size() || ack_msg[0] != ack_msg_.serialize()[0]) {
    next_result_ = Error(YOGI_ERR_DESERIALIZE_MSG_FAILED);
  }
}

bool BranchConnection::check_next_result(CompletionHandler handler) {
  if (next_result_.is_error()) {
    auto res = next_result_;
    context_->post([=] { handler(res); });

    return false;
  }

  return true;
}

void BranchConnection::on_message_received(const SharedBuffer& msg) {
  IncomingMessage::deserialize(*msg, rcv_handler_);
}

std::ostream& operator<<(std::ostream& os, const BranchConnection& conn) {
  os << conn.get_remote_branch_info();
  return os;
}
