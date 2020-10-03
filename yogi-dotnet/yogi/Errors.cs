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
using System.Diagnostics;

public static partial class Yogi
{
    /// <summary>
    /// Represents a result of an operation.
    ///
    /// This is a wrapper around the result code returned by the functions from
    /// the Yogi Core library. A result is represented by a number which is >= 0
    /// in case of success and < 0 in case of a failure.
    /// </summary>
    public class Result : IComparable
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="value">Number as returned by the Yogi Core library function.</param>
        public Result(int value)
        {
            Value = value;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="ec">Error code.</param>
        public Result(ErrorCode ec)
        {
            Value = (int)ec;
        }

        /// <summary>
        /// The number as returned by the Yogi Core library function.
        /// </summary>
        public int Value { get; }

        /// <summary>
        /// Error code associated with this result.
        /// </summary>
        public ErrorCode ErrorCode
        {
            get
            {
                return Value < 0 ? (ErrorCode)Value : ErrorCode.Ok;
            }
        }

        /// <summary>
        /// Returns a human-readable string describing the result.
        /// </summary>
        public override string ToString()
        {
            return Marshal.PtrToStringAnsi(YogiCore.YOGI_GetErrorString(Value));
        }

        public static bool operator <(Result lhs, Result rhs)
        {
            return lhs.Value < rhs.Value;
        }

        public static bool operator >(Result lhs, Result rhs)
        {
            return lhs.Value > rhs.Value;
        }

        public static bool operator ==(Result lhs, Result rhs)
        {
            if (object.ReferenceEquals(lhs, null))
            {
                return object.ReferenceEquals(rhs, null);
            }

            return lhs.Equals(rhs);
        }

        public static bool operator !=(Result lhs, Result rhs)
        {
            return lhs.Value != rhs.Value;
        }

        public static bool operator <=(Result lhs, Result rhs)
        {
            return lhs.Value <= rhs.Value;
        }

        public static bool operator >=(Result lhs, Result rhs)
        {
            return lhs.Value >= rhs.Value;
        }

        public override bool Equals(object obj)
        {
            if (!(obj is Result)) return false;
            return Value == ((Result)obj).Value;
        }

        public static bool operator true(Result res)
        {
            return res.Value >= 0;
        }

        public static bool operator false(Result res)
        {
            return res.Value < 0;
        }

        public static implicit operator bool(Result res)
        {
            return res.Value >= 0;
        }

        public override int GetHashCode()
        {
            return Value.GetHashCode();
        }

        public int CompareTo(object that)
        {
            if (!(that is Result)) return -1;
            if (this == (Result)that) return 0;
            return this < (Result)that ? -1 : +1;
        }
    }

    /// <summary>
    /// Represents the success of an operation.
    /// </summary>
    public class Success : Result
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="value">Number as returned by the Yogi Core library function.</param>
        /// <returns></returns>
        public Success(int value = 0)
        : base(value)
        {
            Debug.Assert(value >= 0);
        }
    }

    /// <summary>
    /// Represents the failure of an operation.
    /// </summary>
    public class Failure : Result
    {

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="ec">Associated error code.</param>
        public Failure(ErrorCode ec)
        : base(ec)
        {
        }
    }

    /// <summary>
    /// A failure of an operation that includes details.
    /// </summary>
    public class DetailedFailure : Failure
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="ec">Error code.</param>
        /// <param name="details">More detailed information about the error.</param>
        /// <returns></returns>
        public DetailedFailure(ErrorCode ec, string details)
        : base(ec)
        {
            Details = details;
        }

        /// <summary>
        /// Details about the error.
        /// </summary>
        public string Details { get; }

        /// <summary>
        /// Returns a human-readable string describing the error in detail.
        /// </summary>
        public override string ToString()
        {
            return base.ToString() + ". Details: " + Details;
        }
    }

    /// <summary>
    /// Base class for all Yogi exceptions.
    /// </summary>
    public abstract class Exception : System.Exception
    {
        /// <summary>
        /// The wrapped Failure or DetailedFailure object.
        /// </summary>
        public abstract Failure Failure { get; }

        /// <summary>
        /// The exception message.
        /// </summary>
        public override string Message
        {
            get
            {
                return Failure.ToString();
            }
        }
    }

    /// <summary>
    /// Exception wrapping a Failure object.
    ///
    /// This exception type is used for failures without a detailed description.
    /// </summary>
    public class FailureException : Exception
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="ec">Error code associated with the failure.</param>
        public FailureException(ErrorCode ec)
        {
            Failure = new Failure(ec);
        }

        public override Failure Failure { get; }
    }

    /// <summary>
    /// Exception wrapping a DetailedFailure object.
    ///
    /// This exception type is used for failures that have detailed information available.
    /// </summary>
    public class DetailedFailureException : FailureException
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="ec">Error code associated with the failure.</param>
        /// <param name="details">Details of the failure.</param>
        public DetailedFailureException(ErrorCode ec, string details) : base(ec)
        {
            Failure = new DetailedFailure(ec, details);
        }

        public override Failure Failure { get; }
    }

    static void CheckErrorCode(int res)
    {
        if (res >= 0) return;

        var details = Marshal.PtrToStringAnsi(YogiCore.YOGI_GetLastErrorDetails());
        if (details.Length == 0)
        {
            throw new FailureException((ErrorCode)res);
        }
        else
        {
            throw new DetailedFailureException((ErrorCode)res, details);
        }
    }

    static bool FalseIfSpecificErrorElseThrow(int res, ErrorCode ec)
    {
        if (res == (int)ec)
        {
            return false;
        }
        else
        {
            CheckErrorCode(res);
            return true;
        }
    }

    static Result ErrorCodeToResult(int res)
    {
        if (res < 0)
        {
            return new Failure((ErrorCode)res);
        }
        else
        {
            return new Success(res);
        }
    }
}
