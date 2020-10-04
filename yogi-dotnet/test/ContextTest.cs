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
    public class ContextTest : TestCase
    {
        Yogi.Context context = CreateContext();

        [Fact]
        public void Create()
        {
            MOCK_ContextCreate((ref IntPtr context) =>
            {
                context = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            new Yogi.Context();
        }

        [Fact]
        public void CreateError()
        {
            MOCK_ContextCreate((ref IntPtr context) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                new Yogi.Context();
            });
        }

        [Fact]
        public void Poll()
        {
            MOCK_ContextPoll((IntPtr context, ref int count) =>
            {
                Assert.Equal(pointer, context);
                count = 5;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(5, context.Poll());
        }

        [Fact]
        public void PollError()
        {
            MOCK_ContextPoll((IntPtr context, ref int count) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                context.Poll();
            });
        }

        [Fact]
        public void PollOne()
        {
            MOCK_ContextPollOne((IntPtr context, ref int count) =>
            {
                Assert.Equal(pointer, context);
                count = 1;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(1, context.PollOne());
        }

        [Fact]
        public void PollOneError()
        {
            MOCK_ContextPollOne((IntPtr context, ref int count) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                context.PollOne();
            });
        }

        [Fact]
        public void Run()
        {
            MOCK_ContextRun((IntPtr context, ref int count, long duration) =>
            {
                Assert.Equal(pointer, context);
                Assert.Equal(1000000L, duration);
                count = 5;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(5, context.Run(Yogi.Duration.FromMilliseconds(1)));
        }

        [Fact]
        public void RunError()
        {
            MOCK_ContextRun((IntPtr context, ref int count, long duration) =>
            {
                Assert.Equal(-1L, duration);
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                context.Run();
            });
        }

        [Fact]
        public void RunOne()
        {
            MOCK_ContextRunOne((IntPtr context, ref int count, long duration) =>
            {
                Assert.Equal(pointer, context);
                Assert.Equal(1000000L, duration);
                count = 1;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(1, context.RunOne(Yogi.Duration.FromMilliseconds(1)));
        }

        [Fact]
        public void RunOneError()
        {
            MOCK_ContextRunOne((IntPtr context, ref int count, long duration) =>
            {
                Assert.Equal(-1L, duration);
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                context.RunOne();
            });
        }

        [Fact]
        public void RunInBackground()
        {
            MOCK_ContextRunInBackground((IntPtr context) =>
            {
                Assert.Equal(pointer, context);
                return (int)Yogi.ErrorCode.Ok;
            });

            context.RunInBackground();
        }

        [Fact]
        public void RunInBackgroundError()
        {
            MOCK_ContextRunInBackground((IntPtr context) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                context.RunInBackground();
            });
        }

        [Fact]
        public void Stop()
        {
            MOCK_ContextStop((IntPtr context) =>
            {
                Assert.Equal(pointer, context);
                return (int)Yogi.ErrorCode.Ok;
            });

            context.Stop();
        }

        [Fact]
        public void StopError()
        {
            MOCK_ContextStop((IntPtr context) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                context.Stop();
            });
        }

        [Fact]
        public void WaitForRunning()
        {
            MOCK_ContextWaitForRunning((IntPtr context, long duration) =>
            {
                Assert.Equal(pointer, context);
                Assert.Equal(1000000L, duration);
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.True(context.WaitForRunning(Yogi.Duration.FromMilliseconds(1)));
        }

        [Fact]
        public void WaitForRunningTimeout()
        {
            MOCK_ContextWaitForRunning((IntPtr context, long duration) =>
            {
                return (int)Yogi.ErrorCode.Timeout;
            });

            Assert.False(context.WaitForRunning(Yogi.Duration.FromMilliseconds(1)));
        }

        [Fact]
        public void WaitForRunningError()
        {
            MOCK_ContextWaitForRunning((IntPtr context, long duration) =>
            {
                Assert.Equal(-1L, duration);
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                context.WaitForRunning();
            });
        }

        [Fact]
        public void WaitForStopped()
        {
            MOCK_ContextWaitForStopped((IntPtr context, long duration) =>
            {
                Assert.Equal(pointer, context);
                Assert.Equal(1000000L, duration);
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.True(context.WaitForStopped(Yogi.Duration.FromMilliseconds(1)));
        }

        [Fact]
        public void WaitForStoppedTimeout()
        {
            MOCK_ContextWaitForStopped((IntPtr context, long duration) =>
            {
                return (int)Yogi.ErrorCode.Timeout;
            });

            Assert.False(context.WaitForStopped(Yogi.Duration.FromMilliseconds(1)));
        }

        [Fact]
        public void WaitForStoppedError()
        {
            MOCK_ContextWaitForStopped((IntPtr context, long duration) =>
            {
                Assert.Equal(-1L, duration);
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                context.WaitForStopped();
            });
        }

        [Fact]
        public void Post()
        {
            MOCK_ContextPost((IntPtr context, ContextPostFnDelegate fn, IntPtr userarg) =>
            {
                Assert.Equal(pointer, context);
                fn(userarg);
                return (int)Yogi.ErrorCode.Ok;
            });

            bool called = false;
            Action handlerFn = () => { called = true; };
            context.Post(handlerFn);
            Assert.True(called);
        }

        [Fact]
        public void PostError()
        {
            MOCK_ContextPost((IntPtr context, ContextPostFnDelegate fn, IntPtr userarg) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                context.Post(() => { });
            });
        }
    }
}
