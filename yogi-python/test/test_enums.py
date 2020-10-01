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

from .common import TestCase


class TestEnums(TestCase):
    def test_error_code(self):
        self.assertEqual(yogi.ErrorCode.OK, 0)
        self.assertEqual(yogi.ErrorCode.UNKNOWN, -1)

    def test_verbosity(self):
        self.assertEqual(yogi.Verbosity.ERROR, 1)
        self.assertEqual(yogi.Verbosity.DEBUG, 4)

    def test_stream(self):
        self.assertEqual(yogi.Stream.STDOUT, 0)
        self.assertEqual(yogi.Stream.STDERR, 1)

    def test_encoding(self):
        self.assertEqual(yogi.Encoding.JSON, 0)
        self.assertEqual(yogi.Encoding.MSGPACK, 1)

    def test_http_status(self):
        self.assertEqual(yogi.HttpStatus.HTTP_400, 400)
        self.assertEqual(yogi.HttpStatus.HTTP_503, 503)

    def test_web_process_action(self):
        self.assertEqual(yogi.WebProcessAction.START, 0)
        self.assertEqual(yogi.WebProcessAction.CANCELED, 2)

    def test_web_process_update(self):
        self.assertEqual(yogi.WebProcessUpdate.STARTED, 0)
        self.assertEqual(yogi.WebProcessUpdate.CANCELED, 4)

    def test_http_methods(self):
        self.assertEqual(yogi.HttpMethods.GET, 1 << 0)
        self.assertEqual(yogi.HttpMethods.DELETE, 1 << 4)

    def test_signals(self):
        self.assertEqual(yogi.Signals.TERM, 1 << 1)
        self.assertEqual(yogi.Signals.USR1, 1 << 24)

    def test_configuration_flags(self):
        self.assertEqual(yogi.ConfigurationFlags.DISABLE_VARIABLES, 1 << 0)
        self.assertEqual(yogi.ConfigurationFlags.MUTABLE_CMD_LINE, 1 << 1)

    def test_command_line_options(self):
        self.assertEqual(yogi.CommandLineOptions.BRANCH_NAME, 1 << 1)
        self.assertEqual(yogi.CommandLineOptions.BRANCH_ALL &
                         yogi.CommandLineOptions.BRANCH_PATH, yogi.CommandLineOptions.BRANCH_PATH)

    def test_branch_events(self):
        self.assertEqual(yogi.BranchEvents.BRANCH_QUERIED, 1 << 1)
        self.assertEqual(yogi.BranchEvents.CONNECT_FINISHED, 1 << 2)
