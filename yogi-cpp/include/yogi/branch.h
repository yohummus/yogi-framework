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

#ifndef _YOGI_BRANCH_H
#define _YOGI_BRANCH_H

//! \file
//!
//! Branches (entry points into the Yogi network).

#include "buffer.h"
#include "configuration.h"
#include "constants.h"
#include "context.h"
#include "detail/query_string.h"
#include "duration.h"
#include "io.h"
#include "json.h"
#include "object.h"
#include "operation_id.h"
#include "payload_view.h"
#include "string_view.h"
#include "timestamp.h"
#include "uuid.h"

#include <unordered_map>

namespace yogi {

////////////////////////////////////////////////////////////////////////////////
/// Information about about a branch.
////////////////////////////////////////////////////////////////////////////////
class BranchInfo {
 public:
  virtual ~BranchInfo() {
  }

  /// Returns the UUID of the branch.
  ///
  /// \returns UUID of the branch.
  const Uuid& uuid() const {
    return uuid_;
  }

  /// Returns the name of the branch.
  ///
  /// \returns The name of the branch.
  std::string name() const {
    return json_["name"];
  }

  /// Returns the description of the branch.
  ///
  /// \returns The description of the branch.
  std::string description() const {
    return json_["description"];
  }

  /// Returns the name of the network.
  ///
  /// \returns The name of the network.
  std::string network_name() const {
    return json_["network_name"];
  }

  /// Returns the  path of the branch.
  ///
  /// \returns The  path of the branch.
  std::string path() const {
    return json_["path"];
  }

  /// Returns the machine's hostname.
  ///
  /// \returns The machine's hostname.
  std::string hostname() const {
    return json_["hostname"];
  }

  /// Returns the ID of the process.
  ///
  /// \returns The ID of the process.
  int pid() const {
    return json_["pid"];
  }

  /// Returns the advertising interval.
  ///
  /// \returns The advertising interval.
  Duration advertising_interval() const {
    float val = json_["advertising_interval"];
    return val < 0 ? Duration::kInf : Duration::from_seconds(val);
  }

  /// Returns the listening port of the TCP server for incoming connections.
  ///
  /// \returns The listening port of the TCP server for incoming connections.
  int tcp_server_port() const {
    return json_["tcp_server_port"];
  }

  /// Returns the time when the branch was started (UTC time).
  ///
  /// \returns The time when the branch was started (UTC time).
  Timestamp start_time() const {
    return Timestamp::parse(static_cast<const std::string&>(json_["start_time"]));
  }

  /// Returns the connection timeout.
  ///
  /// \returns The connection timeout.
  Duration timeout() const {
    float val = json_["timeout"];
    return val < 0 ? Duration::kInf : Duration::from_seconds(val);
  }

  /// Returns true if the branch is in ghost mode.
  ///
  /// \return True if the branch is in ghost mode.
  bool ghost_mode() const {
    return json_["ghost_mode"];
  }

  /// Returns the branch information as JSON-encoded string.
  ///
  /// \returns Branch information as JSON-encoded string.
  const std::string& to_string() const {
    return json_str_;
  }

  /// Returns the branch information as a JSON object.
  ///
  /// \return Branch information as a JSON object.
  const Json& to_json() const {
    return json_;
  }

 protected:
  BranchInfo(const Uuid& uuid, std::string&& json_str) : BranchInfo(uuid, Json::parse(json_str), json_str) {
  }

  BranchInfo(const Uuid& uuid, Json json, std::string json_str) : uuid_(uuid), json_(json), json_str_(json_str) {
  }

 private:
  const Uuid uuid_;
  const Json json_;
  const std::string json_str_;
};

////////////////////////////////////////////////////////////////////////////////
/// Information about a remote branch.
////////////////////////////////////////////////////////////////////////////////
class RemoteBranchInfo : public BranchInfo {
  friend class BranchQueriedEventInfo;
  friend class Branch;

 public:
  /// Returns the address of the TCP server for incoming connections.
  ///
  /// \returns The address of the TCP server for incoming connections.
  std::string tcp_server_address() const {
    return to_json()["tcp_server_address"];
  }

 protected:
  using BranchInfo::BranchInfo;
};

////////////////////////////////////////////////////////////////////////////////
/// Information about a local branch.
////////////////////////////////////////////////////////////////////////////////
class LocalBranchInfo : public BranchInfo {
  friend class Branch;

 public:
  /// Advertising IP address.
  ///
  /// \returns The advertising IP address.
  std::string advertising_address() const {
    return to_json()["advertising_address"];
  }

  /// Advertising port.
  ///
  /// \returns The advertising port.
  int advertising_port() const {
    return to_json()["advertising_port"];
  }

  /// Size of the send queues for remote branches.
  ///
  /// \returns The size of the send queues.
  int tx_queue_size() const {
    return to_json()["tx_queue_size"];
  }

  /// Size of the receive queues for remote branches.
  ///
  /// \returns The size of the receive queues.
  int rx_queue_size() const {
    return to_json()["rx_queue_size"];
  }

 protected:
  LocalBranchInfo(const Uuid& uuid, std::string&& json_str) : BranchInfo(uuid, std::move(json_str)) {
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Information associated with a branch event.
////////////////////////////////////////////////////////////////////////////////
class BranchEventInfo {
  friend class Branch;

 public:
  virtual ~BranchEventInfo() {
  }

  /// Returns the UUID of the branch.
  ///
  /// \returns UUID of the branch.
  const Uuid& uuid() const {
    return uuid_;
  }

  /// Returns the event information as JSON-encoded string.
  ///
  /// \returns Event information as JSON-encoded string.
  const std::string& to_string() const {
    return json_str_;
  }

  /// Returns the event information as a JSON object.
  ///
  /// \return Event information as a JSON object.
  const Json& to_json() const {
    return json_;
  }

 protected:
  BranchEventInfo() : BranchEventInfo({}, "{}") {
  }

  BranchEventInfo(const Uuid& uuid, std::string&& json_str)
      : uuid_(uuid), json_(Json::parse(json_str)), json_str_(std::move(json_str)) {
  }

 private:
  const Uuid uuid_;
  const Json json_;
  const std::string json_str_;
};

////////////////////////////////////////////////////////////////////////////////
/// Information associated with the kBranchDiscovered event.
////////////////////////////////////////////////////////////////////////////////
class BranchDiscoveredEventInfo : public BranchEventInfo {
  friend class Branch;

 public:
  /// Returns the address of the TCP server for incoming connections.
  ///
  /// \returns The address of the TCP server for incoming connections.
  std::string tcp_server_address() const {
    return to_json()["tcp_server_address"];
  }

  /// Returns the listening port of the TCP server for incoming connections.
  ///
  /// \returns The listening port of the TCP server for incoming connections.
  int tcp_server_port() const {
    return to_json()["tcp_server_port"];
  }

 protected:
  BranchDiscoveredEventInfo(const Uuid& uuid, std::string&& json_str) : BranchEventInfo(uuid, std::move(json_str)) {
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Information associated with the kBranchQueried event.
////////////////////////////////////////////////////////////////////////////////
class BranchQueriedEventInfo : public BranchEventInfo {
  friend class Branch;

 public:
  /// Returns the name of the branch.
  ///
  /// \returns The name of the branch.
  std::string name() const {
    return to_json()["name"];
  }

  /// Returns the description of the branch.
  ///
  /// \returns The description of the branch.
  std::string description() const {
    return to_json()["description"];
  }

  /// Returns the name of the network.
  ///
  /// \returns The name of the network.
  std::string network_name() const {
    return to_json()["network_name"];
  }

  /// Returns the  path of the branch.
  ///
  /// \returns The  path of the branch.
  std::string path() const {
    return to_json()["path"];
  }

  /// Returns the machine's hostname.
  ///
  /// \returns The machine's hostname.
  std::string hostname() const {
    return to_json()["hostname"];
  }

  /// Returns the ID of the process.
  ///
  /// \returns The ID of the process.
  int pid() const {
    return to_json()["pid"];
  }

  /// Returns the advertising interval.
  ///
  /// \returns The advertising interval.
  Duration advertising_interval() const {
    float val = to_json()["advertising_interval"];
    return val < 0 ? Duration::kInf : Duration::from_seconds(val);
  }

  /// Returns the address of the TCP server for incoming connections.
  ///
  /// \returns The address of the TCP server for incoming connections.
  std::string tcp_server_address() const {
    return to_json()["tcp_server_address"];
  }

  /// Returns the listening port of the TCP server for incoming connections.
  ///
  /// \returns The listening port of the TCP server for incoming connections.
  int tcp_server_port() const {
    return to_json()["tcp_server_port"];
  }

  /// Returns the time when the branch was started (UTC time).
  ///
  /// \returns The time when the branch was started (UTC time).
  Timestamp start_time() const {
    return Timestamp::parse(static_cast<const std::string&>(to_json()["start_time"]));
  }

  /// Returns the connection timeout.
  ///
  /// \returns The connection timeout.
  Duration timeout() const {
    float val = to_json()["timeout"];
    return val < 0 ? Duration::kInf : Duration::from_seconds(val);
  }

  /// Returns true if the branch is in ghost mode.
  ///
  /// \return True if the branch is in ghost mode.
  bool ghost_mode() const {
    return to_json()["ghost_mode"];
  }

  /// Converts the event information to a RemoteBranchInfo object.
  ///
  /// \returns The converted RemoteBranchInfo object.
  RemoteBranchInfo to_remote_branch_info() const {
    return RemoteBranchInfo(uuid(), to_json(), to_string());
  }

 protected:
  BranchQueriedEventInfo(const Uuid& uuid, std::string&& json_str) : BranchEventInfo(uuid, std::move(json_str)) {
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Information associated with the kConnectFinished event.
////////////////////////////////////////////////////////////////////////////////
class ConnectFinishedEventInfo : public BranchEventInfo {
  friend class Branch;

 protected:
  using BranchEventInfo::BranchEventInfo;
};

////////////////////////////////////////////////////////////////////////////////
/// Information associated with the kConnectionLost event.
////////////////////////////////////////////////////////////////////////////////
class ConnectionLostEventInfo : public BranchEventInfo {
  friend class Branch;

 protected:
  using BranchEventInfo::BranchEventInfo;
};

class Branch;

/// Shared pointer to a branch.
using BranchPtr = std::shared_ptr<Branch>;

////////////////////////////////////////////////////////////////////////////////
/// Entry point into a Yogi network.
///
/// A branch represents an entry point into a Yogi network. It advertises
/// itself via IP broadcasts/multicasts with its unique ID and information
/// required for establishing a connection. If a branch detects other branches
/// on the network, it connects to them via TCP to retrieve further
/// information such as their name, description and network name. If the
/// network names match, two branches attempt to authenticate with each other
/// by securely comparing passwords. Once authentication succeeds and there is
/// no other known branch with the same path then the branches can actively
/// communicate as part of the Yogi network.
///
/// \note
///   Even if the communication between branches is not encrypted, the
///   authentication process via passwords is always done in a secure manner.
////////////////////////////////////////////////////////////////////////////////
class Branch : public ObjectT<Branch> {
 public:
  /// Callback function used in await_event_async().
  ///
  /// \param res    %Result of the wait operation.
  /// \param event  The branch event that occurred.
  /// \param ev_res %Result associated with the event.
  /// \param info   Event information.
  using AwaitEventFn =
      std::function<void(const Result& res, BranchEvents event, const Result& ev_res, BranchEventInfo& info)>;

  /// Callback function used in send_broadcast_async().
  ///
  /// \param res %Result of the send operation.
  /// \param oid ID of the send operation.
  using SendBroadcastFn = std::function<void(const Result& res, OperationId oid)>;

  /// Callback function used in receive_broadcast_async().
  ///
  /// \param res     %Result of the receive operation.
  /// \param source  UUID of the sending branch.
  /// \param payload View on the received payload.
  /// \param buffer  Buffer holding the payload.
  using ReceiveBroadcastFn =
      std::function<void(const Result& res, const Uuid& source, const PayloadView& payload, BufferPtr&& buffer)>;

  /// Callback function used in receive_broadcast_async().
  ///
  /// As opposed to ReceiveBroadcastFn, this function does not include the
  /// buffer parameter.
  ///
  /// \param res     %Result of the receive operation.
  /// \param source  UUID of the sending branch.
  /// \param payload View on the received payload.
  using ReceiveBroadcastSimpleFn =
      std::function<void(const Result& res, const Uuid& source, const PayloadView& payload)>;

  /// Creates a branch.
  ///
  /// The branch is configured via the \p config parameter. The configuration
  /// object will only be used while constructing the branch, i.e. the branch
  /// will not keep any references to it. The supplied configuration must have
  /// the following structure:
  ///
  /// \code
  ///   {
  ///     "name":                 "Fan Controller",
  ///     "description":          "Controls a fan via PWM",
  ///     "path":                 "/Cooling System/Fan Controller",
  ///     "network_name":         "Hardware Control",
  ///     "network_password":     "secret",
  ///     "advertising_interfaces": ["localhost"],
  ///     "advertising_address":  "ff02::8000:2439",
  ///     "advertising_port":     13531,
  ///     "advertising_interval": 1.0,
  ///     "timeout":              3.0,
  ///     "ghost_mode":           false,
  ///     "tx_queue_size":        1000000,
  ///     "rx_queue_size":        100000
  ///   }
  /// \endcode
  ///
  /// All of the properties are optional and if unspecified (or set to _null_),
  /// their respective default values will be used (see \ref constants). The
  /// properties have the following meaning:
  ///  - __name__: Name of the branch (default: PID\@hostname without the
  ///    backslash).
  ///  - __description__: Description of the branch.
  ///  - __path__: Path of the branch in the network (default: /name where name
  ///    is the name of the branch). Must start with a slash.
  ///  - __network_name__: Name of the network to join (default: the machine's
  ///    hostname).
  ///  - __network_password__: Password for the network (default: no password).
  ///  - __advertising_interfaces__: Network interfaces to use for advertising
  ///    and for branch connections. Valid strings are Unix device names
  ///    ("eth0", "en5", "wlan0"), adapter names on Windows ("Ethernet",
  ///     "VMware Network Adapter WMnet1") or MAC addresses
  ///     ("11:22:33:44:55:66"). Furthermore, the special strings "localhost"
  ///     and "all" can be used to denote loopback and all available interfaces
  ///     respectively.
  ///  - __advertising_address__: Multicast address to use for advertising, e.g.
  ///    239.255.0.1 for IPv4 or ff02::8000:1234 for IPv6.
  ///  - __advertising_port__: Port to use for advertising.
  ///  - __advertising_interval__: Time between advertising messages. Must be at
  ///    least 1 ms.
  ///  - __timeout__: Amount of time of inactivity before a connection is
  ///    considered to be broken. Must be at least 1 ms.
  ///  - __ghost_mode__: Set to true to activate ghost mode (default: false).
  ///  - __tx_queue_size__: Size of the send queues for remote branches.
  ///  - __rx_queue_size__: Size of the receive queues for remote branches.
  ///
  /// Advertising and establishing connections can be limited to certain network
  /// interfaces via the _interface_ property. The default is to use all
  /// available interfaces.
  ///
  /// Setting the _ghost_mode_ property to _true_ prevents the branch from
  /// actively participating in the Yogi network, i.e. the branch will not
  /// advertise itself and it will not authenticate in order to join a network.
  /// However, the branch will temporarily connect to other branches in order to
  /// obtain more detailed information such as name, description, network name
  /// and so on. This is useful for obtaining information about active branches
  /// without actually becoming part of the Yogi network.
  ///
  /// \attention
  ///   The _tx_queue_size_ and _rx_queue_size_ properties affect every branch
  ///   connection and can therefore consume a large amount of memory. For
  ///   example, in a network of 10 branches where these properties are set to
  ///   1 MB, the resulting memory used for the queues would be
  ///   10 x 2 x 1 MB = 20 MB for each of the 10 branches. This value grows with
  ///   the number of branches squared.
  ///
  /// \param context %Context to use
  /// \param config  %Branch properties
  /// \param section Section in \p config to use instead of the root section;
  ///                syntax is JSON pointer (RFC 6901)
  ///
  /// \returns The created branch.
  static BranchPtr create(ContextPtr context, const ConfigurationPtr& config = {}, const StringView& section = {}) {
    return BranchPtr(new Branch(context, config, section));
  }

  /// Creates a branch.
  ///
  /// The branch is configured via the \p json parameter. The supplied JSON must
  /// have the following structure:
  ///
  /// \code
  ///   {
  ///     "name":                 "Fan Controller",
  ///     "description":          "Controls a fan via PWM",
  ///     "path":                 "/Cooling System/Fan Controller",
  ///     "network_name":         "Hardware Control",
  ///     "network_password":     "secret",
  ///     "advertising_interfaces": ["localhost"],
  ///     "advertising_address":  "ff02::8000:2439",
  ///     "advertising_port":     13531,
  ///     "advertising_interval": 1.0,
  ///     "timeout":              3.0,
  ///     "ghost_mode":           false,
  ///     "tx_queue_size":        1000000,
  ///     "rx_queue_size":        100000
  ///   }
  /// \endcode
  ///
  /// All of the properties are optional and if unspecified (or set to _null_),
  /// their respective default values will be used (see \ref constants). The
  /// properties have the following meaning:
  ///  - __name__: Name of the branch (default: PID\@hostname without the
  ///    backslash).
  ///  - __description__: Description of the branch.
  ///  - __path__: Path of the branch in the network (default: /name where name
  ///    is the name of the branch). Must start with a slash.
  ///  - __network_name__: Name of the network to join (default: the machine's
  ///    hostname).
  ///  - __network_password__: Password for the network (default: no password).
  ///  - __advertising_interfaces__: Network interfaces to use for advertising
  ///    and for branch connections. Valid strings are Unix device names
  ///    ("eth0", "en5", "wlan0"), adapter names on Windows ("Ethernet",
  ///     "VMware Network Adapter WMnet1") or MAC addresses
  ///     ("11:22:33:44:55:66"). Furthermore, the special strings "localhost"
  ///     and "all" can be used to denote loopback and all available interfaces
  ///     respectively.
  ///  - __advertising_address__: Multicast address to use for advertising, e.g.
  ///    239.255.0.1 for IPv4 or ff02::8000:1234 for IPv6.
  ///  - __advertising_port__: Port to use for advertising.
  ///  - __advertising_interval__: Time between advertising messages. Must be at
  ///    least 1 ms.
  ///  - __timeout__: Amount of time of inactivity before a connection is
  ///    considered to be broken. Must be at least 1 ms.
  ///  - __ghost_mode__: Set to true to activate ghost mode (default: false).
  ///  - __tx_queue_size__: Size of the send queues for remote branches.
  ///  - __rx_queue_size__: Size of the receive queues for remote branches.
  ///
  /// Advertising and establishing connections can be limited to certain network
  /// interfaces via the _interface_ property. The default is to use all
  /// available interfaces.
  ///
  /// Setting the _ghost_mode_ property to _true_ prevents the branch from
  /// actively participating in the Yogi network, i.e. the branch will not
  /// advertise itself and it will not authenticate in order to join a network.
  /// However, the branch will temporarily connect to other branches in order to
  /// obtain more detailed information such as name, description, network name
  /// and so on. This is useful for obtaining information about active branches
  /// without actually becoming part of the Yogi network.
  ///
  /// \attention
  ///   The _tx_queue_size_ and _rx_queue_size_ properties affect every branch
  ///   connection and can therefore consume a large amount of memory. For
  ///   example, in a network of 10 branches where these properties are set to
  ///   1 MB, the resulting memory used for the queues would be
  ///   10 x 2 x 1 MB = 20 MB for each of the 10 branches. This value grows with
  ///   the number of branches squared.
  ///
  /// \param context %Context to use
  /// \param json    %Branch properties
  /// \param section Section in \p json to use instead of the root section;
  ///                syntax is JSON pointer (RFC 6901)
  ///
  /// \returns The created branch.
  static BranchPtr create(ContextPtr context, const JsonView& json, const StringView& section = {}) {
    auto config = Configuration::create(json, yogi::ConfigurationFlags::kDisableVariables);
    return create(context, config, section);
  }

  /// Returns information about the local branch.
  ///
  /// \returns Information about the local branch.
  const LocalBranchInfo& info() const {
    return info_;
  }

  /// Returns the UUID of the branch.
  ///
  /// \returns UUID of the branch.
  const Uuid& uuid() const {
    return info_.uuid();
  }

  /// Returns the name of the branch.
  ///
  /// \returns The name of the branch.
  std::string name() const {
    return info_.name();
  }

  /// Returns the description of the branch.
  ///
  /// \returns The description of the branch.
  std::string description() const {
    return info_.description();
  }

  /// Returns the name of the network.
  ///
  /// \returns The name of the network.
  std::string network_name() const {
    return info_.network_name();
  }

  /// Returns the  path of the branch.
  ///
  /// \returns The  path of the branch.
  std::string path() const {
    return info_.path();
  }

  /// Returns the machine's hostname.
  ///
  /// \returns The machine's hostname.
  std::string hostname() const {
    return info_.hostname();
  }

  /// Returns the ID of the process.
  ///
  /// \returns The ID of the process.
  int pid() const {
    return info_.pid();
  }

  /// Returns the advertising interval.
  ///
  /// \returns The advertising interval.
  Duration advertising_interval() const {
    return info_.advertising_interval();
  }

  /// Returns the listening port of the TCP server for incoming connections.
  ///
  /// \returns The listening port of the TCP server for incoming connections.
  int tcp_server_port() const {
    return info_.tcp_server_port();
  }

  /// Returns the time when the branch was started (UTC time).
  ///
  /// \returns The time when the branch was started (UTC time).
  Timestamp start_time() const {
    return info_.start_time();
  }

  /// Returns the connection timeout.
  ///
  /// \returns The connection timeout.
  Duration timeout() const {
    return info_.timeout();
  }

  /// Returns true if the branch is in ghost mode.
  ///
  /// \return True if the branch is in ghost mode.
  bool ghost_mode() const {
    return info_.ghost_mode();
  }

  /// Advertising IP address.
  ///
  /// \returns The advertising IP address.
  std::string advertising_address() const {
    return info_.advertising_address();
  }

  /// Advertising port.
  ///
  /// \returns The advertising port.
  int advertising_port() const {
    return info_.advertising_port();
  }

  /// Size of the send queues for remote branches.
  ///
  /// \returns The size of the send queues.
  int tx_queue_size() const {
    return info_.tx_queue_size();
  }

  /// Size of the receive queues for remote branches.
  ///
  /// \returns The size of the receive queues.
  int rx_queue_size() const {
    return info_.rx_queue_size();
  }

  /// Retrieves information about all connected remote branches.
  ///
  /// \returns A map mapping the UUID of each connected remote branch to a
  ///          RemoteBranchInfo object with detailed information about the
  ///          branch.
  std::unordered_map<Uuid, RemoteBranchInfo> get_connected_branches() const {
    struct CallbackData {
      Uuid uuid;
      const char* str;
      std::unordered_map<Uuid, RemoteBranchInfo> branches;
    } data;

    detail::query_string([&](auto str, auto size) {
      data.str = str;
      data.branches.clear();

      return detail::YOGI_BranchGetConnectedBranches(
          this->handle(), &data.uuid, str, size,
          [](int res, void* userarg) {
            if (res == static_cast<int>(ErrorCode::kOk)) {
              auto data = static_cast<CallbackData*>(userarg);
              data->branches.emplace(std::make_pair(data->uuid, RemoteBranchInfo(data->uuid, data->str)));
            }
          },
          &data);
    });

    return data.branches;
  }

  /// Waits for a branch event to occur.
  ///
  /// This function will register the handler fn to be executed once one of the
  /// given branch events occurs. If this function is called while a previous
  /// wait operation is still active then the previous opertion will be
  /// canceled, i.e. the handler \p fn for the previous operation will be called
  /// with a cancellation error.
  ///
  /// If successful, the event information passed to the handler function \p fn
  /// contains at least the UUID of the remote branch.
  ///
  /// In case that the internal buffer for reading the event information is too
  /// small, fn will be called with the corresponding error and the event
  /// information is lost. You can set the size of this buffer via the
  /// \p buffer_size parameter.
  ///
  /// \param events      Events to observe.
  /// \param fn          Handler function to call.
  /// \param buffer_size Size of the internal buffer for reading event
  ///                    information.
  void await_event_async(BranchEvents events, AwaitEventFn fn, int buffer_size = 1024) {
    struct CallbackData {
      AwaitEventFn fn;
      Uuid uuid;
      std::vector<char> json;
    };

    auto data = std::make_unique<CallbackData>();
    data->fn  = fn;
    data->json.resize(static_cast<std::size_t>(buffer_size));

    int res = detail::YOGI_BranchAwaitEventAsync(
        handle(), static_cast<int>(events), &data->uuid, data->json.data(), buffer_size,
        [](int res, int event, int ev_res, void* userarg) {
          auto data = std::unique_ptr<CallbackData>(static_cast<CallbackData*>(userarg));
          if (!data->fn) return;

          auto be = static_cast<BranchEvents>(event);

          if (Result(res) && be != BranchEvents::kNone) {
            switch (be) {
              case BranchEvents::kBranchDiscovered:
                call_await_event_fn<BranchDiscoveredEventInfo>(res, be, ev_res, data);
                break;

              case BranchEvents::kBranchQueried:
                call_await_event_fn<BranchQueriedEventInfo>(res, be, ev_res, data);
                break;

              case BranchEvents::kConnectFinished:
                call_await_event_fn<ConnectFinishedEventInfo>(res, be, ev_res, data);
                break;

              case BranchEvents::kConnectionLost:
                call_await_event_fn<ConnectionLostEventInfo>(res, be, ev_res, data);
                break;

              default: {
                bool should_never_get_here = false;
                assert(should_never_get_here);
              }
            }
          } else {
            call_await_event_fn<BranchEventInfo>(res, be, ev_res, data);
          }
        },
        data.get());

    detail::check_error_code(res);
    data.release();
  }

  /// Cancels waiting for a branch event.
  ///
  /// Calling this function will cause the handler registered via
  /// await_event_async() to be called with a cancellation error.
  ///
  /// \return True if the wait operation was cancelled successfully.
  bool cancel_await_event() {
    int res = detail::YOGI_BranchCancelAwaitEvent(handle());
    return detail::false_if_specific_error_else_throw(res, ErrorCode::kOperationNotRunning);
  }

  /// Sends a broadcast message to all connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// Setting the \p block parameter to _false_ will cause the function to
  /// skip sending the message to branches that have a full send queue. If at
  /// least one branch was skipped, the function will return _false_. If the
  /// parameter is set to _true_ instead, the function will block until the
  /// message has been put into the send queues of all connected branches.
  ///
  /// \attention
  ///   Calling this function from within a handler function executed through
  ///   the branch's _context_  with \p block set to _true_ will cause a
  ///   dead-lock if any send queue is full!
  ///
  /// \param payload Payload to send.
  /// \param block   Block until message has been put into all send buffers.
  ///
  /// \return _true_ if the message was successfully put into all send buffers.
  bool send_broadcast(const PayloadView& payload, bool block = true) {
    int res = detail::YOGI_BranchSendBroadcast(handle(), static_cast<int>(payload.encoding()), payload.data(),
                                               payload.size(), block ? 1 : 0);

    if (res == static_cast<int>(ErrorCode::kTxQueueFull)) {
      return false;
    } else {
      detail::check_error_code(res);
      return true;
    }
  }

  /// Sends a broadcast message to all connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// Setting the \p retry parameter to false will cause the function to skip
  /// sending the message to branches that have a full send queue. If at least
  /// one branch was skipped, the handler \p fn will be called with the
  /// #kTxQueueFull error. If the parameter is set to true instead, \p fn will
  /// be called once the message has been put into the send queues of all
  /// connected branches.
  ///
  /// The function returns an ID which uniquely identifies this send operation
  /// until \p fn has been called. It can be used in a subsequent
  /// cancel_send_broadcast() call to abort the operation.
  ///
  /// \note
  ///   The payload will be copied if necessary, i.e. \p payload only needs to
  ///   remain valid until the function returns.
  ///
  /// \param payload Payload to send.
  /// \param retry   Retry sending the message if a send queue is full.
  /// \param fn      Handler to call once the operation finishes.
  ///
  /// \return ID of the send operation.
  OperationId send_broadcast_async(const PayloadView& payload, bool retry, SendBroadcastFn fn) {
    struct CallbackData {
      SendBroadcastFn fn;
    };

    auto data = std::make_unique<CallbackData>();
    data->fn  = fn;

    int res = detail::YOGI_BranchSendBroadcastAsync(
        handle(), static_cast<int>(payload.encoding()), payload.data(), payload.size(), retry ? 1 : 0,
        [](int res, int oid, void* userarg) {
          auto data = std::unique_ptr<CallbackData>(static_cast<CallbackData*>(userarg));
          if (!data->fn) return;

          detail::with_error_code_to_result(res, data->fn, detail::make_operation_id(oid));
        },
        data.get());

    detail::check_error_code(res);
    data.release();

    return detail::make_operation_id(res);
  }

  /// Sends a broadcast message to all connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// The handler function \p fn will be called once the message has been put
  /// into the send queues of all connected branches.
  ///
  /// The function returns an ID which uniquely identifies this send operation
  /// until \p fn has been called. It can be used in a subsequent
  /// cancel_send_broadcast() call to abort the operation.
  ///
  /// \note
  ///   The payload will be copied if necessary, i.e. \p payload only needs to
  ///   remain valid until the function returns.
  ///
  /// \param payload Payload to send.
  /// \param fn      Handler to call once the operation finishes.
  ///
  /// \return ID of the send operation.
  OperationId send_broadcast_async(const PayloadView& payload, SendBroadcastFn fn) {
    return send_broadcast_async(payload, true, fn);
  }

  /// Cancels a send broadcast operation.
  ///
  /// Calling this function will cause the send operation with the specified
  /// operation ID \p oid to be canceled, resulting in the handler function
  /// registered via the send_broadcast_async() call that returned the same \p oid
  /// to be called with the #kCanceled error.
  ///
  /// \note
  ///   If the send operation has already been carried out but the handler
  ///   function has not been called yet, then cancelling the operation will
  ///   fail and _false_ will be returned.
  ///
  /// \param oid ID of the send operation.
  ///
  /// \returns True if the operation has been canceled successfully.
  bool cancel_send_broadcast(OperationId oid) {
    int res = detail::YOGI_BranchCancelSendBroadcast(handle(), oid.value());
    return detail::false_if_specific_error_else_throw(res, ErrorCode::kInvalidOperationId);
  }

  /// Receives a broadcast message from any of the connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// This function will register \p fn to be called once a broadcast message
  /// has been received. The payload will be encoded as MessagePack.
  ///
  /// \note
  ///   This function internally allocates a buffer large enough to hold the
  ///   largest allowed payload. Use a different overload if you need to
  ///   fine-tune the behaviour.
  ///
  /// If this function is called while a previous receive operation is still
  /// active then the previous operation will be canceled with the
  /// #kCanceled error.
  ///
  /// \attention
  ///   Broadcast messages do not get queued, i.e. if a branch is not actively
  ///   receiving broadcast messages then they will be discarded. To ensure that
  ///   no messages get missed, call receive_broadcast_async() again from within
  ///   the handler \p fn.
  ///
  /// \param fn Handler to call for the received broadcast message.
  void receive_broadcast_async(ReceiveBroadcastFn fn) {
    auto buffer = std::make_unique<Buffer>(constants::kMaxMessagePayloadSize);
    receive_broadcast_async(std::move(buffer), fn);
  }

  /// Receives a broadcast message from any of the connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// This function will register \p fn to be called once a broadcast message
  /// has been received. The payload will be encoded as per \p enc.
  ///
  /// \note
  ///   This function internally allocates a buffer large enough to hold the
  ///   largest allowed payload. Use a different overload if you need to
  ///   fine-tune the behaviour.
  ///
  /// If this function is called while a previous receive operation is still
  /// active then the previous operation will be canceled with the
  /// #kCanceled error.
  ///
  /// \attention
  ///   Broadcast messages do not get queued, i.e. if a branch is not actively
  ///   receiving broadcast messages then they will be discarded. To ensure that
  ///   no messages get missed, call receive_broadcast_async() again from within
  ///   the handler \p fn.
  ///
  /// \param enc Encoding to use for the received payload.
  /// \param fn  Handler to call for the received broadcast message.
  void receive_broadcast_async(Encoding enc, ReceiveBroadcastFn fn) {
    auto buffer = std::make_unique<Buffer>(constants::kMaxMessagePayloadSize);
    receive_broadcast_async(enc, std::move(buffer), fn);
  }

  /// Receives a broadcast message from any of the connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// This function will register \p fn to be called once a broadcast message
  /// has been received. The payload will be encoded as MessagePack.
  ///
  /// \attention
  ///   If the received payload does not fit into \p buffer then \p fn will be
  ///   called with the #kBufferTooSmall error and \p buffer containing as much
  ///   received data as possible. In this case, the payload view passed to
  ///   \p fn will be invalid.
  ///
  /// If this function is called while a previous receive operation is still
  /// active then the previous operation will be canceled with the
  /// #kCanceled error.
  ///
  /// \attention
  ///   Broadcast messages do not get queued, i.e. if a branch is not actively
  ///   receiving broadcast messages then they will be discarded. To ensure that
  ///   no messages get missed, call receive_broadcast_async() again from within
  ///   the handler \p fn.
  ///
  /// \param buffer Buffer to use for receiving the payload.
  /// \param fn     Handler to call for the received broadcast message.
  void receive_broadcast_async(BufferPtr&& buffer, ReceiveBroadcastFn fn) {
    receive_broadcast_async(Encoding::kMsgpack, std::move(buffer), fn);
  }

  /// Receives a broadcast message from any of the connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// This function will register \p fn to be called once a broadcast message
  /// has been received. The payload will be encoded as per \p enc.
  ///
  /// \attention
  ///   If the received payload does not fit into \p buffer then \p fn will be
  ///   called with the #kBufferTooSmall error and \p buffer containing as much
  ///   received data as possible. In this case, the payload view passed to
  ///   \p fn will be invalid.
  ///
  /// If this function is called while a previous receive operation is still
  /// active then the previous operation will be canceled with the
  /// #kCanceled error.
  ///
  /// \attention
  ///   Broadcast messages do not get queued, i.e. if a branch is not actively
  ///   receiving broadcast messages then they will be discarded. To ensure that
  ///   no messages get missed, call receive_broadcast_async() again from within
  ///   the handler \p fn.
  ///
  /// \param enc    Encoding to use for the received payload.
  /// \param buffer Buffer to use for receiving the payload.
  /// \param fn     Handler to call for the received broadcast message.
  void receive_broadcast_async(Encoding enc, BufferPtr&& buffer, ReceiveBroadcastFn fn) {
    struct CallbackData {
      ReceiveBroadcastFn fn;
      Uuid uuid;
      BufferPtr buffer;
      Encoding enc;
    };

    auto data    = std::make_unique<CallbackData>();
    data->fn     = fn;
    data->buffer = std::move(buffer);
    data->enc    = enc;

    int res = detail::YOGI_BranchReceiveBroadcastAsync(
        handle(), &data->uuid, static_cast<int>(enc), data->buffer->data(), static_cast<int>(data->buffer->size()),
        [](int res, int size, void* userarg) {
          auto data = std::unique_ptr<CallbackData>(static_cast<CallbackData*>(userarg));
          if (!data->fn) return;

          PayloadView payload;
          if (Result(res)) {
            payload = PayloadView(data->buffer->data(), size, data->enc);
          }

          detail::with_error_code_to_result(res, data->fn, data->uuid, payload, std::move(data->buffer));
        },
        data.get());

    detail::check_error_code(res);
    data.release();
  }

  /// Receives a broadcast message from any of the connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// This function will register \p fn to be called once a broadcast message
  /// has been received. The payload will be encoded as MessagePack.
  ///
  /// \note
  ///   This function internally allocates a buffer large enough to hold the
  ///   largest allowed payload. Use a different overload if you need to
  ///   fine-tune the behaviour.
  ///
  /// If this function is called while a previous receive operation is still
  /// active then the previous operation will be canceled with the
  /// #kCanceled error.
  ///
  /// \attention
  ///   Broadcast messages do not get queued, i.e. if a branch is not actively
  ///   receiving broadcast messages then they will be discarded. To ensure that
  ///   no messages get missed, call receive_broadcast_async() again from within
  ///   the handler \p fn.
  ///
  /// \param fn Handler to call for the received broadcast message.
  void receive_broadcast_async(ReceiveBroadcastSimpleFn fn) {
    auto buffer = std::make_unique<Buffer>(constants::kMaxMessagePayloadSize);
    receive_broadcast_async(std::move(buffer), fn);
  }

  /// Receives a broadcast message from any of the connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// This function will register \p fn to be called once a broadcast message
  /// has been received. The payload will be encoded as per \p enc.
  ///
  /// \note
  ///   This function internally allocates a buffer large enough to hold the
  ///   largest allowed payload. Use a different overload if you need to
  ///   fine-tune the behaviour.
  ///
  /// If this function is called while a previous receive operation is still
  /// active then the previous operation will be canceled with the
  /// #kCanceled error.
  ///
  /// \attention
  ///   Broadcast messages do not get queued, i.e. if a branch is not actively
  ///   receiving broadcast messages then they will be discarded. To ensure that
  ///   no messages get missed, call receive_broadcast_async() again from within
  ///   the handler \p fn.
  ///
  /// \param enc Encoding to use for the received payload.
  /// \param fn  Handler to call for the received broadcast message.
  void receive_broadcast_async(Encoding enc, ReceiveBroadcastSimpleFn fn) {
    auto buffer = std::make_unique<Buffer>(constants::kMaxMessagePayloadSize);
    receive_broadcast_async(enc, std::move(buffer), fn);
  }

  /// Receives a broadcast message from any of the connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// This function will register \p fn to be called once a broadcast message
  /// has been received. The payload will be encoded as MessagePack.
  ///
  /// \attention
  ///   If the received payload does not fit into \p buffer then \p fn will be
  ///   called with the #kBufferTooSmall error and \p buffer containing as much
  ///   received data as possible. In this case, the payload view passed to
  ///   \p fn will be invalid.
  ///
  /// If this function is called while a previous receive operation is still
  /// active then the previous operation will be canceled with the
  /// #kCanceled error.
  ///
  /// \attention
  ///   Broadcast messages do not get queued, i.e. if a branch is not actively
  ///   receiving broadcast messages then they will be discarded. To ensure that
  ///   no messages get missed, call receive_broadcast_async() again from within
  ///   the handler \p fn.
  ///
  /// \param buffer Buffer to use for receiving the payload.
  /// \param fn     Handler to call for the received broadcast message.
  void receive_broadcast_async(BufferPtr&& buffer, ReceiveBroadcastSimpleFn fn) {
    receive_broadcast_async(Encoding::kMsgpack, std::move(buffer), fn);
  }

  /// Receives a broadcast message from any of the connected branches.
  ///
  /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
  /// As opposed to sending messages via terminals, broadcast messages don't
  /// have to comply with a defined schema for the payload; any data that can be
  /// encoded is valid. This implies that validating the data is entirely up to
  /// the user code.
  ///
  /// This function will register \p fn to be called once a broadcast message
  /// has been received. The payload will be encoded as MessagePack.
  ///
  /// \attention
  ///   If the received payload does not fit into \p buffer then \p fn will be
  ///   called with the #kBufferTooSmall error and \p buffer containing as much
  ///   received data as possible. In this case, the payload view passed to
  ///   \p fn will be invalid.
  ///
  /// If this function is called while a previous receive operation is still
  /// active then the previous operation will be canceled with the
  /// #kCanceled error.
  ///
  /// \attention
  ///   Broadcast messages do not get queued, i.e. if a branch is not actively
  ///   receiving broadcast messages then they will be discarded. To ensure that
  ///   no messages get missed, call receive_broadcast_async() again from within
  ///   the handler \p fn.
  ///
  /// \param enc    Encoding to use for the received payload.
  /// \param buffer Buffer to use for receiving the payload.
  /// \param fn     Handler to call for the received broadcast message.
  void receive_broadcast_async(Encoding enc, BufferPtr&& buffer, ReceiveBroadcastSimpleFn fn) {
    receive_broadcast_async(enc, std::move(buffer),
                            [=](auto& res, auto& source, auto& payload, auto&&) { fn(res, source, payload); });
  }

  /// Cancels receiving a broadcast message.
  ///
  /// Calling this function will cause the handler registered via
  /// receive_broadcast_async() to be called with the kCanceled error.
  ///
  /// \note
  ///   If the receive handler has already been scheduled for execution,
  ///   this function will return _false_.
  ///
  /// \returns True if the operation was canceled successfully.
  bool cancel_receive_broadcast() {
    int res = detail::YOGI_BranchCancelReceiveBroadcast(handle());
    return detail::false_if_specific_error_else_throw(res, ErrorCode::kOperationNotRunning);
  }

 private:
  Branch(ContextPtr context, const ConfigurationPtr& config, const StringView& section)
      : ObjectT(detail::call_api_create(detail::YOGI_BranchCreate, get_foreign_handle(context),
                                        get_foreign_handle(config), section),
                {context}),
        info_(query_info()) {
  }

  LocalBranchInfo query_info() {
    Uuid uuid;
    auto json = detail::query_string(
        [&](auto str, auto size) { return detail::YOGI_BranchGetInfo(this->handle(), &uuid, str, size); });

    return LocalBranchInfo(uuid, std::move(json));
  }

  template <typename EventInfo, typename CallbackData>
  static void call_await_event_fn(int res, BranchEvents be, int ev_res, CallbackData&& data) {
    detail::with_error_code_to_result(res, [&](const auto& result) {
      if (result) {
        EventInfo info(data->uuid, data->json.data());
        detail::with_error_code_to_result(ev_res,
                                          [&](const auto& ev_result) { data->fn(result, be, ev_result, info); });
      } else {
        BranchEventInfo info;
        detail::with_error_code_to_result(ev_res,
                                          [&](const auto& ev_result) { data->fn(result, be, ev_result, info); });
      }
    });
  }

  const LocalBranchInfo info_;
};

}  // namespace yogi

#endif  // _YOGI_BRANCH_H
