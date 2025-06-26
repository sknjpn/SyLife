cmake_minimum_required(VERSION 3.16)

# Project information
project(SyLife 
    VERSION 1.0.0 
    DESCRIPTION "Self-designed ecosystem simulator"
    HOMEPAGE_URL "https://github.com/sknjpn/SyLife"
    LANGUAGES CXX
)

# Modern CMake settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# CMake modules path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Project options
option(SYLIFE_BUILD_DESKTOP "Build desktop application" ON)
option(SYLIFE_BUILD_WEB "Build web application" OFF)
option(SYLIFE_BUILD_TESTS "Build tests" ON)
option(SYLIFE_BUILD_EXAMPLES "Build examples" OFF)
option(SYLIFE_BUILD_DOCS "Build documentation" OFF)
option(SYLIFE_ENABLE_COVERAGE "Enable test coverage" OFF)
option(SYLIFE_ENABLE_SANITIZERS "Enable sanitizers" OFF)
option(SYLIFE_ENABLE_LTO "Enable Link Time Optimization" OFF)

# Compiler settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra -Wpedantic)
    add_compile_definitions(SYLIFE_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
    add_compile_definitions(SYLIFE_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-O2 -g -DNDEBUG)
    add_compile_definitions(SYLIFE_RELEASE)
endif()

# Sanitizers
if(SYLIFE_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

# Coverage
if(SYLIFE_ENABLE_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# Link Time Optimization
if(SYLIFE_ENABLE_LTO AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT supported OUTPUT error)
    if(supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "LTO not supported: ${error}")
    endif()
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Global include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find dependencies
if(SYLIFE_BUILD_DESKTOP OR SYLIFE_BUILD_WEB)
    find_package(Siv3D REQUIRED)
else()
    # For testing only, create dummy Siv3D target
    add_library(Siv3D INTERFACE)
    target_include_directories(Siv3D INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/siv3d_stubs)
endif()

# Add subdirectories
add_subdirectory(lib)

# Only build apps if Siv3D is available
if(TARGET Siv3D AND SYLIFE_BUILD_DESKTOP)
    add_subdirectory(apps/desktop)
endif()

if(TARGET Siv3D AND SYLIFE_BUILD_WEB)
    add_subdirectory(apps/web)
endif()

if(SYLIFE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(SYLIFE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SYLIFE_BUILD_DOCS)
    add_subdirectory(docs)
endif()

# Install configuration
include(GNUInstallDirs)

if(SYLIFE_BUILD_DESKTOP)
    install(TARGETS sylife_desktop
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

# Package configuration
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SyLifeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SyLifeConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SyLife"
)

# Create package
set(CPACK_PACKAGE_NAME "SyLife")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "SyLife Project")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

include(CPack)