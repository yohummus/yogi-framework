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
import pytest
import json
from ctypes import memmove
from unittest.mock import patch
from uuid import UUID

from .conftest import Mocks


@pytest.mark.parametrize('info_cls', [yogi.BranchInfo, yogi.RemoteBranchInfo, yogi.LocalBranchInfo,
                                      yogi.BranchEventInfo, yogi.BranchDiscoveredEventInfo, yogi.BranchQueriedEventInfo,
                                      yogi.ConnectFinishedEventInfo, yogi.ConnectionLostEventInfo])
def test_branch_info(info_cls):
    """Verifies that the BranchInfo/BranchEventInfo and derived classes have the expected properties"""
    info_string = json.dumps({
        # BranchInfo
        "uuid": "123e4567-e89b-12d3-a456-426655440000",
        "name": "My Branch",
        "description": "Stuff",
        "network_name": "My Network",
        "path": "/some/path",
        "hostname": "localhost",
        "pid": 3333,
        "advertising_interval": 7.5,
        "tcp_server_port": 11223,
        "start_time": "2018-04-23T18:25:43.511Z",
        "timeout": -1,
        "ghost_mode": False,
        "tx_queue_size": 2000,
        "rx_queue_size": 3000,

        # RemoteBranchInfo
        "tcp_server_address": "192.168.1.1",

        # LocalBranchInfo
        "advertising_address": "239.255.0.1",
        "advertising_port": 12345,
    })

    # Create the branch info object while mocking out the timestamp parse function
    ts = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_milliseconds(123))
    with patch.object(yogi.Timestamp, 'parse', return_value=ts) as mock_method:
        info = info_cls(info_string)
    mock_method.assert_called_once_with('2018-04-23T18:25:43.511Z')

    # Check the info object's properties
    assert str(info) == info_string
    assert isinstance(info.uuid, UUID)
    assert str(info.uuid) == '123e4567-e89b-12d3-a456-426655440000'

    if isinstance(info, yogi.BranchInfo):
        assert info.name == 'My Branch'
        assert info.description == 'Stuff'
        assert info.network_name == 'My Network'
        assert info.path == '/some/path'
        assert info.hostname == 'localhost'
        assert info.pid == 3333
        assert info.advertising_interval == 7.5
        assert info.tcp_server_port == 11223
        assert isinstance(info.start_time, yogi.Timestamp)
        assert info.start_time.milliseconds == 123
        assert info.timeout == float('inf')
        assert info.ghost_mode is False
        assert info.tx_queue_size == 2000
        assert info.rx_queue_size == 3000

    if isinstance(info, yogi.RemoteBranchInfo):
        assert info.tcp_server_address == '192.168.1.1'

    if isinstance(info, yogi.LocalBranchInfo):
        assert info.advertising_address == '239.255.0.1'
        assert info.advertising_port == 12345

    if isinstance(info, yogi.BranchDiscoveredEventInfo):
        assert info.tcp_server_address == '192.168.1.1'
        assert info.tcp_server_port == 11223


@pytest.mark.parametrize('cfg_type', ['Configuration', 'JSON'])
def test_create(cfg_type, mocks: Mocks, context: yogi.Context, configuration: yogi.Configuration, monkeypatch):
    """Verifies that Branch can be instantiated with a given configuration"""
    monkeypatch.setattr(yogi._branch, 'convert_info_fields', lambda x: None)

    create_called = False
    get_info_called = False
    configuration_create_called = False
    configuration_update_from_json_called = False

    def branch_create_fn(branch, context, config, section):
        assert branch
        assert context == 1234
        assert config == 2222
        assert section == b'bar'
        branch.contents.value = 8888
        nonlocal create_called
        create_called = True
        return yogi.ErrorCode.OK

    def branch_get_info_fn(branch, uuid, json, jsonsize):
        assert branch == 8888
        assert uuid is None
        assert json
        assert jsonsize >= 3
        memmove(json, b'{}\0', 3)
        nonlocal get_info_called
        get_info_called = True
        return yogi.ErrorCode.OK

    def configuration_create_fn(config, flags):
        config.contents.value = 2222
        nonlocal configuration_create_called
        configuration_create_called = True
        return yogi.ErrorCode.OK

    def configuration_update_from_json_fn(config, json):
        assert config == 2222
        assert json == b'{}'
        nonlocal configuration_update_from_json_called
        configuration_update_from_json_called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_BranchCreate(branch_create_fn)
    mocks.MOCK_BranchGetInfo(branch_get_info_fn)
    mocks.MOCK_ConfigurationCreate(configuration_create_fn)
    mocks.MOCK_ConfigurationUpdateFromJson(configuration_update_from_json_fn)

    if cfg_type == 'JSON':
        branch = yogi.Branch(context, '{}', 'bar')
        assert configuration_create_called
        assert configuration_update_from_json_called
    else:
        branch = yogi.Branch(context, configuration, 'bar')

    assert create_called
    assert get_info_called

    assert str(branch.info) == '{}'


def test_branch_info_properties(branch: yogi.Branch):
    """Verifies that the Branch class has all the properties that the LocalBranchInfo class has"""
    assert isinstance(branch.info, yogi.LocalBranchInfo)

    for attr in [x for x in dir(branch.info) if not x.startswith('_')]:
        assert getattr(branch, attr) == getattr(branch.info, attr)


