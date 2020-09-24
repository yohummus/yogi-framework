import munch
import stringcase
import textwrap

from common import ROOT, VERSION, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_SUFFIX
from common import replace_block_in_file
from common import generate_copyright_headers
from common import generate_conanfile_py
from gen_yogi_core import make_function_signature


def generate(core_api: munch.Munch) -> None:
    """Replaces the code in yogi-cpp from the loaded core API YAML file"""
    generate_yogi_h()
    generate_api_h(core_api)
    generate_library_h(core_api)
    generate_constants_h(core_api)
    generate_version_h()
    generate_enums_h(core_api)
    generate_test_common_h_cc(core_api)
    generate_test_yogi_inc_files()  # Must come before copyright and cmake generation
    generate_copyright_headers(core_api, 'yogi-cpp')
    generate_cmake_lists_txt(core_api)
    generate_conanfile_py('yogi-cpp')


def generate_yogi_h() -> None:
    """Replaces the code in the yogi.h file"""
    header_lines = [f'#include "yogi/{x.name}"' for x in sorted((ROOT / 'yogi-cpp/include/yogi').glob('*.h'))]

    replace_block_in_file('yogi-cpp/include/yogi.h', header_lines)


def generate_api_h(core_api: munch.Munch) -> None:
    """Replaces the code in the api.h file"""
    header_lines = []
    for name, props in core_api.functions.items():
        header_lines += [f'''
            // {name}
            _YOGI_WEAK_SYMBOL {make_function_signature(f'(*{name})', props)}
            = Library::get_function_address<{make_function_signature('(*)',  props)}>("{name}");
        ''']

    replace_block_in_file('yogi-cpp/include/yogi/detail/api.h', header_lines)


def generate_library_h(core_api: munch.Munch) -> None:
    """Replaces the code in the library.h file"""
    header_lines = [f'const char* bindings_version = "{VERSION}";']

    replace_block_in_file('yogi-cpp/include/yogi/detail/library.h', header_lines)


def generate_constants_h(core_api: munch.Munch) -> None:
    """Replaces the code in the constants.h file"""
    names = [f'k{stringcase.pascalcase(x.lower())}' for x in core_api.constants]

    max_c_type_len = max([len(x.type.c) for x in core_api.constants.values()])
    max_cpp_type_len = max([len(x.type.cpp) for x in core_api.constants.values()])
    max_name_len = max([len(x) for x in names])

    header_lines_0 = []
    header_lines_1 = []
    for val, (name, props) in enumerate(zip(names, core_api.constants.values()), start=1):
        padded_cpp_type = props.type.cpp.ljust(max_cpp_type_len)
        padded_name_and_semicolon = f'{name};'.ljust(max_name_len + 1)
        header_lines_0 += [f'  static const {padded_cpp_type} {padded_name_and_semicolon}  ///< {props.help}']

        padded_c_type = props.type.c.ljust(max_c_type_len)
        padded_cpp_type_and_comma = f'{props.type.cpp},'.ljust(max_cpp_type_len + 1)
        padded_name = name.ljust(max_name_len)
        header_lines_1 += [f'_YOGI_WEAK_SYMBOL const {padded_cpp_type} constants::{padded_name}'
                           f' = detail::get_core_constant<{padded_cpp_type_and_comma} {padded_c_type}>({val:2d});']

    replace_block_in_file('yogi-cpp/include/yogi/constants.h', header_lines_0, block_idx=0)
    replace_block_in_file('yogi-cpp/include/yogi/constants.h', header_lines_1, block_idx=1)


def generate_version_h() -> None:
    """Replaces the code in the version.h file"""
    version_lines = [f'''
        /// Whole version string of the bindings
        #define YOGI_BINDINGS_VERSION "{VERSION}"

        /// Major version number of the bindings
        #define YOGI_BINDINGS_VERSION_MAJOR {VERSION_MAJOR}

        /// Minor version number of the bindings
        #define YOGI_BINDINGS_VERSION_MINOR {VERSION_MINOR}

        /// Patch version number of the bindings
        #define YOGI_BINDINGS_VERSION_PATCH {VERSION_PATCH}

        /// Version suffix of the bindings
        #define YOGI_BINDINGS_VERSION_SUFFIX "{VERSION_SUFFIX}"
    ''']

    replace_block_in_file('yogi-cpp/include/yogi/version.h', version_lines)


def generate_enums_h(core_api: munch.Munch) -> None:
    """Replaces the code in the enums.h file"""
    header_line_blocks = []

    # Normal enums
    for enum, elems in core_api.enums.items():
        names = [f'k{stringcase.pascalcase(str(x).lower())}' for x in elems]
        max_name_len = max([len(x) for x in names])

        # Enum class members
        if enum == 'error_code':
            values = range(0, -len(elems), -1)
        elif enum == 'verbosity':
            values = range(-1, len(elems) - 1)
        elif enum == 'http_status':
            values = elems.keys()
        else:
            values = range(len(elems))

        lines = [f'  {name:{max_name_len}} = {val:3d}, ///< {comment}'
                 for val, name, comment in zip(values, names, elems.values())]

        header_line_blocks += [lines]

        # Case statements for the to_string() function
        padded_names_and_colons = [f'{x}:'.ljust(max_name_len + 1) for x in names]

        lines = [f'  case {stringcase.pascalcase(enum)}::{padded_name_and_colon} return "{name}";'
                 for padded_name_and_colon, name in zip(padded_names_and_colons, names)]

        header_line_blocks += [lines]

    # Flags
    for enum, flags in core_api.flags.items():
        names = [f'k{stringcase.pascalcase(str(x).lower())}' for x in flags]
        max_name_len = max([len(x) for x in names])

        # Enum class members
        lines = []
        for name, props in zip(names, flags.values()):
            lines += ['']
            lines += [f'//! {x}' for x in props.help.split('\n')]

            if 'bit' in props:
                lines += [f'{name} = {0 if props.bit is None else f"(1 << {props.bit})"},']
            else:
                combine_names = [f'k{stringcase.pascalcase(str(x).lower())}' for x in props.combine]
                lines += [f'{name} = ({" | ".join(combine_names)}),']

        # Operators
        enum_name = stringcase.pascalcase(enum)

        lines += [f'''
            }};

            inline {enum_name} operator~({enum_name} flags) {{
              return static_cast<{enum_name}>(~static_cast<int>(flags));
            }}

            inline {enum_name} operator|({enum_name} a, {enum_name} b) {{
              return static_cast<{enum_name}>(static_cast<int>(a) | static_cast<int>(b));
            }}

            inline {enum_name} operator&({enum_name} a, {enum_name} b) {{
              return static_cast<{enum_name}>(static_cast<int>(a) & static_cast<int>(b));
            }}

            inline {enum_name}& operator|=({enum_name}& a, {enum_name} b) {{
              return a = a | b;
            }}

            inline {enum_name}& operator&=({enum_name}& a, {enum_name} b) {{
              return a = a & b;
            }}
        '''.lstrip()]

        # Specialization of the to_string() function
        lines += [f'''
            template <>
            inline std::string to_string<{enum_name}>({enum_name} flags) {{
              std::string ss;
        ''']

        for name in reversed([x for x in names if x != 'kNone']):
            lines += [f'''
                if ((flags & {enum_name}::{name}) == {enum_name}::{name}) {{
                    flags = flags & ~{enum_name}::{name};
                    ss = std::string{{"{name} | "}} + ss;
                }}
            ''']

        lines += [f'''
              return ss.empty() ? "kNone" : ss.substr(0, ss.size() - 3);
            }}
        ''']

        header_line_blocks += [lines]

    for block_idx, lines in enumerate(header_line_blocks):
        replace_block_in_file('yogi-cpp/include/yogi/enums.h', lines, block_idx=block_idx)


def generate_test_common_h_cc(core_api: munch.Munch) -> None:
    """Replaces the code in the test/common.h test/common.cc files"""
    header_lines = []
    source_lines = []
    for name, props in core_api.functions.items():
        mock_name = name.replace('YOGI_', 'MOCK_')
        fn_sig = make_function_signature(f"(*fn)", props)

        header_lines += [
            f'  static void (*{mock_name})({fn_sig});'
        ]

        source_lines += [
            f'void (*Test::{mock_name})({fn_sig})',
            f' = detail::Library::get_function_address<void (*)({fn_sig})>("{mock_name}");',
            f''
        ]

    replace_block_in_file('yogi-cpp/test/common.h', header_lines)
    replace_block_in_file('yogi-cpp/test/common.cc', source_lines)


def generate_test_yogi_inc_files() -> None:
    """Generates the header include test files in test/yogi_inc"""
    for header_file in (ROOT / 'yogi-cpp/include/yogi').glob('*.h'):
        test_file = ROOT / f'yogi-cpp/test/yogi_inc/{header_file.stem}_inc_test.cc'
        if test_file.exists():
            print(f'Skipping {test_file} since it already exists')
            continue

        print(f'Generating {test_file}...')
        content = textwrap.dedent(f'''
            /*
            */

            #include <gtest/gtest.h>

            // Test if the following file can be included without resulting in compiler errors
            #include <yogi/{header_file.name}>

            TEST(HeaderIncludeTest, {header_file.stem}) {{
                SUCCEED();
            }}
        ''')

        with open(test_file, 'w') as f:
            f.write(content)


def generate_cmake_lists_txt(core_api: munch.Munch) -> None:
    """Replaces the code in the CMakeLists.txt file"""
    test_lines = []
    for file in (ROOT / 'yogi-cpp/test').rglob('*.cc'):
        test_lines += [f'    {file.relative_to(ROOT  / "yogi-cpp")}']

    replace_block_in_file('yogi-cpp/CMakeLists.txt', test_lines)
