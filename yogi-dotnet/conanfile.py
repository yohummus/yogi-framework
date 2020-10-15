import os
import sys
import re
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
    exports_sources = "yogi/*", "test/*", "*.targets", "*.sln"

    @property
    def target_framework(self):
        with open(f"{self.source_folder}/yogi/yogi.csproj", "r") as f:
            content = f.read()

        return re.search(r'<TargetFramework>(.*)</TargetFramework>', content).group(1)

    def build(self):
        self.run(f"dotnet build --configuration {self.settings.build_type}", cwd=self.source_folder)

        if self.options.build_tests:
            self.run(f"dotnet test --no-restore --configuration {self.settings.build_type}", cwd=self.source_folder)

    def package(self):
        print(f"yogi/bin/{self.settings.build_type}/{self.target_framework}")
        self.copy("*", src=f"yogi/bin/{self.settings.build_type}/{self.target_framework}", dst="lib")
