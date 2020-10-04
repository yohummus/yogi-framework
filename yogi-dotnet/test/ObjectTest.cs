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
    public class ObjectTest : TestCase
    {
        Yogi.Context obj = CreateContext();

        [Fact]
        public void Format()
        {
            MOCK_FormatObject((IntPtr obj, ref IntPtr str, ref int strsize, string objfmt, string nullstr) =>
            {
                Assert.Equal(obj, pointer);
                Assert.Null(objfmt);
                Assert.Null(nullstr);
                str = helloBytes;
                strsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(helloString, obj.Format());
        }

        [Fact]
        public void FormatError()
        {
            MOCK_FormatObject((IntPtr obj, ref IntPtr str, ref int strsize, string objfmt, string nullstr) =>
            {
                Assert.Equal("foo", objfmt);
                Assert.Equal("bar", nullstr);
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                obj.Format("foo", "bar");
            });
        }

        [Fact]
        public void ToStringFunction()
        {
            MOCK_FormatObject((IntPtr obj, ref IntPtr str, ref int strsize, string objfmt, string nullstr) =>
            {
                Assert.Equal(obj, pointer);
                Assert.Null(objfmt);
                Assert.Null(nullstr);
                str = helloBytes;
                strsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(helloString, obj.ToString());
        }
        [Fact]
        public void Destroy()
        {
            bool called = false;
            MOCK_Destroy((IntPtr obj) =>
            {
                Assert.Equal(pointer, obj);
                called = true;
                return (int)Yogi.ErrorCode.Ok;
            });

            obj.Dispose();
            Assert.True(called);
        }
    }
}
