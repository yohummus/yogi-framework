from conans import ConanFile, CMake


class YogiCoreConan(ConanFile):
    name = "yogi-core"
    license = "LGPL-3.0-only"
    url = "https://github.com/yohummus/yogi-framework"
    description = "Core library of the Yogi Framework"
    settings = "os", "compiler", "build_type", "arch"
    options = {"build_tests": [True, False]}
    default_options = {"build_tests": True}
    generators = "cmake", "cmake_find_package", "virtualrunenv"
    build_requires = "cmake/3.18.2", "gtest/1.10.0"
    requires = "boost/1.74.0", "nlohmann_json/3.9.1", "json-schema-validator/2.1.0"

    # :CODEGEN_BEGIN:
    version = "0.0.1-alpha"
    # :CODEGEN_END:

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        if self.options.build_tests:
            cmake.test()

    def imports(self):
        self.copy("license*", dst="licenses", folder=True, ignore_case=True)
