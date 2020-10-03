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
using Newtonsoft.Json.Linq;

namespace test
{
    public class JsonViewTest : TestCase
    {
        [Fact]
        public void FromString()
        {
            string s = "Hello";
            var bytes = System.Text.Encoding.UTF8.GetBytes(s + '\0');

            var view = new Yogi.JsonView(s);
            Assert.Equal(view.Data, bytes);
            Assert.Equal(view.Size, bytes.Length);

            view = s;
            Assert.Equal(view.Data, bytes);
            Assert.Equal(view.Size, bytes.Length);
        }

        [Fact]
        public void FromJObject()
        {
            var s = "{\"x\":4}";
            JObject json = JObject.Parse(s);
            var bytes = System.Text.Encoding.UTF8.GetBytes(s + '\0');

            var view = new Yogi.JsonView(json);
            Assert.Equal(view.Data, bytes);
            Assert.Equal(view.Size, bytes.Length);

            view = json;
            Assert.Equal(view.Data, bytes);
            Assert.Equal(view.Size, bytes.Length);
        }

        [Fact]
        public void FromJArray()
        {
            var s = "[1,2,3]";
            JArray json = JArray.Parse(s);
            var bytes = System.Text.Encoding.UTF8.GetBytes(s + '\0');

            var view = new Yogi.JsonView(json);
            Assert.Equal(view.Data, bytes);
            Assert.Equal(view.Size, bytes.Length);

            view = json;
            Assert.Equal(view.Data, bytes);
            Assert.Equal(view.Size, bytes.Length);
        }

        [Fact]
        public void Comparison()
        {
            var view1 = new Yogi.JsonView("a");
            var view2 = new Yogi.JsonView("a");
            var view3 = new Yogi.JsonView("b");

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
            var view1 = new Yogi.JsonView("a");
            var view2 = new Yogi.JsonView("b");

            Assert.NotEqual(view1.GetHashCode(), view2.GetHashCode());
        }
    }
}
