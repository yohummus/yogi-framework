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

public static partial class Yogi
{
    /// <summary>
    /// Information about the .NET bindings.
    /// </summary>
    public static class BindingsInfo
    {
        /// <summary>Whole version string of the .NET bindings.</summary>
        public static readonly string Version;

        /// <summary>Major version of the .NET bindings.</summary>
        public static readonly int VersionMajor;

        /// <summary>Minor version of the .NET bindings.</summary>
        public static readonly int VersionMinor;

        /// <summary>Patch version of the .NET bindings.</summary>
        public static readonly int VersionPatch;

        /// <summary>Version suffix of the .NET bindings.</summary>
        public static readonly string VersionSuffix;

        static BindingsInfo()
        {
            // :CODEGEN_BEGIN:
            Version = "0.0.1-alpha";
            VersionMajor = 0;
            VersionMinor = 0;
            VersionPatch = 1;
            VersionSuffix = "-alpha";
            // :CODEGEN_END:
        }
    }

    /// <summary>
    /// Information about the Yogi Core library.
    /// </summary>
    public static class CoreInfo
    {
        /// <summary>Whole version string of the Yogi Core library.</summary>
        public static readonly string Version;

        /// <summary>Major version of the Yogi Core library.</summary>
        public static readonly int VersionMajor;

        /// <summary>Minor version of the Yogi Core library.</summary>
        public static readonly int VersionMinor;

        /// <summary>Patch version of the Yogi Core library.</summary>
        public static readonly int VersionPatch;

        /// <summary>Version suffix of the Yogi Core library.</summary>
        public static readonly string VersionSuffix;

        static CoreInfo()
        {
            Version = Constants.Version;
            VersionMajor = Constants.VersionMajor;
            VersionMinor = Constants.VersionMinor;
            VersionPatch = Constants.VersionPatch;
            VersionSuffix = Constants.VersionSuffix;
        }
    }
}
