project(yogi-core)

# :CODEGEN_BEGIN:
set(version "0.0.1-alpha")
# :CODEGEN_END:

# Integrate Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Compiler and linker flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "/EHsc /permissive- /W4 /w14640")

    set(windows_version 0x0603) # Windows 8.1
    add_definitions(
        -DWINVER=${windows_version}
        -D_WIN32_WINNT=${windows_version}
        -D_SCL_SECURE_NO_WARNINGS
        -D_CRT_SECURE_NO_WARNINGS
        -D_WINSOCK_DEPRECATED_NO_WARNINGS
        -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
        -DNOMINMAX
    )
endif ()

# Include directories
include_directories(include .)


# ========== STATIC LIBRARY ==========
set(src_files
    # :CODEGEN_BEGIN:
    src/util/json_helpers.cc
    src/util/time.cc
    src/network/msg_transport.cc
    src/network/messages.cc
    src/network/tcp_listener.cc
    src/network/transport.cc
    src/network/tcp_transport.cc
    src/objects/context.cc
    src/objects/signal_set.cc
    src/objects/timer.cc
    src/objects/logger.cc
    src/objects/branch.cc
    src/objects/configuration.cc
    src/objects/configuration/cmdline_parser.cc
    src/objects/branch/connection_manager.cc
    src/objects/branch/branch_info.cc
    src/objects/branch/advertising_receiver.cc
    src/objects/branch/broadcast_manager.cc
    src/objects/branch/branch_connection.cc
    src/objects/branch/advertising_sender.cc
    src/schemas/schemas.cc
    src/system/network_info.cc
    src/system/glob.cc
    src/system/process.cc
    src/system/console.cc
    src/lib/lib_logging.cc
    src/lib/lib_configuration.cc
    src/lib/lib_signals.cc
    src/lib/lib_time.cc
    src/lib/lib_web.cc
    src/lib/lib_utilities.cc
    src/lib/lib_context.cc
    src/lib/lib_helpers.cc
    src/lib/lib_branch.cc
    src/lib/lib_objects.cc
    src/api/errors.cc
    src/api/constants.cc
    src/api/object.cc
    src/api/version.cc
    src/data/crypto.cc
    src/data/ringbuffer.cc
    src/data/base64.cc
    # :CODEGEN_END:
)

add_library(yogi-core-static STATIC ${src_files})
target_link_libraries(yogi-core-static ${CONAN_LIBS})

if(WIN32)
    target_link_libraries(yogi-core-static bcrypt.lib IPHLPAPI.lib)
endif()


# ========== SHARED LIBRARY ==========
add_library(yogi-core SHARED ${src_files})
target_link_libraries(yogi-core yogi-core-static)
set_target_properties(yogi-core PROPERTIES VERSION ${version})

if(WIN32)
    set_target_properties(yogi-core PROPERTIES OUTPUT_NAME "libyogi-core.${version}")
endif()


# ========== TESTS ==========
set(test_files
    # :CODEGEN_BEGIN:
    test/common.cc
    test/util/time_test.cc
    test/util/algorithm_test.cc
    test/util/hex_test.cc
    test/util/bind_test.cc
    test/network/tcp_transport_test.cc
    test/network/transport_test.cc
    test/network/msg_transport_test.cc
    test/network/tcp_listener_test.cc
    test/network/messages_test.cc
    test/network/serialize_test.cc
    test/objects/logger_test.cc
    test/objects/configuration_test.cc
    test/objects/context_test.cc
    test/objects/timer_test.cc
    test/objects/branch_test.cc
    test/objects/signal_set_test.cc
    test/objects/configuration/cmdline_parser_test.cc
    test/objects/branch/broadcast_manager_test.cc
    test/objects/branch/connection_manager_test.cc
    test/schemas/schemas_test.cc
    test/system/console_test.cc
    test/system/glob_test.cc
    test/system/process_test.cc
    test/system/network_info_test.cc
    test/api/object_test.cc
    test/api/constants_test.cc
    test/api/version_test.cc
    test/api/errors_test.cc
    test/data/crypto_test.cc
    test/data/base64_test.cc
    test/data/ringbuffer_test.cc
    # :CODEGEN_END:
)

enable_testing()
include(GoogleTest)

add_executable(yogi-core-test ${test_files})
target_link_libraries(yogi-core-test yogi-core)

gtest_discover_tests(yogi-core-test WORKING_DIRECTORY $<TARGET_FILE_DIR:yogi-core>)
