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

#include <src/objects/context.h>

#include <functional>
#include <mutex>
#include <queue>

class SignalSet : public ExposedObjectT<SignalSet, ObjectType::kSignalSet> {
 public:
  using CleanupHandler = void (*)(void* sigarg, void* userarg);
  using AwaitHandler   = void (*)(int res, int signal, void* sigarg, void* userarg);

  static void raise_signal(int signal, void* sigarg, CleanupHandler cleanup_handler, void* userarg);

  SignalSet(ContextPtr context, int signals);
  virtual ~SignalSet();

  bool await_async(AwaitHandler handler, void* userarg);
  bool cancel_await();

 private:
  struct SignalData {
    int signal;
    void* sigarg;
    std::size_t cnt;
    std::function<void()> cleanup_handler;
  };

  using SignalDataPtr = std::shared_ptr<SignalData>;

  void on_signal_raised(const SignalDataPtr& data);
  void deliver_next_signal();

  const ContextPtr context_;
  const int signals_;

  std::mutex mutex_;
  std::function<void(int res, int signal, void* sigarg)> await_handler_;
  std::queue<SignalDataPtr> queue_;
};

typedef std::shared_ptr<SignalSet> SignalSetPtr;
