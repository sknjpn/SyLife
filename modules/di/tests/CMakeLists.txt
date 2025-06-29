# SyLife DI Module Tests
cmake_minimum_required(VERSION 3.20)

# Find Google Test
find_package(GTest REQUIRED)

# Test executable
add_executable(sylife_di_tests
    test_container.cpp
)

target_link_libraries(sylife_di_tests PRIVATE
    sylife_di
    GTest::gtest
    GTest::gtest_main
)

target_compile_features(sylife_di_tests PRIVATE cxx_std_20)

# Add to CTest
include(GoogleTest)
gtest_discover_tests(sylife_di_tests)