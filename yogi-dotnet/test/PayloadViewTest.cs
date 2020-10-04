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
    public class PayloadViewTest : TestCase
    {
        [Fact]
        public void FromBuffer()
        {
            var data = new byte[] { 1, 2, 3, 4, 0 };

            var view = new Yogi.PayloadView(data, 3, Yogi.Encoding.Msgpack);
            Assert.Equal(view.Data, data);
            Assert.Equal(3, view.Size);
            Assert.Equal(Yogi.Encoding.Msgpack, view.Encoding);

            view = new Yogi.PayloadView(data, Yogi.Encoding.Json);
            Assert.Equal(view.Data, data);
            Assert.Equal(view.Size, data.Length);
            Assert.Equal(Yogi.Encoding.Json, view.Encoding);
        }

        [Fact]
        public void FromJsonView()
        {
            var json_view = new Yogi.JsonView("Hello");

            var view = new Yogi.PayloadView(json_view);
            Assert.Equal(view.Data, json_view.Data);
            Assert.Equal(view.Size, json_view.Size);
            Assert.Equal(Yogi.Encoding.Json, view.Encoding);
        }

        [Fact]
        public void FromMsgpackView()
        {
            var msgpack_view = new Yogi.MsgpackView(new byte[] { 1, 2, 3 });

            var view = new Yogi.PayloadView(msgpack_view);
            Assert.Equal(view.Data, msgpack_view.Data);
            Assert.Equal(view.Size, msgpack_view.Size);
            Assert.Equal(Yogi.Encoding.Msgpack, view.Encoding);
        }

        [Fact]
        public void Comparison()
        {
            var view1 = new Yogi.PayloadView(new Yogi.JsonView("Hello"));
            var view2 = new Yogi.PayloadView(new Yogi.JsonView("Hello"));
            var view3 = new Yogi.PayloadView(new Yogi.JsonView("World"));
            var view4 = new Yogi.PayloadView(view1.Data, Yogi.Encoding.Msgpack);

            Assert.True(view1 == view2);
            Assert.False(view1 == view3);
            Assert.False(view1 == view4);

            Assert.False(view1 != view2);
            Assert.True(view1 != view3);
            Assert.True(view1 != view4);

            Assert.False(view1.Equals(new Exception()));
            Assert.False(view1.Equals(null));
            Assert.True(view1.Equals(view2));
            Assert.False(view1.Equals(view3));
            Assert.False(view1.Equals(view4));

            var view5 = new Yogi.PayloadView(new byte[] { 1, 2, 0 }, Yogi.Encoding.Json);
            var view6 = new Yogi.PayloadView(new byte[] { 1, 2, 0, 9 }, 3, Yogi.Encoding.Json);
            Assert.True(view5.Equals(view6));
        }

        [Fact]
        public void GetHashCodeMethod()
        {
            var view1 = new Yogi.PayloadView(new Yogi.JsonView("Hello"));
            var view2 = new Yogi.PayloadView(new Yogi.JsonView("World"));

            Assert.NotEqual(view1.GetHashCode(), view2.GetHashCode());
        }
    }
}
