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

#include <src/api/object.h>
#include <src/objects/logger/log_user.h>
#include <src/util/time.h>

#include <boost/asio/io_context.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>

class Context : public ExposedObjectT<Context, ObjectType::kContext>, public LogUser {
 public:
  Context();
  virtual ~Context();

  boost::asio::io_context& io_context() { return ioc_; }

  int poll();
  int poll_one();
  int run(Duration duration);
  int run_one(Duration duration);
  void run_in_background();
  void stop();
  bool wait_for_running(Duration timeout);
  bool wait_for_stopped(Duration timeout);
  void post(std::function<void()> fn);

 private:
  void set_running_flag_and_reset();
  void clear_running_flag();

  template <typename Fn>
  int run_impl(Fn fn);

  boost::asio::io_context ioc_;
  boost::asio::io_context::work work_;
  bool running_;
  std::mutex mutex_;
  std::condition_variable cv_;
  std::thread thread_;
};

typedef std::shared_ptr<Context> ContextPtr;
typedef std::weak_ptr<Context> ContextWeakPtr;
