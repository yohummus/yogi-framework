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
    options = {"build_tests": [True, False], "gtest_options": "ANY"}
    default_options = {"build_tests": True, "gtest_options": ""}
    generators = "cmake", "virtualenv"
    build_requires = "cmake/3.18.2", "gtest/1.10.0"
    requires = "boost/1.74.0", "nlohmann_json/3.9.1", "json-schema-validator/2.1.0", "msgpack/3.3.0", "openssl/1.1.1g"
    exports_sources = "src/*", "test/*", "include/*", "CMakeLists.txt"

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
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libs = [os.path.basename(self.lib_path)]

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_dir=self.source_folder)
        cmake.build()

        if self.options.build_tests:
            self.run(f"{self.build_folder}/bin/yogi-core-test {self.options.gtest_options}",
                     cwd=f"{self.build_folder}/lib")

    def package(self):
        self.copy("include/*.h")
        self.copy(self.lib_path)

    def imports(self):
        self.copy("license*", dst="licenses", folder=True, ignore_case=True)
