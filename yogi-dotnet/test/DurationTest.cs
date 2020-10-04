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
    public class DurationTest : TestCase
    {
        [Fact]
        public void Zero()
        {
            var dur = Yogi.Duration.Zero;
            Assert.True(dur.IsFinite);
            Assert.Equal(0, dur.NanosecondsCount);
        }

        [Fact]
        public void Infinity()
        {
            var dur = Yogi.Duration.Inf;
            Assert.False(dur.IsFinite);
            Assert.True(dur > Yogi.Duration.Zero);
        }

        [Fact]
        public void NegativeInfinity()
        {
            var dur = Yogi.Duration.NegInf;
            Assert.False(dur.IsFinite);
            Assert.True(dur < Yogi.Duration.Zero);
        }

        [Fact]
        public void FromNanoseconds()
        {
            var dur = Yogi.Duration.FromNanoseconds(123);
            Assert.True(dur.IsFinite);
            Assert.Equal(123, dur.NanosecondsCount);

            dur = Yogi.Duration.FromNanoseconds(444.56);
            Assert.Equal(444, dur.NanosecondsCount);  // Note: no rounding

            dur = Yogi.Duration.FromNanoseconds(double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.Inf, dur);

            dur = Yogi.Duration.FromNanoseconds(double.NegativeInfinity);
            Assert.Equal(Yogi.Duration.NegInf, dur);
        }

        [Fact]
        public void FromMicroseconds()
        {
            var dur = Yogi.Duration.FromMicroseconds(123);
            Assert.True(dur.IsFinite);
            Assert.Equal(123, dur.TotalMicroseconds);

            dur = Yogi.Duration.FromMicroseconds(double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.Inf, dur);

            dur = Yogi.Duration.FromMicroseconds(0.5);
            Assert.Equal(500, dur.NanosecondsCount);
        }

        [Fact]
        public void FromMilliseconds()
        {
            var dur = Yogi.Duration.FromMilliseconds(123);
            Assert.True(dur.IsFinite);
            Assert.Equal(123, dur.TotalMilliseconds);

            dur = Yogi.Duration.FromMilliseconds(double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.Inf, dur);

            dur = Yogi.Duration.FromMilliseconds(0.5);
            Assert.Equal(500, dur.TotalMicroseconds);
        }

        [Fact]
        public void FromSeconds()
        {
            var dur = Yogi.Duration.FromSeconds(123);
            Assert.True(dur.IsFinite);
            Assert.Equal(123, dur.TotalSeconds);

            dur = Yogi.Duration.FromSeconds(double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.Inf, dur);

            dur = Yogi.Duration.FromSeconds(0.5);
            Assert.Equal(500, dur.TotalMilliseconds);
        }

        [Fact]
        public void FromMinutes()
        {
            var dur = Yogi.Duration.FromMinutes(123);
            Assert.True(dur.IsFinite);
            Assert.Equal(123, dur.TotalMinutes);

            dur = Yogi.Duration.FromMinutes(double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.Inf, dur);

            dur = Yogi.Duration.FromMinutes(0.5);
            Assert.Equal(30, dur.TotalSeconds);
        }

        [Fact]
        public void FromHours()
        {
            var dur = Yogi.Duration.FromHours(123);
            Assert.True(dur.IsFinite);
            Assert.Equal(123, dur.TotalHours);

            dur = Yogi.Duration.FromHours(double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.Inf, dur);

            dur = Yogi.Duration.FromHours(0.5);
            Assert.Equal(30, dur.TotalMinutes);
        }

        [Fact]
        public void FromDays()
        {
            var dur = Yogi.Duration.FromDays(123);
            Assert.True(dur.IsFinite);
            Assert.Equal(123, dur.TotalDays);

            dur = Yogi.Duration.FromDays(double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.Inf, dur);

            dur = Yogi.Duration.FromDays(0.5);
            Assert.Equal(12, dur.TotalHours);
        }

        [Fact]
        public void DefaultConstructor()
        {
            var dur = new Yogi.Duration();
            Assert.True(dur.IsFinite);
            Assert.Equal(0, dur.NanosecondsCount);
        }

        [Fact]
        public void TimeSpanConstructor()
        {
            var dur = new Yogi.Duration(TimeSpan.FromSeconds(12));
            Assert.True(dur.IsFinite);
            Assert.Equal(12, dur.TotalSeconds);

            dur = new Yogi.Duration(TimeSpan.FromMilliseconds(33));
            Assert.True(dur.IsFinite);
            Assert.Equal(33, dur.TotalMilliseconds);
        }

        [Fact]
        public void Nanoseconds()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(887, dur.Nanoseconds);
        }

        [Fact]
        public void Microseconds()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(465, dur.Microseconds);
        }

        [Fact]
        public void Milliseconds()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(132, dur.Milliseconds);
        }

        [Fact]
        public void Seconds()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(9, dur.Seconds);
        }

        [Fact]
        public void Minutes()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(33, dur.Minutes);
        }

        [Fact]
        public void Hours()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(21, dur.Hours);
        }

        [Fact]
        public void Days()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(1428, dur.Days);
        }

        [Fact]
        public void NanosecondsCount()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(123456789132465887L, dur.NanosecondsCount);
        }

        [Fact]
        public void TotalNanoseconds()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(123456789132465887.0, dur.TotalNanoseconds, 12);

            Assert.Equal(Yogi.Duration.Inf.TotalNanoseconds,
                double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.NegInf.TotalNanoseconds,
                double.NegativeInfinity);
        }

        [Fact]
        public void TotalMicroseconds()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(123456789132465.887, dur.TotalMicroseconds, 12);

            Assert.Equal(Yogi.Duration.Inf.TotalMicroseconds,
                double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.NegInf.TotalMicroseconds,
                double.NegativeInfinity);
        }

        [Fact]
        public void TotalMilliseconds()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(123456789132.465887, dur.TotalMilliseconds, 12);

            Assert.Equal(Yogi.Duration.Inf.TotalMilliseconds,
                double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.NegInf.TotalMilliseconds,
                double.NegativeInfinity);
        }

        [Fact]
        public void TotalSeconds()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(123456789.132465887, dur.TotalSeconds, 12);

            Assert.Equal(Yogi.Duration.Inf.TotalSeconds,
                double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.NegInf.TotalSeconds,
                double.NegativeInfinity);
        }

        [Fact]
        public void TotalMinutes()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(2057613.1522077648, dur.TotalMinutes, 12);

            Assert.Equal(Yogi.Duration.Inf.TotalMinutes,
                double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.NegInf.TotalMinutes,
                double.NegativeInfinity);
        }

        [Fact]
        public void TotalHours()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(34293.55253679608, dur.TotalHours, 12);

            Assert.Equal(Yogi.Duration.Inf.TotalHours,
                double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.NegInf.TotalHours,
                double.NegativeInfinity);
        }

        [Fact]
        public void TotalDays()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465887L);
            Assert.Equal(1428.8980223665033, dur.TotalDays, 12);

            Assert.Equal(Yogi.Duration.Inf.TotalDays,
                double.PositiveInfinity);
            Assert.Equal(Yogi.Duration.NegInf.TotalDays,
                double.NegativeInfinity);
        }

        [Fact]
        public void Negated()
        {
            var dur = Yogi.Duration.FromNanoseconds(123);
            var negDur = dur.Negated;
            Assert.Equal(dur.NanosecondsCount, -negDur.NanosecondsCount);
            Assert.True(negDur.IsFinite);
        }

        [Fact]
        public void ToTimeSpan()
        {
            var dur = Yogi.Duration.FromNanoseconds(123456789132465800L);
            Assert.Equal(TimeSpan.FromTicks(1234567891324658L), dur.ToTimeSpan());

            Assert.Equal(TimeSpan.MaxValue, Yogi.Duration.Inf.ToTimeSpan());
            Assert.Equal(TimeSpan.MinValue, Yogi.Duration.NegInf.ToTimeSpan());
        }

        [Fact]
        public void IsFinite()
        {
            Assert.True(Yogi.Duration.Zero.IsFinite);
            Assert.True(Yogi.Duration.FromNanoseconds(123).IsFinite);
            Assert.False(Yogi.Duration.Inf.IsFinite);
            Assert.False(Yogi.Duration.NegInf.IsFinite);
        }

        [Fact]
        public void Format()
        {
            MOCK_FormatDuration((long dur, int neg, ref IntPtr str, ref int strsize, string durfmt, string inffmt) =>
            {
                Assert.Equal(123456789123456789L, dur);
                Assert.Equal(0, neg);
                Assert.Equal("bla", durfmt);
                Assert.Equal("moo", inffmt);
                str = helloBytes;
                strsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            var s = Yogi.Duration.FromNanoseconds(123456789123456789L).Format("bla", "moo");
            Assert.Equal("hello", s);
        }

        [Fact]
        public void FormatError()
        {
            MOCK_FormatDuration((long dur, int neg, ref IntPtr str, ref int strsize, string durfmt, string inffmt) =>
            {
                return (int)Yogi.ErrorCode.WrongObjectType;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                Yogi.Duration.FromNanoseconds(1L).Format("bla", "moo");
            });
        }

        [Fact]
        public void FormatInf()
        {
            MOCK_FormatDuration((long dur, int neg, ref IntPtr str, ref int strsize, string durfmt, string inffmt) =>
            {
                Assert.Equal(-1, dur);
                Assert.Equal(1, neg);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.Duration.NegInf.Format();
        }

        [Fact]
        public void ToStringMethod()
        {
            // YOGI_FormatDuration
            MOCK_FormatDuration((long dur, int neg, ref IntPtr str, ref int strsize, string durfmt, string inffmt) =>
            {
                Assert.Equal(123456789123456789L, dur);
                Assert.Equal(0, neg);
                Assert.Null(durfmt);
                Assert.Null(inffmt);
                str = helloBytes;
                strsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            var s = Yogi.Duration.FromNanoseconds(123456789123456789L).ToString();
            Assert.Equal("hello", s);
        }

        [Fact]
        public void ToStringError()
        {
            MOCK_FormatDuration((long dur, int neg, ref IntPtr str, ref int strsize, string durfmt, string inffmt) =>
            {
                return (int)Yogi.ErrorCode.WrongObjectType;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                Yogi.Duration.FromNanoseconds(1L).ToString();
            });
        }

        [Fact]
        public void PlusOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(10);
            var dur2 = Yogi.Duration.FromMicroseconds(3);
            Assert.Equal(3010, (dur1 + dur2).NanosecondsCount);

            Assert.Equal(Yogi.Duration.Inf, dur1 + Yogi.Duration.Inf);
            Assert.Equal(Yogi.Duration.Inf, Yogi.Duration.Inf + Yogi.Duration.Inf);
            Assert.Equal(Yogi.Duration.NegInf, dur1 + Yogi.Duration.NegInf);
            Assert.Equal(Yogi.Duration.NegInf,
                Yogi.Duration.NegInf + Yogi.Duration.NegInf);
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = Yogi.Duration.Inf + Yogi.Duration.NegInf;
            });
        }

        [Fact]
        public void MinusOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(10);
            var dur2 = Yogi.Duration.FromMicroseconds(3);
            Assert.Equal(-2990, (dur1 - dur2).NanosecondsCount);

            Assert.Equal(Yogi.Duration.NegInf, dur1 - Yogi.Duration.Inf);
            Assert.Equal(Yogi.Duration.Inf,
                Yogi.Duration.Inf - Yogi.Duration.NegInf);
            Assert.Equal(Yogi.Duration.Inf, dur1 - Yogi.Duration.NegInf);
            Assert.Equal(Yogi.Duration.NegInf,
                Yogi.Duration.NegInf - Yogi.Duration.Inf);
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = Yogi.Duration.NegInf - Yogi.Duration.NegInf;
            });
        }

        [Fact]
        public void MultiplicationOperator()
        {
            var dur = Yogi.Duration.FromNanoseconds(8);

            Assert.Equal(24, (dur * 3).NanosecondsCount);
            Assert.Equal(28, (dur * 3.5).NanosecondsCount);

            Assert.Equal(Yogi.Duration.Inf, Yogi.Duration.Inf * 5);
            Assert.Equal(Yogi.Duration.NegInf, Yogi.Duration.Inf * -5);
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = Yogi.Duration.Inf * 0;
            });
        }

        [Fact]
        public void DivisionOperator()
        {
            var dur = Yogi.Duration.FromNanoseconds(28);

            Assert.Equal(14, (dur / 2).NanosecondsCount);
            Assert.Equal(8, (dur / 3.5).NanosecondsCount);

            Assert.Equal(Yogi.Duration.Inf, Yogi.Duration.Inf / 5);
            Assert.Equal(Yogi.Duration.NegInf, Yogi.Duration.Inf / -5);
            Assert.Throws<DivideByZeroException>(() =>
            {
                var _ = dur / 0;
            });
        }

        [Fact]
        public void PlusEqualsOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(10);
            var dur2 = Yogi.Duration.FromMicroseconds(3);
            var dur3 = dur1 += dur2;
            Assert.Equal(dur3, dur1);
            Assert.Equal(3010, dur1.NanosecondsCount);
        }

        [Fact]
        public void MinusEqualsOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(10);
            var dur2 = Yogi.Duration.FromMicroseconds(3);
            var dur3 = dur1 -= dur2;
            Assert.Equal(dur3, dur1);
            Assert.Equal(-2990, dur1.NanosecondsCount);
        }

        [Fact]
        public void MultiplyEqualsOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(3);
            var dur2 = dur1 *= 3;
            Assert.Equal(dur2, dur1);
            Assert.Equal(9, dur1.NanosecondsCount);
        }

        [Fact]
        public void DivideEqualsOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(12);
            var dur2 = dur1 /= 3;
            Assert.Equal(dur2, dur1);
            Assert.Equal(4, dur1.NanosecondsCount);
        }

        [Fact]
        public void EqualityOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(1000);
            var dur2 = Yogi.Duration.FromMicroseconds(1);
            var dur3 = Yogi.Duration.FromNanoseconds(2);
            Assert.True(dur1 == dur2);
            Assert.False(dur1 == dur3);
        }

        [Fact]
        public void NotEqualsOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(1000);
            var dur2 = Yogi.Duration.FromMicroseconds(1);
            var dur3 = Yogi.Duration.FromNanoseconds(2);
            Assert.False(dur1 != dur2);
            Assert.True(dur1 != dur3);
        }

        [Fact]
        public void LessThanOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(1);
            var dur2 = Yogi.Duration.FromNanoseconds(1);
            var dur3 = Yogi.Duration.FromNanoseconds(2);
            Assert.False(dur1 < dur2);
            Assert.True(dur2 < dur3);
            Assert.False(dur3 < dur1);

            Assert.False(Yogi.Duration.Inf < Yogi.Duration.Inf);
            Assert.True(Yogi.Duration.NegInf < Yogi.Duration.Inf);
            Assert.False(Yogi.Duration.Inf < Yogi.Duration.Zero);
            Assert.True(Yogi.Duration.NegInf < Yogi.Duration.Zero);
        }

        [Fact]
        public void GreaterThanOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(1);
            var dur2 = Yogi.Duration.FromNanoseconds(1);
            var dur3 = Yogi.Duration.FromNanoseconds(2);
            Assert.False(dur1 > dur2);
            Assert.False(dur2 > dur3);
            Assert.True(dur3 > dur1);

            Assert.False(Yogi.Duration.Inf > Yogi.Duration.Inf);
            Assert.False(Yogi.Duration.NegInf > Yogi.Duration.Inf);
            Assert.True(Yogi.Duration.Inf > Yogi.Duration.Zero);
            Assert.False(Yogi.Duration.NegInf > Yogi.Duration.Zero);
        }

        [Fact]
        public void LessOrEqualOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(1);
            var dur2 = Yogi.Duration.FromNanoseconds(1);
            var dur3 = Yogi.Duration.FromNanoseconds(2);
            Assert.True(dur1 <= dur2);
            Assert.True(dur2 <= dur3);
            Assert.False(dur3 <= dur1);

            Assert.True(Yogi.Duration.Inf <= Yogi.Duration.Inf);
            Assert.True(Yogi.Duration.NegInf <= Yogi.Duration.Inf);
            Assert.False(Yogi.Duration.Inf <= Yogi.Duration.Zero);
            Assert.True(Yogi.Duration.NegInf <= Yogi.Duration.Zero);
        }

        [Fact]
        public void GreaterOrEqualOperator()
        {
            var dur1 = Yogi.Duration.FromNanoseconds(1);
            var dur2 = Yogi.Duration.FromNanoseconds(1);
            var dur3 = Yogi.Duration.FromNanoseconds(2);
            Assert.True(dur1 >= dur2);
            Assert.False(dur2 >= dur3);
            Assert.True(dur3 >= dur1);

            Assert.True(Yogi.Duration.Inf >= Yogi.Duration.Inf);
            Assert.False(Yogi.Duration.NegInf >= Yogi.Duration.Inf);
            Assert.True(Yogi.Duration.Inf >= Yogi.Duration.Zero);
            Assert.False(Yogi.Duration.NegInf >= Yogi.Duration.Zero);
        }

        [Fact]
        public void EqualsMethod()
        {
            Assert.True(Yogi.Duration.FromDays(1).Equals(Yogi.Duration.FromDays(1)));
            Assert.False(Yogi.Duration.FromDays(1).Equals(Yogi.Duration.FromDays(2)));
            Assert.False(Yogi.Duration.FromDays(1).Equals(""));
        }

        [Fact]
        public void GetHashCodeMethod()
        {
            Assert.NotEqual(Yogi.Duration.FromDays(1).GetHashCode(),
                Yogi.Duration.FromDays(2).GetHashCode());
        }

        [Fact]
        public void CompareTo()
        {
            Assert.Equal(-1, Yogi.Duration.FromDays(1).CompareTo(""));
            Assert.Equal(-1, Yogi.Duration.FromDays(1).CompareTo(Yogi.Duration.FromDays(2)));
            Assert.Equal(0, Yogi.Duration.FromDays(1).CompareTo(Yogi.Duration.FromDays(1)));
            Assert.Equal(+1, Yogi.Duration.FromDays(2).CompareTo(Yogi.Duration.FromDays(1)));
        }
    }
}
