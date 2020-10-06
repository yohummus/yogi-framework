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

#include <src/config.h>

#include <src/data/buffer.h>
#include <src/util/time.h>

#include <boost/endian/arithmetic.hpp>
#include <boost/uuid/uuid.hpp>

#include <algorithm>
#include <chrono>
#include <string>
#include <vector>

template <typename BigEndianType, typename T>
inline void serialize_integer(Buffer* buffer, const T& val) {
  BigEndianType big = val;
  buffer->resize(buffer->size() + sizeof(big));
  std::memcpy(buffer->data() + buffer->size() - sizeof(big), &big, sizeof(big));
}

template <typename BigEndianType, typename T>
inline bool deserialize_integer(T* val, const Buffer& buffer, Buffer::const_iterator* it) {
  BigEndianType big;
  if (static_cast<std::size_t>(std::distance(*it, buffer.end())) < sizeof(big)) {
    return false;
  }

  std::copy(*it, *it + sizeof(big), reinterpret_cast<Byte*>(&big));
  *it += sizeof(big);
  *val = big;
  return true;
}

template <typename T>
inline void serialize(Buffer* buffer, const T& val);

template <typename T>
bool deserialize(T* val, const Buffer& buffer, Buffer::const_iterator* it);

// bool
template <>
inline void serialize<bool>(Buffer* buffer, const bool& val) {
  buffer->push_back(val ? 1 : 0);
}

template <>
inline bool deserialize<bool>(bool* val, const Buffer& buffer, Buffer::const_iterator* it) {
  if (std::distance(*it, buffer.cend()) < 1) {
    return false;
  }

  *val = **it ? true : false;
  *it += 1;

  return true;
}

// unsigned short
template <>
inline void serialize<unsigned short>(Buffer* buffer, const unsigned short& val) {
  serialize_integer<boost::endian::big_uint16_t>(buffer, val);
}

template <>
inline bool deserialize<unsigned short>(unsigned short* val, const Buffer& buffer, Buffer::const_iterator* it) {
  return deserialize_integer<boost::endian::big_uint16_t>(val, buffer, it);
}

// int
template <>
inline void serialize<int>(Buffer* buffer, const int& val) {
  serialize_integer<boost::endian::big_int32_t>(buffer, val);
}

template <>
inline bool deserialize<int>(int* val, const Buffer& buffer, Buffer::const_iterator* it) {
  return deserialize_integer<boost::endian::big_int32_t>(val, buffer, it);
}

// long long
template <>
inline void serialize<long long>(Buffer* buffer, const long long& val) {
  serialize_integer<boost::endian::big_int64_t>(buffer, val);
}

template <>
inline bool deserialize<long long>(long long* val, const Buffer& buffer, Buffer::const_iterator* it) {
  return deserialize_integer<boost::endian::big_int64_t>(val, buffer, it);
}

// size_t
template <>
inline void serialize<std::size_t>(Buffer* buffer, const std::size_t& val) {
  serialize_integer<boost::endian::big_uint32_t>(buffer, static_cast<std::uint32_t>(val));
}

template <>
inline bool deserialize<std::size_t>(std::size_t* val, const Buffer& buffer, Buffer::const_iterator* it) {
  return deserialize_integer<boost::endian::big_uint32_t>(val, buffer, it);
}

// std::chrono::nanoseconds
template <>
inline void serialize<std::chrono::nanoseconds>(Buffer* buffer, const std::chrono::nanoseconds& dur) {
  serialize_integer<boost::endian::big_int64_t>(buffer, dur.count());
}

template <>
inline bool deserialize<std::chrono::nanoseconds>(std::chrono::nanoseconds* dur, const Buffer& buffer,
                                                  Buffer::const_iterator* it) {
  std::chrono::nanoseconds::rep n;
  if (!deserialize_integer<boost::endian::big_int64_t>(&n, buffer, it)) {
    return false;
  }

  *dur = std::chrono::nanoseconds(n);
  return true;
}

// Timestamp
template <>
inline void serialize<Timestamp>(Buffer* buffer, const Timestamp& time) {
  serialize(buffer, time.ns_since_epoch());
}

template <>
inline bool deserialize<Timestamp>(Timestamp* time, const Buffer& buffer, Buffer::const_iterator* it) {
  long long n;
  if (!deserialize(&n, buffer, it)) {
    return false;
  }

  *time = Timestamp(n);
  return true;
}

// std::string
template <>
inline void serialize<std::string>(Buffer* buffer, const std::string& str) {
  buffer->insert(buffer->end(), str.c_str(), str.c_str() + str.size() + 1);
}

template <>
inline bool deserialize<std::string>(std::string* str, const Buffer& buffer, Buffer::const_iterator* it) {
  auto end = std::find(*it, buffer.cend(), '\0');
  if (end == buffer.end()) {
    return false;
  }

  *str = std::string(*it, end);
  *it  = end + 1;
  return true;
}

// boost::uuids::uuid
template <>
inline void serialize<boost::uuids::uuid>(Buffer* buffer, const boost::uuids::uuid& uuid) {
  buffer->insert(buffer->end(), uuid.begin(), uuid.end());
}

template <>
inline bool deserialize<boost::uuids::uuid>(boost::uuids::uuid* uuid, const Buffer& buffer,
                                            Buffer::const_iterator* it) {
  if (static_cast<std::size_t>(std::distance(*it, buffer.cend())) < sizeof(*uuid)) {
    return false;
  }

  std::copy_n(*it, sizeof(*uuid), uuid->begin());
  *it += sizeof(*uuid);
  return true;
}
