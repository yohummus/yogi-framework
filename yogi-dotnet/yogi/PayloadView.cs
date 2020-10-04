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

using System.Diagnostics;

public static partial class Yogi
{
    /// <summary>
    /// Helper class for passing and converting different types of user payload.
    /// </summary>
    public class PayloadView
    {
        /// <summary>
        /// Constructs a view from a buffer.
        /// </summary>
        /// <param name="data">Buffer to use.</param>
        /// <param name="size">Number of bytes in the buffer.</param>
        /// <param name="enc">Encoding of the data in buffer.</param>
        public PayloadView(byte[] data, int size, Encoding enc)
        {
            Data = data;
            Size = size;
            Encoding = enc;

            Debug.Assert(size >= 0);
            Debug.Assert(data.Length >= size);
            Debug.Assert(enc != Encoding.Json || data[size - 1] == 0);
        }

        /// <summary>
        /// Constructs a view from a buffer.
        /// </summary>
        /// <param name="data">Buffer to use.</param>
        /// <param name="enc">Encoding of the data in buffer.</param>
        public PayloadView(byte[] data, Encoding enc)
        : this(data, data.Length, enc)
        {
        }

        /// <summary>
        /// Constructs a view from JSON data.
        /// </summary>
        /// <param name="json">The JSON data.</param>
        public PayloadView(JsonView json)
        : this(json.Data, Encoding.Json)
        {
        }

        public static implicit operator PayloadView(JsonView json)
        {
            return new PayloadView(json);
        }

        /// <summary>
        /// Constructs a view from MessagePack data.
        /// </summary>
        /// <param name="msgpack">The MessagePack data.</param>
        public PayloadView(MsgpackView msgpack)
        : this(msgpack.Data, Encoding.Msgpack)
        {
        }

        public static implicit operator PayloadView(MsgpackView msgpack)
        {
            return new PayloadView(msgpack);
        }

        public static bool operator ==(PayloadView lhs, PayloadView rhs)
        {
            if (object.ReferenceEquals(lhs, null))
            {
                return object.ReferenceEquals(rhs, null);
            }

            return lhs.Equals(rhs);
        }

        public static bool operator !=(PayloadView lhs, PayloadView rhs)
        {
            return !(lhs == rhs);
        }

        public override bool Equals(object obj)
        {
            if (!(obj is PayloadView)) return false;

            var other = (PayloadView)obj;
            if (Encoding != other.Encoding) return false;
            if (Size != other.Size) return false;

            for (int i = 0; i < Size; ++i)
            {
                if (Data[i] != other.Data[i]) return false;
            }

            return true;
        }

        public override int GetHashCode()
        {
            return Data.GetHashCode() * 17 + Encoding.GetHashCode();
        }

        /// <summary>
        /// Buffer holding the payload data.
        /// </summary>
        public byte[] Data { get; }

        /// <summary>
        /// Size of the payload data in bytes.
        /// </summary>
        public int Size { get; }

        /// <summary>
        /// Encoding of the payload data.
        /// </summary>
        public Encoding Encoding { get; }
    }
}
