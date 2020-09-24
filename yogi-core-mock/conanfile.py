import os
from conans import ConanFile, CMake, tools


class YogiCoreMockConan(ConanFile):
    name = "yogi-core-mock"

    # :CODEGEN_BEGIN:
    version = "0.0.1-alpha"
    # :CODEGEN_END:

    license = "LGPL-3.0-only"
    url = "https://github.com/yohummus/yogi-framework"
    description = "Core library mock of the Yogi Framework for testing purposes"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "cmake_find_package", "virtualenv"
    build_requires = "cmake/3.18.2"
    requires = f"yogi-core/{version}"

    def package_info(self):
        lib_prefix = "" if tools.detected_os() == "Windows" else "lib"
        lib_suffix = {"Macos": "dylib", "Windows": "dll"}.get(tools.detected_os(), "so")
        lib_filename = f"{lib_prefix}yogi-core-mock.{lib_suffix}"
        self.env_info.YOGI_CORE_LIBRARY = os.path.join(self.cpp_info.lib_paths[0], lib_filename)
        self.cpp_info.libs = ["yogi-core-mock"]

    def export_sources(self):
        self.copy("src/*")
        self.copy("include/*")
        self.copy("../yogi-core/include/yogi_core.h",  dst="include")
        self.copy("CMakeLists.txt")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("include/*.h")
        self.copy("lib/*.so", symlinks=True)
        self.copy("lib/*.dylib", symlinks=True)
        self.copy("lib/*.dll", symlinks=True)
