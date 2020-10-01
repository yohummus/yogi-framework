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


class TestConfigurations(TestCase):
    def setUp(self):
        self.config = self.create_configuration()

    def test_create(self):
        called = False

        def fn(config, flags):
            self.assertEqual(flags, yogi.ConfigurationFlags.NONE)
            config.contents.value = self.pointer.value
            nonlocal called
            called = True
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationCreate(fn)

        config = yogi.Configuration()
        self.assertEqual(config._handle.value, self.pointer.value)
        self.assertEqual(config.flags, yogi.ConfigurationFlags.NONE)
        self.assertTrue(called)

        def fn2(config, flags):
            self.assertEqual(flags, yogi.ConfigurationFlags.MUTABLE_CMD_LINE)
            config.contents.value = self.pointer.value
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationCreate(fn2)

        config = yogi.Configuration(yogi.ConfigurationFlags.MUTABLE_CMD_LINE)

    def test_flags(self):
        config = self.create_configuration(yogi.ConfigurationFlags.DISABLE_VARIABLES)
        self.assertEqual(config.flags, yogi.ConfigurationFlags.DISABLE_VARIABLES)

    def test_update_from_command_line(self):
        def fn(config, argc, argv, options):
            self.assertEqual(config, self.pointer.value)
            self.assertEqual(argc, 3)
            self.assertEqual(argv[0][0:3], b'ax\x00')
            self.assertEqual(argv[1][0:3], b'by\x00')
            self.assertEqual(argv[2][0:3], b'cz\x00')
            self.assertEqual(options, yogi.CommandLineOptions.BRANCH_GHOST_MODE)
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationUpdateFromCommandLine(fn)
        self.config.update_from_command_line(['ax', 'by', 'cz'], yogi.CommandLineOptions.BRANCH_GHOST_MODE)

    def test_update_from_json(self):
        def fn(config, json):
            self.assertEqual(config, self.pointer.value)
            self.assertEqual(json, b'foo')
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationUpdateFromJson(fn)
        self.config.update_from_json('foo')

    def test_update_from_file(self):
        def fn(config, filename):
            self.assertEqual(config, self.pointer.value)
            self.assertEqual(filename, b'foo')
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationUpdateFromFile(fn)
        self.config.update_from_file('foo')

    def test_dump(self):
        def fn(config, json, jsonsize, resvars, indent):
            self.assertEqual(config, self.pointer.value)
            self.assertTrue(json)
            self.assertFalse(jsonsize)
            self.assertTrue(resvars)
            self.assertEqual(indent, -1)
            json.contents.value = self.hello_bytes
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationDump(fn)
        self.assertEqual(self.config.dump(), 'hello')

        def fn2(config, json, jsonsize, resvars, indent):
            self.assertEqual(config, self.pointer.value)
            self.assertTrue(json)
            self.assertFalse(jsonsize)
            self.assertFalse(resvars)
            self.assertEqual(indent, 2)
            json.contents.value = self.hello_bytes
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationDump(fn2)
        self.assertEqual(self.config.dump(resolve_variables=False, indentation=2), 'hello')

        config = self.create_configuration(yogi.ConfigurationFlags.DISABLE_VARIABLES)
        config.dump(indentation=2)

    def test_to_json(self):
        def fn(*, resolve_variables=None, **kwargs):
            self.assertIsNone(resolve_variables)
            return '{"x": 5}'

        self.config.dump = fn
        self.assertEqual(self.config.to_json()['x'], 5)

        def fn2(*, resolve_variables=None, **kwargs):
            self.assertTrue(resolve_variables)
            return '{"x": 3}'

        self.config.dump = fn2
        self.assertEqual(self.config.to_json(resolve_variables=True)['x'], 3)

    def test_write_to_file(self):
        def fn(config, filename, resvars, indent):
            self.assertEqual(config, self.pointer.value)
            self.assertEqual(filename, b'foo')
            self.assertTrue(resvars)
            self.assertEqual(indent, -1)
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationWriteToFile(fn)
        self.config.write_to_file('foo')

        def fn2(config, filename, resvars, indent):
            self.assertFalse(resvars)
            self.assertEqual(indent, 2)
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationWriteToFile(fn2)
        self.config.write_to_file('foo', resolve_variables=False, indentation=2)

        config = self.create_configuration(yogi.ConfigurationFlags.DISABLE_VARIABLES)
        config.write_to_file('foo', indentation=2)

    def test_validate(self):
        def fn(config, section, schema):
            self.assertEqual(config, self.pointer.value)
            self.assertEqual(section, b'foo')
            self.assertEqual(schema, self.pointer.value)
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationValidate(fn)
        schema = self.create_configuration()
        self.config.validate(schema, section='foo')

        def fn2(config, section, schema):
            self.assertIsNone(section)
            return yogi.ErrorCode.OK

        self.MOCK_ConfigurationValidate(fn2)
        self.config.validate(schema)
