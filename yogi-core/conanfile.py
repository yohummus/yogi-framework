import os
from conans import ConanFile, CMake, tools


class YogiCoreConan(ConanFile):
    name = "yogi-core"

    # :CODEGEN_BEGIN:
    version = "0.0.1-alpha"
    # :CODEGEN_END:

    license = "LGPL-3.0-only"
    url = "https://github.com/yohummus/yogi-framework"
    description = "Core library of the Yogi Framework"
    settings = "os", "compiler", "build_type", "arch"
    options = {"build_tests": [True, False]}
    default_options = {"build_tests": True}
    generators = "cmake", "cmake_find_package", "virtualenv"
    build_requires = "cmake/3.18.2", "gtest/1.10.0"
    requires = "boost/1.74.0", "nlohmann_json/3.9.1", "json-schema-validator/2.1.0"
    exports_sources = "src/*", "test/*", "include/*", "CMakeLists.txt"

    def package_info(self):
        lib_prefix = "" if tools.detected_os() == "Windows" else "lib"
        lib_suffix = {"Macos": "dylib", "Windows": "dll"}.get(tools.detected_os(), "so")
        lib_filename = f"{lib_prefix}yogi-core.{lib_suffix}"
        self.env_info.YOGI_CORE_LIBRARY = os.path.join(self.cpp_info.lib_paths[0], lib_filename)
        self.cpp_info.libs = ["yogi-core"]

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        if self.options.build_tests:
            cmake.test()

    def package(self):
        self.copy("include/*.h")
        self.copy("lib/*.so", symlinks=True)
        self.copy("lib/*.dylib", symlinks=True)
        self.copy("lib/*.dll", symlinks=True)

    def imports(self):
        self.copy("license*", dst="licenses", folder=True, ignore_case=True)
