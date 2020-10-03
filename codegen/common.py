import munch
import pathlib
import subprocess
import itertools
from typing import List

ROOT = pathlib.Path(__file__).parent.parent

with open(ROOT / 'version.txt') as f:
    VERSION = f.read().strip()
    VERSION_MAJOR = int(VERSION.split('.')[0])
    VERSION_MINOR = int(VERSION.split('.')[1])
    VERSION_PATCH = int(VERSION.split('.')[2].split('-')[0])
    VERSION_SUFFIX = VERSION[VERSION.index('-'):] if '-' in VERSION else ''


def generate_copyright_headers(core_api: munch.Munch, rel_dir_path: str) -> None:
    """Replaces the copyright headers at the beginning of over source file"""
    raw_copyright_lines = core_api.copyright.strip().split('\n')
    file_extensions = ['h', 'cc', 'cs', 'py']
    files = [file for ext in file_extensions for file in (ROOT / rel_dir_path).rglob(f'*.{ext}')]
    files = [x for x in files if x.name not in ['conanfile.py'] and 'obj' not in x.parts]

    for file in files:
        print(f'Updating copyright header in {file}...')
        with open(file, 'r') as f:
            content = f.read()

        if file.suffix in ['.h', '.cc', '.cs']:
            copyright_text = ''.join([f' * {x}'.rstrip() + '\n' for x in raw_copyright_lines])
            new_content = f'/*\n{copyright_text} */{content[content.index("*/") + 2:]}'
        elif file.suffix == '.py':
            shebang = (content[:content.index('\n')] + '\n\n') if content.startswith('#!') else ''
            copyright_text = ''.join([f'# {x}'.rstrip() + '\n' for x in raw_copyright_lines])
            code_content = '\n'.join(itertools.dropwhile(lambda x: not x or x.startswith('#'), content.split('\n')))
            new_content = f'{shebang}{copyright_text}\n{code_content}'.rstrip() + '\n'
        else:
            new_content = content

        if new_content != content:
            with open(file, 'w') as f:
                f.write(new_content)


def replace_block_in_file(rel_file_path: str, lines: List[str], *, block_idx: int = 0) -> None:
    """Replaces the block_idx'th code block marked by the :CODEGEN_BEGIN: and :CODEGEN_END: tags in the given file with
    the given code"""
    file = ROOT / rel_file_path
    print(f'Generating {file}...')

    with open(file, 'r') as f:
        content = f.read()

    end = 0
    for _ in range(block_idx + 1):
        begin = content.find('\n', content.index(':CODEGEN_BEGIN:', end)) + 1
        end = content.rfind('\n', begin, content.index(':CODEGEN_END:', begin))

    assert begin != -1 and end != -1, 'CODEGEN tags not found'
    new_content = content[:begin] + '\n'.join(lines) + content[end:]

    if file.suffix in ['.h', '.cc']:
        formatted_new_content = subprocess.check_output(['clang-format'], input=new_content, text=True, cwd=file.parent)
    elif file.suffix == '.py':
        formatted_new_content = subprocess.check_output(['autopep8', '--max-line-length=120', '-'], input=new_content,
                                                        text=True)
    else:
        formatted_new_content = new_content

    if formatted_new_content != content:
        with open(file, 'w') as f:
            f.write(formatted_new_content)


def generate_conanfile_py(project_dir_name: str) -> None:
    """Replaces the code in the conanfile.py file"""
    lines = [f'    version = "{VERSION}"']
    replace_block_in_file(f'{project_dir_name}/conanfile.py', lines)
