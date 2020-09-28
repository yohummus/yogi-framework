project(yogi-core-mock)

# :CODEGEN_BEGIN:
set(version "0.0.1-alpha")
# :CODEGEN_END:

# Integrate Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Compiler and linker flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include .)


# ========== SHARED LIBRARY ==========
set(src_files
    src/yogi_core_mock.cc
)

add_library(yogi-core-mock SHARED ${src_files})

set_target_properties(yogi-core-mock PROPERTIES VERSION ${version})


if (WIN32)
    set_target_properties (yogi-core-mock PROPERTIES
        OUTPUT_NAME "libyogi-core-mock.${version}"
    )
endif ()
