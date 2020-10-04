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

using System;
using System.Runtime.InteropServices;

public static partial class Yogi
{
    /// <summary>
    /// Constants built into the Yogi Core library.
    /// </summary>
    public static class Constants
    {
        // :CODEGEN_BEGIN:

        /// <summary>Whole version string of the core library</summary>
        public static readonly string Version;

        /// <summary>Major version number of the core library</summary>
        public static readonly int VersionMajor;

        /// <summary>Minor version number of the core library</summary>
        public static readonly int VersionMinor;

        /// <summary>Patch version number of the core library</summary>
        public static readonly int VersionPatch;

        /// <summary>Suffix of the core library version</summary>
        public static readonly string VersionSuffix;

        /// <summary>Default network interfaces to use for advertising</summary>
        public static readonly string DefaultAdvInterfaces;

        /// <summary>Default multicast addresses to use for advertising</summary>
        public static readonly string DefaultAdvAddress;

        /// <summary>Default port to use for advertising via UDP IPv6 multicasts</summary>
        public static readonly int DefaultAdvPort;

        /// <summary>Default advertising interval in nanoseconds</summary>
        public static readonly Duration DefaultAdvInterval;

        /// <summary>Default connection timeout in nanoseconds</summary>
        public static readonly Duration DefaultConnectionTimeout;

        /// <summary>Default logging verbosity</summary>
        public static readonly Verbosity DefaultLoggerVerbosity;

        /// <summary>Default textual format for timestamps in log entries</summary>
        public static readonly string DefaultLogTimeFormat;

        /// <summary>Default textual format for log entries</summary>
        public static readonly string DefaultLogFormat;

        /// <summary>Maximum size of the payload in a message</summary>
        public static readonly int MaxMessagePayloadSize;

        /// <summary>Default textual format for timestamps</summary>
        public static readonly string DefaultTimeFormat;

        /// <summary>Default string to denote an infinite duration</summary>
        public static readonly string DefaultInfDurationString;

        /// <summary>Default textual format for duration strings</summary>
        public static readonly string DefaultDurationFormat;

        /// <summary>Default string to denote an invalid object handle</summary>
        public static readonly string DefaultInvalidHandleString;

        /// <summary>Default textual format for strings describing an object</summary>
        public static readonly string DefaultObjectFormat;

        /// <summary>Minimum size of a send queue for a remote branch</summary>
        public static readonly int MinTxQueueSize;

        /// <summary>Maximum size of a send queue for a remote branch</summary>
        public static readonly int MaxTxQueueSize;

        /// <summary>Default size of a send queue for a remote branch</summary>
        public static readonly int DefaultTxQueueSize;

        /// <summary>Minimum size of a receive queue for a remote branch</summary>
        public static readonly int MinRxQueueSize;

        /// <summary>Maximum size of a receive queue for a remote branch</summary>
        public static readonly int MaxRxQueueSize;

        /// <summary>Default size of a receive queue for a remote branch</summary>
        public static readonly int DefaultRxQueueSize;

        /// <summary>Default port for the web server to listen on for client connections</summary>
        public static readonly int DefaultWebPort;

        /// <summary>Default network interfaces to use for the web server</summary>
        public static readonly string DefaultWebInterfaces;

        /// <summary>Default timeout for web server connections in nanoseconds</summary>
        public static readonly Duration DefaultWebTimeout;

        /// <summary>Default maximum HTTP header size of incoming requests in bytes</summary>
        public static readonly int DefaultHttpHeaderLimit;

        /// <summary>Default maximum HTTP body size of incoming requests in bytes</summary>
        public static readonly int DefaultHttpBodyLimit;

        /// <summary>Default size of the web server cache in bytes</summary>
        public static readonly int DefaultWebCacheSize;

        /// <summary>Maximum size of the web server cache in bytes</summary>
        public static readonly int MaxWebCacheSize;

        /// <summary>Default user name for the administrator account</summary>
        public static readonly string DefaultAdminUser;

        /// <summary>Default password for the administrator account</summary>
        public static readonly string DefaultAdminPassword;

        /// <summary>Default private key to use for SSL connections</summary>
        public static readonly string DefaultSslPrivateKey;

        /// <summary>Default certificate chain to use for SSL connections</summary>
        public static readonly string DefaultSslCertificateChain;

        /// <summary>Default DH parameters to use for SSL connections</summary>
        public static readonly string DefaultSslDhParams;

        // :CODEGEN_END:

        static Constants()
        {
            // :CODEGEN_BEGIN:
           ExtractConstant(ref Version, 1);
           ExtractConstant(ref VersionMajor, 2);
           ExtractConstant(ref VersionMinor, 3);
           ExtractConstant(ref VersionPatch, 4);
           ExtractConstant(ref VersionSuffix, 5);
           ExtractConstant(ref DefaultAdvInterfaces, 6);
           ExtractConstant(ref DefaultAdvAddress, 7);
           ExtractConstant(ref DefaultAdvPort, 8);
           ExtractConstant(ref DefaultAdvInterval, 9);
           ExtractConstant(ref DefaultConnectionTimeout, 10);
           ExtractConstant(ref DefaultLoggerVerbosity, 11);
           ExtractConstant(ref DefaultLogTimeFormat, 12);
           ExtractConstant(ref DefaultLogFormat, 13);
           ExtractConstant(ref MaxMessagePayloadSize, 14);
           ExtractConstant(ref DefaultTimeFormat, 15);
           ExtractConstant(ref DefaultInfDurationString, 16);
           ExtractConstant(ref DefaultDurationFormat, 17);
           ExtractConstant(ref DefaultInvalidHandleString, 18);
           ExtractConstant(ref DefaultObjectFormat, 19);
           ExtractConstant(ref MinTxQueueSize, 20);
           ExtractConstant(ref MaxTxQueueSize, 21);
           ExtractConstant(ref DefaultTxQueueSize, 22);
           ExtractConstant(ref MinRxQueueSize, 23);
           ExtractConstant(ref MaxRxQueueSize, 24);
           ExtractConstant(ref DefaultRxQueueSize, 25);
           ExtractConstant(ref DefaultWebPort, 26);
           ExtractConstant(ref DefaultWebInterfaces, 27);
           ExtractConstant(ref DefaultWebTimeout, 28);
           ExtractConstant(ref DefaultHttpHeaderLimit, 29);
           ExtractConstant(ref DefaultHttpBodyLimit, 30);
           ExtractConstant(ref DefaultWebCacheSize, 31);
           ExtractConstant(ref MaxWebCacheSize, 32);
           ExtractConstant(ref DefaultAdminUser, 33);
           ExtractConstant(ref DefaultAdminPassword, 34);
           ExtractConstant(ref DefaultSslPrivateKey, 35);
           ExtractConstant(ref DefaultSslCertificateChain, 36);
           ExtractConstant(ref DefaultSslDhParams, 37);
            // :CODEGEN_END:
        }

        static IntPtr GetConstantPtr(int apiId)
        {
            IntPtr p = new IntPtr();
            CheckErrorCode(YogiCore.YOGI_GetConstant(ref p, apiId));
            return p;
        }

        static void ExtractConstant(ref int constant, int apiId)
        {
            constant = GetConstantPtr(apiId).ToInt32();
        }

        static void ExtractConstant(ref string constant, int apiId)
        {
            constant = Marshal.PtrToStringAnsi(GetConstantPtr(apiId));
        }

        static void ExtractConstant(ref Duration constant, int apiId)
        {
            constant = Duration.FromNanoseconds(GetConstantPtr(apiId).ToInt64());
        }

        static void ExtractConstant(ref Verbosity constant, int apiId)
        {
            constant = (Verbosity)GetConstantPtr(apiId).ToInt32();
        }
    }
}
