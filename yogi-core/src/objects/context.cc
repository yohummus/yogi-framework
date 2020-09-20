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
#include <src/objects/context.h>

#include <boost/asio/post.hpp>

#include <chrono>

YOGI_DEFINE_INTERNAL_LOGGER("Context")

Context::Context() : ioc_(1), work_(ioc_), running_(false) { set_logging_prefix(*this); }

Context::~Context() {
  stop();
  wait_for_stopped(Duration::inf());
}

int Context::poll() {
  return run_impl([&] { return ioc_.poll(); });
}

int Context::poll_one() {
  return run_impl([&] { return ioc_.poll_one(); });
}

int Context::run(Duration duration) {
  return run_impl([&] {
    if (duration.is_inf()) {
      return ioc_.run();
    } else {
      return ioc_.run_for(duration.to_chrono_duration());
    }
  });
}

int Context::run_one(Duration duration) {
  return run_impl([&] {
    if (duration.is_inf()) {
      return ioc_.run_one();
    } else {
      return ioc_.run_one_for(duration.to_chrono_duration());
    }
  });
}

void Context::run_in_background() {
  set_running_flag_and_reset();
  thread_ = std::thread([&] {
    try {
      ioc_.run();
    } catch (const std::exception& e) {
      LOG_FAT("Exception caught in context background thread: " << e.what());
    } catch (...) {
      LOG_FAT("Unknown Exception caught in context background thread");
    }

    clear_running_flag();
  });
}

void Context::stop() {
  std::lock_guard<std::mutex> lock{mutex_};
  ioc_.stop();
}

bool Context::wait_for_running(Duration timeout) {
  std::unique_lock<std::mutex> lock{mutex_};

  bool timed_out = false;
  if (timeout.is_inf()) {
    cv_.wait(lock, [&] { return running_; });
  } else {
    timed_out = !cv_.wait_for(lock, timeout.to_chrono_duration(), [&] { return running_; });
  }

  return !timed_out;
}

bool Context::wait_for_stopped(Duration timeout) {
  std::unique_lock<std::mutex> lock{mutex_};

  bool timed_out = false;
  if (timeout.is_inf()) {
    cv_.wait(lock, [&] { return !running_; });
  } else {
    timed_out = !cv_.wait_for(lock, timeout.to_chrono_duration(), [&] { return !running_; });
  }

  if (!timed_out && thread_.joinable()) {
    thread_.join();
  }

  return !timed_out;
}

void Context::post(std::function<void()> fn) { boost::asio::post(ioc_, fn); }

void Context::set_running_flag_and_reset() {
  std::lock_guard<std::mutex> lock{mutex_};

  if (running_) throw Error{YOGI_ERR_BUSY};

  running_ = true;
  ioc_.restart();
  cv_.notify_all();
}

void Context::clear_running_flag() {
  std::lock_guard<std::mutex> lock{mutex_};
  YOGI_ASSERT(running_ == true);
  running_ = false;
  cv_.notify_all();
}

template <typename Fn>
int Context::run_impl(Fn fn) {
  set_running_flag_and_reset();
  int cnt = static_cast<int>(fn());
  clear_running_flag();
  return cnt;
}
