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

#ifndef _YOGI_BUFFER_H
#define _YOGI_BUFFER_H

//! \file
//!
//! Buffers for binary data.

#include <memory>
#include <vector>

namespace yogi {

/// Buffer for binary data.
typedef std::vector<char> Buffer;

/// Shared pointer to a buffer object.
typedef std::unique_ptr<Buffer> BufferPtr;

}  // namespace yogi

#endif  // _YOGI_BUFFER_H
