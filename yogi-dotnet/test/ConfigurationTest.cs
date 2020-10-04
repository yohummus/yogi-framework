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
using Xunit;

namespace test
{
    public class ConfigurationTest : TestCase
    {
        Yogi.Configuration cfg = CreateConfiguration();

        [Fact]
        public void Create()
        {
            MOCK_ConfigurationCreate((ref IntPtr config, int flags) =>
            {
                Assert.Equal((int)Yogi.ConfigurationFlags.None, flags);
                config = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            var cfg = new Yogi.Configuration();
            Assert.Equal(Yogi.ConfigurationFlags.None, cfg.Flags);

            MOCK_ConfigurationCreate((ref IntPtr config, int flags) =>
            {
                Assert.Equal((int)Yogi.ConfigurationFlags.MutableCmdLine, flags);
                config = pointer;
                return (int)Yogi.ErrorCode.Ok;
            });

            cfg = new Yogi.Configuration(Yogi.ConfigurationFlags.MutableCmdLine);
        }

        [Fact]
        public void FlagsProperty()
        {
            var cfg = CreateConfiguration(Yogi.ConfigurationFlags.DisableVariables);
            Assert.Equal(Yogi.ConfigurationFlags.DisableVariables, cfg.Flags);
        }

        [Fact]
        public void UpdateFromCommandLine()
        {
            MOCK_ConfigurationUpdateFromCommandLine((IntPtr config, int argc, IntPtr argv, int options) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal(2, argc);

                var p = Marshal.ReadIntPtr(argv);
                Assert.NotEqual(IntPtr.Zero, p);
                Assert.Equal("ax", Marshal.PtrToStringAnsi(p));

                p = Marshal.ReadIntPtr(argv + IntPtr.Size);
                Assert.NotEqual(IntPtr.Zero, p);
                Assert.Equal("by", Marshal.PtrToStringAnsi(p));

                Assert.Equal((int)Yogi.CommandLineOptions.BranchGhostMode, options);
                return (int)Yogi.ErrorCode.Ok;
            });

            cfg.UpdateFromCommandLine(new[] { "ax", "by" }, Yogi.CommandLineOptions.BranchGhostMode);
        }

        [Fact]
        public void UpdateFromCommandLineError()
        {
            MOCK_ConfigurationUpdateFromCommandLine((IntPtr config, int argc, IntPtr argv, int options) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                cfg.UpdateFromCommandLine(new[] { "ax", "by", "cz" });
            });
        }

        [Fact]
        public void UpdateFromJson()
        {
            MOCK_ConfigurationUpdateFromJson((IntPtr config, IntPtr json) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal("foo", Marshal.PtrToStringAnsi(json));
                return (int)Yogi.ErrorCode.Ok;
            });

            cfg.UpdateFromJson("foo");
        }

        [Fact]
        public void UpdateFromJsonError()
        {
            MOCK_ConfigurationUpdateFromJson((IntPtr config, IntPtr json) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                cfg.UpdateFromJson("foo");
            });
        }

        [Fact]
        public void UpdateFromFile()
        {
            MOCK_ConfigurationUpdateFromFile((IntPtr config, string filename) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal("foo", filename);
                return (int)Yogi.ErrorCode.Ok;
            });

            cfg.UpdateFromFile("foo");
        }

        [Fact]
        public void UpdateFromFileError()
        {
            MOCK_ConfigurationUpdateFromFile((IntPtr config, string filename) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                cfg.UpdateFromFile("foo");
            });
        }

        [Fact]
        public void Dump()
        {
            MOCK_ConfigurationDump((IntPtr config, ref IntPtr json, ref int jsonsize, int resvars, int indent) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal(1, resvars);
                Assert.Equal(-1, indent);
                json = helloBytes;
                jsonsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal("hello", cfg.Dump());

            MOCK_ConfigurationDump((IntPtr config, ref IntPtr json, ref int jsonsize, int resvars, int indent) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal(0, resvars);
                Assert.Equal(2, indent);
                json = helloBytes;
                jsonsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal("hello", cfg.Dump(false, 2));
        }

        [Fact]
        public void DumpError()
        {
            MOCK_ConfigurationDump((IntPtr config, ref IntPtr json, ref int jsonsize, int resvars, int indent) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                cfg.Dump();
            });
        }

        [Fact]
        public void ToStringMethod()
        {
            MOCK_ConfigurationDump((IntPtr config, ref IntPtr json, ref int jsonsize, int resvars, int indent) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal(1, resvars);
                Assert.Equal(-1, indent);
                json = helloBytes;
                jsonsize = helloSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal("hello", cfg.ToString());
        }

        [Fact]
        public void ToJson()
        {
            MOCK_ConfigurationDump((IntPtr config, ref IntPtr json, ref int jsonsize, int resvars, int indent) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal(1, resvars);
                Assert.Equal(-1, indent);
                json = validJsonBytes;
                jsonsize = validJsonSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(5, cfg.ToJson()["x"]);

            MOCK_ConfigurationDump((IntPtr config, ref IntPtr json, ref int jsonsize, int resvars, int indent) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal(0, resvars);
                Assert.Equal(-1, indent);
                json = validJsonBytes;
                jsonsize = validJsonSize;
                return (int)Yogi.ErrorCode.Ok;
            });

            Assert.Equal(5, cfg.ToJson(false)["x"]);
        }

        [Fact]
        public void WriteToFile()
        {
            MOCK_ConfigurationWriteToFile((IntPtr config, string filename, int resvars, int indent) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal(1, resvars);
                Assert.Equal(-1, indent);
                Assert.Equal("foo", filename);
                return (int)Yogi.ErrorCode.Ok;
            });

            cfg.WriteToFile("foo");

            MOCK_ConfigurationWriteToFile((IntPtr config, string filename, int resvars, int indent) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal(0, resvars);
                Assert.Equal(2, indent);
                Assert.Equal("foo", filename);
                return (int)Yogi.ErrorCode.Ok;
            });

            cfg.WriteToFile("foo", false, 2);
        }

        [Fact]
        public void WriteToFileError()
        {
            MOCK_ConfigurationWriteToFile((IntPtr config, string filename, int resvars, int indent) =>
            {
                return (int)Yogi.ErrorCode.Unknown;
            });

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                cfg.WriteToFile("foo");
            });
        }

        [Fact]
        public void Validate()
        {
            MOCK_ConfigurationValidate((IntPtr config, string section, IntPtr schema) =>
            {
                Assert.Equal(pointer, config);
                Assert.Equal("foo", section);
                Assert.Equal(schema, config);
                return (int)Yogi.ErrorCode.Ok;
            });

            var scm = CreateConfiguration();
            cfg.Validate("foo", scm);
        }

        [Fact]
        public void ValidateError()
        {
            MOCK_ConfigurationValidate((IntPtr config, string section, IntPtr schema) =>
            {
                Assert.Equal(pointer, config);
                Assert.Null(section);
                Assert.Equal(schema, config);
                return (int)Yogi.ErrorCode.Unknown;
            });

            var scm = CreateConfiguration();

            Assert.ThrowsAny<Yogi.FailureException>(() =>
            {
                cfg.Validate(scm);
            });
        }
    }
}
