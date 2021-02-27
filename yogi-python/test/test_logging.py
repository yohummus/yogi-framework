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

from .conftest import Mocks


def test_configure_console_logging(mocks: Mocks):
    def fn(verbosity, stream, color, timefmt, fmt):
        assert verbosity == yogi.Verbosity.INFO
        assert stream == yogi.Stream.STDERR
        assert color == 0
        assert timefmt is None
        assert fmt is None
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigureConsoleLogging(fn)
    yogi.configure_console_logging(yogi.Verbosity.INFO, yogi.Stream.STDERR, False)

    def fn2(verbosity, stream, color, timefmt, fmt):
        assert verbosity == yogi.Verbosity.TRACE
        assert stream == yogi.Stream.STDOUT
        assert color == 1
        assert timefmt == b'foo'
        assert fmt == b'bar'
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigureConsoleLogging(fn2)
    yogi.configure_console_logging(yogi.Verbosity.TRACE, yogi.Stream.STDOUT, True, 'foo', 'bar')


def test_disable_console_logging(mocks: Mocks):
    called = False

    def fn(verbosity, stream, color, timefmt, fmt):
        assert verbosity == yogi.Verbosity.NONE
        nonlocal called
        called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigureConsoleLogging(fn)
    yogi.disable_console_logging()
    assert called


def test_configure_hook_logging(mocks: Mocks):
    called = False

    def hook_fn(severity, timestamp, tid, file, line, comp, msg):
        assert isinstance(severity, yogi.Verbosity)
        assert severity == yogi.Verbosity.TRACE
        assert isinstance(timestamp, yogi.Timestamp)
        assert timestamp.duration_since_epoch.nanoseconds_count == 12345
        assert tid == 555
        assert file == 'foo'
        assert line == 111
        assert comp == 'bar'
        assert 'hello' == msg
        nonlocal called
        called = True

    def fn(verbosity, fn_, userarg):
        assert verbosity == yogi.Verbosity.INFO
        assert fn_
        fn_(yogi.Verbosity.TRACE, 12345, 555, b'foo', 111, b'bar', b'hello', userarg)
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigureHookLogging(fn)
    yogi.configure_hook_logging(yogi.Verbosity.INFO, hook_fn)
    assert called


def test_disable_hook_logging(mocks: Mocks):
    called = False

    def fn(verbosity, fn_, userarg):
        assert verbosity == yogi.Verbosity.NONE
        nonlocal called
        called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigureHookLogging(fn)
    yogi.disable_hook_logging()
    assert called


def test_configure_file_logging(mocks: Mocks, hello_bytes: bytes):
    def fn(verbosity, filename, genfn, genfnsize, timefmt, fmt):
        assert verbosity == yogi.Verbosity.INFO
        assert filename == b'foo'
        assert genfn
        assert not genfnsize
        assert timefmt is None
        assert fmt is None
        genfn.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigureFileLogging(fn)
    filename = yogi.configure_file_logging(yogi.Verbosity.INFO, 'foo')
    assert filename == 'hello'

    def fn2(verbosity, filename, genfn, genfnsize, timefmt, fmt):
        assert verbosity == yogi.Verbosity.TRACE
        assert filename == b'moo'
        assert genfn
        assert not genfnsize
        assert timefmt == b'foo'
        assert fmt == b'bar'
        genfn.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigureFileLogging(fn2)
    filename = yogi.configure_file_logging(yogi.Verbosity.TRACE, 'moo', 'foo', 'bar')
    assert filename == 'hello'


def test_disable_file_logging(mocks: Mocks):
    called = False

    def fn(verbosity, filename, genfn, genfnsize, timefmt, fmt):
        assert verbosity == yogi.Verbosity.NONE
        nonlocal called
        called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigureFileLogging(fn)
    yogi.disable_file_logging()
    assert called


def test_set_components_verbosity(mocks: Mocks):
    def fn(components, verbosity, count):
        assert components == b'foo'
        assert verbosity == yogi.Verbosity.WARNING
        assert count
        count.contents.value = 5
        return yogi.ErrorCode.OK

    mocks.MOCK_LoggerSetComponentsVerbosity(fn)
    count = yogi.Logger.set_components_verbosity('foo', yogi.Verbosity.WARNING)
    assert count == 5


def test_create_logger(mocks: Mocks):
    called = False

    def fn(logger, component):
        assert logger
        assert component == b'foo'
        logger.contents.value = 1234
        nonlocal called
        called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_LoggerCreate(fn)
    logger = yogi.Logger('foo')
    assert logger._handle.value == 1234
    assert called


def test_logger_get_verbosity(mocks: Mocks, logger: yogi.Logger):
    def fn(logger, verbosity):
        assert logger == 5555
        assert verbosity
        verbosity.contents.value = yogi.Verbosity.WARNING
        return yogi.ErrorCode.OK

    mocks.MOCK_LoggerGetVerbosity(fn)
    assert logger.verbosity == yogi.Verbosity.WARNING


def test_logger_set_verbosity(mocks: Mocks, logger: yogi.Logger):
    called = False

    def fn(logger, verbosity):
        assert logger == 5555
        assert verbosity == yogi.Verbosity.WARNING
        nonlocal called
        called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_LoggerSetVerbosity(fn)
    logger.verbosity = yogi.Verbosity.WARNING
    assert called


def test_logger_log(mocks: Mocks, logger: yogi.Logger):
    def fn(logger, severity, file, line, msg):
        assert logger == 5555
        assert severity == yogi.Verbosity.WARNING
        assert file == b'foo'
        assert line == 123
        assert msg == b'bar'
        return yogi.ErrorCode.OK

    mocks.MOCK_LoggerLog(fn)
    logger.log(yogi.Verbosity.WARNING, 'bar', 'foo', 123)

    def fn2(logger, severity, file, line, msg):
        assert severity == yogi.Verbosity.ERROR
        assert os.path.basename(__file__) in file.decode()
        assert 1 < line < 1000
        return yogi.ErrorCode.OK

    mocks.MOCK_LoggerLog(fn2)
    logger.log(yogi.Verbosity.ERROR, 'bar')


def test_app_logger(mocks: Mocks):
    def fn(logger, severity, file, line, msg):
        assert logger is None
        return yogi.ErrorCode.OK

    mocks.MOCK_LoggerLog(fn)
    yogi.app_logger.log(yogi.Verbosity.INFO, 'foo')
