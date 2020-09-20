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

YOGI_API int YOGI_WebServerCreate(void** server, void* context, void* branch, void* config, const char* section) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_WebServerAddWorker(void* server, void* context) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_WebRouteCreate(void** route, void* server, const char* baseuri,
                                 void (*fn)(int res, int rid, const char* user, const char* owner, int method,
                                            const char* uri, const char* const* params, void* userarg),
                                 void* userarg) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_WebRouteRespond(void* route, int rid, int finished, int status, const char* contype,
                                  const char* content) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_WebProcessCreate(void** wproc, void* server, const char* name,
                                   void (*fn)(int res, int wpid, const char* user, int wpa, const void* data,
                                              const int* sizes, void* userarg),
                                   void* userarg) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}

YOGI_API int YOGI_WebProcessUpdate(void* wproc, int wpid, int wpu, int exitcode, const void* output, int outsize) {
  // TODO
  return YOGI_ERR_UNKNOWN;
}
