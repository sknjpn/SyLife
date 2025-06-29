# Build Optimization Configuration
# Provides advanced build optimization strategies for SyLife

# Enable modern CMake features
cmake_minimum_required(VERSION 3.20)

# Include guard
if(SYLIFE_BUILD_OPTIMIZATION_INCLUDED)
    return()
endif()
set(SYLIFE_BUILD_OPTIMIZATION_INCLUDED TRUE)

include(ProcessorCount)
include(CheckIPOSupported)

# ===============================================
# Parallel Build Configuration
# ===============================================

# Get processor count
ProcessorCount(PROCESSOR_COUNT)
if(NOT PROCESSOR_COUNT EQUAL 0)
    set(SYLIFE_PARALLEL_JOBS ${PROCESSOR_COUNT} CACHE STRING "Number of parallel build jobs")
else()
    set(SYLIFE_PARALLEL_JOBS 4 CACHE STRING "Number of parallel build jobs (fallback)")
endif()

message(STATUS "SyLife: Using ${SYLIFE_PARALLEL_JOBS} parallel build jobs")

# Set parallel build for generators that support it
if(CMAKE_GENERATOR STREQUAL "Ninja" OR CMAKE_GENERATOR STREQUAL "Unix Makefiles")
    set(CMAKE_BUILD_PARALLEL_LEVEL ${SYLIFE_PARALLEL_JOBS})
endif()

# ===============================================
# Compiler Optimization Settings
# ===============================================

# Function to set optimization flags based on compiler and build type
function(sylife_set_optimization_flags target)
    # Get build type
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(${target} PRIVATE
                -O3                    # Maximum optimization
                -march=native          # Optimize for current CPU
                -mtune=native          # Tune for current CPU
                -flto                  # Link-time optimization
                -ffast-math            # Fast math (if safe for your use case)
                -funroll-loops         # Unroll loops
                -fvectorize            # Enable vectorization
                -fslp-vectorize        # Enable SLP vectorization
            )
            
            # Additional GCC-specific optimizations
            if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
                target_compile_options(${target} PRIVATE
                    -fgraphite-identity    # Enable Graphite optimizations
                    -floop-nest-optimize   # Loop nest optimization
                    -fdevirtualize-at-ltrans # Devirtualization at LTO
                )
            endif()
            
            # Additional Clang-specific optimizations
            if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                target_compile_options(${target} PRIVATE
                    -fvectorize-loops      # Vectorize loops
                    -fslp-vectorize-aggressive # Aggressive SLP vectorization
                )
            endif()
            
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            target_compile_options(${target} PRIVATE
                /O2                    # Maximum optimization
                /Ob2                   # Inline expansion
                /Ot                    # Favor fast code
                /Oy                    # Frame pointer omission
                /GL                    # Whole program optimization
                /arch:AVX2             # Use AVX2 instructions
            )
            
            # Enable link-time code generation for MSVC
            set_target_properties(${target} PROPERTIES
                LINK_FLAGS "/LTCG"
            )
        endif()
    endif()
    
    # Debug optimization settings
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(${target} PRIVATE
                -Og                    # Optimize for debugging
                -g3                    # Maximum debug info
                -fno-omit-frame-pointer # Keep frame pointers for debugging
            )
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            target_compile_options(${target} PRIVATE
                /Od                    # Disable optimization
                /Zi                    # Debug information
                /RTC1                  # Runtime checks
            )
        endif()
    endif()
endfunction()

# ===============================================
# Link-Time Optimization (LTO)
# ===============================================

option(SYLIFE_ENABLE_LTO "Enable Link-Time Optimization" ON)

if(SYLIFE_ENABLE_LTO)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
    if(ipo_supported)
        message(STATUS "SyLife: Link-Time Optimization enabled")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "SyLife: LTO not supported: ${ipo_error}")
    endif()
endif()

# ===============================================
# Precompiled Headers
# ===============================================

option(SYLIFE_ENABLE_PCH "Enable Precompiled Headers" ON)

if(SYLIFE_ENABLE_PCH AND CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
    message(STATUS "SyLife: Precompiled Headers enabled")
    
    # Function to add precompiled headers to target
    function(sylife_add_precompiled_headers target)
        target_precompile_headers(${target} PRIVATE
            # Standard library headers
            <iostream>
            <memory>
            <vector>
            <string>
            <unordered_map>
            <algorithm>
            <chrono>
            <thread>
            <mutex>
            <atomic>
            <functional>
            <type_traits>
            <concepts>
            <ranges>
            
            # Project common headers
            $<$<BOOL:${SYLIFE_BUILD_DESKTOP}>:${CMAKE_SOURCE_DIR}/include/sylife/core/common.h>
        )
    endfunction()
else()
    # Dummy function if PCH not supported
    function(sylife_add_precompiled_headers target)
        # Do nothing
    endfunction()
endif()

# ===============================================
# Build Cache Configuration
# ===============================================

option(SYLIFE_ENABLE_CCACHE "Enable ccache for faster rebuilds" ON)

if(SYLIFE_ENABLE_CCACHE)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        message(STATUS "SyLife: ccache found: ${CCACHE_PROGRAM}")
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
        set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
        
        # Configure ccache for optimal performance
        execute_process(COMMAND ${CCACHE_PROGRAM} --set-config=max_size=2G)
        execute_process(COMMAND ${CCACHE_PROGRAM} --set-config=compression=true)
        execute_process(COMMAND ${CCACHE_PROGRAM} --set-config=sloppiness=pch_defines,time_macros)
    else()
        message(STATUS "SyLife: ccache not found")
    endif()
endif()

# ===============================================
# Unity/Jumbo Builds
# ===============================================

option(SYLIFE_ENABLE_UNITY_BUILD "Enable Unity/Jumbo builds for faster compilation" OFF)

if(SYLIFE_ENABLE_UNITY_BUILD AND CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
    message(STATUS "SyLife: Unity builds enabled")
    set(CMAKE_UNITY_BUILD ON)
    set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)
    
    # Function to enable unity build for target
    function(sylife_enable_unity_build target)
        set_target_properties(${target} PROPERTIES
            UNITY_BUILD ON
            UNITY_BUILD_BATCH_SIZE 8
        )
    endfunction()
else()
    # Dummy function if unity build not supported
    function(sylife_enable_unity_build target)
        # Do nothing
    endfunction()
endif()

# ===============================================
# Memory Usage Optimization
# ===============================================

# Limit memory usage during compilation
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Limit memory usage for template instantiation
    add_compile_options(-ftemplate-depth=256)
    add_compile_options(-fconstexpr-depth=256)
    
    # Use less memory during compilation
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(--param ggc-min-expand=10)
        add_compile_options(--param ggc-min-heapsize=32768)
    endif()
endif()

# ===============================================
# Dependency Management Optimization
# ===============================================

# Function to optimize target dependencies
function(sylife_optimize_dependencies target)
    # Use OBJECT libraries for frequently used code
    get_target_property(target_type ${target} TYPE)
    
    if(target_type STREQUAL "EXECUTABLE" OR target_type STREQUAL "SHARED_LIBRARY")
        # Link libraries privately when possible
        get_target_property(linked_libs ${target} LINK_LIBRARIES)
        if(linked_libs)
            foreach(lib ${linked_libs})
                if(TARGET ${lib})
                    get_target_property(lib_type ${lib} TYPE)
                    if(lib_type STREQUAL "STATIC_LIBRARY")
                        set_target_properties(${target} PROPERTIES
                            LINK_WHAT_YOU_USE ON
                        )
                    endif()
                endif()
            endforeach()
        endif()
    endif()
endfunction()

# ===============================================
# Include What You Use (IWYU)
# ===============================================

option(SYLIFE_ENABLE_IWYU "Enable Include What You Use" OFF)

if(SYLIFE_ENABLE_IWYU)
    find_program(IWYU_PROGRAM include-what-you-use)
    if(IWYU_PROGRAM)
        message(STATUS "SyLife: Include What You Use found: ${IWYU_PROGRAM}")
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PROGRAM})
    else()
        message(STATUS "SyLife: Include What You Use not found")
    endif()
endif()

# ===============================================
# Build Time Profiling
# ===============================================

option(SYLIFE_PROFILE_BUILD_TIME "Profile build time" OFF)

if(SYLIFE_PROFILE_BUILD_TIME)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-ftime-trace)
        message(STATUS "SyLife: Build time profiling enabled (Clang time-trace)")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-ftime-report)
        message(STATUS "SyLife: Build time profiling enabled (GCC time-report)")
    endif()
endif()

# ===============================================
# Modular Build Support
# ===============================================

# Function to setup modular target with optimizations
function(sylife_setup_optimized_target target)
    # Apply optimization flags
    sylife_set_optimization_flags(${target})
    
    # Add precompiled headers
    sylife_add_precompiled_headers(${target})
    
    # Enable unity build if requested
    sylife_enable_unity_build(${target})
    
    # Optimize dependencies
    sylife_optimize_dependencies(${target})
    
    # Set standard properties
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    
    # Enable position independent code for shared libraries
    get_target_property(target_type ${target} TYPE)
    if(target_type STREQUAL "SHARED_LIBRARY")
        set_target_properties(${target} PROPERTIES
            POSITION_INDEPENDENT_CODE ON
        )
    endif()
endfunction()

# ===============================================
# Build Configuration Summary
# ===============================================

function(sylife_print_build_configuration)
    message(STATUS "")
    message(STATUS "=====================================")
    message(STATUS "SyLife Build Configuration Summary")
    message(STATUS "=====================================")
    message(STATUS "Build Type:           ${CMAKE_BUILD_TYPE}")
    message(STATUS "Parallel Jobs:        ${SYLIFE_PARALLEL_JOBS}")
    message(STATUS "Compiler:             ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "Generator:            ${CMAKE_GENERATOR}")
    message(STATUS "LTO Enabled:          ${SYLIFE_ENABLE_LTO}")
    message(STATUS "PCH Enabled:          ${SYLIFE_ENABLE_PCH}")
    message(STATUS "Unity Build:          ${SYLIFE_ENABLE_UNITY_BUILD}")
    message(STATUS "ccache:               ${CCACHE_PROGRAM}")
    message(STATUS "IWYU:                 ${IWYU_PROGRAM}")
    message(STATUS "Build Time Profiling: ${SYLIFE_PROFILE_BUILD_TIME}")
    message(STATUS "=====================================")
    message(STATUS "")
endfunction()

# Print configuration when this file is included
sylife_print_build_configuration()