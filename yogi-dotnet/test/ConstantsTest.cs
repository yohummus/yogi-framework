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
    public class ConstantsTest : TestCase
    {
        [Fact]
        public void Some()
        {
            Assert.True(Yogi.Constants.Version.Length > 4);
            Assert.True(Yogi.Constants.DefaultAdvPort == 1);
            Assert.NotEqual(Yogi.Verbosity.Fatal, Yogi.Constants.DefaultLoggerVerbosity);
            Assert.True(Yogi.Constants.DefaultAdvInterval.TotalMilliseconds > 1);
        }
    }
}
