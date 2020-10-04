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
    public class EnumsTest : TestCase
    {
        [Fact]
        public void ErrorCode()
        {
            Assert.Equal(0, (int)Yogi.ErrorCode.Ok);
            Assert.Equal(-1, (int)Yogi.ErrorCode.Unknown);
        }

        [Fact]
        public void Verbosity()
        {
            Assert.Equal(1, (int)Yogi.Verbosity.Error);
            Assert.Equal(4, (int)Yogi.Verbosity.Debug);
        }

        [Fact]
        public void Stream()
        {
            Assert.Equal(0, (int)Yogi.Stream.Stdout);
            Assert.Equal(1, (int)Yogi.Stream.Stderr);
        }

        [Fact]
        public void Schema()
        {
            Assert.Equal(1, (int)Yogi.Schema.BranchConfig);
            Assert.Equal(4, (int)Yogi.Schema.BranchEvent);
        }

        [Fact]
        public void Encoding()
        {
            Assert.Equal(0, (int)Yogi.Encoding.Json);
            Assert.Equal(1, (int)Yogi.Encoding.Msgpack);
        }

        [Fact]
        public void HttpStatus()
        {
            Assert.Equal(400, (int)Yogi.HttpStatus.Http400);
            Assert.Equal(503, (int)Yogi.HttpStatus.Http503);
        }

        [Fact]
        public void WebProcessAction()
        {
            Assert.Equal(0, (int)Yogi.WebProcessAction.Start);
            Assert.Equal(2, (int)Yogi.WebProcessAction.Canceled);
        }

        [Fact]
        public void WebProcessUpdate()
        {
            Assert.Equal(0, (int)Yogi.WebProcessUpdate.Started);
            Assert.Equal(4, (int)Yogi.WebProcessUpdate.Canceled);
        }

        [Fact]
        public void HttpMethods()
        {
            Assert.Equal(1 << 0, (int)Yogi.HttpMethods.Get);
            Assert.Equal(1 << 4, (int)Yogi.HttpMethods.Delete);
        }

        [Fact]
        public void Signals()
        {
            Assert.Equal(1 << 1, (int)Yogi.Signals.Term);
            Assert.Equal(1 << 24, (int)Yogi.Signals.Usr1);
        }

        [Fact]
        public void ConfigurationFlags()
        {
            Assert.Equal(1 << 0, (int)Yogi.ConfigurationFlags.DisableVariables);
            Assert.Equal(1 << 1, (int)Yogi.ConfigurationFlags.MutableCmdLine);
        }

        [Fact]
        public void CommandLineOptions()
        {
            Assert.Equal(1 << 1, (int)Yogi.CommandLineOptions.BranchName);
            Assert.Equal(Yogi.CommandLineOptions.BranchPath, Yogi.CommandLineOptions.BranchAll & Yogi.CommandLineOptions.BranchPath);
        }

        [Fact]
        public void BranchEvents()
        {
            Assert.Equal(1 << 1, (int)Yogi.BranchEvents.BranchQueried);
            Assert.Equal(1 << 2, (int)Yogi.BranchEvents.ConnectFinished);
        }
    }
}
