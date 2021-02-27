"""
Auto-generates the code for all sub-projects

To run code-generation for specific sub-projects only, list them on the command line, e.g.:
python codegen yogi-core yogi-cpp
"""

import munch
import pathlib
import sys
import importlib

CODEGEN_DIR = pathlib.Path(__file__).parent

# Parse the core api YAML file
with open(CODEGEN_DIR / 'core_api.yaml') as f:
    core_api = munch.Munch().fromYAML(f)

# Generate the code for the various sub-projects
mods_from_cmdline = ['gen_' + x.replace('-', '_') for x in sys.argv[1:]]
all_mods = [x.stem for x in CODEGEN_DIR.glob('gen_*.py')]

for mod in mods_from_cmdline or all_mods:
    importlib.import_module(f'codegen.{mod}').generate(core_api)
