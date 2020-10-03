import munch
import stringcase

from common import ROOT, VERSION, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_SUFFIX
from common import replace_block_in_file
from common import generate_copyright_headers
from common import generate_conanfile_py


def generate(core_api: munch.Munch) -> None:
    """Replaces the code in yogi-dotnet from the loaded core API YAML file"""
    generate_library_cs()
    generate_version_cs()
    generate_yogi_csproj()
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
