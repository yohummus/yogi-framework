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

import json as json_module
from typing import List, Union, Optional, Dict
from ctypes import c_void_p, c_char, c_char_p, byref, POINTER, create_string_buffer

from ._object import Object
from ._library import yogi_core
from ._json_view import JsonView
from ._enums import ConfigurationFlags, CommandLineOptions


class Configuration(Object):
    """Stores program parameters from different sources.

    A configuration represents a set of parameters that usually remain constant
    throughout the runtime of a program. Parameters can come from different
    sources such as the command line or a file. Configurations are used for
    other parts of the library such as application objects, however, they are
    also intended to store user-defined parameters.
    """

    def __init__(self, flags: ConfigurationFlags = ConfigurationFlags.NONE):
        """Create a configuration.

        Args:
            flags: Flags for changing the configuration's behaviour.
        """
        handle = c_void_p()
        yogi_core.YOGI_ConfigurationCreate(byref(handle), flags)
        self._flags = flags
        super().__init__(handle)

    @property
    def flags(self) -> ConfigurationFlags:
        """Configuration flags."""
        return self._flags

    def update_from_command_line(self, argv: List[str], options: CommandLineOptions = ConfigurationFlags.NONE) -> None:
        """Updates the configuration from command line options.

        If parsing the command line, files or any given JSON string fails, or
        if help is requested (e.g. by using the --help switch) then a
        DetailedFailureException will be raised containing detailed
        information about the error or the help text.

        Args:
            argv:    List of command line arguments including the script name.
            options: Options to provide on the command line.
        """
        args = (POINTER(c_char) * (len(argv)))()
        for i, arg in enumerate(argv):
            args[i] = create_string_buffer(arg.encode())

        yogi_core.YOGI_ConfigurationUpdateFromCommandLine(self._handle, len(args), args, options)

    def update_from_json(self, json: Union[JsonView, str, object]) -> None:
        """Updates the configuration from a JSON object or a JSON object
        serialized to a string.

        If parsing fails then a DetailedFailureException will be raised
        containing detailed information about the error.

        Args:
            json: JsonView, serializable object or already serialized object.
        """
        if not isinstance(json, JsonView):
            json = JsonView(json)

        yogi_core.YOGI_ConfigurationUpdateFromJson(self._handle, json.data.obj)

    def update_from_file(self, filename: str) -> None:
        """Updates the configuration from a JSON file.

        If parsing the file fails then a DetailedFailureException will be
        raised containing detailed information about the error.

        Args:
            filename: Path to the JSON file.
        """
        yogi_core.YOGI_ConfigurationUpdateFromFile(self._handle, filename.encode())

    def dump(self, *, resolve_variables: Optional[bool] = None, indentation: Optional[int] = None) -> str:
        """Retrieves the configuration as a JSON-formatted string.

        Args:
            resolve_variables: Resolve all configuration variables. If this is
                               None then variables will be resolved if and
                               only if the configuration supports variables.
            indentation:       Number of space characters to use for
                               indentation. A value of None uses no spaces
                               and omits new lines as well.

        Returns:
            The configuration as a JSON-formatted string.
        """
        if resolve_variables is None:
            resolve_variables = not bool(self._flags & ConfigurationFlags.DISABLE_VARIABLES)

        if indentation is None:
            indentation = -1

        json = c_char_p()
        yogi_core.YOGI_ConfigurationDump(self._handle, byref(json), None, int(resolve_variables), indentation)
        return json.value.decode()

    def to_json(self, *, resolve_variables: Optional[bool] = None) -> Dict[str, object]:
        """Retrieves the configuration as a JSON object.

        Args:
            resolve_variables: Resolve all configuration variables. If this is
                               None then variables will be resolved if and
                               only if the configuration supports variables.

        Returns:
            Dictionary representing the configuration.
        """
        return json_module.loads(self.dump(resolve_variables=resolve_variables))

    def write_to_file(self, filename: str, *, resolve_variables: Optional[bool] = None,
                      indentation: Optional[int] = None) -> None:
        """Writes the configuration to a file in JSON format.

        This is useful for debugging purposes.

        Args:
            filename:          Path to the output file.
            resolve_variables: Resolve all configuration variables. If this is
                               None then variables will be resolved if and
                               only if the configuration supports variables.
            indentation:       Number of space characters to use for
                               indentation. A value of None uses no spaces
                               and omits new lines as well.
        """
        if resolve_variables is None:
            resolve_variables = not bool(self._flags & ConfigurationFlags.DISABLE_VARIABLES)

        if indentation is None:
            indentation = -1

        yogi_core.YOGI_ConfigurationWriteToFile(self._handle, filename.encode(), int(resolve_variables), indentation)

    def validate(self, schema: 'Configuration', *, section: str = None) -> None:
        """Validates the configuration against a JSON Schema.

        The validation is based on JSON Schema draft-07, see
        http://json-schema.org/. The schema to validate against has to be
        supplied in the schema parameter which needs to be a configuration
        object itself.

        If the validation fails, a DetailedFailureException with the
        CONFIGURATION_VALIDATION_FAILED error will be raised, containing a
        human-readable description about the failure.

        The section parameter can be used to specify a section of the
        configuration to validate instead of the whole configuration.

        Args:
            schema:  The schema to use.
            section: Section in the configuration to validate; syntax is
                     JSON pointer (RFC 6901).
        """
        if section:
            section = section.encode()

        yogi_core.YOGI_ConfigurationValidate(self._handle, section, schema._handle)
