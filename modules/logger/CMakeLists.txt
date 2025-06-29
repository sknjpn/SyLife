# SyLife Logger Module
cmake_minimum_required(VERSION 3.20)

project(sylife_logger VERSION 1.0.0 LANGUAGES CXX)

# Source files
set(LOGGER_SOURCES
    src/multi_sink_logger.cpp
    src/console_sink.cpp
    src/file_sink.cpp
    src/default_formatter.cpp
)

# Header files
set(LOGGER_HEADERS
    include/sylife/logger/i_logger.h
    include/sylife/logger/i_log_sink.h
    include/sylife/logger/i_log_formatter.h
    include/sylife/logger/multi_sink_logger.h
    include/sylife/logger/console_sink.h
    include/sylife/logger/file_sink.h
    include/sylife/logger/default_formatter.h
)

# Create the logger library
add_library(sylife_logger ${LOGGER_SOURCES} ${LOGGER_HEADERS})

# Set C++20 standard
target_compile_features(sylife_logger PUBLIC cxx_std_20)

# Include directories
target_include_directories(sylife_logger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Dependencies
target_link_libraries(sylife_logger
    PUBLIC
        sylife_core
        sylife_di
)

# Platform-specific libraries
if(WIN32)
    # Windows-specific libraries if needed
elseif(UNIX)
    # Unix-specific libraries
    find_package(Threads REQUIRED)
    target_link_libraries(sylife_logger PRIVATE Threads::Threads)
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(sylife_logger PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wno-error=unused-parameter
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(sylife_logger PRIVATE
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

# Enable benchmarks if requested
if(SYLIFE_BUILD_BENCHMARKS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks)
    add_subdirectory(benchmarks)
endif()

# Installation
install(TARGETS sylife_logger EXPORT sylife_logger_targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT sylife_logger_targets
    FILE sylife_logger_targets.cmake
    NAMESPACE sylife::
    DESTINATION lib/cmake/sylife_logger
)