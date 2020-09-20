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

#include <src/objects/signal_set.h>

void SignalSet::raise_signal(int signal, void* sigarg, CleanupHandler cleanup_handler, void* userarg) {
  YOGI_ASSERT(signal != 0);

  auto data    = std::make_shared<SignalData>();
  data->signal = signal;
  data->sigarg = sigarg;

  data->cleanup_handler = [=] {
    if (cleanup_handler) {
      cleanup_handler(sigarg, userarg);
    }
  };

  auto sets = ObjectRegister::get_matching<SignalSet>([&](auto& set) { return set->signals_ & signal; });

  data->cnt = sets.size();
  if (data->cnt == 0) {
    data->cleanup_handler();
    return;
  }

  for (auto& set : sets) {
    set->on_signal_raised(data);
  }
}

SignalSet::SignalSet(ContextPtr context, int signals) : context_(context), signals_(signals) {}

bool SignalSet::await_async(AwaitHandler handler, void* userarg) {
  std::lock_guard<std::mutex> lock{mutex_};

  bool canceled = false;
  if (await_handler_) {
    canceled = true;
    context_->post([fn = await_handler_] { fn(YOGI_ERR_CANCELED, 0, nullptr); });
  }

  if (handler) {
    await_handler_ = [=](int res, int signal, void* sigarg) { handler(res, signal, sigarg, userarg); };
  } else {
    await_handler_ = {};
  }

  if (!queue_.empty()) {
    deliver_next_signal();
  }

  return canceled;
}

SignalSet::~SignalSet() {
  while (!queue_.empty()) {
    auto data = queue_.front();
    queue_.pop();

    --data->cnt;
    if (data->cnt == 0) {
      context_->post([=] { data->cleanup_handler(); });
    }
  }

  cancel_await();
}

bool SignalSet::cancel_await() { return await_async({}, nullptr); }

void SignalSet::on_signal_raised(const SignalDataPtr& data) {
  YOGI_ASSERT(signals_ & data->signal);

  std::lock_guard<std::mutex> lock{mutex_};
  queue_.push(data);
  deliver_next_signal();
}

void SignalSet::deliver_next_signal() {
  YOGI_ASSERT(!queue_.empty());
  if (!await_handler_) return;

  auto handler   = await_handler_;
  await_handler_ = {};

  auto data = queue_.front();
  queue_.pop();

  context_->post([=] {
    handler(YOGI_OK, data->signal, data->sigarg);
    --data->cnt;
    if (data->cnt == 0) {
      data->cleanup_handler();
    }
  });
}
