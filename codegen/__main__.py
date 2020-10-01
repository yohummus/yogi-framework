"""
Auto-generates the code for all sub-projects

To run code-generation for specific sub-projects only, list them on the command line, e.g.:
python codegen yogi-core yogi-cpp
"""

import yaml
import munch
import pathlib
import sys

import gen_yogi_core
import gen_yogi_core_mock
import gen_yogi_cpp
import gen_yogi_python

# Parse the core api YAML file
with open(pathlib.Path(__file__).with_name('core_api.yaml')) as f:
    core_api = munch.Munch().fromYAML(f)

# Generate the code for the various sub-projects
projects = sys.argv[1:] or ['yogi_core', 'yogi-core-mock', 'yogi-cpp', 'yogi-python']
for project in projects:
    sys.modules['gen_' + project.replace('-', '_')].generate(core_api)
