# =============================================================================
# Water Engine v2.0.0
# Copyright (C) 2026 Will The Water
# License: MIT (see LICENSE file for full text)
# =============================================================================

# =============================================================================
# WaterEngine/CMakesLists.txt
# =============================================================================

# ========================== LIBRARY OPTIONS ==========================
set(SFML_BUILD_NETWORK OFF)
set(SPDLOG_USE_STD_FORMAT ON)
set(TGUI_BACKEND SFML_GRAPHICS)

# ========================== DEPENDENCIES ==========================
include(FetchContent)

# --- SFML 3.0.2 ---
FetchContent_Declare(
    sfml
    URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.tar.gz
)

# --- Box2D 2.4.2 ---
FetchContent_Declare(
    box2d
    URL https://github.com/erincatto/box2d/archive/refs/tags/v2.4.2.tar.gz
)

# --- spdlog ---
FetchContent_Declare(
    logs
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz
)

# JSON
FetchContent_Declare(
    json 
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

# --- magic_enum ---
FetchContent_Declare(
    enum
    URL https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.7.tar.gz
)

# --- toml++ ---
FetchContent_Declare(
    toml
    URL https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.tar.gz
)

# --- PhysicsFS ---
set(PHYSFS_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_TEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    physicsfs
    URL https://github.com/icculus/physfs/archive/refs/tags/release-3.2.0.tar.gz
)

# --- tgui ---
FetchContent_Declare(
    tgui 
    URL https://github.com/texus/TGUI/archive/refs/tags/v1.11.0.tar.gz
)

# Remaining Dependencies
FetchContent_MakeAvailable(
    sfml
    box2d
    logs
    json
    enum
    toml
    physicsfs
    tgui
)

# =============================================================================
# WaterEngine Library
# =============================================================================

file(GLOB_RECURSE WATER_ENGINE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h"
)

file(GLOB_RECURSE WATER_ENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)

add_library(WaterEngine STATIC
    ${WATER_ENGINE_SOURCES}
    ${WATER_ENGINE_HEADERS}
)

# Include directories
target_include_directories(WaterEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

# Asset paths (Debug: raw folder, Release: packed pak)
set(ASSET_ROOT_PATH "${CMAKE_SOURCE_DIR}/Content/")
set(ASSET_PACK_PATH "Content.pak")

# Compile definitions
target_compile_definitions(WaterEngine PUBLIC
    SFML_STATIC
    $<$<CONFIG:Debug>:WE_DEBUG>
    $<$<CONFIG:Release>:WE_RELEASE>
    $<$<CONFIG:Debug>:USE_RAW_ASSETS>
    $<$<CONFIG:Release>:USE_PACKED_ASSETS>
    $<$<CONFIG:Debug>:ASSET_ROOT_PATH="${ASSET_ROOT_PATH}">
    $<$<CONFIG:Release>:ASSET_PACK_PATH="${ASSET_PACK_PATH}">
)

# Precompiled Header
target_precompile_headers(WaterEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Include/Core/CoreMinimal.h"
)

# Link libraries
target_link_libraries(WaterEngine PUBLIC
    SFML::Graphics
    SFML::Audio
    SFML::Main
    box2d
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    magic_enum::magic_enum
    tomlplusplus::tomlplusplus
    physfs-static
    TGUI::TGUI
)

# Console Settings
if(WIN32)
    target_link_options(WaterEngine PUBLIC
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
    )
endif()

# =============================================================================
# Asset Handling
# =============================================================================

# Debug: Copy Content folder to output directory
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(
        TARGET WaterEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Content
            $<TARGET_FILE_DIR:${DEMO_GAME}>/Content
        COMMENT "Copying Content folder to output directory"
    )
endif()

# Release: Pack Content into Content.pak and copy to output
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CONTENT_PAK_OUTPUT "$<TARGET_FILE_DIR:${DEMO_GAME}>/${ASSET_PACK_PATH}")
    set(CONTENT_SRC_DIR "${CMAKE_SOURCE_DIR}/Content")
    
    # Pack Content folder into Content.pak (Assets/ at root of archive)
    # Working directory is Content/ so paths match Debug mode (Assets/... not Content/Assets/...)
    add_custom_command(
        TARGET WaterEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar cf 
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSET_PACK_PATH}
            --format=zip
            .
        WORKING_DIRECTORY ${CONTENT_SRC_DIR}
        COMMENT "Packing Content.pak"
    )
    
    # Copy pak to DemoGame output directory
    add_custom_command(
        TARGET WaterEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSET_PACK_PATH}
            ${CONTENT_PAK_OUTPUT}
        COMMENT "Copying Content.pak to output directory"
    )
endif()
