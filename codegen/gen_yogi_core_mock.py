import munch

from common import ROOT, VERSION, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_SUFFIX
from common import replace_block_in_file
from common import generate_copyright_headers
from common import generate_conanfile_py
from gen_yogi_core import make_function_signature


def generate(core_api: munch.Munch) -> None:
    """Replaces the code in yogi-core-mock from the loaded core API YAML file"""
    generate_copyright_headers(core_api, 'yogi-core-mock')
    generate_default_functions_h(core_api)
    generate_yogi_core_mock_h(core_api)
    generate_yogi_core_mock_cc(core_api)
    generate_cmake_lists_txt(core_api)
    generate_conanfile_py('yogi-core-mock')


def generate_default_functions_h(core_api: munch.Munch) -> None:
    """Replaces the code in the default_functions.h file"""
    header_lines_0 = [f'return "{VERSION}";']

    replace_block_in_file('yogi-core-mock/src/default_functions.h', header_lines_0, block_idx=0)

    header_lines_1 = []
    for name, props in core_api.constants.items():
        val = {
            'VERSION': f'"{VERSION}"',
            'VERSION_MAJOR': VERSION_MAJOR,
            'VERSION_MINOR': VERSION_MINOR,
            'VERSION_PATCH': VERSION_PATCH,
            'VERSION_SUFFIX': f'"{VERSION_SUFFIX}"',
        }.get(name)

        if val is None:
            val = {
                'const char*': '"foobar"',
                'int': 1,
                'long long': 123456789,
            }[props.type.c]

        header_lines_1 += [f'    case YOGI_CONST_{name}: *static_cast<{props.type.c}*>(dest) = {val}; break;']

    replace_block_in_file('yogi-core-mock/src/default_functions.h', header_lines_1, block_idx=1)


def generate_yogi_core_mock_h(core_api: munch.Munch) -> None:
    """Replaces the code in the yogi_core_mock.h file"""
    header_lines = [f'YOGI_API void MOCK_{name.replace("YOGI_", "")}(decltype({name}) fn);'
                    for name, props in core_api.functions.items()]

    replace_block_in_file('yogi-core-mock/include/yogi_core_mock.h', header_lines)


def generate_yogi_core_mock_cc(core_api: munch.Munch) -> None:
    """Replaces the code in the yogi_core_mock.cc file"""

    # Check which API functions have a default mock implementation
    with open(ROOT / 'yogi-core-mock/src/default_functions.h', 'r') as f:
        content = f.read()

    fns_with_default_impl = [name for name in core_api.functions
                             if f'default_{name.replace("YOGI_", "")}(' in content]

    # Generate the source file
    source_lines_0 = ['']
    source_lines_1 = []
    for name, props in core_api.functions.items():
        name_no_prefix = name.replace('YOGI_', '')
        mock_fn_var = f'mock_{name_no_prefix}_fn'
        default_impl = f'default_{name_no_prefix}' if name in fns_with_default_impl else '{}'
        default_ret_val = '""' if props.return_type == 'const char*' else 'YOGI_ERR_UNKNOWN'

        source_lines_0 += [f'''
            // Mock implementation for {name}
            static std::function<decltype({name})> {mock_fn_var} = {default_impl};

            YOGI_API {make_function_signature(name, props)} {{
              std::lock_guard<std::mutex> lock(global_mock_mutex);
              if (!{mock_fn_var}) {{
                std::cout << "WARNING: Unmonitored mock function call: {name}()" << std::endl;
                return {default_ret_val};
              }}

              return {mock_fn_var}({",".join(props.args)});
            }}

            YOGI_API void MOCK_{name_no_prefix}(decltype({name}) fn) {{
              std::lock_guard<std::mutex> lock(global_mock_mutex);
              {mock_fn_var} = fn ? fn : decltype({mock_fn_var}){{}};
            }}
        ''']

        source_lines_1 += [f'mock_{name_no_prefix}_fn = {default_impl};']

    replace_block_in_file('yogi-core-mock/src/yogi_core_mock.cc', source_lines_0, block_idx=0)
    replace_block_in_file('yogi-core-mock/src/yogi_core_mock.cc', source_lines_1, block_idx=1)


def generate_cmake_lists_txt(core_api: munch.Munch) -> None:
    """Replaces the code in the CMakeLists.txt file"""
    version_lines = [f'set(version "{VERSION}")']

    replace_block_in_file('yogi-core-mock/CMakeLists.txt', version_lines)
