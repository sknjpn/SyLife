cmake_minimum_required (VERSION 3.0)

find_package(PkgConfig)

project(OpenSiv3D_Linux CXX)
enable_language(C)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unknown-pragmas -fPIC -msse4.1")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options ("-fcolor-diagnostics")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options ("-fdiagnostics-color=always")
endif()

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../App/)

pkg_check_modules(OPENCV4 REQUIRED opencv4)
link_directories(
        "/home/sknjpn/boost_1_70_0/stage/lib"
)
include_directories(
        "/usr/include"
        "/home/sknjpn/boost_1_70_0"
	"~/OpenSiv3D/Siv3D/include"
	"~/OpenSiv3D/Siv3D/include/ThirdParty"
)



set(SOURCE_FILES
Asset.cpp
AssetEditor.cpp
Assets.cpp
Assets_makeModel.cpp
CellAsset.cpp
CellAsset_Editor.cpp
CellState.cpp
ChipState.cpp
DraggableViewer.cpp
EggState.cpp
ElementAsset.cpp
ElementAsset_Editor.cpp
ElementState.cpp
Field.cpp
GUIAssetLinker.cpp
GUIAssetLinker_Dragger.cpp
GUIButton.cpp
GUIChecker.cpp
GUIColorPicker.cpp
GUISlider.cpp
GUITextBox.cpp
GUIText.cpp
GUIValuer.cpp
Layer.cpp
Main.cpp
MainViewer_CellMakingViewer_BodySculptor.cpp
MainViewer_CellMakingViewer_BodySculptor_LayerLists.cpp
MainViewer_CellMakingViewer_BodySculptor_LayerLists_Item.cpp
MainViewer_CellMakingViewer_BodySculptor_Workspace.cpp
MainViewer_CellMakingViewer_CellInfo.cpp
MainViewer_CellMakingViewer.cpp
MainViewer_CellMakingViewer_PartsAssembler.cpp
MainViewer_CellMakingViewer_PartsAssembler_PartList.cpp
MainViewer_CellMakingViewer_PartsAssembler_Workspace.cpp
MainViewer_CellStateViewer.cpp
MainViewer_CommandPalette.cpp
MainViewer_CommandPalette_MagnifyingViewer.cpp
MainViewer_CommandPalette_SpeedControllerViewer.cpp
MainViewer.cpp
MainViewer_FieldViewer_CellStateCaptureViewer.cpp
MainViewer_FieldViewer.cpp
MainViewer_FieldViewer_ReleaseViewer.cpp
MainViewer_StatisticsViewer.cpp
Model.cpp
PartAsset_Body.cpp
PartAsset_Body_Editor.cpp
PartAsset.cpp
PartAsset_Needle.cpp
PartAsset_Needle_Editor.cpp
PartAsset_Nucleus.cpp
PartAsset_Nucleus_Editor.cpp
PartAsset_Synthesizer.cpp
PartAsset_Synthesizer_Editor.cpp
PartAsset_Wing.cpp
PartAsset_Wing_Editor.cpp
PartConfig.cpp
Particle.cpp
PartShapeViewer.cpp
PartState_Body.cpp
PartState.cpp
PartState_Needle.cpp
PartState_Nucleus.cpp
PartState_Synthesizer.cpp
PartState_Wing.cpp
Rigidbody.cpp
Shape.cpp
State.cpp
stdafx.cpp
Storage.cpp
Storage_Editor.cpp
Storage_Editor_Element.cpp
Viewer.cpp
World.cpp
)
add_executable(Siv3D_App ${SOURCE_FILES})

target_link_libraries(Siv3D_App
	${OPENCV4_LIBRARIES}
	-lOpenGL
	-lGLEW
	-lX11
	-langelscript
	-lpthread
	-ldl
	-ludev
	-lfreetype
	-lharfbuzz
	-lglib-2.0
	-lgobject-2.0
	-lgio-2.0
	-lpng
	-lturbojpeg
	-lgif
	-lwebp
	-lopenal
	-logg
	-lvorbis
	-lvorbisenc
	-lvorbisfile
	-lboost_filesystem
	-lavformat
	-lavcodec
	-lavutil
	-lswresample
	${PROJECT_SOURCE_DIR}/../OpenSiv3D/Linux/build/libSiv3D.a
	
)

