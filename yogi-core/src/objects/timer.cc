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
#include <src/objects/timer.h>

Timer::Timer(ContextPtr context) : context_(context), timer_(context->io_context()) {}

void Timer::start_async(Duration timeout, HandlerFn fn, void* userarg) {
  timer_.expires_after(timeout.to_chrono_duration());
  timer_.async_wait([=](const auto& ec) {
    YOGI_ASSERT(!ec || ec == boost::asio::error::operation_aborted);
    fn(ec ? YOGI_ERR_CANCELED : YOGI_OK, userarg);
  });
}

bool Timer::cancel() { return timer_.cancel() != 0; }
