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

#ifndef _YOGI_H
#define _YOGI_H

//! \file
//!
//! Includes the whole library.

//! \namespace yogi
//!
//! Main namespace for yogi-cpp.

//! \addtogroup enums Enums
//! Enumerations.

//! \addtogroup freefn Free Functions
//! Free functions.

//! \addtogroup logmacros Logging Macros
//! Macros for creating log entries.

// :CODEGEN_BEGIN:
#include "yogi/branch.h"
#include "yogi/buffer.h"
#include "yogi/configuration.h"
#include "yogi/constants.h"
#include "yogi/context.h"
#include "yogi/duration.h"
#include "yogi/enums.h"
#include "yogi/errors.h"
#include "yogi/io.h"
#include "yogi/json.h"
#include "yogi/json_view.h"
#include "yogi/logging.h"
#include "yogi/msgpack_view.h"
#include "yogi/object.h"
#include "yogi/operation_id.h"
#include "yogi/payload_view.h"
#include "yogi/schemas.h"
#include "yogi/signals.h"
#include "yogi/string_view.h"
#include "yogi/timer.h"
#include "yogi/timestamp.h"
#include "yogi/uuid.h"
#include "yogi/version.h"
// :CODEGEN_END:

#endif  // _YOGI_H
