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

namespace network {
namespace detail {

template <typename BigEndianType, typename T>
inline void SerializeInteger(Buffer* buffer, const T& val) {
  BigEndianType big = val;
  buffer->resize(buffer->size() + sizeof(big));
  std::memcpy(buffer->data() + buffer->size() - sizeof(big), &big, sizeof(big));
}

template <typename BigEndianType, typename T>
inline bool DeserializeInteger(T* val, const Buffer& buffer, Buffer::const_iterator* it) {
  BigEndianType big;
  if (static_cast<std::size_t>(std::distance(*it, buffer.end())) < sizeof(big)) {
    return false;
  }

  std::copy(*it, *it + sizeof(big), reinterpret_cast<Byte*>(&big));
  *it += sizeof(big);
  *val = big;
  return true;
}

}  // namespace detail

template <typename T>
inline void Serialize(Buffer* buffer, const T& val) {
  // static_assert(false, "Missing specialization");
}

template <typename T>
bool Deserialize(T* val, const Buffer& buffer, Buffer::const_iterator* it) {
  // static_assert(false, "Missing specialization");
  return false;
}

// bool
template <>
inline void Serialize<bool>(Buffer* buffer, const bool& val) {
  buffer->push_back(val ? 1 : 0);
}

template <>
inline bool Deserialize<bool>(bool* val, const Buffer& buffer, Buffer::const_iterator* it) {
  if (std::distance(*it, buffer.cend()) < 1) {
    return false;
  }

  *val = **it ? true : false;
  *it += 1;

  return true;
}

// unsigned short
template <>
inline void Serialize<unsigned short>(Buffer* buffer, const unsigned short& val) {
  detail::SerializeInteger<boost::endian::big_uint16_t>(buffer, val);
}

template <>
inline bool Deserialize<unsigned short>(unsigned short* val, const Buffer& buffer, Buffer::const_iterator* it) {
  return detail::DeserializeInteger<boost::endian::big_uint16_t>(val, buffer, it);
}

// int
template <>
inline void Serialize<int>(Buffer* buffer, const int& val) {
  detail::SerializeInteger<boost::endian::big_int32_t>(buffer, val);
}

template <>
inline bool Deserialize<int>(int* val, const Buffer& buffer, Buffer::const_iterator* it) {
  return detail::DeserializeInteger<boost::endian::big_int32_t>(val, buffer, it);
}

// size_t
template <>
inline void Serialize<std::size_t>(Buffer* buffer, const std::size_t& val) {
  detail::SerializeInteger<boost::endian::big_uint32_t>(buffer, static_cast<std::uint32_t>(val));
}

template <>
inline bool Deserialize<std::size_t>(std::size_t* val, const Buffer& buffer, Buffer::const_iterator* it) {
  return detail::DeserializeInteger<boost::endian::big_uint32_t>(val, buffer, it);
}

// std::chrono::nanoseconds
template <>
inline void Serialize<std::chrono::nanoseconds>(Buffer* buffer, const std::chrono::nanoseconds& dur) {
  detail::SerializeInteger<boost::endian::big_int64_t, std::chrono::nanoseconds::rep>(buffer, dur.count());
}

template <>
inline bool Deserialize<std::chrono::nanoseconds>(std::chrono::nanoseconds* dur, const Buffer& buffer,
                                                  Buffer::const_iterator* it) {
  std::chrono::nanoseconds::rep n;
  if (!detail::DeserializeInteger<boost::endian::big_int64_t>(&n, buffer, it)) {
    return false;
  }

  *dur = std::chrono::nanoseconds(n);
  return true;
}

// Timestamp
template <>
inline void Serialize<Timestamp>(Buffer* buffer, const Timestamp& time) {
  Serialize(buffer, time.NanosecondsSinceEpoch());
}

template <>
inline bool Deserialize<Timestamp>(Timestamp* time, const Buffer& buffer, Buffer::const_iterator* it) {
  std::chrono::nanoseconds n;
  if (!Deserialize<std::chrono::nanoseconds>(&n, buffer, it)) {
    return false;
  }

  *time = Timestamp(n);
  return true;
}

// std::string
template <>
inline void Serialize<std::string>(Buffer* buffer, const std::string& str) {
  buffer->insert(buffer->end(), str.c_str(), str.c_str() + str.size() + 1);
}

template <>
inline bool Deserialize<std::string>(std::string* str, const Buffer& buffer, Buffer::const_iterator* it) {
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
inline void Serialize<boost::uuids::uuid>(Buffer* buffer, const boost::uuids::uuid& uuid) {
  buffer->insert(buffer->end(), uuid.begin(), uuid.end());
}

template <>
inline bool Deserialize<boost::uuids::uuid>(boost::uuids::uuid* uuid, const Buffer& buffer,
                                            Buffer::const_iterator* it) {
  if (static_cast<std::size_t>(std::distance(*it, buffer.cend())) < sizeof(*uuid)) {
    return false;
  }

  std::copy_n(*it, sizeof(*uuid), uuid->begin());
  *it += sizeof(*uuid);
  return true;
}

}  // namespace network
