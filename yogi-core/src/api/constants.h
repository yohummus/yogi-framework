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

namespace constants {

// :CODEGEN_BEGIN:
const char* const kVersion                    = YOGI_HDR_VERSION;
int const kVersionMajor                       = YOGI_HDR_VERSION_MAJOR;
int const kVersionMinor                       = YOGI_HDR_VERSION_MINOR;
int const kVersionPatch                       = YOGI_HDR_VERSION_PATCH;
const char* const kVersionSuffix              = YOGI_HDR_VERSION_SUFFIX;
const char* const kDefaultAdvInterfaces       = "[\"localhost\"]";
const char* const kDefaultAdvAddress          = "ff02::8000:2439";
int const kDefaultAdvPort                     = 13531;
long long const kDefaultAdvInterval           = 1000000000ll;
long long const kDefaultConnectionTimeout     = 3000000000ll;
int const kDefaultLoggerVerbosity             = YOGI_VB_INFO;
const char* const kDefaultLogTimeFormat       = "%F %T.%3";
const char* const kDefaultLogFormat           = "$t [T$T] $<$s $c: $m$>";
int const kMaxMessagePayloadSize              = 32768;
const char* const kDefaultTimeFormat          = "%FT%T.%3Z";
const char* const kDefaultInfDurationString   = "%-inf";
const char* const kDefaultDurationFormat      = "%-%dd %T.%3%6%9";
const char* const kDefaultInvalidHandleString = "INVALID HANDLE";
const char* const kDefaultObjectFormat        = "$T [$x]";
int const kMinTxQueueSize                     = 35000;
int const kMaxTxQueueSize                     = 10000000;
int const kDefaultTxQueueSize                 = 35000;
int const kMinRxQueueSize                     = 35000;
int const kMaxRxQueueSize                     = 10000000;
int const kDefaultRxQueueSize                 = 35000;
int const kDefaultWebPort                     = 8443;
const char* const kDefaultWebInterfaces       = "[\"localhost\"]";
long long const kDefaultWebTimeout            = 30000000000ll;
int const kDefaultHttpHeaderLimit             = 1000;
int const kDefaultHttpBodyLimit               = 10000;
int const kDefaultWebCacheSize                = 25000000;
int const kMaxWebCacheSize                    = 1000000000;
const char* const kDefaultAdminUser           = "admin";
const char* const kDefaultAdminPassword       = "admin";
const char* const kDefaultSslPrivateKey =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQDccyREZEfU28sW\n"
    "kdtTxK8XA5pMdAlulFTizhgh9KTs62uKgHWq2zx2ISlZ+4cQfhLTATZBVhwQVLuD\n"
    "RiatrDqPL/cN2m5XER/vFUJw3vJm2u7qKmJGWQ0i4j3O7Yle+uYJHn+35TIUfEX5\n"
    "CgYnpt65lAjfbN1tl2ACbESa4E2nymZMSyOzTFd8xlL+nID2eG3CvKEKsg/bzXIV\n"
    "kf6IlNfpu1tCyeGlwKYaNMymP4Rgp/8Y+Gdp2NZh1JOjqZ9Cavn2divMdEb856hx\n"
    "nnqmoR/IiRyXJU8BdY1x2+NjbBmdcMNHTGioqhmzny+L1Pdfq6KgN4sIj0dQqe2j\n"
    "OfpMvJR1AgMBAAECggEBAJTmSLbRLRt/IduqxQP9s23OByhgsAY6tmNDZVV6EvPb\n"
    "x5WTUucGkf4QRAf9NqtTHI/dH7Jd4TK+qLDwbyubSypv8WUwBptUe8dXCruGOyBx\n"
    "/DG6UwrFF3Rab/kWxFSdD0dVjFq/umXP0st1k9awhXu/m4ad16owNq8sReJ+YrVT\n"
    "ZTIgowAklZ9QKnOCjZ3gbDWS8nn6dgonwU0INJD7iKJ3Mw4rv7Q18/7G7T+YmwHO\n"
    "QdTy8M7MBLKu+ifQjgh9khFZZ8G0/jdV9ZkLbTNR/OfyO7XsPhWMQehRWryN9x2a\n"
    "TekT8K0xJRu6NytDvvdeb139JlQjH6DnVrtMj8I4R/kCgYEA+ADkhZnI3ceHS2Kf\n"
    "+kUbbOJI6/ncm9eHOZ/dEVyIK+iYJO9Eba2XqjIMXl6scZpxRAakHG/zRA23fc+c\n"
    "R0mKEIRxJz9kLxpocW8ib+0LIeb0XdK8xt6JabW4EOtLExu3lIXpa+hkH7xr30Bb\n"
    "+OQZdUpDkk6gBtKbk433w9WfQPcCgYEA447Pn2zgE+Oj8sHEp1qdYbj7CMo0JcQj\n"
    "u4mmb5+BT2FiAlmuetoFGo2/0uGm4ha4cFtj1u58y00Ai8c+dKOrv9LzDHe09jaD\n"
    "uGu4vbwCC0l8wiTKrz5m/Wl3oGAi2tWYUEpDyFBYj9yUEsOtZnhY8S0e2LbrPV6L\n"
    "IftsXcZs5vMCgYEAh3JtXK417zJ3KOvvdY9iwCACNaccHp0ixI0T/tRrppd86Mco\n"
    "t0IU0CZPbQcF5XG1JLL/1GwsD2Hycir+Lkzys7yx0/6iJ7ujiThqNXjgGJ77nXZo\n"
    "FbEamXpe0ha/xOrhY6OTbZTZgh+1RpPu50skwFNT/kckzTUfNQJXbERymtcCgYBn\n"
    "XTJwP1tVjXymQ2AZiK6eehEFgBvJ39s/T5pbb7+kGB8V5Di+MxRIaITuvHM6lYl3\n"
    "9/tU4QWsrzCFTZ7QCpx0SNUAH8wCXubcOYo0HRdsmxfhdCbhDV5o06R0I9cRQh6q\n"
    "seEyN1TTHCXilCHKvvXfr1gopoYaXTyj1fn2arCDAwKBgQDHc9V+9IpZ2F+ZwSYn\n"
    "didwWQfsAZx5EiXtnXnyx0hkNjgebFQAgDJY4vvRphaD0bumywfNMOz/59jjjVZg\n"
    "cWXs9H91XtA7cT7wJi+xiGe6R8zRcVPfImwQchSsCoMGuyLCygJTJPMzGT+YbpBA\n"
    "Umrn/FzslGZsXxwMCR0E8654Kw==\n"
    "-----END PRIVATE KEY-----\n";
const char* const kDefaultSslCertificateChain =
    "-----BEGIN CERTIFICATE-----\n"
    "MIICvjCCAaYCCQCL4CgivAdrbzANBgkqhkiG9w0BAQsFADAhMQswCQYDVQQGEwJV\n"
    "UzESMBAGA1UEAwwJbG9jYWxob3N0MB4XDTE5MDUxODA3MDQyOFoXDTI5MDUxNTA3\n"
    "MDQyOFowITELMAkGA1UEBhMCVVMxEjAQBgNVBAMMCWxvY2FsaG9zdDCCASIwDQYJ\n"
    "KoZIhvcNAQEBBQADggEPADCCAQoCggEBANxzJERkR9TbyxaR21PErxcDmkx0CW6U\n"
    "VOLOGCH0pOzra4qAdarbPHYhKVn7hxB+EtMBNkFWHBBUu4NGJq2sOo8v9w3ablcR\n"
    "H+8VQnDe8mba7uoqYkZZDSLiPc7tiV765gkef7flMhR8RfkKBiem3rmUCN9s3W2X\n"
    "YAJsRJrgTafKZkxLI7NMV3zGUv6cgPZ4bcK8oQqyD9vNchWR/oiU1+m7W0LJ4aXA\n"
    "pho0zKY/hGCn/xj4Z2nY1mHUk6Opn0Jq+fZ2K8x0RvznqHGeeqahH8iJHJclTwF1\n"
    "jXHb42NsGZ1ww0dMaKiqGbOfL4vU91+roqA3iwiPR1Cp7aM5+ky8lHUCAwEAATAN\n"
    "BgkqhkiG9w0BAQsFAAOCAQEAQQezEoFlVkRjB7x1QljRUAVqUSPpk4bERkynB3Nb\n"
    "hajLKKwZMeO8F4GDkAnY2F7ZD6KPQkOlKMVJJVmtVi3OTvutDJnc8jDXZUyejWe+\n"
    "tSwLU2Uo7scPjYynyyPHcNkut+V7XjvhLJr267X0O6iymYnyJBGweVppwwgfxAmH\n"
    "Rzv2uFLf+U7odtQmSC1g/VdImDoJsfASqnycXcqkmiDfv6Pjqp1dvUm9aDCIFdkf\n"
    "DXShGXE+SVXQ61FVMhV62OsNY36mM5lR2kMXwgybRNMWla8Cmu8OhCkftOvVLdW2\n"
    "tAVd+K6fpZe/mdCCuN3pXCCqu/0vPlFoLNMGZrKbLU8W6Q==\n"
    "-----END CERTIFICATE-----\n";
const char* const kDefaultSslDhParams =
    "-----BEGIN DH PARAMETERS-----\n"
    "MIIBCAKCAQEA4iRXTwOzm97m19J21Pg67Vda5Ocw1PS6/CS+0ks//CpvNaYz0hJA\n"
    "YbvCPzQLmxYyUH8NwYutfNrESWtBFs3qEKiZ9zx09cpacXf/gw4VBfclIl2HAlNo\n"
    "5jWzh9VQBc3CxSNJqCRiJUvgnVCx1ec47cH3vkEucw0ewzxdwkpXgOGbja5BxCuO\n"
    "Mtwb+qTXm4XozdAPPWlwryFvwJL60pvtsF7f9S8xZHIe309yeCNnUajmqyKwdJ18\n"
    "P8RNYFHDe8vvaJ7/cnNCMqWFl16hVmBoIYy11H+R8WAphniJKV6fdGTs+7OLSc7Q\n"
    "4/QFcIxwOY+wIaH2OEuOxTrKjAivFrGz8wIBAg==\n"
    "-----END DH PARAMETERS-----\n";
// :CODEGEN_END:

}  // namespace constants

void get_constant(void* dest, int constant);
