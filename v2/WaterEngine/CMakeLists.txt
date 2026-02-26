# =============================================================================
# Water Engine v2.0.0
# Copyright (C) 2026 Will The Water
# License: MIT (see LICENSE file for full text)
# =============================================================================

# =============================================================================
# WaterEngine/CMakesLists.txt
# =============================================================================

cmake_minimum_required(VERSION 3.16)

# ========================== PROJECT SETUP ==========================
project(WaterEngine VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========================== LIBRARY OPTIONS ==========================
set(SFML_BUILD_NETWORK OFF)
set(SPDLOG_USE_STD_FORMAT ON)

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

# --- ImGui ---
FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9.tar.gz
)

FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})

# --- ImGui-SFML ---
FetchContent_Declare(
    imgui_sfml
    URL https://github.com/SFML/imgui-sfml/archive/refs/tags/v3.0.tar.gz
)

# --- PhysicsFS ---
set(PHYSFS_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_TEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    physicsfs
    URL https://github.com/icculus/physfs/archive/refs/tags/release-3.2.0.tar.gz
)

# Disable find_package(SFML)
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)

# Remaining Dependencies
FetchContent_MakeAvailable(
    sfml
    imgui_sfml
    box2d
    logs
    enum
    toml
    physicsfs
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

# Compile definitions
target_compile_definitions(WaterEngine PUBLIC
    SFML_STATIC
    $<$<CONFIG:Debug>:WE_DEBUG>
    $<$<CONFIG:Release>:WE_RELEASE>
)

# Link libraries
target_link_libraries(WaterEngine PUBLIC
    SFML::Graphics
    SFML::Audio
    SFML::Main
    box2d
    spdlog::spdlog
    magic_enum::magic_enum
    tomlplusplus::tomlplusplus
    ImGui-SFML::ImGui-SFML
    physfs-static
)

# Platform-specific settings
if(WIN32)
    target_link_libraries(WaterEngine PUBLIC kernel32 user32)

    target_link_options(WaterEngine PUBLIC
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        $<$<CONFIG:Release>:/SUBSYSTEM:CONSOLE>
    )
endif()