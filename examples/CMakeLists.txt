# SyLife Modular Architecture Examples
cmake_minimum_required(VERSION 3.20)

project(sylife_examples VERSION 1.0.0 LANGUAGES CXX)

# Create example executable
add_executable(modular_example modular_example.cpp)

# Set C++20 standard
target_compile_features(modular_example PRIVATE cxx_std_20)

# Link against our modular libraries
target_link_libraries(modular_example PRIVATE
    sylife_logger
    sylife_di
    sylife_core
)

# Include directories
target_include_directories(modular_example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(modular_example PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(modular_example PRIVATE
        /W4
        /permissive-
    )
endif()

# Create logs directory at build time
add_custom_command(TARGET modular_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:modular_example>/logs
    COMMENT "Creating logs directory"
)