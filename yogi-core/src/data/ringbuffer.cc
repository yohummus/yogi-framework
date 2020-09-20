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

#include <src/data/ringbuffer.h>

LockFreeRingBuffer::LockFreeRingBuffer(size_type capacity) : capacity_(capacity) {
  write_idx_ = 0;
  read_idx_  = 0;
  data_.resize(capacity + 1);
  YOGI_UNUSED(padding_);
}

bool LockFreeRingBuffer::empty() {
  auto wi = write_idx_.load(std::memory_order_relaxed);
  auto ri = read_idx_.load(std::memory_order_relaxed);
  return wi == ri;
}

bool LockFreeRingBuffer::full() {
  auto wi = write_idx_.load(std::memory_order_relaxed);
  auto ri = read_idx_.load(std::memory_order_acquire);
  return available_for_write(wi, ri) == 0;
}

Byte LockFreeRingBuffer::front() const {
  auto ri = read_idx_.load(std::memory_order_relaxed);
  return data_[ri];
}

void LockFreeRingBuffer::pop() {
  write_idx_.load(std::memory_order_acquire);  // TODO: Do we need this?
  auto ri = read_idx_.load(std::memory_order_relaxed);

  YOGI_ASSERT(!empty());

  auto next = next_index(ri);
  read_idx_.store(next, std::memory_order_release);
}

LockFreeRingBuffer::size_type LockFreeRingBuffer::available_for_read() const {
  auto wi = write_idx_.load(std::memory_order_acquire);
  auto ri = read_idx_.load(std::memory_order_relaxed);
  return available_for_read(wi, ri);
}

LockFreeRingBuffer::size_type LockFreeRingBuffer::read(Byte* buffer, size_type max_size) {
  auto wi = write_idx_.load(std::memory_order_acquire);
  auto ri = read_idx_.load(std::memory_order_relaxed);

  auto avail = available_for_read(wi, ri);
  if (avail == 0) {
    return 0;
  }

  max_size = std::min(max_size, avail);

  auto new_ri = ri + max_size;
  if (new_ri > data_.size()) {
    auto count_0 = data_.size() - ri;
    auto count_1 = max_size - count_0;

    std::copy(data_.begin() + static_cast<long>(ri), data_.begin() + static_cast<long>(data_.size()), buffer);
    std::copy(data_.begin(), data_.begin() + static_cast<long>(count_1), buffer + count_0);

    new_ri -= data_.size();
  } else {
    std::copy(data_.begin() + static_cast<long>(ri), data_.begin() + static_cast<long>(ri + max_size), buffer);

    if (new_ri == data_.size()) {
      new_ri = 0;
    }
  }

  read_idx_.store(new_ri, std::memory_order_release);
  return max_size;
}

LockFreeRingBuffer::size_type LockFreeRingBuffer::discard(size_type max_size) {
  auto wi = write_idx_.load(std::memory_order_acquire);
  auto ri = read_idx_.load(std::memory_order_relaxed);

  auto avail = available_for_read(wi, ri);
  if (avail == 0) {
    return 0;
  }

  max_size = std::min(max_size, avail);

  auto new_ri = ri + max_size;
  if (new_ri > data_.size()) {
    new_ri -= data_.size();
  } else {
    if (new_ri == data_.size()) {
      new_ri = 0;
    }
  }

  read_idx_.store(new_ri, std::memory_order_release);
  return max_size;
}

void LockFreeRingBuffer::commit_first_read_array(size_type n) {
  YOGI_ASSERT(n <= boost::asio::buffer_size(first_read_array()));

  write_idx_.load(std::memory_order_acquire);  // TODO: Do we need this?
  auto ri = read_idx_.load(std::memory_order_relaxed);

  ri += n;
  if (ri == data_.size()) {
    ri = 0;
  }

  read_idx_.store(ri, std::memory_order_release);
}

boost::asio::const_buffers_1 LockFreeRingBuffer::first_read_array() const {
  auto wi = write_idx_.load(std::memory_order_relaxed);
  auto ri = read_idx_.load(std::memory_order_relaxed);

  if (wi < ri) {
    return boost::asio::buffer(data_.data() + ri, data_.size() - ri);
  } else {
    return boost::asio::buffer(data_.data() + ri, wi - ri);
  }
}

LockFreeRingBuffer::size_type LockFreeRingBuffer::available_for_write() const {
  auto wi = write_idx_.load(std::memory_order_relaxed);
  auto ri = read_idx_.load(std::memory_order_acquire);
  return available_for_write(wi, ri);
}

LockFreeRingBuffer::size_type LockFreeRingBuffer::write(const Byte* data, size_type size) {
  auto wi = write_idx_.load(std::memory_order_relaxed);
  auto ri = read_idx_.load(std::memory_order_acquire);

  auto avail = available_for_write(wi, ri);
  if (avail == 0) {
    return 0;
  }

  size_type input_cnt = size;
  input_cnt           = std::min(input_cnt, avail);

  auto new_wi = wi + input_cnt;
  auto last   = data + input_cnt;

  if (new_wi > data_.size()) {
    auto count_0  = data_.size() - wi;
    auto midpoint = data + count_0;

    std::uninitialized_copy(data, midpoint, data_.begin() + static_cast<long>(wi));
    std::uninitialized_copy(midpoint, last, data_.begin());

    new_wi -= data_.size();
  } else {
    std::uninitialized_copy(data, last, data_.begin() + static_cast<long>(wi));

    if (new_wi == data_.size()) {
      new_wi = 0;
    }
  }

  write_idx_.store(new_wi, std::memory_order_release);
  return input_cnt;
}

void LockFreeRingBuffer::commit_first_write_array(size_type n) {
  YOGI_ASSERT(n <= boost::asio::buffer_size(first_write_array()));

  auto wi = write_idx_.load(std::memory_order_relaxed);
  read_idx_.load(std::memory_order_acquire);  // TODO: do we need this?

  wi += n;
  if (wi >= data_.size()) {
    wi -= data_.size();
  }

  write_idx_.store(wi, std::memory_order_release);
}

boost::asio::mutable_buffers_1 LockFreeRingBuffer::first_write_array() {
  auto wi = write_idx_.load(std::memory_order_relaxed);
  auto ri = read_idx_.load(std::memory_order_relaxed);

  if (wi < ri) {
    return boost::asio::buffer(data_.data() + wi, ri - wi - 1);
  }

  return boost::asio::buffer(data_.data() + wi, data_.size() - wi - (ri == 0 ? 1 : 0));
}

LockFreeRingBuffer::size_type LockFreeRingBuffer::available_for_read(size_type write_idx, size_type read_idx) const {
  if (write_idx >= read_idx) {
    return write_idx - read_idx;
  }

  return write_idx + data_.size() - read_idx;
}

LockFreeRingBuffer::size_type LockFreeRingBuffer::available_for_write(size_type write_idx, size_type read_idx) const {
  auto n = read_idx - write_idx - 1;
  if (write_idx >= read_idx) {
    n += data_.size();
  }

  return n;
}

LockFreeRingBuffer::size_type LockFreeRingBuffer::next_index(size_type idx) const {
  idx += 1;
  if (idx >= data_.size()) {
    idx -= data_.size();
  }

  return idx;
}
