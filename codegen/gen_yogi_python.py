import munch
import stringcase
import textwrap

from common import ROOT, VERSION, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_SUFFIX
from common import replace_block_in_file
from common import generate_copyright_headers
from common import generate_conanfile_py


def generate(core_api: munch.Munch) -> None:
    """Replaces the code in yogi-python from the loaded core API YAML file"""
    generate_common_py(core_api)
    generate_version_py()
    generate_constants_py(core_api)
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
