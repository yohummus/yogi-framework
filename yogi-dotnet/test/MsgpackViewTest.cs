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
    public class MsgpackViewTest : TestCase
    {
        [Fact]
        public void FromBuffer()
        {
            var data = new byte[] { 1, 2, 3 };

            var view = new Yogi.MsgpackView(data);
            Assert.Equal(view.Data, data);
            Assert.Equal(view.Size, data.Length);

            view = data;
            Assert.Equal(view.Data, data);
            Assert.Equal(view.Size, data.Length);
        }

        [Fact]
        public void Comparison()
        {
            var view1 = new Yogi.MsgpackView(new byte[] { 11 });
            var view2 = new Yogi.MsgpackView(new byte[] { 11 });
            var view3 = new Yogi.MsgpackView(new byte[] { 55 });

            Assert.True(view1 == view2);
            Assert.False(view1 == view3);

            Assert.False(view1 != view2);
            Assert.True(view1 != view3);

            Assert.False(view1.Equals(new Exception()));
            Assert.False(view1.Equals(null));
            Assert.True(view1.Equals(view2));
            Assert.False(view1.Equals(view3));
        }

        [Fact]
        public void GetHashCodeMethod()
        {
            var view1 = new Yogi.MsgpackView(new byte[] { 11 });
            var view2 = new Yogi.MsgpackView(new byte[] { 55 });

            Assert.NotEqual(view1.GetHashCode(), view2.GetHashCode());
        }
    }
}
