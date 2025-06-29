# ECS (Entity Component System) Module
cmake_minimum_required(VERSION 3.20)

# Create the ECS library
add_library(sylife_ecs STATIC
    # Core ECS framework
    src/entity_manager.cpp
    src/component_storage.cpp
    src/system_manager.cpp
    src/world.cpp
    src/event_bus.cpp
    
    # Example components
    src/components/position_component.cpp
    src/components/velocity_component.cpp
    src/components/health_component.cpp
    src/components/collision_component.cpp
    
    # Example systems
    src/systems/movement_system.cpp
    src/systems/collision_system.cpp
    src/systems/lifecycle_system.cpp
)

# Set target properties
set_target_properties(sylife_ecs PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

# Include directories
target_include_directories(sylife_ecs
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(sylife_ecs
    PUBLIC
        sylife_di
        sylife_logger
)

# Compiler-specific options
target_compile_options(sylife_ecs PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

# Enable optimizations for Release builds
target_compile_options(sylife_ecs PRIVATE
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
    $<$<CONFIG:Debug>:-O0 -g>
)

# Tests
option(SYLIFE_BUILD_TESTS "Build tests" ON)
if(SYLIFE_BUILD_TESTS)
    add_subdirectory(tests)
endif()

# Install targets
install(TARGETS sylife_ecs
    EXPORT SyLifeECSTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Export targets
install(EXPORT SyLifeECSTargets
    FILE SyLifeECSTargets.cmake
    NAMESPACE SyLife::
    DESTINATION lib/cmake/SyLife
)