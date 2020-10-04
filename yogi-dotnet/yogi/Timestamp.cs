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
    /// Represents a UTC timestamp.
    ///
    /// Timestamps are expressed in nanoseconds since 01/01/1970 UTC.
    /// </summary>
    public class Timestamp : IComparable
    {
        /// <summary>
        /// Creates a timestamp from a duration since the epoch 01/01/1970 UTC.
        /// </summary>
        /// <param name="duration">Duration since the epoch.</param>
        /// <returns>Timestamp object.</returns>
        public static Timestamp FromDurationSinceEpoch(Duration duration)
        {
            if (!duration.IsFinite || duration < Duration.Zero)
            {
                throw new ArithmeticException("Invalid duration value for use as a timestamp");
            }

            var t = new Timestamp();
            t.durSinceEpoch = duration;
            return t;
        }

        /// <summary>
        /// Creates a timestamp from the current time.
        /// </summary>
        /// <returns>Timestamp corresponding to the current time.</returns>
        public static Timestamp Now
        {
            get
            {
                long timestamp = -1;
                int res = YogiCore.YOGI_GetCurrentTime(ref timestamp);
                CheckErrorCode(res);
                return FromDurationSinceEpoch(Duration.FromNanoseconds(timestamp));
            }
        }

        /// <summary>
        /// Converts a string into a timestamp.
        ///
        /// The fmt parameter describes the textual format of the conversion. The
        /// following placeholders are supported:
        ///     %Y: Four digit year.
        ///     %m: Month name as a decimal 01 to 12.
        ///     %d: Day of the month as decimal 01 to 31.
        ///     %F: Equivalent to %Y-%m-%d (the ISO 8601 date format).
        ///     %H: The hour as a decimal number using a 24-hour clock (00 to 23).
        ///     %M: The minute as a decimal 00 to 59.
        ///     %S: Seconds as a decimal 00 to 59.
        ///     %T: Equivalent to %H:%M:%S (the ISO 8601 time format).
        ///     %3: Milliseconds as decimal number 000 to 999.
        ///     %6: Microseconds as decimal number 000 to 999.
        ///     %9: Nanoseconds as decimal number 000 to 999.
        ///
        /// By default, the timestamp will be parsed in the format
        /// "2009-02-11T12:53:09.123Z".
        /// </summary>
        /// <param name="s">String to parse.</param>
        /// <param name="fmt">Format of the time string.</param>
        /// <returns>The parsed timestamp.</returns>
        public static Timestamp Parse(string s, [Optional] string fmt)
        {
            long timestamp = -1;
            int res = YogiCore.YOGI_ParseTime(ref timestamp, s, fmt);
            CheckErrorCode(res);
            return FromDurationSinceEpoch(Duration.FromNanoseconds(timestamp));
        }

        /// <summary>
        /// Constructs the timestamp using the epoch 01/01/1970 UTC.
        /// </summary>
        public Timestamp()
        {
            durSinceEpoch = Duration.Zero;
        }

        /// <summary>
        /// The duration since 01/01/1970 UTC.
        /// </summary>
        public Duration DurationSinceEpoch { get { return durSinceEpoch; } }

        /// <summary>
        /// The nanoseconds fraction of the timestamp (0-999).
        /// </summary>
        public int Nanoseconds { get { return durSinceEpoch.Nanoseconds; } }

        /// <summary>
        /// The microseconds fraction of the timestamp (0-999).
        /// </summary>
        public int Microseconds { get { return durSinceEpoch.Microseconds; } }

        /// <summary>
        /// The Milliseconds fraction of the timestamp (0-999).
        /// </summary>
        public int Milliseconds { get { return durSinceEpoch.Milliseconds; } }

        /// <summary>
        /// Converts the timestamp to a string.
        ///
        /// The fmt parameter describes the textual format of the conversion. The
        /// following placeholders are supported:
        ///     %Y: Four digit year.
        ///     %m: Month name as a decimal 01 to 12.
        ///     %d: Day of the month as decimal 01 to 31.
        ///     %F: Equivalent to %Y-%m-%d (the ISO 8601 date format).
        ///     %H: The hour as a decimal number using a 24-hour clock (00 to 23).
        ///     %M: The minute as a decimal 00 to 59.
        ///     %S: Seconds as a decimal 00 to 59.
        ///     %T: Equivalent to %H:%M:%S (the ISO 8601 time format).
        ///     %3: Milliseconds as decimal number 000 to 999.
        ///     %6: Microseconds as decimal number 000 to 999.
        ///     %9: Nanoseconds as decimal number 000 to 999.
        ///
        /// By default, the timestamp will be formatted as an ISO-8601 string with
        /// up to millisecond resolution, e.g. "2018-04-23T18:25:43.511Z".
        /// </summary>
        /// <param name="fmt">Format of the time string.</param>
        /// <returns>The timestamp as a string.</returns>
        public string Format([Optional] string fmt)
        {
            var str = new IntPtr();
            int strsize = 0;
            int res = YogiCore.YOGI_FormatTime(durSinceEpoch.NanosecondsCount, ref str, ref strsize, fmt);
            CheckErrorCode(res);
            return Marshal.PtrToStringAnsi(str);
        }

        /// <summary>
        /// Returns the time in ISO-8601 format up to milliseonds.
        ///
        /// Example: "2018-04-23T18:25:43.511Z".
        /// </summary>
        /// <returns>The time in ISO-8601 format.</returns>
        public override string ToString()
        {
            return Format();
        }

        public static Timestamp operator +(Timestamp lhs, Duration rhs)
        {
            return FromDurationSinceEpoch(lhs.durSinceEpoch + rhs);
        }

        public static Timestamp operator -(Timestamp lhs, Duration rhs)
        {
            return FromDurationSinceEpoch(lhs.durSinceEpoch - rhs);
        }

        public static Duration operator -(Timestamp lhs, Timestamp rhs)
        {
            return lhs.durSinceEpoch - rhs.durSinceEpoch;
        }

        public static bool operator ==(Timestamp lhs, Timestamp rhs)
        {
            if (object.ReferenceEquals(lhs, null))
            {
                return object.ReferenceEquals(rhs, null);
            }

            return lhs.Equals(rhs);
        }

        public static bool operator !=(Timestamp lhs, Timestamp rhs)
        {
            return lhs.durSinceEpoch != rhs.durSinceEpoch;
        }

        public override bool Equals(object obj)
        {
            if (!(obj is Timestamp)) return false;
            return durSinceEpoch == ((Timestamp)obj).durSinceEpoch;
        }

        public static bool operator <(Timestamp lhs, Timestamp rhs)
        {
            return lhs.durSinceEpoch < rhs.durSinceEpoch;
        }

        public static bool operator >(Timestamp lhs, Timestamp rhs)
        {
            return lhs.durSinceEpoch > rhs.durSinceEpoch;
        }

        public static bool operator <=(Timestamp lhs, Timestamp rhs)
        {
            return lhs.durSinceEpoch <= rhs.durSinceEpoch;
        }

        public static bool operator >=(Timestamp lhs, Timestamp rhs)
        {
            return lhs.durSinceEpoch >= rhs.durSinceEpoch;
        }

        public override int GetHashCode()
        {
            return durSinceEpoch.GetHashCode();
        }

        public int CompareTo(object that)
        {
            if (!(that is Timestamp)) return -1;
            if (this == (Timestamp)that) return 0;
            return this < (Timestamp)that ? -1 : +1;
        }

        Duration durSinceEpoch;
    }
}
