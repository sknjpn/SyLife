# SyLife Core Module
cmake_minimum_required(VERSION 3.20)

project(sylife_core VERSION 1.0.0 LANGUAGES CXX)

# Create the core library
add_library(sylife_core INTERFACE)

# Set C++20 standard
target_compile_features(sylife_core INTERFACE cxx_std_20)

# Include directories
target_include_directories(sylife_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(sylife_core INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wno-error=unused-parameter
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(sylife_core INTERFACE
        /W4
        /WX
        /permissive-
    )
endif()

# Enable testing if requested
if(SYLIFE_BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(TARGETS sylife_core EXPORT sylife_core_targets)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT sylife_core_targets
    FILE sylife_core_targets.cmake
    NAMESPACE sylife::
    DESTINATION lib/cmake/sylife_core
)