cmake_minimum_required(VERSION 3.12)
project(SyLife_Linux_App CXX C)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fPIC -msse4.1 -D_GLFW_X11")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wextra -Wno-unknown-pragmas -fPIC -msse4.1 -D_GLFW_X11")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

if (NOT CMAKE_CONFIGURATION_TYPES AND 
    NOT CMAKE_NO_BUILD_TYPE AND
    NOT CMAKE_BUILD_TYPE AND
    CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    message(STATUS "[!] Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options ("-fcolor-diagnostics")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options ("-fdiagnostics-color=always")
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
add_executable(Siv3DTest
  CellState.cpp
  EditorViewer.cpp
  EditorViewer_AssetList.cpp
  EditorViewer_AssetViewer.cpp
  EggState.cpp
  Layer.cpp
  Main.cpp
  MainViewer.cpp
  MainViewer_CellAssetViewer.cpp
  MainViewer_CellAssetViewer_CellStateViewer.cpp
  MainViewer_CellBook.cpp
  MainViewer_CellBook_Item.cpp
  MainViewer_CellMakingViewer.cpp
  MainViewer_CellMakingViewer_BodySculptor.cpp
  MainViewer_CellMakingViewer_BodySculptor_ColorSelector.cpp
  MainViewer_CellMakingViewer_BodySculptor_Workspace.cpp
  MainViewer_CellMakingViewer_CellInfo.cpp
  MainViewer_CellMakingViewer_PartsAssembler.cpp
  MainViewer_CellMakingViewer_PartsAssembler_PartList.cpp
  MainViewer_CellMakingViewer_PartsAssembler_Workspace.cpp
  MainViewer_CellMakingViewer_PartsAssembler_Workspace_TrashBox.cpp
  MainViewer_CommandPalette.cpp
  MainViewer_FieldViewer.cpp
  MainViewer_FieldViewer_CellStateCaptureViewer.cpp
  MainViewer_FieldViewer_ReleaseViewer.cpp
  MainViewer_StatisticsViewer.cpp
  Object.cpp
  Part_BodyAsset.cpp
  Part_BodyState.cpp
  Part_DepoisonerAsset.cpp
  Part_DepoisonerState.cpp
  Part_EyeAsset.cpp
  Part_EyeState.cpp
  Part_NeedleAsset.cpp
  Part_NeedleState.cpp
  Part_NucleusAsset.cpp
  Part_NucleusState.cpp
  Part_PoisonerAsset.cpp
  Part_PoisonerState.cpp
  Part_SynthesizerAsset.cpp
  Part_SynthesizerState.cpp
  Part_WingAsset.cpp
  Part_WingState.cpp
  PartAsset.cpp
  PartConfig.cpp
  Particle.cpp
  PartState.cpp
  ProteinAsset.cpp
  ProteinAsset_Editor.cpp
  Rigidbody.cpp
  Shape.cpp
  stdafx.cpp
  Storage.cpp
  TileState.cpp
  TitleViewer.cpp
  TitleViewer_WorldGenerator.cpp
  World.cpp
  World_makeModel.cpp
  Asset.cpp
  Asset_Editor.cpp
  CellAsset.cpp
  CellAsset_Log.cpp
  )
target_include_directories(Siv3DTest PRIVATE
  "/usr/include"
  "../../Siv3D/include"
  "../../Siv3D/include/ThirdParty"
  )
target_link_libraries(Siv3DTest PRIVATE
  ${PROJECT_SOURCE_DIR}/../build/libSiv3D.a
  -lasound
  -lavcodec
  -lavformat
  -lavutil
  -lcurl
  -ldl
  -lfreetype
  -lGL
  -lglib-2.0
  -lgobject-2.0
  -lgif
  -lgio-2.0
  -lgdk-3
  -lgtk-3
  -lharfbuzz
	-logg
  -lopencv_core
  -lopencv_imgcodecs
  -lopencv_imgproc
  -lopencv_objdetect
  -lopencv_photo
  -lopencv_videoio
  -lopus
  -lopusfile
  -lpthread
  -lpng
  -lSoundTouch
  -lswresample
  -ltiff
  -lturbojpeg
  -luuid
	-lvorbis
	-lvorbisenc
	-lvorbisfile
  -lwebp
  -lX11
  -lXft
  )


if(BUILD_TESTING)
enable_testing()
add_test(
  NAME Test
  COMMAND Siv3DTest
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endif()