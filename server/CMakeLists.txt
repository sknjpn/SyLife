cmake_minimum_required(VERSION 3.16)
project(SyLife VERSION 1.0.0 LANGUAGES CXX)

# Modern CMake settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler settings (allow override)
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(WARNING "Untested compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(SYLIFE_BUILD_TESTS "Build SyLife tests" ON)
option(SYLIFE_ENABLE_COVERAGE "Enable test coverage" OFF)

# Compiler flags
add_compile_definitions(SYLIFE_SERVER)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
endif()

if(SYLIFE_ENABLE_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# Source file groups for better organization
set(CORE_SOURCES
  ../src/Asset.cpp
  ../src/CellAsset.cpp
  ../src/CellState.cpp
  ../src/EggState.cpp
  ../src/Object.cpp
  ../src/PartAsset.cpp
  ../src/PartState.cpp
  ../src/ProteinAsset.cpp
  ../src/Storage.cpp
  ../src/World.cpp
  ../src/World_makeModel.cpp
  ../src/stdafx.cpp
)

set(PHYSICS_SOURCES
  ../src/Particle.cpp
  ../src/Rigidbody.cpp
  ../src/Shape.cpp
  ../src/TileState.cpp
)

set(PART_SOURCES
  ../src/PartConfig.cpp
  ../src/Part_BodyAsset.cpp
  ../src/Part_BodyState.cpp
  ../src/Part_DepoisonerAsset.cpp
  ../src/Part_DepoisonerState.cpp
  ../src/Part_EyeAsset.cpp
  ../src/Part_EyeState.cpp
  ../src/Part_NeedleAsset.cpp
  ../src/Part_NeedleState.cpp
  ../src/Part_NucleusAsset.cpp
  ../src/Part_NucleusState.cpp
  ../src/Part_PoisonerAsset.cpp
  ../src/Part_PoisonerState.cpp
  ../src/Part_SynthesizerAsset.cpp
  ../src/Part_SynthesizerState.cpp
  ../src/Part_WingAsset.cpp
  ../src/Part_WingState.cpp
)

set(NETWORK_SOURCES
  ../src/WebSocketClient.cpp
  ../src/WebSocketServer.cpp
)

set(UI_SOURCES
  ../src/Asset_Editor.cpp
  ../src/CellAsset_Log.cpp
  ../src/EditorViewer_AssetList.cpp
  ../src/EditorViewer_AssetViewer.cpp
  ../src/EditorViewer.cpp
  ../src/Layer.cpp
  ../src/MainViewer_CellAssetViewer_CellStateViewer.cpp
  ../src/MainViewer_CellAssetViewer.cpp
  ../src/MainViewer_CellBook.cpp
  ../src/MainViewer_CellBook_Item.cpp
  ../src/MainViewer_CellMakingViewer_BodySculptor_ColorSelector.cpp
  ../src/MainViewer_CellMakingViewer_BodySculptor.cpp
  ../src/MainViewer_CellMakingViewer_BodySculptor_Workspace.cpp
  ../src/MainViewer_CellMakingViewer_CellInfo.cpp
  ../src/MainViewer_CellMakingViewer.cpp
  ../src/MainViewer_CellMakingViewer_PartsAssembler.cpp
  ../src/MainViewer_CellMakingViewer_PartsAssembler_PartList.cpp
  ../src/MainViewer_CellMakingViewer_PartsAssembler_Workspace.cpp
  ../src/MainViewer_CellMakingViewer_PartsAssembler_Workspace_TrashBox.cpp
  ../src/MainViewer_CommandPalette.cpp
  ../src/MainViewer.cpp
  ../src/MainViewer_FieldViewer_CellStateCaptureViewer.cpp
  ../src/MainViewer_FieldViewer.cpp
  ../src/MainViewer_FieldViewer_ReleaseViewer.cpp
  ../src/MainViewer_StatisticsViewer.cpp
  ../src/ProteinAsset_Editor.cpp
  ../src/TitleViewer.cpp
  ../src/TitleViewer_WorldGenerator.cpp
)

# Create SyLife library for code reuse
add_library(SyLifeCore STATIC
  ${CORE_SOURCES}
  ${PHYSICS_SOURCES}
  ${PART_SOURCES}
  ${NETWORK_SOURCES}
)

# Main executable
add_executable(SyLife
  ../src/Main.cpp
  ${UI_SOURCES}
)

# Configure SyLifeCore library
target_include_directories(SyLifeCore PUBLIC 
  ../src
  ../thirdparty
)

target_compile_features(SyLifeCore PUBLIC cxx_std_20)

# Find dependencies
find_package(Siv3D REQUIRED)

# Link libraries to SyLifeCore
target_link_libraries(SyLifeCore PUBLIC Siv3D::Siv3D)

# Configure main executable
target_include_directories(SyLife PRIVATE 
  ../src
  ../thirdparty
)

target_link_libraries(SyLife PRIVATE SyLifeCore)
target_compile_features(SyLife PRIVATE cxx_std_20)

# Enable testing
if(SYLIFE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(../tests tests)
endif()

# Install configuration
install(TARGETS SyLife
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Create uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif()

# Add custom targets for development
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS sylife_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running SyLife tests"
)

add_custom_target(run_coverage
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMAND lcov --capture --directory . --output-file coverage.info
    COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
    COMMAND lcov --list coverage.info
    COMMAND genhtml coverage.info --output-directory coverage_html
    DEPENDS sylife_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating test coverage report"
)
