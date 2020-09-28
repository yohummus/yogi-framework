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

    def export_sources(self):
        self.copy("src/*")
        self.copy("include/*")
        self.copy("../yogi-core/include/yogi_core.h", dst="include")
        self.copy("CMakeLists.txt")

    @property
    def lib_path(self):
        lut = {
            "Macos": f"lib/lib{self.name}.{self.version}.dylib",
            "Windows": f"bin/lib{self.name}.{self.version}.dll",
            "Linux": f"lib/lib{self.name}.so.{self.version}",
        }
        return lut.get(tools.detected_os(), lut["Linux"])

    def package_info(self):
        self.env_info.YOGI_CORE_LIBRARY = os.path.join(self.package_folder, self.lib_path)
        self.cpp_info.includedirs = ['include']
        self.cpp_info.libs = [self.name]

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_dir=self.source_folder)
        cmake.build()

    def package(self):
        self.copy("include/*.h")
        self.copy("../yogi-core/include/yogi_core.h",  dst="include")
        self.copy(self.lib_path)
