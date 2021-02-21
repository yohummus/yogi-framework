project(yogi-cpp)

# Integrate Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Compiler and linker flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include .)


# ========== TESTS ==========
set(test_files
    # :CODEGEN_BEGIN:
    test/common.cc
    test/yogi_test.cc
    test/yogi/io_test.cc
    test/yogi/object_test.cc
    test/yogi/signals_test.cc
    test/yogi/payload_view_test.cc
    test/yogi/constants_test.cc
    test/yogi/json_view_test.cc
    test/yogi/msgpack_view_test.cc
    test/yogi/configuration_test.cc
    test/yogi/json_test.cc
    test/yogi/context_test.cc
    test/yogi/version_test.cc
    test/yogi/operation_id_test.cc
    test/yogi/timestamp_test.cc
    test/yogi/enums_test.cc
    test/yogi/timer_test.cc
    test/yogi/duration_test.cc
    test/yogi/logging_test.cc
    test/yogi/uuid_test.cc
    test/yogi/branch_test.cc
    test/yogi/string_view_test.cc
    test/yogi/schemas_test.cc
    test/yogi/errors_test.cc
    test/yogi_inc/schemas_inc_test.cc
    test/yogi_inc/timestamp_inc_test.cc
    test/yogi_inc/branch_inc_test.cc
    test/yogi_inc/version_inc_test.cc
    test/yogi_inc/timer_inc_test.cc
    test/yogi_inc/json_view_inc_test.cc
    test/yogi_inc/operation_id_inc_test.cc
    test/yogi_inc/io_inc_test.cc
    test/yogi_inc/errors_inc_test.cc
    test/yogi_inc/json_inc_test.cc
    test/yogi_inc/duration_inc_test.cc
    test/yogi_inc/string_view_inc_test.cc
    test/yogi_inc/object_inc_test.cc
    test/yogi_inc/uuid_inc_test.cc
    test/yogi_inc/constants_inc_test.cc
    test/yogi_inc/enums_inc_test.cc
    test/yogi_inc/signals_inc_test.cc
    test/yogi_inc/configuration_inc_test.cc
    test/yogi_inc/buffer_inc_test.cc
    test/yogi_inc/payload_view_inc_test.cc
    test/yogi_inc/logging_inc_test.cc
    test/yogi_inc/context_inc_test.cc
    test/yogi_inc/msgpack_view_inc_test.cc
    # :CODEGEN_END:
)

enable_testing()
include(GoogleTest)

add_executable(yogi-cpp-test ${test_files})
target_link_libraries(yogi-cpp-test ${CMAKE_DL_LIBS} ${CONAN_LIBS})

gtest_discover_tests(yogi-cpp-test)
