import munch

from common import replace_in_file
from common import generate_copyright_headers
from gen_yogi_core import make_function_signature


def generate(core_api: munch.Munch) -> None:
    """Replaces the code in yogi-core-mock from the loaded core API YAML file"""
    generate_copyright_headers(core_api, 'yogi-core-mock')
    generate_yogi_core_mock_h_cc(core_api)
    generate_cmake_lists_txt(core_api)
    generate_conanfile_py(core_api)


def generate_yogi_core_mock_h_cc(core_api: munch.Munch) -> None:
    """Replaces the code in the yogi_core_mock.h and yogi_core_mock.cc files"""
    header_lines = ['']
    source_lines_0 = ['']
    source_lines_1 = []
    for name, props in core_api.functions.items():
        name_no_prefix = name.replace('YOGI_', '')
        mock_fn_var = f'mock_{name_no_prefix}_fn'

        header_lines += [f'''
            // {name}
            YOGI_API {make_function_signature(name, props)};
            YOGI_API void MOCK_{name_no_prefix}(decltype({name}) fn);
        ''']

        source_lines_0 += [f'''
            // Mock implementation for {name}
            static std::function<decltype({name})> {mock_fn_var};

            YOGI_API {make_function_signature(name, props)} {{
              std::lock_guard<std::mutex> lock(global_mock_mutex);
              if (!{mock_fn_var}) {{
                std::cout << "WARNING: Unmonitored mock function call: {name}()" << std::endl;
                return {{}};
              }}

              return {mock_fn_var}({",".join(props.args)});
            }}

            YOGI_API void MOCK_{name_no_prefix}(decltype({name}) fn) {{
              std::lock_guard<std::mutex> lock(global_mock_mutex);
              {mock_fn_var} = fn ? fn : decltype({mock_fn_var}){{}};
            }}
        ''']

        source_lines_1 += [f'mock_{name_no_prefix}_fn = {{}};']

    replace_in_file('yogi-core-mock/src/yogi_core_mock.h', header_lines)
    replace_in_file('yogi-core-mock/src/yogi_core_mock.cc', source_lines_0, block_idx=0)
    replace_in_file('yogi-core-mock/src/yogi_core_mock.cc', source_lines_1, block_idx=1)


def generate_cmake_lists_txt(core_api: munch.Munch) -> None:
    """Replaces the code in the CMakeLists.txt file"""
    v = core_api.version
    lines = [f'    VERSION {v.major}.{v.minor}.{v.patch}{v.suffix}',
             f'    SOVERSION {v.major}']

    replace_in_file('yogi-core-mock/CMakeLists.txt', lines)


def generate_conanfile_py(core_api: munch.Munch) -> None:
    """Replaces the code in the conanfile.py file"""
    v = core_api.version
    lines = [f'    version = "{v.major}.{v.minor}.{v.patch}{v.suffix}"']

    replace_in_file('yogi-core-mock/conanfile.py', lines)


def make_function_signature(fn_name: str, fn_props: munch.Munch) -> str:
    """Creates the function signature for the given function"""
    fn_args = []
    for arg_name, arg_type in fn_props.args.items():
        if isinstance(arg_type, munch.Munch):
            fn_args += [make_function_signature(f'(*{arg_name})', arg_type)]
        else:
            fn_args += [f'{arg_type} {arg_name}']

    return f'{fn_props.return_type} {fn_name}({",".join(fn_args)})'
