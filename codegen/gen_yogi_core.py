import munch
import stringcase
import jinja2
import subprocess
import json
import hashlib

from common import ROOT
from common import replace_in_file
from common import generate_copyright_headers


def generate(core_api: munch.Munch) -> None:
    """Replaces the code in yogi-core from the loaded core API YAML file"""
    generate_copyright_headers(core_api, 'yogi-core')
    generate_constants_h_cc(core_api)
    generate_constants_test_cc(core_api)
    generate_errors_cc(core_api)
    generate_errors_test_cc(core_api)
    generate_yogi_core_h(core_api)
    generate_schemas_h_cc()
    generate_cmake_lists_txt(core_api)
    generate_conanfile_py(core_api)


def generate_constants_h_cc(core_api: munch.Munch) -> None:
    """Replaces the code in the constants.h and constants.cc files"""
    header_lines = []
    source_lines = []
    for name, props in core_api.constants.items():
        const_name = f'k{stringcase.pascalcase(name.lower())}'

        if isinstance(props.value, str):
            if '\n' in props.value:
                default_val = '\n'.join([f'"{x}\\n"' for x in props.value.rstrip().split('\n')])
            elif props.value.startswith('YOGI_'):
                default_val = props.value
            else:
                default_val = f'"{props.value}"'
        elif isinstance(props.value, float):
            default_val = f'{props.value:.0f}'
        elif isinstance(props.value, list):
            default_val = '"' + json.dumps(props.value).replace('"', '\\"') + '"'
        else:
            default_val = props.value

        if props.type.c == 'long long':
            default_val += 'll'

        header_lines += [f'{props.type.c} const {const_name} = {default_val};']

        source_line = f'    case YOGI_CONST_{name}: *static_cast<{props.type.c}*>(dest) = {const_name}; break;'
        if props.type.c == 'long long':
            source_line += ' // NOLINT(google-runtime-int)'

        source_lines += [source_line]

    replace_in_file('yogi-core/src/api/constants.h', header_lines)
    replace_in_file('yogi-core/src/api/constants.cc', source_lines)


def generate_constants_test_cc(core_api: munch.Munch) -> None:
    """Replaces the code in the constants_test.cc file"""
    lines = []
    for name in core_api.constants:
        const_name = f'k{stringcase.pascalcase(name.lower())}'
        lines += [f'check(YOGI_CONST_{name}, {const_name});']

    replace_in_file('yogi-core/test/api/constants_test.cc', lines)


def generate_errors_cc(core_api: munch.Munch) -> None:
    """Replaces the code in the errors.cc file"""
    lines = []
    for idx, (name, description) in enumerate(core_api.error_codes.items()):
        prefix = 'ERR_' if idx else ''
        lines += [f'    case YOGI_{prefix}{name}: return "{description}";']

    replace_in_file('yogi-core/src/api/errors.cc', lines)


def generate_errors_test_cc(core_api: munch.Munch) -> None:
    """Replaces the code in the errors_test.cc file"""
    lines = [f'int kLastError = YOGI_ERR_{list(core_api.error_codes)[-1]};']

    replace_in_file('yogi-core/test/api/errors_test.cc', lines)


def generate_yogi_core_h(core_api: munch.Munch) -> None:
    """Generates the yogi_core.h files from yogi_core.h.template"""
    env = jinja2.Environment()
    env.filters['to_bit'] = lambda x: '0' if x is None else f'(1 << {x})'
    env.filters['or_flags'] = lambda x, prefix = '': f'({" | ".join([prefix + y for y in x])})'
    env.filters['prefix_lines'] = lambda x, prefix: '\n'.join([f'{prefix}{y}' for y in x.rstrip().split('\n')])
    env.filters['to_function_declaration'] = lambda x, fn_name: f'YOGI_API {make_function_signature(fn_name, x[fn_name])};'
    env.filters['ns_to_s'] = lambda x: x / 1e9
    env.filters['sha256'] = lambda x: hashlib.sha256(x.encode()).hexdigest().upper()
    env.filters['multi_line_string_to_array'] = lambda x, prefix: ',\n'.join(
        [f'{prefix}"{x}"' for x in x.rstrip().split('\n')])

    file = ROOT / 'yogi-core/include/yogi_core.h'
    with open(file.with_suffix('.h.jinja2'), 'r') as f:
        template = env.from_string(f.read())

    content = template.render(core_api=core_api)
    formatted_content = subprocess.check_output(['clang-format'], input=content, text=True, cwd=file.parent)

    with open(file, 'w') as f:
        f.write(formatted_content)


def generate_schemas_h_cc() -> None:
    """Replaces the code in the schemas.h and schemas.cc files"""
    schema_files = (ROOT / 'yogi-core/src/schemas').glob('*.schema.json')

    header_lines = []
    source_lines_0 = []
    source_lines_1 = []
    for schema_file in schema_files:
        schema_file_name = schema_file.name[:schema_file.name.index('.')]
        var_name = f'k{stringcase.pascalcase(schema_file_name)}'
        macro_name = f'YOGI_SCM_{stringcase.constcase(schema_file_name)}'

        # Header file
        header_lines += [f'extern const char {var_name}[];  // {macro_name}']

        # First code block in source file
        source_lines_0 += [f'    case {macro_name}: return schemas::{var_name};']

        # Second code block in source files
        with open(schema_file, 'r') as f:
            schema_content = f.read()

        source_lines_1 += ['', f'// {macro_name}']
        source_lines_1 += [f'const char {var_name}[] = R"raw({schema_content})raw";']

    replace_in_file('yogi-core/src/schemas/schemas.h', header_lines)
    replace_in_file('yogi-core/src/schemas/schemas.cc', source_lines_0, block_idx=0)
    replace_in_file('yogi-core/src/schemas/schemas.cc', source_lines_1, block_idx=1)


def generate_cmake_lists_txt(core_api: munch.Munch) -> None:
    """Replaces the code in the CMakeLists.txt file"""
    src_lines = []
    for file in (ROOT / 'yogi-core/src').rglob('*.cc'):
        src_lines += [f'    {file.relative_to(ROOT  / "yogi-core")}']

    replace_in_file('yogi-core/CMakeLists.txt', src_lines, block_idx=0)

    v = core_api.version
    version_lines = [f'    VERSION {v.major}.{v.minor}.{v.patch}{v.suffix}',
                     f'    SOVERSION {v.major}']

    replace_in_file('yogi-core/CMakeLists.txt', version_lines, block_idx=1)

    test_lines = []
    for file in (ROOT / 'yogi-core/test').rglob('*.cc'):
        test_lines += [f'    {file.relative_to(ROOT  / "yogi-core")}']

    replace_in_file('yogi-core/CMakeLists.txt', test_lines, block_idx=2)


def generate_conanfile_py(core_api: munch.Munch) -> None:
    """Replaces the code in the conanfile.py file"""
    v = core_api.version
    lines = [f'    version = "{v.major}.{v.minor}.{v.patch}{v.suffix}"']

    replace_in_file('yogi-core/conanfile.py', lines)


def make_function_signature(fn_name: str, fn_props: munch.Munch) -> str:
    """Creates the function signature for the given function"""
    fn_args = []
    for arg_name, arg_type in fn_props.args.items():
        if isinstance(arg_type, munch.Munch):
            fn_args += [make_function_signature(f'(*{arg_name})', arg_type)]
        else:
            fn_args += [f'{arg_type} {arg_name}']

    return f'{fn_props.return_type} {fn_name}({",".join(fn_args)})'
