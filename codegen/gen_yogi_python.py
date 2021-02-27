import munch
import textwrap

from .common import VERSION, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_SUFFIX
from .common import replace_block_in_file
from .common import generate_copyright_headers
from .common import generate_conanfile_py


def generate(core_api: munch.Munch) -> None:
    """Replaces the code in yogi-python from the loaded core API YAML file"""
    generate_common_py(core_api)
    generate_version_py()
    generate_constants_py(core_api)
    generate_library_py(core_api)
    generate_enums_py(core_api)
    generate_copyright_headers(core_api, 'yogi-python')
    generate_conanfile_py('yogi-python')


def generate_common_py(core_api: munch.Munch) -> None:
    """Replaces the code in the test/common.py file"""
    lines = []
    for name, props in core_api.functions.items():
        name_no_prefix = name.replace('YOGI_', '')
        block = textwrap.dedent(f'''
            def MOCK_{name_no_prefix}(self, fn):
                mock_fn = yogi._library.yogi_core.MOCK_{name_no_prefix}
                mock_fn.restype = None
                mock_fn.argtypes = [{make_cfunctype_mock_code(core_api, props)}]
                wrapped_fn = mock_fn.argtypes[0](fn)
                self.keepalive.append(wrapped_fn)
                mock_fn(wrapped_fn)
        '''.rstrip())

        # Fix indentation
        lines += [f'    {x}' if x else '' for x in block.split('\n')]

    replace_block_in_file('yogi-python/test/common.py', lines + [''])


def generate_version_py() -> None:
    """Replaces the code in the _version.py file"""
    block = textwrap.dedent(f'''
        VERSION: str = '{VERSION}'
        VERSION_MAJOR: int = {VERSION_MAJOR}
        VERSION_MINOR: int = {VERSION_MINOR}
        VERSION_PATCH: int = {VERSION_PATCH}
        VERSION_SUFFIX: str = '{VERSION_SUFFIX}'
    '''.strip())

    # Fix indentation
    lines = [f'    {x}' for x in block.split('\n')]

    replace_block_in_file('yogi-python/yogi/_version.py', lines)


def generate_constants_py(core_api: munch.Munch) -> None:
    """Replaces the code in the _constants.py file"""
    lines = ['    """Constants built into the Yogi Core library.',
             '',
             '    Attributes:']

    max_name_len = max([len(x) for x in core_api.constants])
    for name, props in core_api.constants.items():
        lines += [f'        {name:{max_name_len}s}  {props.help}']
    lines += ['    """']

    for val, (name, props) in enumerate(core_api.constants.items(), start=1):
        wrap_start = {
            'Duration': 'Duration.from_nanoseconds(',
            'Verbosity': 'Verbosity(',
        }.get(props.type.py, '')
        wrap_end = ')' if wrap_start else ''

        ctypes_type = core_api.type_mappings[props.type.c].py

        lines += [f'    {name}: {props.type.py} = {wrap_start}get_constant({val}, {ctypes_type}){wrap_end}']

    replace_block_in_file('yogi-python/yogi/_constants.py', lines)


def generate_library_py(core_api: munch.Munch) -> None:
    """Replaces the code in the _library.py file"""
    version_lines = [f"bindings_version = '{VERSION}'"]

    replace_block_in_file('yogi-python/yogi/_library.py', version_lines, block_idx=0)

    api_fn_lines = []
    for name, props in core_api.functions.items():
        restype = core_api.type_mappings[props.return_type].py

        fns_returning_c_int = ['YOGI_CheckBindingsCompatibility',
                               'YOGI_TimerCancel',
                               'YOGI_SignalSetCancelAwaitSignal',
                               'YOGI_BranchCancelAwaitEvent',
                               'MOCK_BranchCancelReceiveBroadcast']

        if restype == 'c_int' and name not in fns_returning_c_int:
            restype = 'api_result_handler'

        argtypes = []
        for arg_name, c_type in props.args.items():
            if isinstance(c_type, munch.Munch):
                argtypes += [make_cfunctype_mock_code(core_api, c_type)]
            elif arg_name == 'sigarg':
                argtypes += ['py_object']
            else:
                argtypes += [core_api.type_mappings[c_type].py]

        api_fn_lines += [textwrap.dedent(f'''
            yogi_core.{name}.restype = {restype}
            yogi_core.{name}.argtypes = [{', '.join(argtypes)}]
        ''').lstrip()]

    replace_block_in_file('yogi-python/yogi/_library.py', api_fn_lines, block_idx=1)


def generate_enums_py(core_api: munch.Munch) -> None:
    """Replaces the code in the _enums.py file"""
    line_blocks = []

    # Normal enums
    for enum, elems in core_api.enums.items():
        name_prefix = ''
        if enum == 'error_code':
            values = range(0, -len(elems), -1)
        elif enum == 'verbosity':
            values = range(-1, len(elems) - 1)
        elif enum == 'http_status':
            name_prefix = 'HTTP_'
            values = elems.keys()
        else:
            values = range(len(elems))

        lines = [f"    {name_prefix}{name} = {val}, '{comment}'"
                 for val, (name, comment) in zip(values, elems.items())]

        line_blocks += [lines]

    # Flags
    for enum, flags in core_api.flags.items():
        lines = []
        for name, props in flags.items():
            line = f'    {name} = '

            if 'bit' in props:
                line += '0' if props.bit is None else f'1 << {props.bit}'
            else:
                combine_names = [f'{x}[0]' for x in props.combine]
                line += ' | \\\n'.join(combine_names)

            if '\n' in props.help:
                indented_comment = '\n'.join([f'        {x}' for x in props.help.split('\n')])
                comment = f"textwrap.dedent(r'''\n{indented_comment}''').strip()\n"
            else:
                comment = f"'{props.help}'"

            line += f', {comment}'
            if 'combine' in props and not line.endswith('\n'):
                line += '\n'

            lines += [line]

        line_blocks += [lines]

    for block_idx, lines in enumerate(line_blocks):
        replace_block_in_file('yogi-python/yogi/_enums.py', lines, block_idx=block_idx)


def make_cfunctype_mock_code(core_api: munch.Munch, fn_props: munch.Munch) -> str:
    """Creates the CFUNCTYPE() code for the mock function"""
    py_types = []
    for arg_name, c_type in [(None, fn_props.return_type)] + list(fn_props.args.items()):
        if isinstance(c_type, munch.Munch):
            py_types += [make_cfunctype_mock_code(core_api, c_type)]
        elif arg_name == 'sigarg':
            py_types += ['py_object']
        else:
            py_types += [core_api.type_mappings[c_type].py]

    return f'CFUNCTYPE({", ".join(py_types)})'
