# SyLife Logger Module Tests
cmake_minimum_required(VERSION 3.20)

# Find Google Test
find_package(GTest REQUIRED)

# Test source files
set(LOGGER_TEST_SOURCES
    test_logger_interfaces.cpp
    test_multi_sink_logger.cpp
    test_console_sink.cpp
    test_file_sink.cpp
    test_default_formatter.cpp
    test_integration.cpp
)

# Create test executable
add_executable(sylife_logger_tests ${LOGGER_TEST_SOURCES})

# Link dependencies
target_link_libraries(sylife_logger_tests
    PRIVATE
        sylife_logger
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
)

# Set C++20 standard
target_compile_features(sylife_logger_tests PRIVATE cxx_std_20)

# Include directories
target_include_directories(sylife_logger_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Enable all warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(sylife_logger_tests PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(sylife_logger_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    PROPERTIES
        LABELS "logger"
)

# Create a test that runs with different configurations
add_test(NAME logger_unit_tests COMMAND sylife_logger_tests)
add_test(NAME logger_stress_test COMMAND sylife_logger_tests --gtest_repeat=10)

# Set test properties
set_tests_properties(logger_unit_tests PROPERTIES
    TIMEOUT 60
    LABELS "unit;logger"
)

set_tests_properties(logger_stress_test PROPERTIES
    TIMEOUT 300
    LABELS "stress;logger"
)