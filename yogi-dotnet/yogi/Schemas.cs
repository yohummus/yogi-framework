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

using System.Runtime.InteropServices;

public static partial class Yogi
{
    /// <summary>
    /// Returns a built-in JSON Schema.
    ///
    /// The built-in schemas may contain references to other schemas. These
    /// references are based on the schema filenames as set in the $id property.
    ///
    /// The schema will be returned formatted and with whitespace.
    /// </summary>
    /// <param name="schema">Schema to retrieve</param>
    /// <returns>Schema as serialized JSON</returns>
    public static string GetSchema(Schema schema)
    {
        return Marshal.PtrToStringAnsi(YogiCore.YOGI_GetSchema((int)schema));
    }
}
