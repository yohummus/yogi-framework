from conans import ConanFile, CMake


class YogiCoreMockConan(ConanFile):
    name = "yogi-core-mock"
    license = "LGPL-3.0-only"
    url = "https://github.com/yohummus/yogi-framework"
    description = "Core library mock of the Yogi Framework for testing purposes"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    build_requires = "cmake/3.18.2"

    # :CODEGEN_BEGIN:
    version = "0.0.1-alpha"
    # :CODEGEN_END:

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
