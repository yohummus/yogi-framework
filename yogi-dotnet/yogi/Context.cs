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
    /// Scheduler for the execution of asynchronous operations.
    ///
    /// Once an asynchronous operation finishes, the corresponding handler function
    /// is added to the context's event queue and executed through once of the
    /// Poll... or Run... functions.
    /// </summary>
    public class Context : Object
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        public Context()
        : base(Create())
        {
        }

        /// <summary>
        /// Runs the context's event processing loop to execute ready handlers.
        ///
        /// This function runs handlers (internal and user-supplied such as functions
        /// registered through Post()) that are ready to run, without blocking, until
        /// the Stop() function has been called or there are no more ready handlers.
        ///
        /// This function must be called from outside any handler functions that are
        /// being executed through the context.
        /// </summary>
        /// <returns>Number of executed handlers.</returns>
        public int Poll()
        {
            int count = -1;
            int res = YogiCore.YOGI_ContextPoll(Handle, ref count);
            CheckErrorCode(res);
            return count;
        }

        /// <summary>
        /// Runs the context's event processing loop to execute at most one ready handler.
        ///
        /// This function runs at most one handler (internal and user-supplied such as
        /// functions registered through Post()) that are ready to run, without blocking.
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        /// <returns>Number of executed handlers.</returns>
        public int PollOne()
        {
            int count = -1;
            int res = YogiCore.YOGI_ContextPollOne(Handle, ref count);
            CheckErrorCode(res);
            return count;
        }

        /// <summary>
        /// Runs the context's event processing loop for the specified duration.
        ///
        /// This function blocks while running the context's event processing loop and
        /// calling dispatched handlers (internal and user-supplied such as functions
        /// registered through Post()) for the specified duration unless Stop() is called
        /// within that time.
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        /// <param name="duration">Duration (infinity by default).</param>
        /// <returns>Number of executed handlers.</returns>
        public int Run([Optional] Duration duration)
        {
            long dur = duration == null ? -1 : DurationToApiDuration(duration);

            int count = -1;
            int res = YogiCore.YOGI_ContextRun(Handle, ref count, dur);
            CheckErrorCode(res);
            return count;
        }

        /// <summary>
        /// Runs the context's event processing loop for the specified duration.
        ///
        /// This function blocks while running the context's event processing loop and
        /// calling dispatched handlers (internal and user-supplied such as functions
        /// registered through Post()) for the specified duration unless Stop() is called
        /// within that time.
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        /// <param name="duration">Duration.</param>
        /// <returns>Number of executed handlers.</returns>
        public int Run(TimeSpan duration)
        {
            return Run(new Duration(duration));
        }

        /// <summary>
        /// Runs the context's event processing loop for the specified duration to execute
        /// at most one handler.
        ///
        /// This function blocks while running the context's event processing loop and
        /// calling dispatched handlers (internal and user-supplied such as functions
        /// registered through Post()) for the specified duration until a single handler
        /// function has been executed, unless Stop() is called within that time.
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        /// <param name="duration">Duration (infinity by default).</param>
        /// <returns>Number of executed handlers.</returns>
        public int RunOne([Optional] Duration duration)
        {
            long dur = duration == null ? -1 : DurationToApiDuration(duration);

            int count = -1;
            int res = YogiCore.YOGI_ContextRunOne(Handle, ref count, dur);
            CheckErrorCode(res);
            return count;
        }

        /// <summary>
        /// Runs the context's event processing loop for the specified duration to execute
        /// at most one handler.
        ///
        /// This function blocks while running the context's event processing loop and
        /// calling dispatched handlers (internal and user-supplied such as functions
        /// registered through Post()) for the specified duration until a single handler
        /// function has been executed, unless Stop() is called within that time.
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        /// <param name="duration">Duration.</param>
        /// <returns>Number of executed handlers.</returns>
        public int RunOne(TimeSpan duration)
        {
            return RunOne(new Duration(duration));
        }

        /// <summary>
        /// Starts an internal thread for running the context's event processing loop.
        ///
        /// This function starts a threads that runs the context's event processing loop
        /// in the background. It relieves the user from having to start a thread and
        /// calling the appropriate Run... or Poll... functions themself. The thread can
        /// be stopped using Stop().
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        public void RunInBackground()
        {
            int res = YogiCore.YOGI_ContextRunInBackground(Handle);
            CheckErrorCode(res);
        }

        /// <summary>
        /// Stops the context's event processing loop.
        ///
        /// This function signals the context to stop running its event processing loop.
        /// This causes Run... functions to return as soon as possible and it terminates
        /// the thread started via RunInBackground().
        /// </summary>
        public void Stop()
        {
            int res = YogiCore.YOGI_ContextStop(Handle);
            CheckErrorCode(res);
        }

        /// <summary>
        /// Blocks until the context's event processing loop is being run or until the
        /// specified timeout is reached.
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        /// <param name="duration">Maximum time to wait (infinity by default).</param>
        /// <returns>True if the context's event processing loop is running within the
        /// specified duration and false otherwise.</returns>
        public bool WaitForRunning([Optional] Duration duration)
        {
            long dur = duration == null ? -1 : DurationToApiDuration(duration);

            int res = YogiCore.YOGI_ContextWaitForRunning(Handle, dur);
            if ((ErrorCode)res == ErrorCode.Timeout) return false;
            CheckErrorCode(res);
            return true;
        }

        /// <summary>
        /// Blocks until the context's event processing loop is being run or until the
        /// specified timeout is reached.
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        /// <param name="duration">Maximum time to wait.</param>
        /// <returns>True if the context's event processing loop is running within the
        /// specified duration and false otherwise.</returns>
        public bool WaitForRunning(TimeSpan duration)
        {
            return WaitForRunning(new Duration(duration));
        }

        /// <summary>
        /// Blocks until no thread is running the context's event processing
        /// loop or until the specified timeout is reached.
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        /// <param name="duration">Maximum time to wait (infinity by default).</param>
        /// <returns>True if the context's event processing loop is not running within the
        /// specified duration and false otherwise.</returns>
        public bool WaitForStopped([Optional] Duration duration)
        {
            long dur = duration == null ? -1 : DurationToApiDuration(duration);

            int res = YogiCore.YOGI_ContextWaitForStopped(Handle, dur);
            if ((ErrorCode)res == ErrorCode.Timeout) return false;
            CheckErrorCode(res);
            return true;
        }

        /// <summary>
        /// Blocks until no thread is running the context's event processing
        /// loop or until the specified timeout is reached.
        ///
        /// This function must be called from outside any handler functions that are being
        /// executed through the context.
        /// </summary>
        /// <param name="duration">Maximum time to wait.</param>
        /// <returns>True if the context's event processing loop is not running within the
        /// specified duration and false otherwise.</returns>
        public bool WaitForStopped(TimeSpan duration)
        {
            return WaitForStopped(new Duration(duration));
        }

        /// <summary>
        /// Adds the given function to the context's event processing queue to
        /// be executed and returns immediately.
        ///
        /// The handler fn will only be executed after this function returns and
        /// only by a thread running the context's event processing loop.
        /// </summary>
        /// <param name="fn">The function to call from within the context.</param>
        public void Post(Action fn)
        {
            YogiCore.ContextPostFnDelegate wrapper = (userarg) =>
            {
                try
                {
                    fn();
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
                int res = YogiCore.YOGI_ContextPost(Handle, wrapper, wrapperPtr);
                CheckErrorCode(res);
            }
            catch
            {
                wrapperHandle.Free();
                throw;
            }

        }

        static IntPtr Create()
        {
            var handle = new IntPtr();
            int res = YogiCore.YOGI_ContextCreate(ref handle);
            CheckErrorCode(res);
            return handle;
        }
    }
}
