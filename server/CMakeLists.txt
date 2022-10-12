cmake_minimum_required(VERSION 3.12)
project(OpenSiv3D_Linux_App CXX)

set(CMAKE_C_COMPILER "gcc-9")
set(CMAKE_CXX_COMPILER "g++-9")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(SyLife
  ../src/Asset.cpp
  ../src/Asset_Editor.cpp
  ../src/CellAsset.cpp
  ../src/CellAsset_Log.cpp
  ../src/CellState.cpp
  ../src/EditorViewer_AssetList.cpp
  ../src/EditorViewer_AssetViewer.cpp
  ../src/EditorViewer.cpp
  ../src/EggState.cpp
  ../src/Layer.cpp
  ../src/Main.cpp
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
  ../src/Object.cpp
  ../src/PartAsset.cpp
  ../src/Part_BodyAsset.cpp
  ../src/Part_BodyState.cpp
  ../src/PartConfig.cpp
  ../src/Part_DepoisonerAsset.cpp
  ../src/Part_DepoisonerState.cpp
  ../src/Part_EyeAsset.cpp
  ../src/Part_EyeState.cpp
  ../src/Particle.cpp
  ../src/Part_NeedleAsset.cpp
  ../src/Part_NeedleState.cpp
  ../src/Part_NucleusAsset.cpp
  ../src/Part_NucleusState.cpp
  ../src/Part_PoisonerAsset.cpp
  ../src/Part_PoisonerState.cpp
  ../src/PartState.cpp
  ../src/Part_SynthesizerAsset.cpp
  ../src/Part_SynthesizerState.cpp
  ../src/Part_WingAsset.cpp
  ../src/Part_WingState.cpp
  ../src/ProteinAsset.cpp
  ../src/ProteinAsset_Editor.cpp
  ../src/Rigidbody.cpp
  ../src/Shape.cpp
  ../src/stdafx.cpp
  ../src/Storage.cpp
  ../src/TileState.cpp
  ../src/TitleViewer.cpp
  ../src/TitleViewer_WorldGenerator.cpp
  ../src/World.cpp
  ../src/World_makeModel.cpp
)

find_package(Siv3D)

target_link_libraries(SyLife PUBLIC Siv3D::Siv3D)

target_compile_features(SyLife PRIVATE cxx_std_20)
