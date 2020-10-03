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
using Xunit;

namespace test
{
    public class TimestampTest : TestCase
    {
        [Fact]
        public void FromDurationSinceEpoch()
        {
            Assert.Equal(Yogi.Duration.FromDays(123), Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromDays(123)).DurationSinceEpoch);
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.Inf);
            });
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(-1));
            });
        }

        [Fact]
        public void Now()
        {
            MOCK_GetCurrentTime((ref long timestamp) =>
            {
                timestamp = 5555555555L;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(5555555555L, Yogi.Timestamp.Now.DurationSinceEpoch.NanosecondsCount);
        }

        [Fact]
        public void NowError()
        {
            MOCK_GetCurrentTime((ref long timestamp) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() => Yogi.Timestamp.Now);
        }

        [Fact]
        public void Parse()
        {
            MOCK_ParseTime((ref long timestamp, string str, string timefmt) =>
            {
                Assert.Equal("foo", str);
                Assert.Null(timefmt);
                timestamp = 123456789123000000L;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(123456789123000000L, Yogi.Timestamp.Parse("foo").DurationSinceEpoch.NanosecondsCount);
        }

        [Fact]
        public void ParseError()
        {
            MOCK_ParseTime((ref long timestamp, string str, string timefmt) =>
            {
                Assert.Equal("foo", str);
                Assert.Equal("bar", timefmt);
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() => Yogi.Timestamp.Parse("foo", "bar"));
        }

        [Fact]
        public void Constructor()
        {
            Assert.Equal(0, new Yogi.Timestamp().DurationSinceEpoch.NanosecondsCount);
        }

        [Fact]
        public void DurationSinceEpoch()
        {
            var dur = Yogi.Duration.FromDays(123);
            var t = Yogi.Timestamp.FromDurationSinceEpoch(dur);
            Assert.Equal(dur, t.DurationSinceEpoch);
        }

        [Fact]
        public void Nanoseconds()
        {
            Assert.Equal(789, Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(123456789L)).Nanoseconds);
        }

        [Fact]
        public void Microseconds()
        {
            Assert.Equal(456, Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(123456789L)).Microseconds);
        }

        [Fact]
        public void Milliseconds()
        {
            Assert.Equal(123, Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(123456789L)).Milliseconds);
        }

        [Fact]
        public void Format()
        {
            MOCK_FormatTime((long timestamp, ref IntPtr str, ref int strsize, string timefmt) =>
            {
                Assert.Equal(123456789123456789L, timestamp);
                Assert.Null(timefmt);
                str = helloBytes;
                strsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            var t = Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(123456789123456789L));
            Assert.Equal(helloString, t.Format());
        }

        [Fact]
        public void FormatError()
        {
            MOCK_FormatTime((long timestamp, ref IntPtr str, ref int strsize, string timefmt) =>
            {
                Assert.Equal("foo", timefmt);
                return (int)Yogi.ErrorCode.Unknown;
            });

            var t = Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(123456789123456789L));
            Assert.Throws<Yogi.FailureException>(() => t.Format("foo"));
        }

        [Fact]
        public void ToStringMethod()
        {
            MOCK_FormatTime((long timestamp, ref IntPtr str, ref int strsize, string timefmt) =>
            {
                Assert.Equal(123456789123456789L, timestamp);
                Assert.Null(timefmt);
                str = helloBytes;
                strsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            var t = Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(123456789123456789L));
            Assert.Equal(helloString, t.ToString());
        }

        [Fact]
        public void Operators()
        {
            var t = new Yogi.Timestamp();
            t += Yogi.Duration.FromNanoseconds(8);
            Assert.Equal(8, t.DurationSinceEpoch.NanosecondsCount);
            Assert.Throws<ArithmeticException>(() =>
            {
                t += Yogi.Duration.FromMilliseconds(-1);
            });

            t -= Yogi.Duration.FromNanoseconds(1);
            Assert.Equal(7, t.DurationSinceEpoch.NanosecondsCount);
            Assert.Throws<ArithmeticException>(() =>
            {
                t -= Yogi.Duration.FromMilliseconds(1);
            });

            Assert.Equal(12, (t + Yogi.Duration.FromNanoseconds(5)
                ).DurationSinceEpoch.NanosecondsCount);
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = t + Yogi.Duration.FromMilliseconds(-1);
            });

            Assert.Equal(5, (t - Yogi.Duration.FromNanoseconds(2)
                ).DurationSinceEpoch.NanosecondsCount);
            Assert.Equal(4, (t - Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(3))).NanosecondsCount);
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = t - Yogi.Duration.FromMilliseconds(1);
            });

            Assert.True(t == Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(7)));
            Assert.False(t == Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(8)));
            Assert.False(t != Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(7)));
            Assert.True(t != Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(8)));

            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                < Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                < Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                < Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));

            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                > Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                > Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                > Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));

            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                <= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                <= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                <= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));

            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                >= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                >= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                >= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
        }

        [Fact]
        public void EqualsMethod()
        {
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .Equals(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .Equals(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .Equals(""));
        }

        [Fact]
        public void GetHashCodeMethod()
        {
            Assert.NotEqual(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1)),
                Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
        }

        [Fact]
        public void CompareTo()
        {
            Assert.Equal(-1, Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .CompareTo(""));
            Assert.Equal(-1, Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .CompareTo(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))));
            Assert.Equal(0, Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .CompareTo(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))));
            Assert.Equal(+1, Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                .CompareTo(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))));
        }
    }
}
