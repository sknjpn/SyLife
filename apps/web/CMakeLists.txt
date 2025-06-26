# SyLife Web Application

if(NOT EMSCRIPTEN)
    message(WARNING "Web application requires Emscripten. Use emcmake to configure.")
    return()
endif()

set(WEB_SOURCES
    main.cpp
    web_application.cpp
    web_config.cpp
)

set(WEB_HEADERS
    web_application.h
    web_config.h
)

add_executable(sylife_web ${WEB_SOURCES} ${WEB_HEADERS})

target_include_directories(sylife_web PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(sylife_web PRIVATE
    sylife_combined
)

target_compile_features(sylife_web PRIVATE cxx_std_20)

# Emscripten-specific settings
target_compile_definitions(sylife_web PRIVATE 
    SYLIFE_WEB
    _XM_NO_INTRINSICS_
)

target_compile_options(sylife_web PRIVATE
    -Wno-deprecated-builtins
)

# Emscripten linker flags
set(EMSCRIPTEN_LINK_FLAGS
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/freetype"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/giflib"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/harfbuzz"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/opencv"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/opus"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/png"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/tiff"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/turbojpeg"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/SDL2"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/webp"
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/zlib"
    "-lSiv3D -lfreetype -lgif -lharfbuzz -lopencv_core -lopencv_imgproc"
    "-lopencv_objdetect -lopencv_photo -lturbojpeg -ltiff -lopus -lopusfile"
    "-lpng -lSDL2 -lwebp -lz -lwebsocket.js"
    "--emrun"
    "-sFULL_ES3=1"
    "-sMIN_WEBGL_VERSION=2"
    "-sMAX_WEBGL_VERSION=2"
    "-sUSE_GLFW=3"
    "-sUSE_OGG=1"
    "-sUSE_VORBIS=1"
    "-sALLOW_MEMORY_GROWTH=1"
    "-sERROR_ON_UNDEFINED_SYMBOLS=0"
    "-sASYNCIFY=1"
    "-sASYNCIFY_IGNORE_INDIRECT=1"
    "-sMODULARIZE=1"
    "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../../resources@/resources"
    "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../../assets@/assets"
    "--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/Templates/Embeddable/web-player.html"
    "--pre-js ${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/Siv3D.pre.js"
    "--post-js ${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/Siv3D.post.js"
    "--post-js ${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/Templates/Embeddable/web-player.js"
    "--js-library ${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/lib/Siv3D/Siv3D.js"
)

string(REPLACE ";" " " EMSCRIPTEN_LINK_FLAGS_STR "${EMSCRIPTEN_LINK_FLAGS}")
set_target_properties(sylife_web PROPERTIES LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS_STR}")

# Build type specific flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_options(sylife_web PRIVATE
        "-s EXCEPTION_CATCHING_ALLOWED=[\"main\",\"_ZN3s3d7TryMainEv\"]"
        "-O0 -g"
    )
else()
    target_link_options(sylife_web PRIVATE "-O2")
endif()

# Set output directory
set_target_properties(sylife_web PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/web"
    OUTPUT_NAME "index"
    SUFFIX ".html"
)

# Include OpenSiv3D headers for web
target_include_directories(sylife_web PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../web/OpenSiv3D/include/ThirdParty
)