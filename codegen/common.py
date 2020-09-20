import munch
import pathlib
import subprocess
from typing import List

ROOT = pathlib.Path(__file__).parent.parent


def generate_copyright_headers(core_api: munch.Munch, rel_dir_path: str) -> None:
    """Replaces the copyright headers at the beginning of over source file"""
    copyright_text = ''.join([f' * {x}'.rstrip() + '\n' for x in core_api.copyright.strip().split('\n')])

    file_extensions = ['h', 'cc']
    files = [file for ext in file_extensions for file in (ROOT / rel_dir_path).rglob(f'*.{ext}')]

    for file in files:
        with open(file, 'r') as f:
            content = f.read()

        new_content = f'/*\n{copyright_text} {content[content.index("*/"):]}'

        if new_content != content:
            print(f'Updating copyright header in {file}...')
            with open(file, 'w') as f:
                f.write(new_content)


def replace_in_file(rel_file_path: str, lines: List[str], *, block_idx: int = 0) -> None:
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
    else:
        formatted_new_content = new_content

    with open(file, 'w') as f:
        f.write(formatted_new_content)
