import os
import sys
from conans import ConanFile, CMake, tools


class YogiDotnetConan(ConanFile):
    name = "yogi-dotnet"

    # :CODEGEN_BEGIN:
    version = "0.0.1-alpha"
    # :CODEGEN_END:

    license = "LGPL-3.0-only"
    url = "https://github.com/yohummus/yogi-framework"
    description = ".NET Core bindings for the Yogi Framework"
    settings = "os", "compiler", "build_type", "arch"
    options = {"build_tests": [True, False]}
    default_options = {"build_tests": True}
    generators = "virtualenv"
    build_requires = f"yogi-core-mock/{version}"
    requires = f"yogi-core/{version}"
    exports_sources = "yogi/*", "test/*", "*.targets", ".sln"

    def build(self):
        build_type = self.settings.get_safe("build_type", default="Release")

        self.run(f"dotnet build --configuration {build_type}", cwd=self.source_folder)

        if self.options.build_tests:
            self.run(f"dotnet test --no-restore --configuration {build_type}", cwd=self.source_folder)

    def package(self):
        pass
