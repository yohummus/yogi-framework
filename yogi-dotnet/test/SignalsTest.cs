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
using System.Runtime.InteropServices;
using Xunit;

namespace test
{
    public class SignalsTest : TestCase
    {
        Yogi.SignalSet sigset = CreateSignalSet();

        public static Yogi.SignalSet CreateSignalSet()
        {
            MOCK_SignalSetCreate((ref IntPtr sigset, IntPtr context, int signals) =>
            {
                sigset = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            return new Yogi.SignalSet(CreateContext(), Yogi.Signals.Term);
        }

        [Fact]
        public void CreateSet()
        {
            MOCK_SignalSetCreate((ref IntPtr sigset, IntPtr context, int signals) =>
            {
                Assert.Equal(pointer, context);
                Assert.Equal((int)Yogi.Signals.Term, signals);
                sigset = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            new Yogi.SignalSet(CreateContext(), Yogi.Signals.Term);
        }

        [Fact]
        public void CreateSetError()
        {
            MOCK_SignalSetCreate((ref IntPtr sigset, IntPtr context, int signals) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                new Yogi.SignalSet(CreateContext(), Yogi.Signals.Term);
            });
        }

        [Fact]
        public void RaiseSignal()
        {
            bool called = false;

            Yogi.RaiseSignalFnDelegate fn = () =>
            {
                called = true;
            };

            MOCK_RaiseSignal((int signal, IntPtr sigarg, RaiseSignalFnDelegate fn_, IntPtr userarg) =>
            {
                Assert.Equal((int)Yogi.Signals.Term, signal);
                Assert.NotNull(fn_);
                fn_(sigarg, userarg);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.RaiseSignal(Yogi.Signals.Term, fn);
            Assert.True(called);

            called = false;

            Yogi.RaiseSignalFnDelegate<string> fn2 = (string sigarg) =>
            {
                Assert.Null(sigarg);
                called = true;
            };

            Yogi.RaiseSignal(Yogi.Signals.Term, fn);
            Assert.True(called);
        }

        [Fact]
        public void RaiseSignalNoHandler()
        {
            bool called = false;

            MOCK_RaiseSignal((int signal, IntPtr sigarg, RaiseSignalFnDelegate fn_, IntPtr userarg) =>
            {
                called = true;
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.RaiseSignal(Yogi.Signals.Term);
            Assert.True(called);

            called = false;
            Yogi.RaiseSignal(Yogi.Signals.Term, "foo");
            Assert.True(called);
        }

        [Fact]
        public void RaiseSignalWithSigarg()
        {
            bool called = false;

            Yogi.RaiseSignalFnDelegate<string> fn = (string sigarg) =>
            {
                Assert.Equal("foo", sigarg);
                called = true;
            };

            MOCK_RaiseSignal((int signal, IntPtr sigarg, RaiseSignalFnDelegate fn_, IntPtr userarg) =>
            {
                Assert.Equal((int)Yogi.Signals.Term, signal);
                Assert.NotNull(fn_);
                fn_(sigarg, userarg);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.RaiseSignal(Yogi.Signals.Term, "foo", fn);
            Assert.True(called);
        }

        [Fact]
        public void RaiseSignalError()
        {
            MOCK_RaiseSignal((int signal, IntPtr sigarg, RaiseSignalFnDelegate fn_, IntPtr userarg) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                Yogi.RaiseSignal(Yogi.Signals.Term);
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                Yogi.RaiseSignal(Yogi.Signals.Term, "foo");
            });
        }

        [Fact]
        public void AwaitSignal()
        {
            bool called = false;

            Yogi.SignalSet.AwaitSignalFnDelegate fn = (Yogi.Result res, Yogi.Signals signal) =>
            {
                Assert.IsType<Yogi.Failure>(res);
                Assert.Equal(Yogi.ErrorCode.WrongObjectType, res.ErrorCode);
                Assert.Equal(Yogi.Signals.Usr6, signal);
                called = true;
            };

            MOCK_SignalSetAwaitSignalAsync((IntPtr sigset, SignalSetAwaitSignalAsyncFnDelegate fn_, IntPtr userarg) =>
            {
                Assert.Equal(pointer, sigset);
                Assert.NotNull(fn_);
                fn_((int)Yogi.ErrorCode.WrongObjectType, (int)Yogi.Signals.Usr6, IntPtr.Zero, userarg);
                return (int)Yogi.ErrorCode.Ok;
            });

            sigset.AwaitSignalAsync(fn);
            Assert.True(called);
        }

        [Fact]
        public void AwaitSignalWithSigarg()
        {
            bool called = false;

            Yogi.SignalSet.AwaitSignalFnDelegate<string> fn = (Yogi.Result res, Yogi.Signals signal, string sigarg) =>
            {
                Assert.IsType<Yogi.Failure>(res);
                Assert.Equal(Yogi.ErrorCode.WrongObjectType, res.ErrorCode);
                Assert.Equal(Yogi.Signals.Usr6, signal);
                Assert.Equal("foo", sigarg);
                called = true;
            };

            MOCK_SignalSetAwaitSignalAsync((IntPtr sigset, SignalSetAwaitSignalAsyncFnDelegate fn_, IntPtr userarg) =>
            {
                Assert.Equal(pointer, sigset);
                Assert.NotNull(fn_);

                var sigargHandle = GCHandle.Alloc("foo");
                var sigargPtr = GCHandle.ToIntPtr(sigargHandle);

                fn_((int)Yogi.ErrorCode.WrongObjectType, (int)Yogi.Signals.Usr6, sigargPtr, userarg);
                return (int)Yogi.ErrorCode.Ok;
            });

            sigset.AwaitSignalAsync(fn);
            Assert.True(called);
        }

        [Fact]
        public void AwaitSignalError()
        {
            MOCK_SignalSetAwaitSignalAsync((IntPtr sigset, SignalSetAwaitSignalAsyncFnDelegate fn_, IntPtr userarg) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                sigset.AwaitSignalAsync((result, signal) => { });
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                sigset.AwaitSignalAsync<string>((result, signal, sigarg) => { });
            });
        }

        [Fact]
        public void CancelAwaitSignal()
        {
            MOCK_SignalSetCancelAwaitSignal((IntPtr sigset) =>
            {
                Assert.Equal(pointer, sigset);
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.True(sigset.CancelAwaitSignal());

            MOCK_SignalSetCancelAwaitSignal((IntPtr sigset) =>
            {
                Assert.Equal(pointer, sigset);
                return (int)Yogi.ErrorCode.OperationNotRunning;
            });

            Assert.False(sigset.CancelAwaitSignal());
        }

        [Fact]
        public void CancelAwaitSignalError()
        {
            MOCK_SignalSetCancelAwaitSignal((IntPtr sigset) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                sigset.CancelAwaitSignal();
            });
        }
    }
}
