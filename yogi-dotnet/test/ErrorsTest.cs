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

using Xunit;

namespace test
{
    public class ErrorsTest : TestCase
    {
        [Fact]
        public void Result()
        {
            Assert.False(new Yogi.Result(-1));
            Assert.True(new Yogi.Result(0));
            Assert.True(new Yogi.Result(1));
            Assert.Equal(10, new Yogi.Result(10).Value);
            Assert.Equal(new Yogi.Result(3), new Yogi.Result(3));
            Assert.NotEqual(new Yogi.Result(2), new Yogi.Result(3));
            Assert.True(new Yogi.Result(-3).ToString().Length > 5);
            Assert.NotEqual(new Yogi.Result(1).GetHashCode(), new Yogi.Result(2).GetHashCode());
            Assert.IsType<Yogi.ErrorCode>(new Yogi.Result(-1).ErrorCode);
            Assert.Equal(new Yogi.Result(0).ErrorCode, new Yogi.Result(1).ErrorCode);

            Assert.True(new Yogi.Result(-2) < new Yogi.Result(-1));
            Assert.False(new Yogi.Result(-1) < new Yogi.Result(-2));
            Assert.False(new Yogi.Result(-2) > new Yogi.Result(-1));
            Assert.True(new Yogi.Result(-1) > new Yogi.Result(-2));
            Assert.True(new Yogi.Result(-2) <= new Yogi.Result(-1));
            Assert.True(new Yogi.Result(-1) <= new Yogi.Result(-1));
            Assert.False(new Yogi.Result(-2) <= new Yogi.Result(-3));
            Assert.False(new Yogi.Result(-2) >= new Yogi.Result(-1));
            Assert.True(new Yogi.Result(-1) >= new Yogi.Result(-1));
            Assert.True(new Yogi.Result(-2) >= new Yogi.Result(-3));
            Assert.True(new Yogi.Result(-1) == new Yogi.Result(-1));
            Assert.False(new Yogi.Result(-1) == new Yogi.Result(-2));
            Assert.True(new Yogi.Result(-1) != new Yogi.Result(-2));
            Assert.False(new Yogi.Result(-1) != new Yogi.Result(-1));

            Assert.True(new Yogi.Result(-1).Equals(new Yogi.Result(-1)));
            Assert.False(new Yogi.Result(-1).Equals(new Yogi.Result(-2)));
            Assert.False(new Yogi.Result(-1).Equals(""));

            Assert.NotEqual(new Yogi.Result(-1).GetHashCode(), new Yogi.Result(-2).GetHashCode());

            Assert.Equal(new Yogi.Result(-1).CompareTo(""), -1);
            Assert.Equal(-1, new Yogi.Result(-2).CompareTo(new Yogi.Result(-1)));
            Assert.Equal(0, new Yogi.Result(-1).CompareTo(new Yogi.Result(-1)));
            Assert.Equal(+1, new Yogi.Result(-1).CompareTo(new Yogi.Result(-2)));
        }

        [Fact]
        public void Failure()
        {
            Assert.False(new Yogi.Failure(Yogi.ErrorCode.Busy));
            Assert.Equal(new Yogi.Failure(Yogi.ErrorCode.Busy).ToString(), new Yogi.Result(Yogi.ErrorCode.Busy).ToString());
            Assert.Equal(new Yogi.Failure(Yogi.ErrorCode.Busy), new Yogi.Result(Yogi.ErrorCode.Busy));
            Assert.True(typeof(Yogi.Failure).IsSubclassOf(typeof(Yogi.Result)));
        }

        [Fact]
        public void DetailedFailure()
        {
            Assert.Equal("ab", new Yogi.DetailedFailure(Yogi.ErrorCode.Busy, "ab").Details);
            Assert.True(typeof(Yogi.DetailedFailure).IsSubclassOf(typeof(Yogi.Failure)));
        }

        [Fact]
        public void Exception()
        {
            Assert.True(typeof(Yogi.Exception).IsSubclassOf(typeof(System.Exception)));
        }

        [Fact]
        public void FailureException()
        {
            Assert.True(typeof(Yogi.FailureException).IsSubclassOf(typeof(Yogi.Exception)));
            Assert.Equal(new Yogi.FailureException(Yogi.ErrorCode.Busy).Message,
                new Yogi.Failure(Yogi.ErrorCode.Busy).ToString());
        }

        [Fact]
        public void DetailedFailureException()
        {
            Assert.True(typeof(Yogi.DetailedFailureException)
                .IsSubclassOf(typeof(Yogi.FailureException)));
            Assert.Equal(new Yogi.DetailedFailureException(Yogi.ErrorCode.Busy, "ab").Message,
                new Yogi.DetailedFailure(Yogi.ErrorCode.Busy, "ab").ToString());
        }

        [Fact]
        public void Success()
        {
            Assert.True(new Yogi.Success(0));
            Assert.True(new Yogi.Success(1));
            Assert.Equal(new Yogi.Success(1).ToString(), new Yogi.Result(1).ToString());
            Assert.Equal(new Yogi.Success(1), new Yogi.Result(1));
            Assert.True(typeof(Yogi.Success).IsSubclassOf(typeof(Yogi.Result)));
        }
    }
}
