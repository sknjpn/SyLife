# SyLife Test Suite with New Architecture

cmake_minimum_required(VERSION 3.16)

# Google Test setup
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(GTEST gtest QUIET)
    pkg_check_modules(GMOCK gmock QUIET)
endif()

if(GTEST_FOUND AND GMOCK_FOUND)
    # Use system GoogleTest
    add_library(gtest_main INTERFACE)
    target_link_libraries(gtest_main INTERFACE ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES})
    target_include_directories(gtest_main INTERFACE ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
    
    add_library(gmock_main INTERFACE)
    target_link_libraries(gmock_main INTERFACE ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES})
    target_include_directories(gmock_main INTERFACE ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
else()
    # Create minimal stubs for testing
    add_library(gtest_main INTERFACE)
    add_library(gmock_main INTERFACE)
    target_include_directories(gtest_main INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stubs)
    target_include_directories(gmock_main INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stubs)
endif()

# Test categories
set(CORE_TESTS
    core/test_object.cpp
    core/test_world.cpp
    core/test_world_factory.cpp
)

set(ASSET_TESTS
    assets/test_asset.cpp
    assets/test_asset_manager.cpp
    assets/test_cell_asset.cpp
    assets/test_part_asset.cpp
    assets/test_protein_asset.cpp
    assets/test_asset_factory.cpp
    assets/test_asset_validation.cpp
)

set(STATE_TESTS
    states/test_cell_state.cpp
    states/test_egg_state.cpp
    states/test_part_state.cpp
    states/test_tile_state.cpp
    states/test_state_manager.cpp
    states/test_state_serializer.cpp
)

set(PHYSICS_TESTS
    physics/test_particle.cpp
    physics/test_rigidbody.cpp
    physics/test_collision_detector.cpp
    physics/test_physics_world.cpp
    physics/test_spatial_partitioning.cpp
    physics/test_force_generator.cpp
)

set(NETWORK_TESTS
    network/test_websocket_server.cpp
    network/test_websocket_client.cpp
    network/test_security_manager.cpp
    network/test_rate_limiter.cpp
    network/test_input_validator.cpp
    network/test_connection_security.cpp
)

set(PARTS_TESTS
    parts/test_part_nucleus.cpp
    parts/test_part_wing.cpp
    parts/test_part_eye.cpp
    parts/test_part_needle.cpp
    parts/test_part_factory.cpp
    parts/test_part_behavior.cpp
)

set(UI_TESTS
    ui/test_viewer_base.cpp
    ui/test_main_viewer.cpp
    ui/test_gui_components.cpp
    ui/test_rendering_engine.cpp
    ui/test_input_manager.cpp
)

set(UTILS_TESTS
    utils/test_storage.cpp
    utils/test_json_helper.cpp
    utils/test_math_utils.cpp
    utils/test_file_utils.cpp
    utils/test_performance_profiler.cpp
    utils/test_memory_pool.cpp
)

set(INTEGRATION_TESTS
    integration/test_simulation_integration.cpp
    integration/test_asset_state_integration.cpp
    integration/test_physics_integration.cpp
    integration/test_network_integration.cpp
    integration/test_performance_integration.cpp
    integration/test_security_integration.cpp
)

# Test utilities
set(TEST_UTILITIES
    common/test_common.cpp
    common/test_fixtures.cpp
    common/test_generators.cpp
    common/test_matchers.cpp
    common/test_mocks.cpp
)

# Main test executable
add_executable(sylife_tests
    ${TEST_UTILITIES}
    ${CORE_TESTS}
    ${ASSET_TESTS}
    ${STATE_TESTS}
    ${PHYSICS_TESTS}
    ${NETWORK_TESTS}
    ${PARTS_TESTS}
    ${UI_TESTS}
    ${UTILS_TESTS}
    ${INTEGRATION_TESTS}
)

# Include directories
target_include_directories(sylife_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty
)

# Link libraries
target_link_libraries(sylife_tests PRIVATE
    gtest_main
    gmock_main
    sylife_combined
)

# Platform-specific definitions
if(NOT DEFINED SYLIFE_WEB)
    target_compile_definitions(sylife_tests PRIVATE SYLIFE_DESKTOP)
else()
    target_compile_definitions(sylife_tests PRIVATE SYLIFE_WEB)
endif()

target_compile_features(sylife_tests PRIVATE cxx_std_20)

# Test discovery
include(GoogleTest)
gtest_discover_tests(sylife_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Custom test targets
add_custom_target(test_core
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "Core.*"
    DEPENDS sylife_tests
    COMMENT "Running core tests"
)

add_custom_target(test_assets
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "Assets.*"
    DEPENDS sylife_tests
    COMMENT "Running asset tests"
)

add_custom_target(test_physics
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "Physics.*"
    DEPENDS sylife_tests
    COMMENT "Running physics tests"
)

add_custom_target(test_network
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "Network.*"
    DEPENDS sylife_tests
    COMMENT "Running network tests"
)

add_custom_target(test_integration
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "Integration.*"
    DEPENDS sylife_tests
    COMMENT "Running integration tests"
)

add_custom_target(test_security
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R ".*Security.*"
    DEPENDS sylife_tests
    COMMENT "Running security tests"
)

# Performance tests (separate executable for performance testing)
add_executable(sylife_performance_tests
    performance/test_world_performance.cpp
    performance/test_physics_performance.cpp
    performance/test_asset_performance.cpp
    performance/test_memory_performance.cpp
    ${TEST_UTILITIES}
)

target_include_directories(sylife_performance_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty
)

target_link_libraries(sylife_performance_tests PRIVATE
    gtest_main
    sylife_combined
)

target_compile_features(sylife_performance_tests PRIVATE cxx_std_20)

# Benchmark target
add_custom_target(benchmark
    COMMAND sylife_performance_tests --gtest_filter="*Performance*"
    DEPENDS sylife_performance_tests
    COMMENT "Running performance benchmarks"
)

# Coverage target (requires SYLIFE_ENABLE_COVERAGE=ON)
if(SYLIFE_ENABLE_COVERAGE)
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND lcov --capture --directory . --output-file coverage.info
        COMMAND lcov --remove coverage.info '/usr/*' '*/tests/*' '*/thirdparty/*' --output-file coverage.info
        COMMAND lcov --list coverage.info
        COMMAND genhtml coverage.info --output-directory coverage_html
        DEPENDS sylife_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating test coverage report"
    )
endif()

# Memory test target (requires valgrind or similar)
add_custom_target(memtest
    COMMAND valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all 
            --track-origins=yes --verbose $<TARGET_FILE:sylife_tests>
    DEPENDS sylife_tests
    COMMENT "Running memory tests with valgrind"
)

# Stress test target
add_custom_target(stress
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --repeat until-fail:100
    DEPENDS sylife_tests
    COMMENT "Running stress tests"
)