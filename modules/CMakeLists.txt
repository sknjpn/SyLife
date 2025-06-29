# SyLife Modules - Modern Modular Architecture
cmake_minimum_required(VERSION 3.20)
project(SyLifeModules LANGUAGES CXX)

# Add each module as a subdirectory
add_subdirectory(core)
add_subdirectory(di)
add_subdirectory(logger)

# Create an umbrella target that includes all modules
add_library(sylife_modules INTERFACE)
target_link_libraries(sylife_modules INTERFACE
    sylife_core
    sylife_di
    sylife_logger
)

# Export the umbrella target for easy consumption
target_include_directories(sylife_modules INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)