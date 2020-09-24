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

#include <src/data/buffer.h>

#include <boost/asio/buffer.hpp>

#include <atomic>
#include <cstddef>
#include <memory>
#include <vector>

// Implementation based on the lock-free single-producer/single-consumer
// ringbuffer implementation in boost by Tim Blechmann (spsc_queue).
class LockFreeRingBuffer {
 public:
  using size_type = std::size_t;

  explicit LockFreeRingBuffer(size_type capacity);

  size_type capacity() const {
    return capacity_;
  };

  bool empty();
  bool full();
  Byte front() const;
  void pop();
  size_type available_for_read() const;
  size_type read(Byte* buffer, size_type max_size);
  size_type discard(size_type max_size);
  void commit_first_read_array(size_type n);
  boost::asio::const_buffers_1 first_read_array() const;
  size_type available_for_write() const;
  size_type write(const Byte* data, size_type size);
  void commit_first_write_array(size_type n);
  boost::asio::mutable_buffers_1 first_write_array();

  template <typename Fn>
  void pop_until(Fn fn) {
    auto wi = write_idx_.load(std::memory_order_acquire);
    auto ri = read_idx_.load(std::memory_order_relaxed);

    while (available_for_read(wi, ri) > 0) {
      auto byte = data_[ri];
      ri        = next_index(ri);
      if (fn(byte)) break;
    }

    read_idx_.store(ri, std::memory_order_release);
  }

 private:
  size_type available_for_read(size_type write_idx, size_type read_idx) const;
  size_type available_for_write(size_type write_idx, size_type read_idx) const;
  size_type next_index(size_type idx) const;

  static constexpr int kCacheLineSize = 64;
  std::atomic<std::size_t> write_idx_;
  Byte padding_[kCacheLineSize - sizeof(std::size_t)];
  std::atomic<std::size_t> read_idx_;
  const size_type capacity_;
  Buffer data_;
};
