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

#include <src/lib/lib_helpers.h>

YOGI_API int YOGI_BranchCreate(void** branch, void* context, void* config, const char* section) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_BranchGetInfo(void* branch, void* uuid, char* json, int jsonsize) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_BranchGetConnectedBranches(void* branch, void* uuid, char* json, int jsonsize,
                                             void (*fn)(int res, void* userarg), void* userarg) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_BranchAwaitEventAsync(void* branch, int events, void* uuid, char* json, int jsonsize,
                                        void (*fn)(int res, int event, int evres, void* userarg), void* userarg) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_BranchCancelAwaitEvent(void* branch) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_BranchSendBroadcast(void* branch, int enc, const void* data, int datasize, int block) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_BranchSendBroadcastAsync(void* branch, int enc, const void* data, int datasize, int retry,
                                           void (*fn)(int res, int oid, void* userarg), void* userarg) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_BranchCancelSendBroadcast(void* branch, int oid) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_BranchReceiveBroadcastAsync(void* branch, void* uuid, int enc, void* data, int datasize,
                                              void (*fn)(int res, int size, void* userarg), void* userarg) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_BranchCancelReceiveBroadcast(void* branch) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}
