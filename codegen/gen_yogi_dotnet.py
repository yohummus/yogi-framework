import munch
import stringcase

from .common import VERSION, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_SUFFIX
from .common import replace_block_in_file
from .common import generate_copyright_headers
from .common import generate_conanfile_py


def generate(core_api: munch.Munch) -> None:
    """Replaces the code in yogi-dotnet from the loaded core API YAML file"""
    generate_library_cs()
    generate_version_cs()
    generate_yogi_csproj()
    generate_constants_cs(core_api)
    generate_enums_cs(core_api)
    generate_api_cs(core_api)
    generate_common_cs(core_api)
    generate_copyright_headers(core_api, 'yogi-dotnet')
    generate_conanfile_py('yogi-dotnet')


def generate_library_cs() -> None:
    """Replaces the code in the Library.cs file"""
    lines = [f'            var bindingsVersion = "{VERSION}";']

    replace_block_in_file('yogi-dotnet/yogi/Library.cs', lines)


def generate_version_cs() -> None:
    """Replaces the code in the Version.cs file"""
    lines = [f'            Version = "{VERSION}";',
             f'            VersionMajor = {VERSION_MAJOR};',
             f'            VersionMinor = {VERSION_MINOR};',
             f'            VersionPatch = {VERSION_PATCH};',
             f'            VersionSuffix = "{VERSION_SUFFIX}";']

    replace_block_in_file('yogi-dotnet/yogi/Version.cs', lines)


def generate_yogi_csproj() -> None:
    """Replaces the code in the yogi.csproj file"""
    lines = [f'    <Version>{VERSION}</Version>']

    replace_block_in_file('yogi-dotnet/yogi/yogi.csproj', lines)


def generate_constants_cs(core_api: munch.Munch) -> None:
    """Replaces the code in the Constants.cs file"""
    member_lines = []
    extract_lines = []

    for val, (name, props) in enumerate(core_api.constants.items(), start=1):
        cs_name = stringcase.pascalcase(name.lower())

        member_lines += [f'',
                         f'        /// <summary>{props.help}</summary>',
                         f'        public static readonly {props.type.cs} {cs_name};']

        extract_lines += [f'           ExtractConstant(ref {cs_name}, {val});']

    replace_block_in_file('yogi-dotnet/yogi/Constants.cs', member_lines + [''], block_idx=0)
    replace_block_in_file('yogi-dotnet/yogi/Constants.cs', extract_lines, block_idx=1)


def generate_enums_cs(core_api: munch.Munch) -> None:
    """Replaces the code in the Enums.cs file"""
    line_blocks = []

    # Normal enums
    for enum, elems in core_api.enums.items():
        name_prefix = ''
        if enum == 'error_code':
            values = range(0, -len(elems), -1)
        elif enum == 'verbosity':
            values = range(-1, len(elems) - 1)
        elif enum == 'http_status':
            name_prefix = 'Http'
            values = elems.keys()
        else:
            values = range(len(elems))

        lines = []
        for val, (name, comment) in zip(values, elems.items()):
            lines += [f'',
                      f'        /// <summary>{comment}</summary>',
                      f'        {name_prefix}{stringcase.pascalcase(str(name).lower())} = {val},']

        line_blocks += [lines]

    # Flags
    for enum, flags in core_api.flags.items():
        lines = []
        for name, props in flags.items():
            lines += ['']

            # Help text
            if '\n' in props.help:
                lines += [f'        /// <summary>']
                for comment_line in props.help.rstrip().split('\n'):
                    lines += [f'        /// {comment_line}']
                lines += [f'        /// </summary>']
            else:
                lines += [f'        /// <summary>{props.help}</summary>']

            # Code line
            line = f'        {stringcase.pascalcase(name.lower())} = '
            if 'bit' in props:
                line += '0' if props.bit is None else f'1 << {props.bit}'
            else:
                line += ' | '.join([stringcase.pascalcase(x.lower()) for x in props.combine])

            lines += [f'{line},']

        line_blocks += [lines]

    for block_idx, lines in enumerate(line_blocks):
        replace_block_in_file('yogi-dotnet/yogi/Enums.cs', lines + [''], block_idx=block_idx)


def generate_api_cs(core_api: munch.Munch) -> None:
    """Replaces the code in the Api.cs file"""
    api_fn_lines = []
    for name, props in core_api.functions.items():
        name_no_prefix = name.replace('YOGI_', '')

        api_fn_lines += [f'',
                         f'        // {name}',
                         make_api_fn_delegates_code(core_api, name, props),
                         f'        public static {name_no_prefix}Delegate {name}',
                         f'            = Library.GetDelegateForFunction<{name_no_prefix}Delegate>("{name}");']

    replace_block_in_file('yogi-dotnet/yogi/Api.cs', api_fn_lines + [''])


def generate_common_cs(core_api: munch.Munch) -> None:
    """Replaces the code in the test/Common.cs file"""
    api_fn_lines = []
    for name, props in core_api.functions.items():
        name_no_prefix = name.replace('YOGI_', '')
        mock_name = f'MOCK_{name_no_prefix}'
        mock_delegate_name = f'{name_no_prefix}MockDelegate'

        api_fn_delegates_code = make_api_fn_delegates_code(core_api, name, props)
        for s in ['SafeHandle', 'string[]', 'byte[]']:
            api_fn_delegates_code = api_fn_delegates_code.replace(s, 'IntPtr')

        api_fn_lines += [f'',
                         f'        // MOCK_{name_no_prefix}',
                         api_fn_delegates_code,
                         f'        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]',
                         f'        internal delegate void {mock_delegate_name}({name_no_prefix}Delegate fn);',
                         f'',
                         f'        internal static {mock_delegate_name} {mock_name}',
                         f'            = Yogi.Library.GetDelegateForFunction<{mock_delegate_name}>("{mock_name}");']

    replace_block_in_file('yogi-dotnet/test/Common.cs', api_fn_lines + [''])


def make_api_fn_delegates_code(core_api: munch.Munch, fn_name: str, props: munch.Munch) -> str:
    """Creates the code for defining the function delegates for an API function"""
    lines = []

    fn_name_no_prefix = fn_name.replace('YOGI_', '')
    ret_type = 'IntPtr' if props.return_type == 'const char*' else core_api.type_mappings[props.return_type].cs

    args = []
    for arg_name, c_type in props.args.items():
        if isinstance(c_type, munch.Munch):
            arg_fn_name = f'{fn_name_no_prefix}{arg_name.capitalize()}'
            arg_type = f'{arg_fn_name}Delegate'
            lines += [make_api_fn_delegates_code(core_api, arg_fn_name, c_type)]
        else:
            if fn_name == 'YOGI_GetConstant' and arg_name == 'dest':
                arg_type = 'ref IntPtr'
            elif fn_name == 'YOGI_ConfigurationUpdateFromJson' and arg_name == 'json':
                arg_type = 'byte[]'
            elif fn_name != 'YOGI_Destroy' and c_type == 'void*' and arg_name not in ['userarg', 'sigarg', 'uuid',
                                                                                      'data']:
                arg_type = 'SafeHandle'
            else:
                arg_type = core_api.type_mappings[c_type].cs

        args += [f'{arg_type} {arg_name}']

    lines += [f'        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]',
              f'        public delegate {ret_type} {fn_name_no_prefix}Delegate({", ".join(args)});',
              f'']

    return '\n'.join(lines)
