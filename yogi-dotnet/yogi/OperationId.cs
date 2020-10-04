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
    /// Represents the ID of an asynchronous operation.
    /// </summary>
    public struct OperationId
    {
        /// <summary>
        /// Constructs the operation ID from a value.

        /// </summary>
        /// <param name="value">Numerical value of the operation ID.</param>
        public OperationId(int value = 0)
        {
            Value = value;
        }

        /// <summary>
        /// Numertical value of the ID.
        /// </summary>
        public int Value { get; }

        /// <summary>
        /// True if the operation ID has a valid numerical value.
        /// </summary>
        public bool IsValid
        {
            get { return Value > 0; }
        }

        public static bool operator ==(OperationId lhs, OperationId rhs)
        {
            if (object.ReferenceEquals(lhs, null))
            {
                return object.ReferenceEquals(rhs, null);
            }

            return lhs.Equals(rhs);
        }

        public static bool operator !=(OperationId lhs, OperationId rhs)
        {
            return !(lhs == rhs);
        }

        public override bool Equals(object obj)
        {
            if (!(obj is OperationId))
            {
                return false;
            }

            return Value == ((OperationId)obj).Value;
        }

        public static bool operator <(OperationId lhs, OperationId rhs)
        {
            return lhs.Value < rhs.Value;
        }

        public static bool operator >(OperationId lhs, OperationId rhs)
        {
            return lhs.Value > rhs.Value;
        }

        public static bool operator <=(OperationId lhs, OperationId rhs)
        {
            return !(lhs > rhs);
        }

        public static bool operator >=(OperationId lhs, OperationId rhs)
        {
            return !(lhs < rhs);
        }

        public override int GetHashCode()
        {
            return Value.GetHashCode();
        }
    }
}
