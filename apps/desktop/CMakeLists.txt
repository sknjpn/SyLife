# SyLife Desktop Application

set(DESKTOP_SOURCES
    main.cpp
    application.cpp
    desktop_config.cpp
)

set(DESKTOP_HEADERS
    application.h
    desktop_config.h
)

add_executable(sylife_desktop ${DESKTOP_SOURCES} ${DESKTOP_HEADERS})

target_include_directories(sylife_desktop PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(sylife_desktop PRIVATE
    sylife_combined
    Siv3D::Siv3D
)

target_compile_features(sylife_desktop PRIVATE cxx_std_20)

# Platform-specific definitions
target_compile_definitions(sylife_desktop PRIVATE SYLIFE_DESKTOP)

if(WIN32)
    target_compile_definitions(sylife_desktop PRIVATE SYLIFE_WINDOWS)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(sylife_desktop PRIVATE SYLIFE_LINUX)
elseif(APPLE)
    target_compile_definitions(sylife_desktop PRIVATE SYLIFE_MACOS)
endif()

# Set output name
set_target_properties(sylife_desktop PROPERTIES OUTPUT_NAME "SyLife")

# Resources
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../resources)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../resources 
         DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../assets)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../assets 
         DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# Install
install(TARGETS sylife_desktop
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../resources)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../resources
        DESTINATION ${CMAKE_INSTALL_DATADIR}/sylife
    )
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../assets)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../assets
        DESTINATION ${CMAKE_INSTALL_DATADIR}/sylife
    )
endif()