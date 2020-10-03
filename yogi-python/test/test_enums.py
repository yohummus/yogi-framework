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


def test_error_code():
    """Checks the ErrorCode enum"""
    assert yogi.ErrorCode.OK == 0
    assert yogi.ErrorCode.UNKNOWN == -1


def test_verbosity():
    """Checks the Verbosity enum"""
    assert yogi.Verbosity.ERROR == 1
    assert yogi.Verbosity.DEBUG == 4


def test_stream():
    """Checks the Stream enum"""
    assert yogi.Stream.STDOUT == 0
    assert yogi.Stream.STDERR == 1


def test_encoding():
    """Checks the Encoding enum"""
    assert yogi.Encoding.JSON == 0
    assert yogi.Encoding.MSGPACK == 1


def test_http_status():
    """Checks the HttpStatus enum"""
    assert yogi.HttpStatus.HTTP_400 == 400
    assert yogi.HttpStatus.HTTP_503 == 503


def test_web_process_action():
    """Checks the WebProcessAction enum"""
    assert yogi.WebProcessAction.START == 0
    assert yogi.WebProcessAction.CANCELED == 2


def test_web_process_update():
    """Checks the WebProcessUpdate enum"""
    assert yogi.WebProcessUpdate.STARTED == 0
    assert yogi.WebProcessUpdate.CANCELED == 4


def test_http_methods():
    """Checks the HttpMethods flags"""
    assert yogi.HttpMethods.GET == 1 << 0
    assert yogi.HttpMethods.DELETE == 1 << 4


def test_signals():
    """Checks the Signals flags"""
    assert yogi.Signals.TERM == 1 << 1
    assert yogi.Signals.USR1 == 1 << 24


def test_configuration_flags():
    """Checks the ConfigurationFlags flags"""
    assert yogi.ConfigurationFlags.DISABLE_VARIABLES == 1 << 0
    assert yogi.ConfigurationFlags.MUTABLE_CMD_LINE == 1 << 1


def test_command_line_options():
    """Checks the CommandLineOptions flags"""
    assert yogi.CommandLineOptions.BRANCH_NAME == 1 << 1
    assert yogi.CommandLineOptions.BRANCH_ALL & yogi.CommandLineOptions.BRANCH_PATH \
        == yogi.CommandLineOptions.BRANCH_PATH


def test_branch_events():
    """Checks the BranchEvents flags"""
    assert yogi.BranchEvents.BRANCH_QUERIED == 1 << 1
    assert yogi.BranchEvents.CONNECT_FINISHED == 1 << 2
