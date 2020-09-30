import os
import sys
from conans import ConanFile, CMake, tools


class YogiPythonConan(ConanFile):
    name = "yogi-python"

    # :CODEGEN_BEGIN:
    version = "0.0.1-alpha"
    # :CODEGEN_END:

    license = "LGPL-3.0-only"
    url = "https://github.com/yohummus/yogi-framework"
    description = "Python bindings for the Yogi Framework"
    settings = "os", "compiler", "build_type", "arch"
    options = {"build_tests": [True, False]}
    default_options = {"build_tests": True}
    generators = "virtualenv"
    build_requires = f"yogi-core-mock/{version}"
    requires = f"yogi-core/{version}"
    exports_sources = "yogi/*", "test/*", "requirements.txt"

    def build(self):
        if not self.options.build_tests:
            return  # No need to do anything

        commands = [f"{sys.executable} -m venv yogi-python-venv",
                    f". yogi-python-venv/bin/activate",
                    f"pip install -r {self.source_folder}/requirements.txt",
                    f"pip install pytest",
                    f"py.test {self.source_folder}"]
        self.run(" && ".join(commands))

    def package(self):
        self.copy("yogi/*.py")
