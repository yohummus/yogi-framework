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

import json
from uuid import UUID

from ._timestamp import Timestamp


class BranchInfo:
    """Information about a branch."""

    def __init__(self, info_string: str):
        self._info_string = info_string
        self._info = json.loads(info_string)
        convert_info_fields(self._info)

    def __str__(self) -> str:
        return self._info_string

    @property
    def uuid(self) -> UUID:
        """UUID of the branch."""
        return self._info["uuid"]

    @property
    def name(self) -> str:
        """Name of the branch."""
        return self._info["name"]

    @property
    def description(self) -> str:
        """Description of the branch."""
        return self._info["description"]

    @property
    def network_name(self) -> str:
        """Name of the network."""
        return self._info["network_name"]

    @property
    def path(self) -> str:
        """Path of the branch."""
        return self._info["path"]

    @property
    def hostname(self) -> str:
        """The machine's hostname."""
        return self._info["hostname"]

    @property
    def pid(self) -> int:
        """ID of the process."""
        return self._info["pid"]

    @property
    def advertising_interval(self) -> float:
        """Advertising interval."""
        return self._info["advertising_interval"]

    @property
    def tcp_server_port(self) -> int:
        """Listening port of the TCP server for incoming connections."""
        return self._info["tcp_server_port"]

    @property
    def start_time(self) -> Timestamp:
        """Time when the branch was started."""
        return self._info["start_time"]

    @property
    def timeout(self) -> float:
        """Connection timeout."""
        return self._info["timeout"]

    @property
    def ghost_mode(self) -> bool:
        """True if the branch is in ghost mode."""
        return self._info["ghost_mode"]

    @property
    def tx_queue_size(self) -> int:
        """Size of the send queue for remote branches."""
        return self._info["tx_queue_size"]

    @property
    def rx_queue_size(self) -> int:
        """Size of the receive queue for remote branches."""
        return self._info["rx_queue_size"]


class RemoteBranchInfo(BranchInfo):
    """Information about a remote branch."""

    def __init__(self, info_string: str):
        BranchInfo.__init__(self, info_string)

    @property
    def tcp_server_address(self) -> str:
        """Address of the TCP server for incoming connections."""
        return self._info["tcp_server_address"]


class LocalBranchInfo(BranchInfo):
    """Information about a local branch."""

    def __init__(self, info_string: str):
        BranchInfo.__init__(self, info_string)

    @property
    def advertising_address(self) -> str:
        """Advertising IP address."""
        return self._info["advertising_address"]

    @property
    def advertising_port(self) -> int:
        """Advertising port."""
        return self._info["advertising_port"]


class BranchEventInfo:
    """Information associated with a branch event."""

    def __init__(self, info_string: str):
        self._info_string = info_string
        self._info = json.loads(info_string)
        convert_info_fields(self._info)

    def __str__(self) -> str:
        return self._info_string

    @property
    def uuid(self) -> UUID:
        """UUID of the branch."""
        return self._info["uuid"]


class BranchDiscoveredEventInfo(BranchEventInfo):
    """Information associated with the BRANCH_DISCOVERED event."""

    def __init__(self, info_string: str):
        BranchEventInfo.__init__(self, info_string)

    @property
    def tcp_server_address(self) -> str:
        """Address of the TCP server for incoming connections."""
        return self._info["tcp_server_address"]

    @property
    def tcp_server_port(self) -> int:
        """Listening port of the TCP server for incoming connections."""
        return self._info["tcp_server_port"]


class BranchQueriedEventInfo(BranchEventInfo, RemoteBranchInfo):
    """Information associated with the BRANCH_QUERIED event."""

    def __init__(self, info_string: str):
        BranchEventInfo.__init__(self, info_string)
        # No need to call RemoteBranchInfo's constructor here


class ConnectFinishedEventInfo(BranchEventInfo):
    """Information associated with the CONNECT_FINISHED event."""

    def __init__(self, info_string: str):
        BranchEventInfo.__init__(self, info_string)


class ConnectionLostEventInfo(BranchEventInfo):
    """Information associated with the CONNECTION_LOST event."""

    def __init__(self, info_string: str):
        BranchEventInfo.__init__(self, info_string)


def convert_info_fields(info):
    if info["advertising_interval"] == -1:
        info["advertising_interval"] = float("inf")

    if info["timeout"] == -1:
        info["timeout"] = float("inf")

    info["uuid"] = UUID(info["uuid"])
    info["start_time"] = Timestamp.parse(info["start_time"])
