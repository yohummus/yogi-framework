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
using System.ComponentModel;

public static partial class Yogi
{
    internal class Library
    {
        public interface LibUtils
        {
            IntPtr LoadLibrary(string filename);
            void FreeLibrary(IntPtr libHandle);
            IntPtr GetProcAddress(IntPtr libHandle, string fnName);
        }

        public class WindowsLibUtils : LibUtils
        {
            void LibUtils.FreeLibrary(IntPtr handle)
            {
                FreeLibrary(handle);
            }

            IntPtr LibUtils.GetProcAddress(IntPtr dllHandle, string name)
            {
                return GetProcAddress(dllHandle, name);
            }

            IntPtr LibUtils.LoadLibrary(string fileName)
            {
                var dll = LoadLibrary(fileName);

                if (dll == IntPtr.Zero)
                {
                    var desc = new Win32Exception(Marshal.GetLastWin32Error()).Message;
                    throw new DllNotFoundException($"Could not load library {filename}: " + desc);
                }

                return dll;
            }

            [DllImport("kernel32")]
            static extern IntPtr LoadLibrary(string fileName);

            [DllImport("kernel32.dll")]
            static extern int FreeLibrary(IntPtr handle);

            [DllImport("kernel32.dll")]
            static extern IntPtr GetProcAddress(IntPtr handle, string procedureName);
        }

        public class UnixLibUtils : LibUtils
        {
            public IntPtr LoadLibrary(string fileName)
            {
                // clear previous errors if any
                dlerror();

                var res = dlopen(fileName, RTLD_NOW);
                var errPtr = dlerror();
                if (errPtr != IntPtr.Zero)
                {
                    throw new DllNotFoundException($"Could not load library {fileName}: "
                        + Marshal.PtrToStringAnsi(errPtr));
                }

                return res;
            }

            public void FreeLibrary(IntPtr handle)
            {
                dlclose(handle);
            }

            public IntPtr GetProcAddress(IntPtr dllHandle, string name)
            {
                // clear previous errors if any
                dlerror();

                var res = dlsym(dllHandle, name);

                var errPtr = dlerror();
                if (errPtr != IntPtr.Zero)
                {
                    throw new System.Exception("dlsym: " + Marshal.PtrToStringAnsi(errPtr));
                }

                return res;
            }

            const int RTLD_NOW = 2;

            [DllImport("libdl")]
            static extern IntPtr dlopen(String fileName, int flags);

            [DllImport("libdl")]
            static extern IntPtr dlsym(IntPtr handle, String symbol);

            [DllImport("libdl")]
            static extern int dlclose(IntPtr handle);

            [DllImport("libdl")]
            static extern IntPtr dlerror();
        }

        static LibUtils utils;
        static string filename;
        static IntPtr dll;

        static Library()
        {
            utils = RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                           ? (LibUtils)new WindowsLibUtils()
                           : (LibUtils)new UnixLibUtils();

            filename = Environment.GetEnvironmentVariable("YOGI_CORE_LIBRARY");
            if (filename == null)
            {
                if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
                {
                    string architecture = System.Environment.Is64BitProcess ? "x64" : "x86";
                    filename = $"yogi-core-{architecture}.dll";
                }
                else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
                {
                    filename = "libyogi-core.dylib";
                }
                else
                {
                    filename = "libyogi-core.so";
                }
            }

            dll = utils.LoadLibrary(filename);
            CheckVersionCompatibility();
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int CheckBindingsCompatibilityDelegate(string bindver,
            [MarshalAs(UnmanagedType.LPStr)] StringBuilder err, int errsize);

        static void CheckVersionCompatibility()
        {
            var fn = GetDelegateForFunction<CheckBindingsCompatibilityDelegate>(
                "YOGI_CheckBindingsCompatibility");

            // :BEGIN_CODEGEN: gen_dotnet_version
            var bindingsVersion = "0.0.3-alpha";
            // :END_CODEGEN:

            var err = new StringBuilder(256);
            int res = fn(bindingsVersion, err, err.Capacity);
            if (res < 0)
            {
                throw new ExternalException(err.ToString());
            }
        }

        internal static T GetDelegateForFunction<T>(string functionName) where T : class
        {
            IntPtr fn = utils.GetProcAddress(dll, functionName);
            if (fn == IntPtr.Zero)
            {
                throw new MissingMethodException($"Function {functionName} is missing in {filename}");
            }

            return Marshal.GetDelegateForFunctionPointer(fn, typeof(T)) as T;
        }
    }
}
