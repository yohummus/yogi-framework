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

from .conftest import Mocks


def test_create(mocks: Mocks):
    """Verifies that Configuration can be instantiated"""
    called = False

    def fn(config, flags):
        assert flags == yogi.ConfigurationFlags.NONE
        config.contents.value = 1234
        nonlocal called
        called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationCreate(fn)

    configuration = yogi.Configuration()
    assert configuration._handle.value == 1234
    assert configuration.flags == yogi.ConfigurationFlags.NONE
    assert called

    def fn2(config, flags):
        assert flags == yogi.ConfigurationFlags.MUTABLE_CMD_LINE
        config.contents.value = 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationCreate(fn2)

    configuration = yogi.Configuration(yogi.ConfigurationFlags.MUTABLE_CMD_LINE)


def test_flags(mocks: Mocks):
    """Verifies that the flag property returns the flags set during construction"""
    configuration = create_configuration(mocks, yogi.ConfigurationFlags.DISABLE_VARIABLES)
    assert configuration.flags == yogi.ConfigurationFlags.DISABLE_VARIABLES


def test_update_from_command_line(mocks: Mocks, configuration: yogi.Configuration):
    """Verifies that a configuration can be updated from command line arguments"""
    def fn(config, argc, argv, options):
        assert config == 1234
        assert argc == 3
        assert argv[0][0:3] == b'ax\x00'
        assert argv[1][0:3] == b'by\x00'
        assert argv[2][0:3] == b'cz\x00'
        assert options == yogi.CommandLineOptions.BRANCH_GHOST_MODE
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationUpdateFromCommandLine(fn)
    configuration.update_from_command_line(['ax', 'by', 'cz'], yogi.CommandLineOptions.BRANCH_GHOST_MODE)


def test_update_from_json(mocks: Mocks, configuration: yogi.Configuration):
    """Verifies that a configuration can be updated from a JSON string"""
    def fn(config, json):
        assert config == 1234
        assert json == b'foo'
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationUpdateFromJson(fn)
    configuration.update_from_json('foo')


def test_update_from_file(mocks: Mocks, configuration: yogi.Configuration):
    """Verifies that a configuration can be updated from a JSON file"""
    def fn(config, filename):
        assert config == 1234
        assert filename == b'foo'
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationUpdateFromFile(fn)
    configuration.update_from_file('foo')


def test_dump(mocks: Mocks, configuration: yogi.Configuration, hello_bytes: bytes):
    """Verifies that a configuration can be dumped as JSON"""
    def fn(config, json, jsonsize, resvars, indent):
        assert config == 1234
        assert json
        assert not jsonsize
        assert resvars
        assert indent == -1
        json.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationDump(fn)
    assert configuration.dump() == 'hello'

    def fn2(config, json, jsonsize, resvars, indent):
        assert config == 1234
        assert json
        assert not jsonsize
        assert not resvars
        assert indent == 2
        json.contents.value = hello_bytes
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationDump(fn2)
    assert configuration.dump(resolve_variables=False, indentation=2) == 'hello'

    configuration = create_configuration(mocks, yogi.ConfigurationFlags.DISABLE_VARIABLES)
    configuration.dump(indentation=2)


def test_to_json(mocks: Mocks, configuration: yogi.Configuration):
    """Verifies that Configuration has the to_json() convenience function"""
    def fn(*, resolve_variables=None, **kwargs):
        assert resolve_variables is None
        return '{"x": 5}'

    configuration.dump = fn
    assert configuration.to_json()['x'] == 5

    def fn2(*, resolve_variables=None, **kwargs):
        assert resolve_variables
        return '{"x": 3}'

    configuration.dump = fn2
    assert configuration.to_json(resolve_variables=True)['x'] == 3


def test_write_to_file(mocks: Mocks, configuration: yogi.Configuration):
    """Verifies that a configuration can be written to a file"""
    def fn(config, filename, resvars, indent):
        assert config == 1234
        assert filename == b'foo'
        assert resvars
        assert indent == -1
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationWriteToFile(fn)
    configuration.write_to_file('foo')

    def fn2(config, filename, resvars, indent):
        assert not resvars
        assert indent == 2
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationWriteToFile(fn2)
    configuration.write_to_file('foo', resolve_variables=False, indentation=2)

    configuration = create_configuration(mocks, yogi.ConfigurationFlags.DISABLE_VARIABLES)
    configuration.write_to_file('foo', indentation=2)


def test_validate(mocks: Mocks, configuration: yogi.Configuration):
    """Verifies that a configuration can be validated"""
    def fn(config, section, schema):
        assert config == 1234
        assert section == b'foo'
        assert schema == 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationValidate(fn)
    schema = create_configuration(mocks)
    configuration.validate(schema, section='foo')

    def fn2(config, section, schema):
        assert section is None
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationValidate(fn2)
    configuration.validate(schema)


def create_configuration(mocks: Mocks, flags=yogi.ConfigurationFlags.NONE):
    """Helper function to create a configuration with certain flags"""
    def fn(config, flags):
        config.contents.value = 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationCreate(fn)
    return yogi.Configuration(flags)
