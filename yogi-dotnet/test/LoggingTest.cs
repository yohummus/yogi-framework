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
    public class LoggingTest : TestCase
    {
        static Yogi.Logger CreateLogger(string component)
        {
            MOCK_LoggerCreate((ref IntPtr logger, string comp_) =>
            {
                logger = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            return new Yogi.Logger(component);
        }

        [Fact]
        public void ConfigureConsoleLogging()
        {
            MOCK_ConfigureConsoleLogging((int verbosity, int stream, int color, string timefmt, string fmt) =>
            {
                Assert.Equal((int)Yogi.Verbosity.Info, verbosity);
                Assert.Equal((int)Yogi.Stream.Stderr, stream);
                Assert.Equal(0, (int)color);
                Assert.Null(timefmt);
                Assert.Null(fmt);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.ConfigureConsoleLogging(Yogi.Verbosity.Info, Yogi.Stream.Stderr, false);

            MOCK_ConfigureConsoleLogging((int verbosity, int stream, int color, string timefmt, string fmt) =>
            {
                Assert.Equal((int)Yogi.Verbosity.Trace, verbosity);
                Assert.Equal((int)Yogi.Stream.Stdout, stream);
                Assert.Equal(1, (int)color);
                Assert.Equal("foo", timefmt);
                Assert.Equal("bar", fmt);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.ConfigureConsoleLogging(Yogi.Verbosity.Trace, Yogi.Stream.Stdout, true, "foo", "bar");
        }

        [Fact]
        public void ConfigureConsoleLoggingError()
        {
            MOCK_ConfigureConsoleLogging((int verbosity, int stream, int color, string timefmt, string fmt) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                Yogi.ConfigureConsoleLogging(Yogi.Verbosity.Info, Yogi.Stream.Stderr, false);
            });
        }

        [Fact]
        public void DisableConsoleLogging()
        {
            MOCK_ConfigureConsoleLogging((int verbosity, int stream, int color, string timefmt, string fmt) =>
            {
                Assert.Equal((int)Yogi.Verbosity.None, verbosity);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.DisableConsoleLogging();
        }

        [Fact]
        public void DisableConsoleLoggingError()
        {
            MOCK_ConfigureConsoleLogging((int verbosity, int stream, int color, string timefmt, string fmt) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                Yogi.DisableConsoleLogging();
            });
        }

        [Fact]
        public void ConfigureHookLogging()
        {
            bool called = false;
            Yogi.LogToHookFnDelegate fn = (Yogi.Verbosity severity, Yogi.Timestamp timestamp, int tid, string file,
                                           int line, string comp, string msg) =>
            {
                Assert.Equal(Yogi.Verbosity.Trace, severity);
                Assert.Equal(12345L, timestamp.DurationSinceEpoch.NanosecondsCount);
                Assert.Equal(555, tid);
                Assert.Equal("foo", file);
                Assert.Equal(111, line);
                Assert.Equal("bar", comp);
                Assert.Equal("hello", msg);
                called = true;
            };

            MOCK_ConfigureHookLogging((int verbosity, ConfigureHookLoggingFnDelegate fn_, IntPtr userarg) =>
            {
                Assert.Equal((int)Yogi.Verbosity.Info, verbosity);
                Assert.NotNull(fn_);
                fn_((int)Yogi.Verbosity.Trace, 12345, 555, "foo", 111, "bar", "hello", userarg);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.ConfigureHookLogging(Yogi.Verbosity.Info, fn);
            Assert.True(called);
        }

        [Fact]
        public void ConfigureHookLoggingWithNullFn()
        {
            MOCK_ConfigureHookLogging((int verbosity, ConfigureHookLoggingFnDelegate fn, IntPtr userarg) =>
            {
                Assert.Null(fn);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.ConfigureHookLogging(Yogi.Verbosity.Info, null);
        }

        [Fact]
        public void ConfigureHookLoggingError()
        {
            MOCK_ConfigureHookLogging((int verbosity, ConfigureHookLoggingFnDelegate fn_, IntPtr userarg) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Yogi.LogToHookFnDelegate fn = (Yogi.Verbosity severity, Yogi.Timestamp timestamp, int tid, string file,
                                           int line, string comp, string msg) =>
            { };

            Assert.Throws<Yogi.FailureException>(() =>
            {
                Yogi.ConfigureHookLogging(Yogi.Verbosity.Info, fn);
            });
        }

        [Fact]
        public void DisableHookLogging()
        {
            MOCK_ConfigureHookLogging((int verbosity, ConfigureHookLoggingFnDelegate fn_, IntPtr userarg) =>
            {
                Assert.Equal((int)Yogi.Verbosity.None, verbosity);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.DisableHookLogging();
        }

        [Fact]
        public void DisableHookLoggingError()
        {
            MOCK_ConfigureHookLogging((int verbosity, ConfigureHookLoggingFnDelegate fn_, IntPtr userarg) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                Yogi.DisableHookLogging();
            });
        }

        [Fact]
        public void ConfigureFileLogging()
        {
            MOCK_ConfigureFileLogging((int verbosity, string filename, ref IntPtr genfn, ref int genfnsize,
                                       string timefmt, string fmt) =>
            {
                Assert.Equal((int)Yogi.Verbosity.Info, verbosity);
                Assert.Equal("foo", filename);
                Assert.Null(timefmt);
                Assert.Null(fmt);
                genfn = helloBytes;
                genfnsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal("hello", Yogi.ConfigureFileLogging(Yogi.Verbosity.Info, "foo"));


            MOCK_ConfigureFileLogging((int verbosity, string filename, ref IntPtr genfn, ref int genfnsize,
                                       string timefmt, string fmt) =>
            {
                Assert.Equal((int)Yogi.Verbosity.Trace, verbosity);
                Assert.Equal("moo", filename);
                Assert.Equal("foo", timefmt);
                Assert.Equal("bar", fmt);
                genfn = helloBytes;
                genfnsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal("hello", Yogi.ConfigureFileLogging(Yogi.Verbosity.Trace, "moo", "foo", "bar"));
        }

        [Fact]
        public void ConfigureFileLoggingError()
        {
            MOCK_ConfigureFileLogging((int verbosity, string filename, ref IntPtr genfn, ref int genfnsize,
                                       string timefmt, string fmt) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                Yogi.ConfigureFileLogging(Yogi.Verbosity.Info, "foo");
            });
        }

        [Fact]
        public void DisableFileLogging()
        {
            MOCK_ConfigureFileLogging((int verbosity, string filename, ref IntPtr genfn, ref int genfnsize,
                                       string timefmt, string fmt) =>
            {
                Assert.Equal((int)Yogi.Verbosity.None, verbosity);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.DisableFileLogging();
        }

        [Fact]
        public void DisableFileLoggingError()
        {
            MOCK_ConfigureFileLogging((int verbosity, string filename, ref IntPtr genfn, ref int genfnsize,
                                       string timefmt, string fmt) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                Yogi.DisableFileLogging();
            });
        }

        [Fact]
        public void SetComponentsVerbosity()
        {
            MOCK_LoggerSetComponentsVerbosity((string components, int verbosity, ref int count) =>
            {
                Assert.Equal("foo", components);
                Assert.Equal((int)Yogi.Verbosity.Warning, verbosity);
                count = 5;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(5, Yogi.Logger.SetComponentsVerbosity("foo", Yogi.Verbosity.Warning));
        }

        [Fact]
        public void SetComponentsVerbosityError()
        {
            MOCK_LoggerSetComponentsVerbosity((string components, int verbosity, ref int count) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                Yogi.Logger.SetComponentsVerbosity("foo", Yogi.Verbosity.Warning);
            });
        }

        [Fact]
        public void LoggerCreate()
        {
            MOCK_LoggerCreate((ref IntPtr logger, string comp_) =>
            {
                logger = pointer;
                Assert.Equal("foo", comp_);
                return (int)Yogi.ErrorCode.Ok;
            });

            new Yogi.Logger("foo");
        }

        [Fact]
        public void LoggerCreateError()
        {
            MOCK_LoggerCreate((ref IntPtr logger, string comp_) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                new Yogi.Logger("foo");
            });
        }

        [Fact]
        public void LoggerGetVerbosity()
        {
            var logger = CreateLogger("foo");

            MOCK_LoggerGetVerbosity((IntPtr logger_, ref int verbosity) =>
            {
                Assert.Equal(pointer, logger_);
                verbosity = (int)Yogi.Verbosity.Warning;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(Yogi.Verbosity.Warning, logger.Verbosity);
        }

        [Fact]
        public void LoggerGetVerbosityError()
        {
            var logger = CreateLogger("foo");

            MOCK_LoggerGetVerbosity((IntPtr logger_, ref int verbosity) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                Assert.Equal(Yogi.Verbosity.Warning, logger.Verbosity);
            });
        }

        [Fact]
        public void LoggerSetVerbosity()
        {
            var logger = CreateLogger("foo");

            MOCK_LoggerSetVerbosity((IntPtr logger_, int verbosity) =>
            {
                Assert.Equal(pointer, logger_);
                verbosity = (int)Yogi.Verbosity.Warning;
                return (int)Yogi.ErrorCode.Ok;
            });

            logger.Verbosity = Yogi.Verbosity.Warning;
        }

        [Fact]
        public void LoggerSetVerbosityError()
        {
            var logger = CreateLogger("foo");

            MOCK_LoggerSetVerbosity((IntPtr logger_, int verbosity) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                logger.Verbosity = Yogi.Verbosity.Warning;
            });
        }

        [Fact]
        public void Log()
        {
            var logger = CreateLogger("foo");

            MOCK_LoggerLog((IntPtr logger_, int severity, string file, int line, string msg) =>
            {
                Assert.Equal(pointer, logger_);
                Assert.Equal((int)Yogi.Verbosity.Warning, severity);
                Assert.Equal("foo", file);
                Assert.Equal(123, line);
                Assert.Equal("bar", msg);
                return (int)Yogi.ErrorCode.Ok;
            });

            logger.Log(Yogi.Verbosity.Warning, "bar", "foo", 123);

            MOCK_LoggerLog((IntPtr logger_, int severity, string file, int line, string msg) =>
            {
                Assert.Equal((int)Yogi.Verbosity.Error, severity);
                Assert.Contains("LoggingTest", file);
                Assert.InRange(line, 1, 1000);
                return (int)Yogi.ErrorCode.Ok;
            });

            logger.Log(Yogi.Verbosity.Error, "bar");
        }

        [Fact]
        public void LogError()
        {
            var logger = CreateLogger("foo");

            MOCK_LoggerLog((IntPtr logger_, int severity, string file, int line, string msg) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.Throws<Yogi.FailureException>(() =>
            {
                logger.Log(Yogi.Verbosity.Warning, "bar", "foo", 123);
            });
        }

        [Fact]
        public void AppLogger()
        {
            Assert.IsType<Yogi.AppLoggerType>(Yogi.AppLogger);
            Assert.Equal("AppLogger", Yogi.AppLogger.ToString());

            MOCK_LoggerLog((IntPtr logger, int severity, string file, int line, string msg) =>
            {
                Assert.Equal(IntPtr.Zero, logger);
                return (int)Yogi.ErrorCode.Ok;
            });

            Yogi.AppLogger.Log(Yogi.Verbosity.Info, "foo");
        }
    }
}
