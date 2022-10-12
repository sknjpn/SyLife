cmake_minimum_required(VERSION 3.12)
project(OpenSiv3D_Linux_App CXX)

set(CMAKE_C_COMPILER "emcc")
set(CMAKE_CXX_COMPILER "em++")

add_compile_options(-std=c++20)
add_compile_options(-D_XM_NO_INTRINSICS_)
add_compile_options(-Wno-deprecated-builtins)

#file(READ link.txt CMAKE_EXE_LINKER_FLAGS) 

set(CMAKE_EXE_LINKER_FLAGS "-L../OpenSiv3D/lib -L../OpenSiv3D/lib/Siv3D/freetype -L../OpenSiv3D/lib/Siv3D/giflib -L../OpenSiv3D/lib/Siv3D/harfbuzz -L../OpenSiv3D/lib/Siv3D/opencv -L../OpenSiv3D/lib/Siv3D/opus -L../OpenSiv3D/lib/Siv3D/png -L../OpenSiv3D/lib/Siv3D/tiff -L../OpenSiv3D/lib/Siv3D/turbojpeg -L../OpenSiv3D/lib/Siv3D/SDL2 -L../OpenSiv3D/lib/Siv3D/webp -L../OpenSiv3D/lib/Siv3D/zlib -lSiv3D -lfreetype -lgif -lharfbuzz -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_photo -lturbojpeg -ltiff -lopus -lopusfile -lpng -lSDL2 -lwebp -lz --emrun -sFULL_ES3=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sUSE_GLFW=3 -sUSE_OGG=1 -sUSE_VORBIS=1 -sALLOW_MEMORY_GROWTH=1 -sERROR_ON_UNDEFINED_SYMBOLS=0 -sASYNCIFY=1 -sASYNCIFY_IGNORE_INDIRECT=1 -sASYNCIFY_IMPORTS=\"['siv3dRequestAnimationFrame','siv3dGetClipboardText','siv3dDecodeImageFromFile','siv3dSleepUntilWaked','invoke_vi','invoke_v']\" -sASYNCIFY_ADD=\"['main','Main()','dynCall_v','dynCall_vi','s3d::TryMain()','s3d::CSystem::init()','s3d::System::Update()','s3d::AACDecoder::decode(*)const','s3d::MP3Decoder::decode(*)const','s3d::CAudioDecoder::decode(*)','s3d::AudioDecoder::Decode(*)','s3d::Wave::Wave(*)','s3d::Audio::Audio(*)','s3d::Clipboard::GetText(*)','s3d::CClipboard::getText(*)','s3d::GenericDecoder::decode(*)const','s3d::CImageDecoder::decode(*)','s3d::Image::Image(*)','s3d::Texture::Texture(*)','s3d::ImageDecoder::Decode(*)','s3d::ImageDecoder::GetImageInfo(*)','s3d::CRenderer2D_GLES3::init()','s3d::CRenderer2D_WebGPU::init()','s3d::Clipboard::GetText(*)','s3d::CClipboard::getText(*)','s3d::SimpleHTTP::Save(*)','s3d::SimpleHTTP::Load(*)','s3d::SimpleHTTP::Get(*)','s3d::SimpleHTTP::Post(*)','s3d::VideoReader::VideoReader(*)','s3d::VideoReader::open(*)','s3d::Platform::Web::FetchFileIfNotExists(*)']\" -sMODULARIZE=1 --preload-file ../resources@/resources --shell-file ../OpenSiv3D/Templates/Embeddable/web-player.html --pre-js ../OpenSiv3D/lib/Siv3D/Siv3D.pre.js --post-js ../OpenSiv3D/lib/Siv3D/Siv3D.post.js --post-js ../OpenSiv3D/Templates/Embeddable/web-player.js --js-library ../OpenSiv3D/lib/Siv3D/Siv3D.js")

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s EXCEPTION_CATCHING_ALLOWED=[\"main\",\"_ZN3s3d7TryMainEv\"]")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(index.html
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

target_include_directories(index.html PUBLIC
  ./src
  ./OpenSiv3D/include
  ./OpenSiv3D/include/ThirdParty
)
