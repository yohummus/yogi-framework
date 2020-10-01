import yaml
import munch
import pathlib

import gen_yogi_core
import gen_yogi_core_mock
import gen_yogi_cpp
import gen_yogi_python

# Parse the core api YAML file
with open(pathlib.Path(__file__).with_name('core_api.yaml')) as f:
    core_api = munch.Munch().fromYAML(f)

# Generate the code for the various sub-projects
gen_yogi_core.generate(core_api)
gen_yogi_core_mock.generate(core_api)
gen_yogi_cpp.generate(core_api)
gen_yogi_python.generate(core_api)
