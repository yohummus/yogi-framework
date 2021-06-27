#!/usr/bin/env python3
# PYTHON_ARGCOMPLETE_OK

import argparse
import argcomplete
import sys
import pathlib
import os
import subprocess
from typing import Any

ROOT = pathlib.Path(__file__).parent.resolve()
BUILD_DIR = ROOT / 'build'


def run_bootstrap(args: Any) -> None:
    for proj in ['yogi-core', 'yogi-core-mock', 'yogi-cpp', 'yogi-dotnet', 'yogi-python']:
        proj_build_dir = BUILD_DIR / proj
        proj_build_dir.mkdir(parents=True, exist_ok=True)
        subprocess.check_call(f'conan install ../../{proj}', shell=True, cwd=proj_build_dir)
        subprocess.check_call(f'conan build ../../{proj}', shell=True, cwd=proj_build_dir)
        subprocess.check_call(f'conan export-pkg ../../{proj} --force', shell=True, cwd=proj_build_dir)


def run_codegen(args: Any) -> None:
    subprocess.check_call('python -m codegen', shell=True, cwd=ROOT)


def run_build(args: Any) -> None:
    proj_map = {'build-core': 'yogi-core',
                'build-mock': 'yogi-core-mock',
                'build-cpp': 'yogi-cpp',
                'build-py': 'yogi-python',
                'build-net': 'yogi-dotnet'}

    projects = proj_map.values() if args.command == 'build' else [proj_map[args.command]]
    for proj in projects:
        subprocess.check_call(f'conan build ../../{proj}', shell=True, cwd=BUILD_DIR / proj)
        subprocess.check_call(f'conan export-pkg ../../{proj} --force', shell=True, cwd=BUILD_DIR / proj)


def parse_cmdline() -> Any:
    parser = argparse.ArgumentParser(description='Helper tools for development')

    subparsers = parser.add_subparsers(dest='command', help='command to execute')
    subparsers.required = True

    cmd_bootstrap = subparsers.add_parser('bootstrap', help='Setup the development environment from scratch')
    cmd_codegen = subparsers.add_parser('codegen', help='Run code generation for all sub-projects')
    cmd_build = subparsers.add_parser('build', help='Build all sub-projects')
    cmd_build_core = subparsers.add_parser('build-core', help='Build yogi-core')
    cmd_build_mock = subparsers.add_parser('build-mock', help='Build yogi-core-mock')
    cmd_build_cpp = subparsers.add_parser('build-cpp', help='Build yogi-cpp')
    cmd_build_py = subparsers.add_parser('build-py', help='Build yogi-python')
    cmd_build_net = subparsers.add_parser('build-net', help='Build yogi-dotnet')

    argcomplete.autocomplete(parser)
    args = parser.parse_args()
    return args


def main() -> None:
    args = parse_cmdline()

    if args.command == 'bootstrap':
        run_bootstrap(args)
    elif args.command == 'codegen':
        run_codegen(args)
    elif args.command.startswith('build'):
        run_build(args)


if __name__ == '__main__':
    try:
        main()
    except subprocess.CalledProcessError as e:
        print(f'ERROR: {e}')
        sys.exit(1)
