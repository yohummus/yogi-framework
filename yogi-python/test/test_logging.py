# This file is part of the Yogi Framework
# https://github.com/yohummus/yogi-framework.
#
# Copyright (c) 2020 Johannes Bergmann.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

import yogi
import os.path

from .common import TestCase


class TestLogging(TestCase):
    def create_logger(self, component='foo'):
        def fn(logger, component):
            logger.contents.value = self.pointer.value
            return yogi.ErrorCode.OK

        self.MOCK_LoggerCreate(fn)
        return yogi.Logger(component)

    def test_configure_console_logging(self):
        def fn(verbosity, stream, color, timefmt, fmt):
            self.assertEqual(verbosity, yogi.Verbosity.INFO)
            self.assertEqual(stream, yogi.Stream.STDERR)
            self.assertEqual(color, 0)
            self.assertIsNone(timefmt)
            self.assertIsNone(fmt)
            return yogi.ErrorCode.OK

        self.MOCK_ConfigureConsoleLogging(fn)
        yogi.configure_console_logging(yogi.Verbosity.INFO, yogi.Stream.STDERR, False)

        def fn2(verbosity, stream, color, timefmt, fmt):
            self.assertEqual(verbosity, yogi.Verbosity.TRACE)
            self.assertEqual(stream, yogi.Stream.STDOUT)
            self.assertEqual(color, 1)
            self.assertEqual(timefmt, b'foo')
            self.assertEqual(fmt, b'bar')
            return yogi.ErrorCode.OK

        self.MOCK_ConfigureConsoleLogging(fn2)
        yogi.configure_console_logging(yogi.Verbosity.TRACE, yogi.Stream.STDOUT, True, 'foo', 'bar')

    def test_disable_console_logging(self):
        called = False

        def fn(verbosity, stream, color, timefmt, fmt):
            self.assertEqual(verbosity, yogi.Verbosity.NONE)
            nonlocal called
            called = True
            return yogi.ErrorCode.OK

        self.MOCK_ConfigureConsoleLogging(fn)
        yogi.disable_console_logging()
        self.assertTrue(called)

    def test_configure_hook_logging(self):
        called = False

        def hook_fn(severity, timestamp, tid, file, line, comp, msg):
            self.assertIsInstance(severity, yogi.Verbosity)
            self.assertEqual(severity, yogi.Verbosity.TRACE)
            self.assertIsInstance(timestamp, yogi.Timestamp)
            self.assertEqual(timestamp.duration_since_epoch.nanoseconds_count, 12345)
            self.assertEqual(tid, 555)
            self.assertEqual(file, 'foo')
            self.assertEqual(line, 111)
            self.assertEqual(comp, 'bar')
            self.assertEqual('hello', msg)
            nonlocal called
            called = True

        def fn(verbosity, fn_, userarg):
            self.assertEqual(verbosity, yogi.Verbosity.INFO)
            self.assertTrue(fn_)
            fn_(yogi.Verbosity.TRACE, 12345, 555, b'foo', 111, b'bar', b'hello', userarg)
            return yogi.ErrorCode.OK

        self.MOCK_ConfigureHookLogging(fn)
        yogi.configure_hook_logging(yogi.Verbosity.INFO, hook_fn)
        self.assertTrue(called)

    def test_disable_hook_logging(self):
        called = False

        def fn(verbosity, fn_, userarg):
            self.assertEqual(verbosity, yogi.Verbosity.NONE)
            nonlocal called
            called = True
            return yogi.ErrorCode.OK

        self.MOCK_ConfigureHookLogging(fn)
        yogi.disable_hook_logging()
        self.assertTrue(called)

    def test_configure_file_logging(self):
        def fn(verbosity, filename, genfn, genfnsize, timefmt, fmt):
            self.assertEqual(verbosity, yogi.Verbosity.INFO)
            self.assertEqual(filename, b'foo')
            self.assertTrue(genfn)
            self.assertFalse(genfnsize)
            self.assertIsNone(timefmt)
            self.assertIsNone(fmt)
            genfn.contents.value = self.hello_bytes
            return yogi.ErrorCode.OK

        self.MOCK_ConfigureFileLogging(fn)
        filename = yogi.configure_file_logging(yogi.Verbosity.INFO, 'foo')
        self.assertEqual(filename, 'hello')

        def fn2(verbosity, filename, genfn, genfnsize, timefmt, fmt):
            self.assertEqual(verbosity, yogi.Verbosity.TRACE)
            self.assertEqual(filename, b'moo')
            self.assertTrue(genfn)
            self.assertFalse(genfnsize)
            self.assertEqual(timefmt, b'foo')
            self.assertEqual(fmt, b'bar')
            genfn.contents.value = self.hello_bytes
            return yogi.ErrorCode.OK

        self.MOCK_ConfigureFileLogging(fn2)
        filename = yogi.configure_file_logging(yogi.Verbosity.TRACE, 'moo', 'foo', 'bar')
        self.assertEqual(filename, 'hello')

    def test_disable_file_logging(self):
        called = False

        def fn(verbosity, filename, genfn, genfnsize, timefmt, fmt):
            self.assertEqual(verbosity, yogi.Verbosity.NONE)
            nonlocal called
            called = True
            return yogi.ErrorCode.OK

        self.MOCK_ConfigureFileLogging(fn)
        yogi.disable_file_logging()
        self.assertTrue(called)

    def test_set_components_verbosity(self):
        def fn(components, verbosity, count):
            self.assertEqual(components, b'foo')
            self.assertEqual(verbosity, yogi.Verbosity.WARNING)
            self.assertTrue(count)
            count.contents.value = 5
            return yogi.ErrorCode.OK

        self.MOCK_LoggerSetComponentsVerbosity(fn)
        count = yogi.Logger.set_components_verbosity('foo', yogi.Verbosity.WARNING)
        self.assertEqual(count, 5)

    def test_create_logger(self):
        called = False

        def fn(logger, component):
            self.assertTrue(logger)
            self.assertEqual(component, b'foo')
            logger.contents.value = self.pointer.value
            nonlocal called
            called = True
            return yogi.ErrorCode.OK

        self.MOCK_LoggerCreate(fn)
        logger = yogi.Logger('foo')
        self.assertEqual(logger._handle.value, self.pointer.value)
        self.assertTrue(called)

    def test_logger_get_verbosity(self):
        logger = self.create_logger()

        def fn(logger, verbosity):
            self.assertEqual(logger, self.pointer.value)
            self.assertTrue(verbosity)
            verbosity.contents.value = yogi.Verbosity.WARNING
            return yogi.ErrorCode.OK

        self.MOCK_LoggerGetVerbosity(fn)
        self.assertEqual(logger.verbosity, yogi.Verbosity.WARNING)

    def test_logger_set_verbosity(self):
        logger = self.create_logger()
        called = False

        def fn(logger, verbosity):
            self.assertEqual(logger, self.pointer.value)
            self.assertEqual(verbosity, yogi.Verbosity.WARNING)
            nonlocal called
            called = True
            return yogi.ErrorCode.OK

        self.MOCK_LoggerSetVerbosity(fn)
        logger.verbosity = yogi.Verbosity.WARNING
        self.assertTrue(called)

    def test_logger_log(self):
        logger = self.create_logger()

        def fn(logger, severity, file, line, msg):
            self.assertEqual(logger, self.pointer.value)
            self.assertEqual(severity, yogi.Verbosity.WARNING)
            self.assertEqual(file, b'foo')
            self.assertEqual(line, 123)
            self.assertEqual(msg, b'bar')
            return yogi.ErrorCode.OK

        self.MOCK_LoggerLog(fn)
        logger.log(yogi.Verbosity.WARNING, 'bar', 'foo', 123)

        def fn2(logger, severity, file, line, msg):
            self.assertEqual(severity, yogi.Verbosity.ERROR)
            self.assertTrue(os.path.basename(__file__) in file.decode())
            self.assertTrue(1 < line < 1000)
            return yogi.ErrorCode.OK

        self.MOCK_LoggerLog(fn2)
        logger.log(yogi.Verbosity.ERROR, 'bar')

    def test_app_logger(self):
        def fn(logger, severity, file, line, msg):
            self.assertIsNone(logger)
            return yogi.ErrorCode.OK

        self.MOCK_LoggerLog(fn)
        yogi.app_logger.log(yogi.Verbosity.INFO, 'foo')
