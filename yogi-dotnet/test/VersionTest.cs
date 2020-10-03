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

using System.Reflection;
using Xunit;

namespace test
{
    public class VersionTest : TestCase
    {
        [Fact]
        public void BindingsInfo()
        {
            var vVersion = Yogi.BindingsInfo.Version;
            var vMajor = Yogi.BindingsInfo.VersionMajor;
            var vMinor = Yogi.BindingsInfo.VersionMinor;
            var vPatch = Yogi.BindingsInfo.VersionPatch;
            var vSuffix = Yogi.BindingsInfo.VersionSuffix;

            Assert.Equal(string.Format("{0}.{1}.{2}{3}", vMajor, vMinor, vPatch, vSuffix), vVersion);
            Assert.True(vMajor >= 0);
            Assert.True(vMinor >= 0);
            Assert.True(vPatch >= 0);
            Assert.Contains(vSuffix, vVersion);
        }

        [Fact]
        public void CoreInfo()
        {
            Assert.Equal(Yogi.BindingsInfo.Version, Yogi.CoreInfo.Version);
            Assert.Equal(Yogi.BindingsInfo.VersionMajor, Yogi.CoreInfo.VersionMajor);
            Assert.Equal(Yogi.BindingsInfo.VersionMinor, Yogi.CoreInfo.VersionMinor);
            Assert.Equal(Yogi.BindingsInfo.VersionPatch, Yogi.CoreInfo.VersionPatch);
            Assert.Equal(Yogi.BindingsInfo.VersionSuffix, Yogi.CoreInfo.VersionSuffix);
        }

        [Fact]
        public void AssemblyVersion()
        {
            var v = Assembly.GetAssembly(typeof(Yogi)).GetName().Version;
            var s = string.Format("{0}.{1}.{2}", v.Major, v.Minor, v.Build);
            Assert.Equal(Yogi.BindingsInfo.Version, s + Yogi.BindingsInfo.VersionSuffix);
        }

        [Fact]
        public void InformationalVersion()
        {
            var assembly = Assembly.GetAssembly(typeof(Yogi));
            var attr = assembly.GetCustomAttribute<AssemblyInformationalVersionAttribute>();
            Assert.Equal(Yogi.BindingsInfo.Version, attr.InformationalVersion);
        }
    }
}
