import os
from conans import ConanFile, CMake, tools


class YogiCppConan(ConanFile):
    name = "yogi-cpp"

    # :CODEGEN_BEGIN:
    version = "0.0.1-alpha"
    # :CODEGEN_END:

    license = "LGPL-3.0-only"
    url = "https://github.com/yohummus/yogi-framework"
    description = "C++ bindings for the Yogi Framework"
    settings = "os", "compiler", "build_type", "arch"
    options = {"build_tests": [True, False]}
    default_options = {"build_tests": True}
    generators = "cmake"
    build_requires = f"yogi-core-mock/{version}", "cmake/3.18.4", "gtest/1.10.0", "nlohmann_json/3.9.1", "msgpack/3.3.0"
    exports_sources = "test/*", "include/*", "CMakeLists.txt"

    def build(self):
        if not self.options.build_tests:
            return  # This is a header-only library

        # Build and run the tests
        cmake = CMake(self)
        cmake.configure(source_dir=self.source_folder)
        cmake.build()

        self.run(f"{self.build_folder}/bin/yogi-cpp-test")

    def package(self):
        self.copy("include/*.h")
