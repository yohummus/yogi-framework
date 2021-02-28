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

using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Net;
using System.IO;
using System.Collections.Generic;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using MessagePack;

public static partial class Yogi
{
    /// <summary>
    /// Information about about a branch.
    /// </summary>
    public class BranchInfo
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="json">JSON string to parse.</param>
        internal BranchInfo(string json)
        {
            this.json = json;

            JsonReader reader = new JsonTextReader(new StringReader(json));
            reader.DateParseHandling = DateParseHandling.None;
            Data = JObject.Load(reader);

            Uuid = Guid.Parse((string)Data["uuid"]);
            Name = (string)Data["name"];
            Description = (string)Data["description"];
            NetworkName = (string)Data["network_name"];
            Path = (string)Data["path"];
            Hostname = (string)Data["hostname"];
            Pid = (int)Data["pid"];
            AdvertisingInterval = Duration.FromJson(Data["advertising_interval"]);
            TcpServerPort = (int)Data["tcp_server_port"];
            StartTime = Timestamp.Parse((string)Data["start_time"]);
            Timeout = Duration.FromJson(Data["timeout"]);
            GhostMode = (bool)Data["ghost_mode"];
        }

        public override string ToString()
        {
            return json;
        }

        string json;

        protected JObject Data { get; }

        /// <summary>UUID of the branch.</summary>
        public Guid Uuid { get; }

        /// <summary>Name of the branch.</summary>
        public string Name { get; }

        /// <summary>Description of the branch.</summary>
        public string Description { get; }

        /// <summary>Name of the network.</summary>
        public string NetworkName { get; }

        /// <summary>Path of the branch.</summary>
        public string Path { get; }

        /// <summary>The machine's hostname.</summary>
        public string Hostname { get; }

        /// <summary>ID of the process.</summary>
        public int Pid { get; }

        /// <summary>Advertising interval.</summary>
        public Duration AdvertisingInterval { get; }

        /// <summary>Listening port of the TCP server for incoming connections.</summary>
        public int TcpServerPort { get; }

        /// <summary>Time when the branch was started.</summary>
        public Timestamp StartTime { get; }

        /// <summary>Connection timeout.</summary>
        public Duration Timeout { get; }

        /// <summary>True if the branch is in ghost mode.</summary>
        public bool GhostMode { get; }
    }

    /// <summary>
    /// Information about a remote branch.
    /// </summary>
    public class RemoteBranchInfo : BranchInfo
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="json">JSON string to parse.</param>
        internal RemoteBranchInfo(string json)
        : base(json)
        {
            TcpServerAddress = IPAddress.Parse((string)Data["tcp_server_address"]);
        }

        /// <summary>Address of the TCP server for incoming connections.</summary>
        public IPAddress TcpServerAddress { get; }
    }

    /// <summary>
    /// Information about a local branch.
    /// </summary>
    public class LocalBranchInfo : BranchInfo
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="json">JSON string to parse.</param>
        internal LocalBranchInfo(string json)
        : base(json)
        {
            AdvertisingAddress = IPAddress.Parse((string)Data["advertising_address"]);
            AdvertisingPort = (int)Data["advertising_port"];
            TxQueueSize = (int)Data["tx_queue_size"];
            RxQueueSize = (int)Data["rx_queue_size"];
        }

        /// <summary>Advertising IP address.</summary>
        public IPAddress AdvertisingAddress { get; }

        /// <summary>Advertising port.</summary>
        public int AdvertisingPort { get; }

        /// <summary>Size of the send queue for remote branches.</summary>
        public int TxQueueSize { get; }

        /// <summary>Size of the receive queue for remote branches.</summary>
        public int RxQueueSize { get; }
    }

    /// <summary>
    /// Information associated with a branch event.
    /// </summary>
    public class BranchEventInfo
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="json">JSON string to parse.</param>
        internal BranchEventInfo(string json)
        {
            this.json = json;
            Data = JObject.Parse(json);

            Uuid = Guid.Parse((string)Data["uuid"]);
        }

        public override string ToString()
        {
            return json;
        }

        string json;

        protected JObject Data { get; }

        /// <summary>UUID of the branch.</summary>
        public Guid Uuid { get; }
    }

    /// <summary>
    /// Information associated with the BranchDiscovered branch event.
    /// </summary>
    public class BranchDiscoveredEventInfo : BranchEventInfo
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="json">JSON string to parse.</param>
        internal BranchDiscoveredEventInfo(string json)
        : base(json)
        {
            TcpServerAddress = IPAddress.Parse((string)Data["tcp_server_address"]);
            TcpServerPort = (int)Data["tcp_server_port"];
        }

        /// <summary>Address of the TCP server for incoming connections.</summary>
        public IPAddress TcpServerAddress { get; }

        /// <summary>Listening port of the TCP server for incoming connections.</summary>
        public int TcpServerPort { get; }
    }

    /// <summary>
    /// Information associated with the BranchQueried branch event.
    /// </summary>
    public class BranchQueriedEventInfo : BranchEventInfo
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="json">JSON string to parse.</param>
        internal BranchQueriedEventInfo(string json)
        : base(json)
        {
            info = new RemoteBranchInfo(json);
        }

        /// <summary>Name of the branch.</summary>
        public string Name { get { return info.Name; } }

        /// <summary>Description of the branch.</summary>
        public string Description { get { return info.Description; } }

        /// <summary>Name of the network.</summary>
        public string NetworkName { get { return info.NetworkName; } }

        /// <summary>Path of the branch.</summary>
        public string Path { get { return info.Path; } }

        /// <summary>The machine's hostname..</summary>
        public string Hostname { get { return info.Hostname; } }

        /// <summary>ID of the process.</summary>
        public int Pid { get { return info.Pid; } }

        /// <summary>Advertising interval.</summary>
        public Duration AdvertisingInterval { get { return info.AdvertisingInterval; } }

        /// <summary>Address of the TCP server for incoming connections.</summary>
        public IPAddress TcpServerAddress { get { return info.TcpServerAddress; } }

        /// <summary>Listening port of the TCP server for incoming connections.</summary>
        public int TcpServerPort { get { return info.TcpServerPort; } }

        /// <summary>Time when the branch was started.</summary>
        public Timestamp StartTime { get { return info.StartTime; } }

        /// <summary>Connection timeout.</summary>
        public Duration Timeout { get { return info.Timeout; } }

        /// <summary>True if the branch is in ghost mode.</summary>
        public bool GhostMode { get { return info.GhostMode; } }

        /// <summary>
        /// Converts the event information to a RemoteBranchInfo object.
        /// </summary>
        /// <returns>Converted RemoteBranchInfo object.</returns>
        public RemoteBranchInfo ToRemoteBranchInfo()
        {
            return info;
        }

        RemoteBranchInfo info;
    }

    /// <summary>
    /// Information associated with the ConnectFinished branch event.
    /// </summary>
    public class ConnectFinishedEventInfo : BranchEventInfo
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="json">JSON string to parse.</param>
        internal ConnectFinishedEventInfo(string json)
        : base(json)
        {
        }
    }

    /// <summary>
    /// Information associated with the ConnectionLost branch event.
    /// </summary>
    public class ConnectionLostEventInfo : BranchEventInfo
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="json">JSON string to parse.</param>
        internal ConnectionLostEventInfo(string json)
        : base(json)
        {
        }
    }

    /// <summary>
    /// Entry point into a Yogi network.
    ///
    /// A branch represents an entry point into a YOGI network. It advertises
    /// itself via IP broadcasts/multicasts with its unique ID and information
    /// required for establishing a connection. If a branch detects other branches
    /// on the network, it connects to them via TCP to retrieve further
    /// information such as their name, description and network name. If the
    /// network names match, two branches attempt to authenticate with each other
    /// by securely comparing passwords. Once authentication succeeds and there is
    /// no other known branch with the same path then the branches can actively
    /// communicate as part of the Yogi network.
    ///
    /// Note: Even though the authentication process via passwords is done in a
    ///       secure manner, any further communication is done in plain text.
    /// </summary>
    public class Branch : Object
    {
        /// <summary>
        /// Creates the branch.
        ///
        /// The branch is configured via the props parameter. The configuration
        /// object will only be used while constructing the branch, i.e. the branch
        /// will not keep any references to it. The supplied configuration must have
        /// the following structure:
        ///
        ///    {
        ///      "name":                 "Fan Controller",
        ///      "description":          "Controls a fan via PWM",
        ///      "path":                 "/Cooling System/Fan Controller",
        ///      "network_name":         "Hardware Control",
        ///      "network_password":     "secret",
        ///      "advertising_interfaces": ["localhost"],
        ///      "advertising_address":  "ff02::8000:2439",
        ///      "advertising_port":     13531,
        ///      "advertising_interval": 1.0,
        ///      "timeout":              3.0,
        ///      "ghost_mode":           false
        ///    }
        ///
        /// All of the properties are optional and if unspecified (or set to null),
        /// their respective default values will be used. The properties have the
        /// following meaning:
        ///  - name: Name of the branch (default: PID@hostname).
        ///  - description: Description of the branch.
        ///  - path: Path of the branch in the network (default: /name where name is
        ///    the name of the branch). Must start with a slash.
        ///  - network_name: Name of the network to join (default: the machine's
        ///    hostname).
        ///  - network_password: Password for the network (default: no password).
        ///  - advertising_interfaces: Network interfaces to use for advertising.
        ///    Valid strings are Unix device names ("eth0", "en5", "wlan0"), adapter
        ///    names on Windows ("Ethernet", "VMware Network Adapter WMnet1") or
        ///    MAC addresses ("11:22:33:44:55:66"). Furthermore, the special strings
        ///    "localhost" and "all" can be used to denote loopback and all available
        ///    interfaces respectively.
        ///  - advertising_address: Multicast address to use for advertising, e.g.
        ///    239.255.0.1 for IPv4 or ff02::8000:1234 for IPv6.
        ///  - advertising_port: Port to use for advertising.
        ///  - advertising_interval: Time between advertising messages. Must be at
        ///    least 1 ms.
        ///  __timeout__: Amount of time of inactivity before a connection is
        ///    considered to be broken. Must be at least 1 ms.
        ///  - ghost_mode: Set to true to activate ghost mode (default: false).
        ///  - tx_queue_size: Size of the send queues for remote branches.
        ///  - rx_queue_size: Size of the receive queues for remote branches.
        ///
        /// Advertising and establishing connections can be limited to certain network
        /// interfaces via the _interface_ property. The default is to use all
        /// available interfaces.
        ///
        /// Setting the ghost_mode property to true prevents the branch from actively
        /// participating in the Yogi network, i.e. the branch will not advertise itself
        /// and it will not authenticate in order to join a network. However, the branch
        /// will temporarily connect to other branches in order to obtain more detailed
        /// information such as name, description, network name and so on. This is useful
        /// for obtaining information about active branches without actually becoming
        /// part of the Yogi network.
        ///
        /// Attention:
        ///   The tx_queue_size and rx_queue_size properties affect every branch
        ///   connection and can therefore consume a large amount of memory. For
        ///   example, in a network of 10 branches where these properties are set to 1 MB,
        ///   the resulting memory used for the queues would be 10 x 2 x 1 MB = 20 MB
        ///   for each of the 10 branches. This value grows with the number of branches squared.
        /// </summary>
        /// <param name="context">The context to use.</param>
        /// <param name="config">Branch properties.</param>
        /// <param name="section">Section in config to use instead of the root section.
        /// Syntax is JSON pointer (RFC 6901).</param>
        public Branch(Context context, [Optional] Configuration config, [Optional] string section)
        : base(Create(context, config, section), new Object[] { context })
        {
            Info = GetInfo();
        }

        /// <summary>
        /// Creates the branch.
        ///
        /// The branch is configured via the props parameter. The supplied JSON must
        /// have the following structure:
        ///
        ///    {
        ///      "name":                 "Fan Controller",
        ///      "description":          "Controls a fan via PWM",
        ///      "path":                 "/Cooling System/Fan Controller",
        ///      "network_name":         "Hardware Control",
        ///      "network_password":     "secret",
        ///      "advertising_interfaces": ["localhost"],
        ///      "advertising_address":  "ff02::8000:2439",
        ///      "advertising_port":     13531,
        ///      "advertising_interval": 1.0,
        ///      "timeout":              3.0,
        ///      "ghost_mode":           false
        ///    }
        ///
        /// All of the properties are optional and if unspecified (or set to null),
        /// their respective default values will be used. The properties have the
        /// following meaning:
        ///  - name: Name of the branch (default: PID@hostname).
        ///  - description: Description of the branch.
        ///  - path: Path of the branch in the network (default: /name where name is
        ///    the name of the branch). Must start with a slash.
        ///  - network_name: Name of the network to join (default: the machine's
        ///    hostname).
        ///  - network_password: Password for the network (default: no password).
        ///  - advertising_interfaces: Network interfaces to use for advertising.
        ///    Valid strings are Unix device names ("eth0", "en5", "wlan0"), adapter
        ///    names on Windows ("Ethernet", "VMware Network Adapter WMnet1") or
        ///    MAC addresses ("11:22:33:44:55:66"). Furthermore, the special strings
        ///    "localhost" and "all" can be used to denote loopback and all available
        ///    interfaces respectively.
        ///  - advertising_address: Multicast address to use for advertising, e.g.
        ///    239.255.0.1 for IPv4 or ff02::8000:1234 for IPv6.
        ///  - advertising_port: Port to use for advertising.
        ///  - advertising_interval: Time between advertising messages. Must be at
        ///    least 1 ms.
        ///  __timeout__: Amount of time of inactivity before a connection is
        ///    considered to be broken. Must be at least 1 ms.
        ///  - ghost_mode: Set to true to activate ghost mode (default: false).
        ///  - tx_queue_size: Size of the send queues for remote branches.
        ///  - rx_queue_size: Size of the receive queues for remote branches.
        ///
        /// Advertising and establishing connections can be limited to certain network
        /// interfaces via the _interface_ property. The default is to use all
        /// available interfaces.
        ///
        /// Setting the ghost_mode property to true prevents the branch from actively
        /// participating in the Yogi network, i.e. the branch will not advertise itself
        /// and it will not authenticate in order to join a network. However, the branch
        /// will temporarily connect to other branches in order to obtain more detailed
        /// information such as name, description, network name and so on. This is useful
        /// for obtaining information about active branches without actually becoming
        /// part of the Yogi network.
        ///
        /// Attention:
        ///   The tx_queue_size and rx_queue_size properties affect every branch
        ///   connection and can therefore consume a large amount of memory. For
        ///   example, in a network of 10 branches where these properties are set to 1 MB,
        ///   the resulting memory used for the queues would be 10 x 2 x 1 MB = 20 MB
        ///   for each of the 10 branches. This value grows with the number of branches squared.
        /// </summary>
        /// <param name="context">The context to use.</param>
        /// <param name="json">Branch properties.</param>
        /// <param name="section">Section in props to use instead of the root section.
        /// Syntax is JSON pointer (RFC 6901).</param>
        public Branch(Context context, JsonView json, [Optional] string section)
        : this(context, MakeConfigurationFromJson(json), section)
        {
        }

        /// <summary>Information about the local branch.</summary>
        public LocalBranchInfo Info { get; }

        /// <summary>UUID of the branch.</summary>
        public Guid Uuid { get { return Info.Uuid; } }

        /// <summary>Name of the branch.</summary>
        public string Name { get { return Info.Name; } }

        /// <summary>Description of the branch.</summary>
        public string Description { get { return Info.Description; } }

        /// <summary>Name of the network.</summary>
        public string NetName { get { return Info.NetworkName; } }

        /// <summary>Path of the branch.</summary>
        public string Path { get { return Info.Path; } }

        /// <summary>The machine's hostname..</summary>
        public string Hostname { get { return Info.Hostname; } }

        /// <summary>ID of the process.</summary>
        public int Pid { get { return Info.Pid; } }

        /// <summary>Advertising interval.</summary>
        public Duration AdvertisingInterval { get { return Info.AdvertisingInterval; } }

        /// <summary>Listening port of the TCP server for incoming connections.</summary>
        public int TcpServerPort { get { return Info.TcpServerPort; } }

        /// <summary>Time when the branch was started.</summary>
        public Timestamp StartTime { get { return Info.StartTime; } }

        /// <summary>Connection timeout.</summary>
        public Duration Timeout { get { return Info.Timeout; } }

        /// <summary>True if the branch is in ghost mode.</summary>
        public bool GhostMode { get { return Info.GhostMode; } }

        /// <summary>Advertising IP address.</summary>
        public IPAddress AdvertisingAddress { get { return Info.AdvertisingAddress; } }

        /// <summary>Advertising port.</summary>
        public int AdvertisingPort { get { return Info.AdvertisingPort; } }

        /// <summary>Size of the send queue for remote branches.</summary>
        public int TxQueueSize { get { return Info.TxQueueSize; } }

        /// <summary>Size of the receive queue for remote branches.</summary>
        public int RxQueueSize { get { return Info.RxQueueSize; } }

        /// <summary>
        /// Retrieves information about all connected remote branches.
        /// </summary>
        /// <returns>Dictionary mapping the UUID of each connected remote branch to a
        /// RemoteBranchInfo object with detailed information about the branch.</returns>
        public Dictionary<Guid, RemoteBranchInfo> GetConnectedBranches()
        {
            var branches = new Dictionary<Guid, RemoteBranchInfo>();
            StringBuilder json;
            var size = 1024;

            int res;
            do
            {
                branches.Clear();
                json = new StringBuilder(size);
                size *= 2;

                YogiCore.BranchGetConnectedBranchesFnDelegate fn = (ec, userarg) =>
                {
                    if (ec != (int)ErrorCode.Ok) return;
                    var info = new RemoteBranchInfo(json.ToString());
                    branches.Add(info.Uuid, info);
                };

                res = YogiCore.YOGI_BranchGetConnectedBranches(Handle, IntPtr.Zero, json, json.Capacity, fn,
                                                               IntPtr.Zero);
            }
            while (res == (int)ErrorCode.BufferTooSmall);
            CheckErrorCode(res);

            return branches;
        }

        /// <summary>
        /// Delegate for the await event handler function.
        /// </summary>
        /// <param name="res">Result of the wait operation.</param>
        /// <param name="ev">The event that occurred.</param>
        /// <param name="evres">Result associated with the event.</param>
        /// <param name="info">Event information.</param>
        public delegate void AwaitEventFnDelegate(Result res, BranchEvents ev, Result evres,
                                                  [Optional] BranchEventInfo info);

        /// <summary>
        /// Waits for a branch event to occur.
        ///
        /// This function will register the handler fn to be executed once one of the given branch
        /// events occurs. If this function is called while a previous wait operation is still
        /// active then the previous opertion will be canceled, i.e. the handler fn for the
        /// previous operation will be called with a cancellation error.
        ///
        /// If successful, the event information passed to the handler function fn contains at
        /// least the UUID of the remote branch.
        ///
        /// In case that the internal buffer for reading the event information is too small, fn
        /// will be called with the corresponding error and the event information is lost. You can
        /// set the size of this buffer via the bufferSize parameter.
        /// </summary>
        /// <param name="events">Events to observe.</param>
        /// <param name="fn">Handler function to call.</param>
        /// <param name="bufferSize">Size of the internal buffer for reading the event
        /// information.</param>
        public void AwaitEventAsync(BranchEvents events, AwaitEventFnDelegate fn,
                                    int bufferSize = 1024)
        {
            var json = new StringBuilder(bufferSize);

            YogiCore.BranchAwaitEventAsyncFnDelegate wrapper = (res, ev, evres, userarg) =>
            {
                var result = ErrorCodeToResult(res);
                BranchEventInfo info = null;
                if (result)
                {
                    var jsonStr = json.ToString();
                    switch (ev)
                    {
                        case (int)BranchEvents.BranchDiscovered:
                            info = new BranchDiscoveredEventInfo(jsonStr);
                            break;

                        case (int)BranchEvents.BranchQueried:
                            info = new BranchQueriedEventInfo(jsonStr);
                            break;

                        case (int)BranchEvents.ConnectFinished:
                            info = new ConnectFinishedEventInfo(jsonStr);
                            break;

                        case (int)BranchEvents.ConnectionLost:
                            info = new ConnectionLostEventInfo(jsonStr);
                            break;
                    }
                }

                try
                {
                    fn(result, (BranchEvents)ev, ErrorCodeToResult(evres), info);
                }
                finally
                {
                    GCHandle.FromIntPtr(userarg).Free();
                }
            };
            var wrapperHandle = GCHandle.Alloc(wrapper);

            try
            {
                var wrapperPtr = GCHandle.ToIntPtr(wrapperHandle);
                int res = YogiCore.YOGI_BranchAwaitEventAsync(Handle, (int)events, IntPtr.Zero, json, bufferSize,
                                                              wrapper, wrapperPtr);
                CheckErrorCode(res);
            }
            catch
            {
                wrapperHandle.Free();
                throw;
            }
        }

        /// <summary>
        /// Cancels waiting for a branch event.
        ///
        /// Calling this function will cause the handler registered via AwaitEventAsync()
        /// to be called with a cancellation error.
        /// </summary>
        /// <returns>True if the wait operation was cancelled successfully.</returns>
        public bool CancelAwaitEvent()
        {
            int res = YogiCore.YOGI_BranchCancelAwaitEvent(Handle);
            return FalseIfSpecificErrorElseThrow(res, ErrorCode.OperationNotRunning);
        }

        /// <summary>
        /// Sends a broadcast message to all connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// Setting the block parameter to false will cause the function to skip sending
        /// the message to branches that have a full send queue. If at least one branch
        /// was skipped, the function will return false. If the parameter is set to true
        /// instead, the function will block until the message has been put into the send
        /// queues of all connected branches.
        ///
        /// Attention: Calling this function from within a handler function executed
        ///            through the branch's context with block set to true will cause a
        ///            dead-lock if any send queue is full!
        /// </summary>
        /// <param name="payload">Payload to send.</param>
        /// <param name="block">Block until message has been put into all send buffers.</param>
        /// <returns>True if the message was successfully put into all send buffers.</returns>
        public bool SendBroadcast(PayloadView payload, bool block = true)
        {
            int res = YogiCore.YOGI_BranchSendBroadcast(Handle, (int)payload.Encoding, payload.Data,
                                                        payload.Size, block ? 1 : 0);

            if (res == (int)ErrorCode.TxQueueFull) return false;
            CheckErrorCode(res);
            return true;
        }

        /// <summary>
        /// Delegate for the send broadcast handler function.
        /// </summary>
        /// <param name="res">Result of the send operation.</param>
        /// <param name="oid">ID of the send operation.</param>
        public delegate void SendBroadcastFnDelegate(Result res, OperationId oid);

        /// <summary>
        /// Sends a broadcast message to all connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// Setting the retry parameter to false will cause the function to skip sending
        /// the message to branches that have a full send queue. If at least one branch
        /// was skipped, the handler fn will be called with the TxQueueFull error. If the
        /// parameter is set to true instead, fn will be called once the message has been
        /// put into the send queues of all connected branches.
        ///
        /// The function returns an ID which uniquely identifies this send operation
        /// until fn has been called. It can be used in a subsequent CancelSendBroadcast()
        /// call to abort the operation.
        /// </summary>
        /// <param name="payload">Payload to send.</param>
        /// <param name="retry">Retry sending the message if a send queue is full.</param>
        /// <param name="fn">Handler to call once the operation finished.</param>
        /// <returns>ID of the send operation..</returns>
        public OperationId SendBroadcastAsync(PayloadView payload, bool retry,
                                              SendBroadcastFnDelegate fn)
        {
            YogiCore.BranchSendBroadcastAsyncFnDelegate wrapper = (res, oid, userarg) =>
            {
                try
                {
                    fn(ErrorCodeToResult(res), new OperationId(oid));
                }
                finally
                {
                    GCHandle.FromIntPtr(userarg).Free();
                }
            };
            var wrapperHandle = GCHandle.Alloc(wrapper);

            try
            {
                var wrapperPtr = GCHandle.ToIntPtr(wrapperHandle);
                int res = YogiCore.YOGI_BranchSendBroadcastAsync(Handle, (int)payload.Encoding, payload.Data,
                                                                 payload.Size, retry ? 1 : 0, wrapper, wrapperPtr);
                CheckErrorCode(res);
                return new OperationId(res);
            }
            catch
            {
                wrapperHandle.Free();
                throw;
            }
        }

        /// <summary>
        /// Sends a broadcast message to all connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// The handler function fn will be called once the message has been put into the
        /// send queues of all connected branches.
        ///
        /// The function returns an ID which uniquely identifies this send operation
        /// until fn has been called. It can be used in a subsequent CancelSendBroadcast()
        /// call to abort the operation.
        /// </summary>
        /// <param name="payload">Payload to send.</param>
        /// <param name="fn">Handler to call once the operation finished.</param>
        /// <returns>ID of the send operation..</returns>
        public OperationId SendBroadcastAsync(PayloadView payload, SendBroadcastFnDelegate fn)
        {
            return SendBroadcastAsync(payload, true, fn);
        }

        /// <summary>
        /// Cancels a send broadcast operation.
        ///
        /// Calling this function will cause the send operation with the specified
        /// operation ID to be canceled, resulting in the handler function registered
        /// via the SendBroadcastAsync() call that returned the same operation ID
        /// to be called with the Canceled error.
        ///
        /// Note: If the send operation has already been carried out but the handler
        ///       function has not been called yet, then cancelling the operation will
        ///       fail and false will be returned.
        /// </summary>
        /// <param name="oid">ID of the send operation.</param>
        /// <returns>True if the operation has been canceled successfully.</returns>
        public bool CancelSendBroadcast(OperationId oid)
        {
            int res = YogiCore.YOGI_BranchCancelSendBroadcast(Handle, oid.Value);
            return FalseIfSpecificErrorElseThrow(res, ErrorCode.InvalidOperationId);
        }

        /// <summary>
        /// Delegate for the send broadcast handler function.
        /// </summary>
        /// <param name="res">Result of the receive operation.</param>
        /// <param name="source">UUID of the sending branch.</param>
        /// <param name="payload">View on the received payload.</param>
        /// <param name="buffer">Buffer holding the payload.</param>
        public delegate void ReceiveBroadcastFnDelegate(Result res, Guid source,
                                                        PayloadView payload, byte[] buffer);

        /// <summary>
        /// Receives a broadcast message from any of the connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// This function will register fn to be called once a broadcast message
        /// has been received. The payload will be encoded as MessagePack.
        ///
        /// If this function is called while a previous receive operation is still
        /// active then the previous operation will be canceled with the Canceled error.
        ///
        /// Attention: Broadcast messages do not get queued, i.e. if a branch is not
        ///            actively receiving broadcast messages then they will be discarded.
        ///            To ensure that no messages get missed, call ReceiveBroadcastAsync()
        ///            again from within the handler fn.
        /// </summary>
        /// <param name="fn">Handler to call for the received broadcast message.</param>
        public void ReceiveBroadcastAsync(ReceiveBroadcastFnDelegate fn)
        {
            var buffer = new byte[Constants.MaxMessagePayloadSize];
            ReceiveBroadcastAsync(buffer, fn);
        }

        /// <summary>
        /// Receives a broadcast message from any of the connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// This function will register fn to be called once a broadcast message
        /// has been received. The payload will be encoded as per enc.
        ///
        /// If this function is called while a previous receive operation is still
        /// active then the previous operation will be canceled with the Canceled error.
        ///
        /// Attention: Broadcast messages do not get queued, i.e. if a branch is not
        ///            actively receiving broadcast messages then they will be discarded.
        ///            To ensure that no messages get missed, call ReceiveBroadcastAsync()
        ///            again from within the handler fn.
        /// </summary>
        /// <param name="enc">Encoding to use for the received payload.</param>
        /// <param name="fn">Handler to call for the received broadcast message.</param>
        public void ReceiveBroadcastAsync(Encoding enc, ReceiveBroadcastFnDelegate fn)
        {
            var buffer = new byte[Constants.MaxMessagePayloadSize];
            ReceiveBroadcastAsync(enc, buffer, fn);
        }

        /// <summary>
        /// Receives a broadcast message from any of the connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// This function will register fn to be called once a broadcast message
        /// has been received. The payload will be encoded as MessagePack.
        ///
        /// Attention: If the received payload does not fit into buffer then fn will be
        ///            called with the BufferTooSmall error and buffer containing as
        ///            much received data as possible. In this case, the payload view
        ///            passed to fn will be invalid.
        ///
        /// If this function is called while a previous receive operation is still
        /// active then the previous operation will be canceled with the Canceled error.
        ///
        /// Attention: Broadcast messages do not get queued, i.e. if a branch is not
        ///            actively receiving broadcast messages then they will be discarded.
        ///            To ensure that no messages get missed, call ReceiveBroadcastAsync()
        ///            again from within the handler fn.
        /// </summary>
        /// <param name="buffer">Buffer to use for receiving the payload.</param>
        /// <param name="fn">Handler to call for the received broadcast message.</param>
        public void ReceiveBroadcastAsync(byte[] buffer, ReceiveBroadcastFnDelegate fn)
        {
            ReceiveBroadcastAsync(Encoding.Msgpack, buffer, fn);
        }

        /// <summary>
        /// Receives a broadcast message from any of the connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// This function will register fn to be called once a broadcast message
        /// has been received. The payload will be encoded as per enc.
        ///
        /// Attention: If the received payload does not fit into buffer then fn will be
        ///            called with the BufferTooSmall error and buffer containing as
        ///            much received data as possible. In this case, the payload view
        ///            passed to fn will be invalid.
        ///
        /// If this function is called while a previous receive operation is still
        /// active then the previous operation will be canceled with the Canceled error.
        ///
        /// Attention: Broadcast messages do not get queued, i.e. if a branch is not
        ///            actively receiving broadcast messages then they will be discarded.
        ///            To ensure that no messages get missed, call ReceiveBroadcastAsync()
        ///            again from within the handler fn.
        /// </summary>
        /// <param name="enc">Encoding to use for the received payload.</param>
        /// <param name="buffer">Buffer to use for receiving the payload.</param>
        /// <param name="fn">Handler to call for the received broadcast message.</param>
        public void ReceiveBroadcastAsync(Encoding enc, byte[] buffer, ReceiveBroadcastFnDelegate fn)
        {
            var uuid = Marshal.AllocHGlobal(16);
            var bufferHandle = GCHandle.Alloc(buffer, GCHandleType.Pinned);

            YogiCore.BranchReceiveBroadcastAsyncFnDelegate wrapper = (res, size, userarg) =>
            {
                try
                {
                    var source = MakeGuidFromVoidPtr(uuid);
                    var payload = new PayloadView(buffer, size, enc);
                    fn(ErrorCodeToResult(res), source, payload, buffer);
                }
                finally
                {
                    Marshal.FreeHGlobal(uuid);
                    bufferHandle.Free();
                    GCHandle.FromIntPtr(userarg).Free();
                }
            };
            var wrapperHandle = GCHandle.Alloc(wrapper);

            try
            {
                var wrapperPtr = GCHandle.ToIntPtr(wrapperHandle);
                int res = YogiCore.YOGI_BranchReceiveBroadcastAsync(Handle, uuid, (int)enc, buffer, buffer.Length,
                                                                    wrapper, wrapperPtr);
                CheckErrorCode(res);
            }
            catch
            {
                wrapperHandle.Free();
                bufferHandle.Free();
                Marshal.FreeHGlobal(uuid);
                throw;
            }
        }

        /// <summary>
        /// Delegate for the send broadcast handler function.
        ///
        /// As opposed to ReceiveBroadcastFnDelegate, this function does not include the buffer
        /// parameter.
        /// </summary>
        /// <param name="res">Result of the receive operation.</param>
        /// <param name="source">UUID of the sending branch.</param>
        /// <param name="payload">View on the received payload.</param>
        public delegate void ReceiveBroadcastSimpleFnDelegate(Result res, Guid source,
                                                              PayloadView payload);

        /// <summary>
        /// Receives a broadcast message from any of the connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// This function will register fn to be called once a broadcast message
        /// has been received. The payload will be encoded as MessagePack.
        ///
        /// If this function is called while a previous receive operation is still
        /// active then the previous operation will be canceled with the Canceled error.
        ///
        /// Attention: Broadcast messages do not get queued, i.e. if a branch is not
        ///            actively receiving broadcast messages then they will be discarded.
        ///            To ensure that no messages get missed, call ReceiveBroadcastAsync()
        ///            again from within the handler fn.
        /// </summary>
        /// <param name="fn">Handler to call for the received broadcast message.</param>
        public void ReceiveBroadcastAsync(ReceiveBroadcastSimpleFnDelegate fn)
        {
            var buffer = new byte[Constants.MaxMessagePayloadSize];
            ReceiveBroadcastAsync(buffer, fn);
        }

        /// <summary>
        /// Receives a broadcast message from any of the connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// This function will register fn to be called once a broadcast message
        /// has been received. The payload will be encoded as per enc.
        ///
        /// If this function is called while a previous receive operation is still
        /// active then the previous operation will be canceled with the Canceled error.
        ///
        /// Attention: Broadcast messages do not get queued, i.e. if a branch is not
        ///            actively receiving broadcast messages then they will be discarded.
        ///            To ensure that no messages get missed, call ReceiveBroadcastAsync()
        ///            again from within the handler fn.
        /// </summary>
        /// <param name="enc">Encoding to use for the received payload.</param>
        /// <param name="fn">Handler to call for the received broadcast message.</param>
        public void ReceiveBroadcastAsync(Encoding enc, ReceiveBroadcastSimpleFnDelegate fn)
        {
            var buffer = new byte[Constants.MaxMessagePayloadSize];
            ReceiveBroadcastAsync(enc, buffer, fn);
        }

        /// <summary>
        /// Receives a broadcast message from any of the connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// This function will register fn to be called once a broadcast message
        /// has been received. The payload will be encoded as MessagePack.
        ///
        /// Attention: If the received payload does not fit into buffer then fn will be
        ///            called with the BufferTooSmall error and buffer containing as
        ///            much received data as possible. In this case, the payload view
        ///            passed to fn will be invalid.
        ///
        /// If this function is called while a previous receive operation is still
        /// active then the previous operation will be canceled with the Canceled error.
        ///
        /// Attention: Broadcast messages do not get queued, i.e. if a branch is not
        ///            actively receiving broadcast messages then they will be discarded.
        ///            To ensure that no messages get missed, call ReceiveBroadcastAsync()
        ///            again from within the handler fn.
        /// </summary>
        /// <param name="buffer">Buffer to use for receiving the payload.</param>
        /// <param name="fn">Handler to call for the received broadcast message.</param>
        public void ReceiveBroadcastAsync(byte[] buffer, ReceiveBroadcastSimpleFnDelegate fn)
        {
            ReceiveBroadcastAsync(Encoding.Msgpack, buffer, fn);
        }

        /// <summary>
        /// Receives a broadcast message from any of the connected branches.
        ///
        /// Broadcast messages contain arbitrary data encoded as JSON or MessagePack.
        /// As opposed to sending messages via terminals, broadcast messages don't
        /// have to comply with a defined schema for the payload; any data that can be
        /// encoded is valid. This implies that validating the data is entirely up to
        /// the user code.
        ///
        /// This function will register fn to be called once a broadcast message
        /// has been received. The payload will be encoded as per enc.
        ///
        /// Attention: If the received payload does not fit into buffer then fn will be
        ///            called with the BufferTooSmall error and buffer containing as
        ///            much received data as possible. In this case, the payload view
        ///            passed to fn will be invalid.
        ///
        /// If this function is called while a previous receive operation is still
        /// active then the previous operation will be canceled with the Canceled error.
        ///
        /// Attention: Broadcast messages do not get queued, i.e. if a branch is not
        ///            actively receiving broadcast messages then they will be discarded.
        ///            To ensure that no messages get missed, call ReceiveBroadcastAsync()
        ///            again from within the handler fn.
        /// </summary>
        /// <param name="enc">Encoding to use for the received payload.</param>
        /// <param name="buffer">Buffer to use for receiving the payload.</param>
        /// <param name="fn">Handler to call for the received broadcast message.</param>
        public void ReceiveBroadcastAsync(Encoding enc, byte[] buffer, ReceiveBroadcastSimpleFnDelegate fn)
        {
            ReceiveBroadcastAsync(enc, buffer, (Result res, Guid source, PayloadView payload,
                                                byte[] _) => fn(res, source, payload));
        }

        /// <summary>
        /// Cancels a receive broadcast operation.
        ///
        /// Calling this function will cause the handler registered via
        /// ReceiveBroadcastAsync() to be called with the Canceled error.
        ///
        /// Note: If the receive handler has already been scheduled for execution
        ///       this function will return false.
        /// </summary>
        /// <returns>True if the operation has been canceled successfully.</returns>
        public bool CancelReceiveBroadcast()
        {
            int res = YogiCore.YOGI_BranchCancelReceiveBroadcast(Handle);
            if (res == (int)ErrorCode.OperationNotRunning) return false;
            CheckErrorCode(res);
            return true;
        }

        static IntPtr Create(Context context, [Optional] Configuration config,
                             [Optional] string section)
        {
            var handle = new IntPtr();
            int res = YogiCore.YOGI_BranchCreate(ref handle, context.Handle, config == null ? null : config.Handle,
                                                 section);
            CheckErrorCode(res);
            return handle;
        }

        static Configuration MakeConfigurationFromJson(JsonView json)
        {
            var config = new Configuration();
            config.UpdateFromJson(json);

            return config;
        }

        static Guid MakeGuidFromVoidPtr(IntPtr uuid)
        {
            var bytes = new byte[16];
            Marshal.Copy(uuid, bytes, 0, 16);
            Array.Reverse(bytes, 0, 4);
            Array.Reverse(bytes, 4, 2);
            Array.Reverse(bytes, 6, 2);
            return new Guid(bytes);
        }

        LocalBranchInfo GetInfo()
        {
            StringBuilder json;
            var size = 1024;
            int res;
            do
            {
                json = new StringBuilder(size);
                size *= 2;
                res = YogiCore.YOGI_BranchGetInfo(Handle, IntPtr.Zero, json, json.Capacity);
            }
            while (res == (int)ErrorCode.BufferTooSmall);
            CheckErrorCode(res);
            return new LocalBranchInfo(json.ToString());
        }
    }
}
