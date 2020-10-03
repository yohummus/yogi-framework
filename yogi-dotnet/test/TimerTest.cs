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
    public class TimerTest : TestCase
    {
        Yogi.Timer timer = CreateTimer();

        public static Yogi.Timer CreateTimer()
        {
            MOCK_TimerCreate((ref IntPtr timer, IntPtr context) =>
            {
                timer = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            return new Yogi.Timer(CreateContext());
        }

        [Fact]
        public void Create()
        {
            MOCK_TimerCreate((ref IntPtr timer, IntPtr context) =>
            {
                Assert.Equal(context, pointer);
                timer = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            new Yogi.Timer(CreateContext());
        }

        [Fact]
        public void CreateError()
        {
            MOCK_TimerCreate((ref IntPtr timer, IntPtr context) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                new Yogi.Timer(CreateContext());
            });
        }

        [Fact]
        public void StartAsync()
        {
            bool called = false;

            Yogi.Timer.HandlerFnDelegate fn = (Yogi.Result result) =>
            {
                Assert.IsType<Yogi.Success>(result);
                Assert.Equal(Yogi.ErrorCode.Ok, result.ErrorCode);
                called = true;
            };

            MOCK_TimerStartAsync((IntPtr timer, long duration, TimerStartAsyncFnDelegate fn_, IntPtr userarg) =>
            {
                Assert.Equal(pointer, timer);
                Assert.Equal(1234, duration);
                Assert.NotNull(fn_);
                fn_((int)Yogi.ErrorCode.Ok, userarg);
                return (int)Yogi.ErrorCode.Ok;
            });

            timer.StartAsync(Yogi.Duration.FromNanoseconds(1234), fn);
            Assert.True(called);
        }

        [Fact]
        public void StartAsyncCallbackCancel()
        {
            bool called = false;

            Yogi.Timer.HandlerFnDelegate fn = (Yogi.Result result) =>
            {
                Assert.IsType<Yogi.Failure>(result);
                Assert.Equal(Yogi.ErrorCode.Canceled, result.ErrorCode);
                called = true;
            };

            MOCK_TimerStartAsync((IntPtr timer, long duration, TimerStartAsyncFnDelegate fn_, IntPtr userarg) =>
            {
                Assert.NotNull(fn_);
                fn_((int)Yogi.ErrorCode.Canceled, userarg);
                return (int)Yogi.ErrorCode.Ok;
            });

            timer.StartAsync(Yogi.Duration.FromNanoseconds(1234), fn);
            Assert.True(called);
        }

        [Fact]
        public void StartSyncError()
        {
            Yogi.Timer.HandlerFnDelegate fn = (Yogi.Result result) =>
            {
            };

            MOCK_TimerStartAsync((IntPtr timer, long duration, TimerStartAsyncFnDelegate fn_, IntPtr userarg) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                timer.StartAsync(Yogi.Duration.FromNanoseconds(1234), fn);
            });
        }

        [Fact]
        public void Cancel()
        {
            MOCK_TimerCancel((IntPtr timer) =>
            {
                Assert.Equal(timer, pointer);
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.True(timer.Cancel());

            MOCK_TimerCancel((IntPtr timer) =>
            {
                Assert.Equal(timer, pointer);
                return (int)Yogi.ErrorCode.TimerExpired;
            });

            Assert.False(timer.Cancel());
        }

        [Fact]
        public void CancelError()
        {
            MOCK_TimerCancel((IntPtr timer) =>
            {
                Assert.Equal(timer, pointer);
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                timer.Cancel();
            });
        }
    }
}
