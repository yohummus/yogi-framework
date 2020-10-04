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

using System.Linq;

public static partial class Yogi
{
    /// <summary>
    /// Helper class for passing different types of MessagePack data to functions.
    /// </summary>
    public class MsgpackView
    {
        /// <summary>
        /// Constructs a view from a buffer containing serialized MessagePack data.
        /// </summary>
        /// <param name="data">Buffer containing serialized MessagePack data.</param>
        public MsgpackView(byte[] data)
        {
            Data = data;
        }

        public static implicit operator MsgpackView(byte[] data)
        {
            return new MsgpackView(data);
        }

        public static bool operator ==(MsgpackView lhs, MsgpackView rhs)
        {
            if (object.ReferenceEquals(lhs, null))
            {
                return object.ReferenceEquals(rhs, null);
            }

            return lhs.Equals(rhs);
        }

        public static bool operator !=(MsgpackView lhs, MsgpackView rhs)
        {
            return !(lhs == rhs);
        }

        public override bool Equals(object obj)
        {
            if (!(obj is MsgpackView))
            {
                return false;
            }

            return Data.SequenceEqual(((MsgpackView)obj).Data);
        }

        public override int GetHashCode()
        {
            return Data.GetHashCode();
        }

        /// <summary>
        /// Buffer holding the serialized MessagePack data.
        /// </summary>
        public byte[] Data { get; }

        /// <summary>
        /// Length of the serialized MessagePack data in bytes.
        /// </summary>
        public int Size
        {
            get { return Data == null ? 0 : Data.Length; }
        }
    }
}
