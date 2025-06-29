# SyLife Dependency Injection Module
cmake_minimum_required(VERSION 3.20)

project(sylife_di VERSION 1.0.0 LANGUAGES CXX)

# Create the DI library
add_library(sylife_di INTERFACE)

# Set C++20 standard
target_compile_features(sylife_di INTERFACE cxx_std_20)

# Include directories
target_include_directories(sylife_di INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Dependencies
target_link_libraries(sylife_di INTERFACE
    sylife_core
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(sylife_di INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wno-error=unused-parameter
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(sylife_di INTERFACE
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
install(TARGETS sylife_di EXPORT sylife_di_targets)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT sylife_di_targets
    FILE sylife_di_targets.cmake
    NAMESPACE sylife::
    DESTINATION lib/cmake/sylife_di
)