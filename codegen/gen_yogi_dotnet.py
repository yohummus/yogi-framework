import munch
import stringcase

from common import ROOT, VERSION, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_SUFFIX
from common import replace_block_in_file
from common import generate_copyright_headers
from common import generate_conanfile_py


def generate(core_api: munch.Munch) -> None:
    """Replaces the code in yogi-python from the loaded core API YAML file"""
    generate_copyright_headers(core_api, 'yogi-dotnet')
    generate_conanfile_py('yogi-dotnet')
