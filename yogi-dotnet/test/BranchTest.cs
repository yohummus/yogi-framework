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
using System.Net;
using System.Collections.Generic;
using Xunit;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace test
{
    public class BranchTest : TestCase
    {
        static string MakeBranchInfoString(string uuid = "123e4567-e89b-12d3-a456-426655440000")
        {
            return $@"{{
                ""uuid"": ""{uuid}"",
                ""name"": ""My Branch"",
                ""description"": ""Stuff"",
                ""network_name"": ""My Network"",
                ""path"": ""/some/path"",
                ""hostname"": ""localhost"",
                ""pid"": 3333,
                ""advertising_interval"": 7.5,
                ""tcp_server_port"": 11223,
                ""start_time"": ""2018-04-23T18:25:43.511Z"",
                ""timeout"": -1,
                ""ghost_mode"": false,
                ""tx_queue_size"": 2000,
                ""rx_queue_size"": 3000,

                ""tcp_server_address"": ""192.168.1.1"",

                ""advertising_address"": ""239.255.0.1"",
                ""advertising_port"": 12345,
            }}";
        }

        [Fact]
        public void BranchInfo()
        {
            // Mock out the timestamp parse function
            MOCK_ParseTime((ref long timestamp, string str, string timefmt) =>
            {
                Assert.Equal("2018-04-23T18:25:43.511Z", str);
                Assert.Null(timefmt);
                timestamp = 123456789123000000L;
                return (int)Yogi.ErrorCode.Ok;
            });

            // Create and check the BranchInfo class
            var branchInfo = new Yogi.BranchInfo(MakeBranchInfoString());
            Assert.IsType<Guid>(branchInfo.Uuid);
            Assert.Equal("123e4567-e89b-12d3-a456-426655440000", branchInfo.Uuid.ToString());
            Assert.Equal("My Branch", branchInfo.Name);
            Assert.Equal("Stuff", branchInfo.Description);
            Assert.Equal("My Network", branchInfo.NetworkName);
            Assert.Equal("/some/path", branchInfo.Path);
            Assert.Equal("localhost", branchInfo.Hostname);
            Assert.Equal(3333, branchInfo.Pid);
            Assert.Equal(7.5, branchInfo.AdvertisingInterval.TotalSeconds, precision: 5);
            Assert.Equal(11223, branchInfo.TcpServerPort);
            Assert.Equal(123, branchInfo.StartTime.Milliseconds);
            Assert.Equal(Yogi.Duration.Inf, branchInfo.Timeout);
            Assert.False(branchInfo.GhostMode);

            // Create and check the RemoteBranchInfo class
            var remoteBranchInfo = new Yogi.RemoteBranchInfo(MakeBranchInfoString());
            Assert.Equal("192.168.1.1", remoteBranchInfo.TcpServerAddress.ToString());

            // Create and check the LocalBranchInfo class
            var localBranchInfo = new Yogi.LocalBranchInfo(MakeBranchInfoString());
            Assert.IsType<IPAddress>(localBranchInfo.AdvertisingAddress);
            Assert.Equal("239.255.0.1", localBranchInfo.AdvertisingAddress.ToString());
            Assert.Equal(12345, localBranchInfo.AdvertisingPort);
            Assert.Equal(2000, localBranchInfo.TxQueueSize);
            Assert.Equal(3000, localBranchInfo.RxQueueSize);
        }

        [Fact]
        public void BranchEventInfo()
        {
            // Mock out the timestamp parse function
            MOCK_ParseTime((ref long timestamp, string str, string timefmt) =>
            {
                Assert.Equal("2018-04-23T18:25:43.511Z", str);
                Assert.Null(timefmt);
                timestamp = 123456789123000000L;
                return (int)Yogi.ErrorCode.Ok;
            });

            // Create and check the BranchEventInfo class
            var eventInfo = new Yogi.BranchEventInfo(MakeBranchInfoString());
            Assert.IsType<Guid>(eventInfo.Uuid);
            Assert.Equal("123e4567-e89b-12d3-a456-426655440000", eventInfo.Uuid.ToString());

            // Create and check the BranchDiscoveredEventInfo class
            var discoveredEventInfo = new Yogi.BranchDiscoveredEventInfo(MakeBranchInfoString());
            Assert.Equal("192.168.1.1", discoveredEventInfo.TcpServerAddress.ToString());
            Assert.Equal(11223, discoveredEventInfo.TcpServerPort);

            // Create and check the BranchQueriedEventInfo class
            var queriedEventInfo = new Yogi.BranchQueriedEventInfo(MakeBranchInfoString());
            Assert.Equal("My Branch", queriedEventInfo.Name);
            Assert.Equal("Stuff", queriedEventInfo.Description);
            Assert.Equal("My Network", queriedEventInfo.NetworkName);
            Assert.Equal("/some/path", queriedEventInfo.Path);
            Assert.Equal("localhost", queriedEventInfo.Hostname);
            Assert.Equal(3333, queriedEventInfo.Pid);
            Assert.Equal(7.5, queriedEventInfo.AdvertisingInterval.TotalSeconds, precision: 5);
            Assert.Equal("192.168.1.1", queriedEventInfo.TcpServerAddress.ToString());
            Assert.Equal(11223, queriedEventInfo.TcpServerPort);
            Assert.Equal(123, queriedEventInfo.StartTime.Milliseconds);
            Assert.Equal(Yogi.Duration.Inf, queriedEventInfo.Timeout);
            Assert.False(queriedEventInfo.GhostMode);
        }
    }
}

/*

                [Fact]
                public void Info()
                {
                    var props = new JObject();
                    props["name"] = "My Branch";
                    props["description"] = "Stuff";
                    props["network_name"] = "My Network";
                    props["network_password"] = "Password";
                    props["path"] = "/some/path";
                    props["advertising_address"] = "239.255.0.1";
                    props["advertising_port"] = 12345;
                    props["advertising_interval"] = 7;
                    props["timeout"] = -1;

                    var branch = new Yogi.Branch(context, props);

                    var info = branch.Info;
                    Assert.IsType<Guid>(info.Uuid);
                    Assert.Equal("My Branch", info.Name);
                    Assert.Equal("Stuff", info.Description);
                    Assert.Equal("My Network", info.NetworkName);
                    Assert.Equal("/some/path", info.Path);
                    Assert.Equal(Dns.GetHostName(), info.Hostname);
                    Assert.Equal(System.Diagnostics.Process.GetCurrentProcess().Id, info.Pid);
                    Assert.IsType<IPAddress>(info.AdvertisingAddress);
                    Assert.Equal("239.255.0.1", info.AdvertisingAddress.ToString());
                    Assert.Equal(12345, info.AdvertisingPort);
                    Assert.Equal(7, info.AdvertisingInterval.TotalSeconds, precision: 5);
                    Assert.True(info.TcpServerPort > 0);
                    Assert.True(info.StartTime < Yogi.CurrentTime);
                    Assert.Equal(Yogi.Duration.Infinity, info.Timeout);
                    Assert.False(info.GhostMode);
                    Assert.True(info.TxQueueSize > 1000);
                    Assert.True(info.RxQueueSize > 1000);

                    Assert.Equal(info.Uuid, branch.Uuid);
                    Assert.Equal(info.Name, branch.Name);
                    Assert.Equal(info.Description, branch.Description);
                    Assert.Equal(info.NetworkName, branch.NetName);
                    Assert.Equal(info.Path, branch.Path);
                    Assert.Equal(info.Hostname, branch.Hostname);
                    Assert.Equal(info.Pid, branch.Pid);
                    Assert.Equal(info.AdvertisingAddress, branch.AdvertisingAddress);
                    Assert.Equal(info.AdvertisingPort, branch.AdvertisingPort);
                    Assert.Equal(info.AdvertisingInterval, branch.AdvertisingInterval);
                    Assert.Equal(info.TcpServerPort, branch.TcpServerPort);
                    Assert.Equal(info.StartTime, branch.StartTime);
                    Assert.Equal(info.Timeout, branch.Timeout);
                    Assert.Equal(info.GhostMode, branch.GhostMode);
                    Assert.Equal(info.TxQueueSize, branch.TxQueueSize);
                    Assert.Equal(info.RxQueueSize, branch.RxQueueSize);
                }

                [Fact]
                public void GetConnectedBranches()
                {
                    var branch = new Yogi.Branch(context, "{\"name\":\"My Branch\"}");
                    var branch_a = new Yogi.Branch(context, "{\"name\":\"A\"}");
                    var branch_b = new Yogi.Branch(context, "{\"name\":\"B\"}");

                    while (!branch.GetConnectedBranches().ContainsKey(branch_a.Uuid)
                        || !branch.GetConnectedBranches().ContainsKey(branch_b.Uuid))
                    {
                        context.RunOne();
                    }
                    var branches = branch.GetConnectedBranches();

                    Assert.True(branches.ContainsKey(branch_a.Uuid));
                    Assert.Equal(branches[branch_a.Uuid].Name, branch_a.Name);
                    Assert.IsType<Yogi.RemoteBranchInfo>(branches[branch_a.Uuid]);
                    Assert.IsType<IPAddress>(branches[branch_a.Uuid].TcpServerAddress);

                    Assert.True(branches.ContainsKey(branch_b.Uuid));
                    Assert.Equal(branches[branch_b.Uuid].Name, branch_b.Name);
                }

                [Fact]
                public void AwaitEventAsync()
                {
                    var branch = new Yogi.Branch(context, "{\"name\":\"My Branch\"}");
                    var branch_a = new Yogi.Branch(context, "{\"name\":\"A\"}");

                    var events = Yogi.BranchEvents.BranchQueried | Yogi.BranchEvents.ConnectionLost;
                    bool called = false;
                    branch.AwaitEventAsync(events, (res, ev, evres, info) =>
                    {
                        Assert.IsType<Yogi.Success>(res);
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.IsType<Yogi.BranchEvents>(ev);
                        Assert.Equal(Yogi.BranchEvents.BranchQueried, ev);
                        Assert.IsType<Yogi.Success>(evres);
                        Assert.Equal(Yogi.ErrorCode.Ok, evres.ErrorCode);
                        Assert.IsType<Yogi.BranchQueriedEventInfo>(info);
                        Assert.Equal(branch_a.Uuid, info.Uuid);
                        Assert.Equal(branch_a.StartTime, (info as Yogi.BranchQueriedEventInfo).StartTime);
                        Assert.Equal(branch_a.Timeout, (info as Yogi.BranchQueriedEventInfo).Timeout);
                        Assert.Equal(branch_a.GhostMode, (info as Yogi.BranchQueriedEventInfo).GhostMode);
                        called = true;
                    });

                    GC.Collect();

                    while (!called) context.RunOne();

                    Assert.True(called);

                    GC.KeepAlive(branch);
                    GC.KeepAlive(branch_a);
                }

                [Fact]
                public void CancelAwaitEvent()
                {
                    var branch = new Yogi.Branch(context, "{\"name\":\"My Branch\"}");

                    bool called = false;
                    branch.AwaitEventAsync(Yogi.BranchEvents.All, (res, ev, evres, info) =>
                    {
                        Assert.IsType<Yogi.Failure>(res);
                        Assert.Equal(Yogi.ErrorCode.Canceled, res.ErrorCode);
                        Assert.IsType<Yogi.BranchEvents>(ev);
                        Assert.Equal(Yogi.BranchEvents.None, ev);
                        Assert.IsType<Yogi.Success>(evres);
                        Assert.Equal(Yogi.ErrorCode.Ok, evres.ErrorCode);
                        Assert.Null(info);
                        called = true;
                    });

                    branch.CancelAwaitEvent();
                    context.Poll();
                    Assert.True(called);

                    GC.KeepAlive(branch);
                }

                [Fact]
                public void SendBroadcast()
                {
                    var branchA = new Yogi.Branch(context,
                                                  "{\"name\":\"a\", \"_transceive_byte_limit\": 5}");
                    var branchB = new Yogi.Branch(context, "{\"name\":\"b\"}");
                    RunContextUntilBranchesAreConnected(context, branchA, branchB);
                    context.RunInBackground();

                    // Receive a broadcast to verify that it has actually been sent
                    bool broadcastReceived = false;
                    branchB.ReceiveBroadcastAsync(Yogi.EncodingType.Json, (res, _, payload) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(bigJsonView, payload);
                        broadcastReceived = true;
                    });

                    GC.Collect();

                    // Blocking
                    for (int i = 0; i < 3; ++i)
                    {
                        Assert.True(branchA.SendBroadcast(bigJsonView, true));
                    }

                    // Non-blocking
                    while (branchA.SendBroadcast(bigJsonView, false))
                        ;

                    context.Stop();
                    context.WaitForStopped();

                    // Verify that a broadcast has actually been sent
                    while (!broadcastReceived) context.RunOne();

                    GC.KeepAlive(branchA);
                    GC.KeepAlive(branchB);
                }

                [Fact]
                public void SendBroadcastAsync()
                {
                    var branchA = new Yogi.Branch(context,
                                                  "{\"name\":\"a\", \"_transceive_byte_limit\": 5}");
                    var branchB = new Yogi.Branch(context, "{\"name\":\"b\"}");
                    RunContextUntilBranchesAreConnected(context, branchA, branchB);

                    // Receive a broadcast to verify that it has actually been sent
                    bool broadcastReceived = false;
                    branchB.ReceiveBroadcastAsync(Yogi.EncodingType.Json, (res, _, payload) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(bigJsonView, payload);
                        broadcastReceived = true;
                    });

                    GC.Collect();

                    // Send with retry = true
                    int n = 3;
                    var results = new List<Yogi.Result>();
                    for (int i = 0; i < n; ++i)
                    {
                        var oid = branchA.SendBroadcastAsync(bigJsonView, true, (res, opid) =>
                        {
                            Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                            Assert.True(opid.IsValid);
                            results.Add(res);
                        });
                        Assert.True(oid.IsValid);
                    }

                    while (results.Count != n) context.Poll();

                    // Send with retry = false
                    do
                    {
                        branchA.SendBroadcastAsync(bigJsonView, false, (res, _) =>
                        {
                            results.Add(res);
                        });

                        context.PollOne();
                    } while (results[results.Count - 1].ErrorCode == Yogi.ErrorCode.Ok);

                    Assert.Equal(Yogi.ErrorCode.TxQueueFull, results[results.Count - 1].ErrorCode);

                    // Verify that a broadcast has actually been sent
                    while (!broadcastReceived) context.RunOne();

                    GC.KeepAlive(branchA);
                    GC.KeepAlive(branchB);
                }

                [Fact]
                public void CancelSendBroadcast()
                {
                    var branchA = new Yogi.Branch(context,
                                                  "{\"name\":\"a\", \"_transceive_byte_limit\": 5}");
                    var branchB = new Yogi.Branch(context, "{\"name\":\"b\"}");
                    RunContextUntilBranchesAreConnected(context, branchA, branchB);

                    var oidToEc = new Dictionary<Yogi.OperationId, Yogi.ErrorCode>();
                    while (true)
                    {
                        var oid = branchA.SendBroadcastAsync(bigJsonView, (res, opid) =>
                        {
                            oidToEc[opid] = res.ErrorCode;
                        });
                        Assert.True(oid.IsValid);

                        GC.Collect();

                        oidToEc[oid] = Yogi.ErrorCode.Unknown;

                        if (branchA.CancelSendBroadcast(oid))
                        {
                            context.Poll();
                            Assert.Equal(Yogi.ErrorCode.Canceled, oidToEc[oid]);
                            break;
                        }
                    }

                    GC.KeepAlive(branchA);
                    GC.KeepAlive(branchB);
                }

                [Fact]
                public void ReceiveBroadcastAsync()
                {
                    var branchA = new Yogi.Branch(context, "{\"name\":\"a\"}");
                    var branchB = new Yogi.Branch(context, "{\"name\":\"b\"}");
                    RunContextUntilBranchesAreConnected(context, branchA, branchB);

                    // Simplest form
                    var uuidB = branchB.Uuid;
                    bool called = false;
                    branchA.ReceiveBroadcastAsync((res, source, payload) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(uuidB, source);
                        Assert.Equal(msgpackView, payload);
                        called = true;
                    });

                    branchB.SendBroadcastAsync(msgpackView, (_1, _2) => { });
                    GC.Collect();
                    while (!called) context.RunOne();

                    // With buffer in handler function
                    called = false;
                    branchA.ReceiveBroadcastAsync((res, _, payload, buf) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(msgpackView, payload);
                        Assert.True(buf.Length >= Yogi.Constants.MaxMessagePayloadSize);
                        called = true;
                    });

                    branchB.SendBroadcastAsync(msgpackView, (_1, _2) => { });
                    GC.Collect();
                    while (!called) context.RunOne();

                    // With encoding
                    called = false;
                    branchA.ReceiveBroadcastAsync(Yogi.EncodingType.Json, (res, _, payload) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(jsonView, payload);
                        called = true;
                    });

                    branchB.SendBroadcastAsync(msgpackView, (_1, _2) => { });
                    GC.Collect();
                    while (!called) context.RunOne();

                    // With encoding and with buffer in handler function
                    called = false;
                    branchA.ReceiveBroadcastAsync(Yogi.EncodingType.Json, (res, _, payload, buf) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(jsonView, payload);
                        Assert.True(buf.Length >= Yogi.Constants.MaxMessagePayloadSize);
                        called = true;
                    });

                    branchB.SendBroadcastAsync(msgpackView, (_1, _2) => { });
                    GC.Collect();
                    while (!called) context.RunOne();

                    // With custom buffer
                    var buffer = new byte[123];
                    called = false;
                    branchA.ReceiveBroadcastAsync(buffer, (res, _, payload) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(msgpackView, payload);
                        called = true;
                    });

                    branchB.SendBroadcastAsync(msgpackView, (_1, _2) => { });
                    GC.Collect();
                    while (!called) context.RunOne();

                    // With custom buffer and with buffer in handler function
                    buffer = new byte[123];
                    called = false;
                    branchA.ReceiveBroadcastAsync(buffer, (res, _, payload, buf) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(msgpackView, payload);
                        Assert.Equal(buffer, buf);
                        called = true;
                    });

                    branchB.SendBroadcastAsync(msgpackView, (_1, _2) => { });
                    GC.Collect();
                    while (!called) context.RunOne();

                    // With custom buffer and encoding
                    buffer = new byte[123];
                    called = false;
                    branchA.ReceiveBroadcastAsync(Yogi.EncodingType.Json, buffer, (res, _, payload) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(jsonView, payload);
                        called = true;
                    });

                    branchB.SendBroadcastAsync(msgpackView, (_1, _2) => { });
                    GC.Collect();
                    while (!called) context.RunOne();

                    // With custom buffer and encoding and buffer in handler function
                    buffer = new byte[123];
                    called = false;
                    branchA.ReceiveBroadcastAsync(Yogi.EncodingType.Json, buffer, (res, _, payload, buf) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Ok, res.ErrorCode);
                        Assert.Equal(jsonView, payload);
                        Assert.Equal(buffer, buf);
                        called = true;
                    });

                    branchB.SendBroadcastAsync(msgpackView, (_1, _2) => { });
                    GC.Collect();
                    while (!called) context.RunOne();

                    GC.KeepAlive(branchA);
                    GC.KeepAlive(branchB);
                }

                [Fact]
                public void CancelReceiveBroadcast()
                {
                    var branch = new Yogi.Branch(context, "{\"name\":\"a\"}");

                    Assert.False(branch.CancelReceiveBroadcast());

                    bool called = false;
                    branch.ReceiveBroadcastAsync((res, _2, _3, _4) =>
                    {
                        Assert.Equal(Yogi.ErrorCode.Canceled, res.ErrorCode);
                        called = true;
                    });

                    GC.Collect();

                    Assert.True(branch.CancelReceiveBroadcast());
                    context.Poll();
                    Assert.True(called);

                    GC.KeepAlive(branch);
                }*/
