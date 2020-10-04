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

public static partial class Yogi
{
    /// <summary>
    /// Delegate for the raise signal cleanup handler function.
    /// </summary>
    public delegate void RaiseSignalFnDelegate();

    /// <summary>
    /// Raises a signal.
    ///
    /// Signals in Yogi are intended to be used similarly to POSIX signals. They
    /// have to be raised explicitly by the user (e.g. when receiving an actual
    /// POSIX signal like SIGINT) via this function. A signal will be received by
    /// all signal sets containing that signal.
    ///
    /// The cleanup handler fn will be called once all signal handlers have been
    /// called.
    ///
    /// Note: The cleanup handler fn can get called either from within the
    ///       RaiseSignal() function or from any context within the program.
    /// </summary>
    /// <param name="signal">The signal to raise.</param>
    /// <param name="fn">Function to call after all signal sets have been notified.</param>
    public static void RaiseSignal(Signals signal, [Optional] RaiseSignalFnDelegate fn)
    {
        RaiseSignal<object>(signal, null, (sigarg) =>
        {
            if (fn != null)
            {
                fn();
            }
        });
    }

    /// <summary>
    /// Delegate for the raise signal cleanup handler function.
    /// </summary>
    /// <param name="sigarg">Value of the sigarg parameter passed to RaiseSignal().</param>
    /// <typeparam name="T">Type of the sigarg parameter.</typeparam>
    public delegate void RaiseSignalFnDelegate<T>([Optional] T sigarg) where T : class;

    /// <summary>
    /// Raises a signal.
    ///
    /// Signals in Yogi are intended to be used similarly to POSIX signals. They
    /// have to be raised explicitly by the user (e.g. when receiving an actual
    /// POSIX signal like SIGINT) via this function. A signal will be received by
    /// all signal sets containing that signal.
    ///
    /// The sigarg parameter can be used to deliver user-defined data to the
    /// signal handlers.
    ///
    /// The cleanup handler fn will be called once all signal handlers have been
    /// called. Once fn has been called, the sigarg parameter is not used any more
    /// and can be cleaned up.
    ///
    /// Note: The cleanup handler fn can get called either from within the
    ///       RaiseSignal() function or from any context within the program.
    /// </summary>
    /// <typeparam name="T">Type of the sigarg parameter.</typeparam>
    /// <param name="signal">The signal to raise.</param>
    /// <param name="sigarg">Object to pass to the signal sets.</param>
    /// <param name="fn">Function call after all signal sets have been notified.</param>
    public static void RaiseSignal<T>(Signals signal, T sigarg,
        [Optional] RaiseSignalFnDelegate<T> fn) where T : class
    {
        YogiCore.RaiseSignalFnDelegate wrapper = (sigargPtr, userarg) =>
        {
            try
            {
                if (fn != null)
                {
                    T obj = null;
                    if (sigargPtr != IntPtr.Zero)
                    {
                        obj = (T)GCHandle.FromIntPtr(sigargPtr).Target;
                    }

                    fn(obj);
                }
            }
            finally
            {
                GCHandle.FromIntPtr(sigargPtr).Free();
                GCHandle.FromIntPtr(userarg).Free();
            }
        };
        var wrapperHandle = GCHandle.Alloc(wrapper);
        var sigargHandle = GCHandle.Alloc(sigarg);

        try
        {
            var wrapperPtr = GCHandle.ToIntPtr(wrapperHandle);
            var sigargPtr = GCHandle.ToIntPtr(sigargHandle);
            int res = YogiCore.YOGI_RaiseSignal((int)signal, sigargPtr, wrapper, wrapperPtr);
            CheckErrorCode(res);
        }
        catch
        {
            sigargHandle.Free();
            wrapperHandle.Free();
            throw;
        }
    }

    /// <summary>
    /// Manages a set of signals that the user can wait on
    ///
    /// Signal sets are used to receive signals raised via RaiseSignal(). The signals
    /// are queued until they can be delivered by means of calls to AwaitSignalAsync().
    /// </summary>
    public class SignalSet : Object
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="context">The context to use.</param>
        /// <param name="signals">Signals to listen for.</param>
        public SignalSet(Context context, Signals signals)
        : base(Create(context, signals), new Object[] { context })
        {
        }

        /// <summary>
        /// Delegate for the AwaitSignalAsync<T>() function.
        /// </summary>
        /// <typeparam name="T">Type of the sigarg parameter passed to RaiseSignal().</typeparam>
        /// <param name="res">Result of the wait operation.</param>
        /// <param name="signal">The raised signal.</param>
        /// <param name="sigarg">Value of the sigarg parameter passed to RaiseSignal().</param>
        public delegate void AwaitSignalFnDelegate<T>(Result res, Signals signal,
            [Optional] T sigarg) where T : class;

        /// <summary>
        /// Waits for a signal to be raised.
        ///
        /// The handler \p fn will be called after one of the signals in the set
        /// is caught.
        /// </summary>
        /// <typeparam name="T">Type of the sigarg parameter passed to RaiseSignal().</typeparam>
        /// <param name="fn">Handler function to call.</param>
        public void AwaitSignalAsync<T>(AwaitSignalFnDelegate<T> fn) where T : class
        {
            YogiCore.SignalSetAwaitSignalAsyncFnDelegate wrapper = (ec, signal, sigargPtr, userarg) =>
            {
                try
                {
                    T obj = null;
                    if (sigargPtr != IntPtr.Zero)
                    {
                        obj = (T)GCHandle.FromIntPtr(sigargPtr).Target;
                    }

                    fn(ErrorCodeToResult(ec), (Signals)signal, obj);
                }
                finally
                {
                    GCHandle.FromIntPtr(userarg).Free();
                }
            };
            var wrapperHandle = GCHandle.Alloc(wrapper);

            try
            {
                var wrapperPtr = GCHandle.ToIntPtr(wrapperHandle);
                int res = YogiCore.YOGI_SignalSetAwaitSignalAsync(Handle, wrapper, wrapperPtr);
                CheckErrorCode(res);
            }
            catch
            {
                wrapperHandle.Free();
                throw;
            }
        }

        /// <summary>
        /// Delegate for the AwaitSignalAsync() function.
        /// </summary>
        /// <param name="res">Result of the wait operation.</param>
        /// <param name="signal">The raised signal.</param>
        public delegate void AwaitSignalFnDelegate(Result res, Signals signal);

        /// <summary>
        /// Waits for a signal to be raised.
        ///
        /// The handler fn will be called after one of the signals in the set
        /// is caught.
        /// </summary>
        /// <param name="fn">Handler function to call.</param>
        public void AwaitSignalAsync(AwaitSignalFnDelegate fn)
        {
            AwaitSignalAsync<object>((res, signal, sigarg) => { fn(res, signal); });
        }

        /// <summary>
        /// Cancels waiting for a signal.
        ///
        /// This causes the handler function registered via AwaitSignalAsync() to be
        /// called with a cancellation error.
        /// </summary>
        /// <returns>True if the wait operation was cancelled successfully.</returns>
        public bool CancelAwaitSignal()
        {
            int res = YogiCore.YOGI_SignalSetCancelAwaitSignal(Handle);
            return FalseIfSpecificErrorElseThrow(res, ErrorCode.OperationNotRunning);
        }

        static IntPtr Create(Context context, Signals signals)
        {
            var handle = new IntPtr();
            int res = YogiCore.YOGI_SignalSetCreate(ref handle, context.Handle, (int)signals);
            CheckErrorCode(res);
            return handle;
        }
    }
}
