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

#include <boost/container/small_vector.hpp>

#include <cstddef>
#include <memory>
#include <vector>

// Byte
typedef unsigned char Byte;

// Buffer
typedef std::vector<Byte> Buffer;
typedef std::shared_ptr<Buffer> SharedBuffer;

template <typename... Args>
SharedBuffer make_shared_buffer(Args&&... args) {
  return std::make_shared<Buffer>(std::forward<Args>(args)...);
}

// SmallBuffer
typedef boost::container::small_vector<Byte, 32> SmallBuffer;
typedef std::shared_ptr<SmallBuffer> SharedSmallBuffer;

template <typename... Args>
SharedSmallBuffer make_shared_small_buffer(Args&&... args) {
  return std::make_shared<SmallBuffer>(std::forward<Args>(args)...);
}
