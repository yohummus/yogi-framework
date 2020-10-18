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

#include <src/objects/branch/broadcast_manager.h>
#include <src/util/algorithm.h>

YOGI_DEFINE_INTERNAL_LOGGER("Branch.BroadcastManager")

BroadcastManager::BroadcastManager(ContextPtr context, ConnectionManager& conn_manager)
    : context_(context), conn_manager_(conn_manager) {
}

BroadcastManager::~BroadcastManager() {
}

void BroadcastManager::start(LocalBranchInfoPtr info) {
  set_logging_prefix(info->logging_prefix());
}

Result BroadcastManager::send_broadcast(const Payload& payload, bool block) {
  Result result;
  send_broadcast_async(payload, block, [&](auto& res, auto) {
    std::lock_guard<std::mutex> lock(this->tx_sync_mutex_);
    result = res;
    this->tx_sync_cv_.notify_all();
  });

  std::unique_lock<std::mutex> lock(tx_sync_mutex_);
  tx_sync_cv_.wait(lock, [&] { return result != Result(); });

  return result;
}

BroadcastManager::SendBroadcastOperationId BroadcastManager::send_broadcast_async(const Payload& payload, bool retry,
                                                                                  SendBroadcastHandler handler) {
  messages::BroadcastOutgoing msg(payload);

  auto oid = conn_manager_.make_operation_id();

  if (retry) {
    std::shared_ptr<int> pending_handlers;

    std::lock_guard<std::mutex> lock(tx_oids_mutex_);
    conn_manager_.foreach_running_session(
        [&](auto& conn) { this->send_now_or_later(&pending_handlers, &msg, conn, handler, oid); });

    store_oid_for_later_or_call_handler_now(pending_handlers, handler, oid);
  } else {
    bool all_sent = true;
    conn_manager_.foreach_running_session([&](auto& conn) {
      if (!conn->try_send(msg)) {
        all_sent = false;
      }
    });

    if (all_sent) {
      context_->post([=] { handler(Success(), oid); });
    } else {
      context_->post([=] { handler(Error(YOGI_ERR_TX_QUEUE_FULL), oid); });
    }
  }

  return oid;
}

bool BroadcastManager::cancel_send_broadcast(SendBroadcastOperationId oid) {
  {
    std::lock_guard<std::mutex> lock(tx_oids_mutex_);
    auto it = find(tx_active_oids_, oid);
    if (it == tx_active_oids_.end()) return false;
    tx_active_oids_.erase(it);
  }

  bool canceled = false;
  conn_manager_.foreach_running_session([&](auto& conn) { canceled |= conn->cancel_send(oid); });

  return canceled;
}

void BroadcastManager::receive_broadcast(int encoding, boost::asio::mutable_buffer data,
                                         ReceiveBroadcastHandler handler) {
  YOGI_ASSERT(handler);

  std::lock_guard<std::recursive_mutex> lock(rx_mutex_);

  if (rx_handler_) {
    auto old_handler = rx_handler_;
    context_->post([=] { old_handler(Error(YOGI_ERR_CANCELED), {}, 0); });
  }

  rx_encoding_ = encoding;
  rx_data_     = data;
  rx_handler_  = handler;
}

bool BroadcastManager::cancel_receive_broadcast() {
  std::lock_guard<std::recursive_mutex> lock(rx_mutex_);

  if (rx_handler_) {
    auto handler = rx_handler_;
    rx_handler_  = {};
    context_->post([=] { handler(Error(YOGI_ERR_CANCELED), {}, 0); });
    return true;
  }

  return false;
}

void BroadcastManager::on_broadcast_received(const messages::BroadcastIncoming& msg, const BranchConnectionPtr& conn) {
  std::lock_guard<std::recursive_mutex> lock(rx_mutex_);

  if (rx_handler_) {
    auto handler  = rx_handler_;
    rx_handler_   = {};
    std::size_t n = 0;
    auto res      = msg.get_payload().serialize_to_user_buffer(rx_data_, rx_encoding_, &n);
    handler(res, conn->get_remote_branch_info()->get_uuid(), n);
  }
}

void BroadcastManager::send_now_or_later(SharedCounter* pending_handlers, OutgoingMessage* msg,
                                         BranchConnectionPtr conn, SendBroadcastHandler handler,
                                         SendBroadcastOperationId oid) {
  try {
    if (!conn->try_send(*msg)) {
      create_and_increment_counter(pending_handlers);

      try {
        auto& pending_handlers_ref = *pending_handlers;
        auto weak_self             = std::weak_ptr<BroadcastManager>{shared_from_this()};
        conn->send_async(msg, oid, [=](auto&) {
          bool success = false;

          {
            std::lock_guard<std::mutex> lock(tx_oids_mutex_);

            YOGI_ASSERT(pending_handlers_ref);
            bool is_last_handler = --*pending_handlers_ref == 0;
            if (!is_last_handler) return;

            if (auto self = weak_self.lock()) {
              success = self->remove_active_oid(oid);
            }
          }

          if (success) {
            handler(Success(), oid);
          } else {
            handler(Error(YOGI_ERR_CANCELED), oid);
          }
        });
      } catch (...) {
        --**pending_handlers;
        throw;
      }
    }
  } catch (const Error& err) {
    LOG_ERR("Could not send broadcast to " << conn << ": " << err);
  }
}

void BroadcastManager::store_oid_for_later_or_call_handler_now(SharedCounter pending_handlers,
                                                               SendBroadcastHandler handler,
                                                               SendBroadcastOperationId oid) {
  if (pending_handlers) {
    tx_active_oids_.push_back(oid);
  } else {
    context_->post([=] { handler(Success(), oid); });
  }
}

void BroadcastManager::create_and_increment_counter(SharedCounter* counter) {
  if (*counter) {
    ++**counter;
  } else {
    *counter = std::make_shared<int>(1);
  }
}

bool BroadcastManager::remove_active_oid(SendBroadcastOperationId oid) {
  auto it = find(tx_active_oids_, oid);
  if (it != tx_active_oids_.end()) {
    tx_active_oids_.erase(it);
    return true;
  }

  return false;
}
