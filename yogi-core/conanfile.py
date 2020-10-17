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
    build_requires = "cmake/3.18.4", "gtest/1.10.0"
    requires = "boost/1.74.0", "nlohmann_json/3.9.1", "json-schema-validator/2.1.0", "msgpack/3.3.0", "openssl/1.1.1h"
    exports_sources = "src/*", "test/*", "include/*", "CMakeLists.txt"

    def make_lib_path(self, version):
        version_ext = f".{version}" if version else ""
        lut = {
            "Macos": f"lib/lib{self.name}{version_ext}.dylib",
            "Windows": f"bin/lib{self.name}{version_ext}.dll",
            "Linux": f"lib/lib{self.name}.so{version_ext}",
        }
        return lut.get(tools.detected_os(), lut["Linux"])

    @property
    def lib_symlink_chain(self):
        return [self.make_lib_path(self.version),
                self.make_lib_path('.'.join(self.version.split('.')[:2])),
                self.make_lib_path(self.version.split('.')[0]),
                self.make_lib_path(None)]

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_dir=self.source_folder)
        cmake.build()

        # Create the chain of library version symlinks
        for symlink_target, symlink_name in zip(self.lib_symlink_chain, self.lib_symlink_chain[1:]):
            if os.path.exists(symlink_name):
                os.unlink(symlink_name)

            os.symlink(os.path.basename(symlink_target), symlink_name)

        # Run the tests
        if self.options.build_tests:
            self.run(f"{self.build_folder}/bin/yogi-core-test {self.options.gtest_options}",
                     cwd=f"{self.build_folder}/lib")

    def package(self):
        self.copy("include/*.h")
        self.copy("*.lib")

        for path in self.lib_symlink_chain:
            self.copy(path, symlinks=True)

    def package_info(self):
        self.env_info.YOGI_CORE_LIBRARY = os.path.join(self.package_folder, self.make_lib_path(self.version))
        self.cpp_info.includedirs = ["include"]
