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
from uuid import UUID

from .conftest import Mocks


def make_branch_info_string(uuid: str = "123e4567-e89b-12d3-a456-426655440000") -> str:
    return json.dumps({
        # BranchInfo
        "uuid": uuid,
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


@pytest.mark.parametrize("info_cls", [yogi.BranchInfo, yogi.RemoteBranchInfo, yogi.LocalBranchInfo,
                                      yogi.BranchEventInfo, yogi.BranchDiscoveredEventInfo, yogi.BranchQueriedEventInfo,
                                      yogi.ConnectFinishedEventInfo, yogi.ConnectionLostEventInfo])
def test_branch_info(info_cls, mocker):
    """Verifies that the BranchInfo/BranchEventInfo and derived classes have the expected properties"""
    branch_info_string = make_branch_info_string()

    # Create the branch info object while mocking out the timestamp parse function
    ts = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_milliseconds(123))
    mock_parse = mocker.patch.object(yogi.Timestamp, "parse", return_value=ts)
    info = info_cls(branch_info_string)

    # Check the info object"s properties
    assert str(info) == branch_info_string
    assert isinstance(info.uuid, UUID)
    assert str(info.uuid) == "123e4567-e89b-12d3-a456-426655440000"

    if isinstance(info, yogi.BranchInfo):
        assert info.name == "My Branch"
        assert info.description == "Stuff"
        assert info.network_name == "My Network"
        assert info.path == "/some/path"
        assert info.hostname == "localhost"
        assert info.pid == 3333
        assert info.advertising_interval == 7.5
        assert info.tcp_server_port == 11223
        assert isinstance(info.start_time, yogi.Timestamp)
        assert info.start_time.milliseconds == 123
        assert info.timeout == float("inf")
        assert info.ghost_mode is False
        assert info.tx_queue_size == 2000
        assert info.rx_queue_size == 3000

    if isinstance(info, yogi.RemoteBranchInfo):
        assert info.tcp_server_address == "192.168.1.1"

    if isinstance(info, yogi.LocalBranchInfo):
        assert info.advertising_address == "239.255.0.1"
        assert info.advertising_port == 12345

    if isinstance(info, yogi.BranchDiscoveredEventInfo):
        assert info.tcp_server_address == "192.168.1.1"
        assert info.tcp_server_port == 11223

    mock_parse.assert_called_once_with("2018-04-23T18:25:43.511Z")


@pytest.mark.parametrize("cfg_type", ["Configuration", "JSON"])
def test_create(cfg_type, mocks: Mocks, context: yogi.Context, configuration: yogi.Configuration, monkeypatch):
    """Verifies that Branch can be instantiated with a given configuration"""
    create_called = False
    get_info_called = False
    configuration_create_called = False
    configuration_update_from_json_called = False

    def branch_create_fn(branch, context, config, section):
        assert branch
        assert context == 1234
        assert config == 2222
        assert section == b"bar"
        branch.contents.value = 8888
        nonlocal create_called
        create_called = True
        return yogi.ErrorCode.OK

    def branch_get_info_fn(branch, uuid, json, jsonsize):
        assert branch == 8888
        assert uuid is None
        assert json
        assert jsonsize >= 3
        memmove(json, b"{}\0", 3)
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
        assert json == b"{}"
        nonlocal configuration_update_from_json_called
        configuration_update_from_json_called = True
        return yogi.ErrorCode.OK

    mocks.MOCK_BranchCreate(branch_create_fn)
    mocks.MOCK_BranchGetInfo(branch_get_info_fn)
    mocks.MOCK_ConfigurationCreate(configuration_create_fn)
    mocks.MOCK_ConfigurationUpdateFromJson(configuration_update_from_json_fn)

    if cfg_type == "JSON":
        branch = yogi.Branch(context, "{}", "bar")
        assert configuration_create_called
        assert configuration_update_from_json_called
    else:
        branch = yogi.Branch(context, configuration, "bar")

    assert create_called
    assert get_info_called

    assert str(branch.info) == "{}"


def test_branch_info_properties(branch: yogi.Branch, mocker):
    """Verifies that the Branch class has all the properties that the LocalBranchInfo class has"""
    ts = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_milliseconds(123))
    mock_parse = mocker.patch.object(yogi.Timestamp, "parse", return_value=ts)
    assert isinstance(branch.info, yogi.LocalBranchInfo)

    for attr in [x for x in dir(branch.info) if not x.startswith("_")]:
        assert getattr(branch, attr) == getattr(branch.info, attr)

    mock_parse.assert_called_once_with("2018-04-23T18:25:43.511Z")


def test_get_connected_branches(mocks: Mocks, branch: yogi.Branch, mocker):
    """Verifies that the Branch class has a method for listing all remote branches that it is currently connected to"""
    branch_info_bytes_1 = make_branch_info_string("11111111-e89b-12d3-a456-426655440000").encode() + b"\0"
    branch_info_bytes_2 = make_branch_info_string("22222222-e89b-12d3-a456-426655440000").encode() + b"\0"

    trigger_error = True

    def fn(branch, uuid, json, jsonsize, handler_fn, userarg):
        assert branch == 8888
        assert uuid is None
        assert json
        assert jsonsize >= len(branch_info_bytes_1)
        memmove(json, branch_info_bytes_1, len(branch_info_bytes_1))
        handler_fn(yogi.ErrorCode.OK, userarg)

        nonlocal trigger_error
        if trigger_error:
            trigger_error = False  # No error next time
            handler_fn(yogi.ErrorCode.BUFFER_TOO_SMALL, userarg)
            return yogi.ErrorCode.BUFFER_TOO_SMALL
        else:
            memmove(json, branch_info_bytes_2, len(branch_info_bytes_2))
            handler_fn(yogi.ErrorCode.OK, userarg)
            return yogi.ErrorCode.OK

    mocks.MOCK_BranchGetConnectedBranches(fn)

    ts = yogi.Timestamp.from_duration_since_epoch(yogi.Duration.from_milliseconds(123))
    mocker.patch.object(yogi.Timestamp, "parse", return_value=ts)
    infos = branch.get_connected_branches()

    assert set([str(x).split("-")[0] for x in infos.keys()]) == {"11111111", "22222222"}
    assert list(infos.values())[0].name == "My Branch"


def test_await_event_async(mocks: Mocks, branch: yogi.Branch):
    """Verifies that the Branch class has a method to wait for branch events"""
    branch_info_bytes = make_branch_info_string().encode() + b"\0"
    called = False

    def fn(branch, events, uuid, json, jsonsize, handler_fn, userarg):
        assert branch == 8888
        assert events == yogi.BranchEvents.BRANCH_QUERIED | yogi.BranchEvents.BRANCH_DISCOVERED
        assert uuid is None
        assert json
        assert jsonsize >= len(branch_info_bytes)
        memmove(json, branch_info_bytes, len(branch_info_bytes))
        assert handler_fn
        handler_fn(yogi.ErrorCode.OK, yogi.BranchEvents.BRANCH_DISCOVERED, yogi.ErrorCode.BUSY, userarg)
        return yogi.ErrorCode.OK

    def callback_fn(res, ev, evres, info):
        assert isinstance(res, yogi.Result)
        assert res == yogi.Success()
        assert isinstance(ev, yogi.BranchEvents)
        assert ev == yogi.BranchEvents.BRANCH_DISCOVERED
        assert isinstance(evres, yogi.Result)
        assert evres.error_code == yogi.ErrorCode.BUSY
        assert isinstance(info, yogi.BranchDiscoveredEventInfo)
        nonlocal called
        called = True

    mocks.MOCK_BranchAwaitEventAsync(fn)
    branch.await_event_async(yogi.BranchEvents.BRANCH_QUERIED | yogi.BranchEvents.BRANCH_DISCOVERED, callback_fn)
    assert called


def test_cancel_await_event(mocks: Mocks, branch: yogi.Branch):
    """Verifies that a wait for branch events operation can be cancelled"""
    def fn(branch):
        assert branch == 8888
        return yogi.ErrorCode.OK

    mocks.MOCK_BranchCancelAwaitEvent(fn)
    assert branch.cancel_await_event()

    def fn2(branch):
        assert branch == 8888
        return yogi.ErrorCode.OPERATION_NOT_RUNNING

    mocks.MOCK_BranchCancelAwaitEvent(fn2)
    assert not branch.cancel_await_event()


def test_cancel_receive_broadcast(mocks: Mocks, branch: yogi.Branch):
    """Verifies that a receive broadcast operation can be cancelled"""
    def fn(branch):
        assert branch == 8888
        return yogi.ErrorCode.OK

    mocks.MOCK_BranchCancelReceiveBroadcast(fn)
    assert branch.cancel_receive_broadcast()

    def fn2(branch):
        assert branch == 8888
        return yogi.ErrorCode.OPERATION_NOT_RUNNING

    mocks.MOCK_BranchCancelReceiveBroadcast(fn2)
    assert not branch.cancel_receive_broadcast()
