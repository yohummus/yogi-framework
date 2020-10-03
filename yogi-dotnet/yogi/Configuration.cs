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
using System.Collections.Generic;
using Newtonsoft.Json.Linq;

public static partial class Yogi
{
    /// <summary>
    /// A configuration represents a set of parameters that usually remain constant
    /// throughout the runtime of a program. Parameters can come from different
    /// sources such as the command line or a file. Configurations are used for
    /// other parts of the library such as application objects, however, they are
    /// also intended to store user-defined parameters.
    /// </summary>
    public class Configuration : Object
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="flags">Flags for behaviour adjustments.</param>
        public Configuration(ConfigurationFlags flags = ConfigurationFlags.None)
        : base(Create(flags))
        {
            Flags = flags;
        }

        /// <summary>
        /// Updates the configuration from command line options.
        ///
        /// If parsing the command line, files or any given JSON string fails, or
        /// if help is requested (e.g. by using the --help switch) then a
        /// DetailedFailureException will be raised containing detailed
        /// information about the error or the help text.
        /// </summary>
        /// <param name="argv">List of command line arguments including the script name.</param>
        /// <param name="options">Options to provide on the command line.</param>
        public void UpdateFromCommandLine(IEnumerable<string> argv,
                                          CommandLineOptions options = CommandLineOptions.None)
        {
            var args = new List<string>();
            args.AddRange(argv);

            int res = YogiCore.YOGI_ConfigurationUpdateFromCommandLine(Handle, args.Count, args.ToArray(),
                                                                       (int)options);
            CheckErrorCode(res);
        }

        /// <summary>
        /// Updates the configuration from a JSON object serialized to a string.
        ///
        /// If parsing fails then a DetailedFailureException with the
        /// ConfigurationValidationFailed error will be raised containing detailed
        /// information about the failure.
        /// </summary>
        /// <param name="json">JSON object.</param>
        public void UpdateFromJson(JsonView json)
        {
            int res = YogiCore.YOGI_ConfigurationUpdateFromJson(Handle, json.Data);
            CheckErrorCode(res);
        }

        /// <summary>
        /// Updates the configuration from a JSON file.
        ///
        /// If parsing fails then a DetailedFailureException with the
        /// ConfigurationValidationFailed error will be raised containing detailed
        /// information about the failure.
        /// </summary>
        /// <param name="filename">Path to the JSON file.</param>
        public void UpdateFromFile(string filename)
        {
            int res = YogiCore.YOGI_ConfigurationUpdateFromFile(Handle, filename);
            CheckErrorCode(res);
        }

        /// <summary>
        /// Retrieves the configuration as a JSON-formatted string.
        /// </summary>
        /// <param name="resolveVariables">Resolve all configuration variables.
        /// By default, variables get resolved if the configuration supports them.</param>
        /// <param name="indentation">Number of space characters to use for indentation.
        /// By default, no spaces are used and new lines are omitted as well.</param>
        /// <returns>The configuration serialized to a string.</returns>
        public string Dump([Optional] bool? resolveVariables, [Optional] int? indentation)
        {
            if (resolveVariables == null)
            {
                resolveVariables = (Flags & ConfigurationFlags.DisableVariables)
                                    != ConfigurationFlags.DisableVariables;
            }

            if (indentation == null)
            {
                indentation = -1;
            }

            var json = new IntPtr();
            int jsonsize = 0;
            int res = YogiCore.YOGI_ConfigurationDump(Handle, ref json, ref jsonsize, resolveVariables == true ? 1 : 0,
                                                      (int)indentation);
            CheckErrorCode(res);
            return Marshal.PtrToStringAnsi(json);
        }

        /// <summary>
        /// Retrieves the configuration as a JSON object.
        /// </summary>
        /// <param name="resolveVariables">Resolve all configuration variables.
        /// By default, variables get resolved if the configuration supports them.</param>
        /// <returns>JSON object representing the configuration.</returns>
        public JObject ToJson([Optional] bool? resolveVariables)
        {
            return JObject.Parse(Dump(resolveVariables));
        }

        /// <summary>
        /// Writes the configuration to a file in JSON format.
        /// </summary>
        /// <param name="filename">Path to the output file.</param>
        /// <param name="resolveVariables">Resolve all configuration variables.
        /// By default, variables get resolved if the configuration supports them.</param>
        /// <param name="indentation">Number of space characters to use for indentation.
        /// By default, no spaces are used and new lines are omitted as well.</param>
        public void WriteToFile(string filename, [Optional] bool? resolveVariables,
            [Optional] int? indentation)
        {
            if (resolveVariables == null)
            {
                resolveVariables = (Flags & ConfigurationFlags.DisableVariables)
                                    != ConfigurationFlags.DisableVariables;
            }

            if (indentation == null)
            {
                indentation = -1;
            }

            int res = YogiCore.YOGI_ConfigurationWriteToFile(Handle, filename, resolveVariables == true ? 1 : 0,
                                                             (int)indentation);
            CheckErrorCode(res);
        }

        /// <summary>
        /// Validates a section of the configuration against a JSON Schema.
        ///
        /// The validation is based on JSON Schema draft-07, see
        /// http://json-schema.org/. The schema to validate against has to be
        /// supplied in \p schema which needs to be a configuration object itself.
        ///
        /// If the validation fails, the kConfigurationValidationFailed error will be
        /// thrown, containing a human-readable description about the failure.
        /// </summary>
        /// <param name="section">Section in the configuration to validate; syntax is
        /// JSON pointer (RFC 6901).</param>
        /// <param name="schema">The schema to use.</param>
        public void Validate(string section, Configuration schema)
        {
            int res = YogiCore.YOGI_ConfigurationValidate(Handle, section, schema.Handle);
            CheckErrorCode(res);
        }

        /// <summary>
        /// Validates the configuration against a JSON Schema.
        ///
        /// The validation is based on JSON Schema draft-07, see
        /// http://json-schema.org/. The schema to validate against has to be
        /// supplied in \p schema which needs to be a configuration object itself.
        ///
        /// If the validation fails, the kConfigurationValidationFailed error will be
        /// thrown, containing a human-readable description about the failure.
        /// </summary>
        /// <param name="section">Section in the configuration to validate; syntax is
        /// JSON pointer (RFC 6901).</param>
        /// <param name="schema">The schema to use.</param>
        public void Validate(Configuration schema)
        {
            Validate(null, schema);
        }

        /// <summary>
        /// Configuration flags.
        /// </summary>
        public readonly ConfigurationFlags Flags;

        /// <summary>
        /// Serializes the configuration to a string.
        /// </summary>
        /// <returns>Configuration serialized to a string.</returns>
        public override string ToString()
        {
            return Dump();
        }

        static IntPtr Create(ConfigurationFlags flags)
        {
            var handle = new IntPtr();
            int res = YogiCore.YOGI_ConfigurationCreate(ref handle, (int)flags);
            CheckErrorCode(res);
            return handle;
        }
    }
}
