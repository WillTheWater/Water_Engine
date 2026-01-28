# =============================================================================
# Water Engine v2.0.0
# Copyright (C) 2026 Will The Water
# License: MIT (see LICENSE file for full text)
# =============================================================================

# =============================================================================
# WaterEngine/CMakesLists.txt
# =============================================================================

include(FetchContent)

# ========================== LIBRARY OPTIONS ==========================
set(SFML_BUILD_NETWORK OFF)       # Disables Network Module
set(SPDLOG_USE_STD_FORMAT ON)     # Uses std::format
set(TGUI_BACKEND SFML_GRAPHICS)   # Uses SFML graphics backend

# SFML 3.0.2
FetchContent_Declare(sfml URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.tar.gz)
set(SFML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Box2D 2.4.2
FetchContent_Declare(box2d URL https://github.com/erincatto/box2d/archive/refs/tags/v2.4.2.tar.gz)
set(BOX2D_BUILD_SHARED OFF CACHE BOOL "" FORCE)

# JSON
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)

# spdlog
FetchContent_Declare(logs URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz)

# TGUI
FetchContent_Declare(tgui URL https://github.com/texus/TGUI/archive/refs/tags/v1.11.0.tar.gz)

# magic_enum
FetchContent_Declare(enum URL https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.7.tar.gz)

# PhysicsFS
FetchContent_Declare(physicsfs URL https://github.com/icculus/physfs/archive/refs/tags/release-3.2.0.tar.gz)

FetchContent_MakeAvailable(sfml box2d json logs tgui enum physicsfs)
# =============================================================================

# =============================================================================
# WaterEngine Library
# =============================================================================

file(GLOB_RECURSE ENGINE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h
)

add_library(${WATER_ENGINE} STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(${WATER_ENGINE} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(${WATER_ENGINE} PUBLIC
    SFML::Graphics
    SFML::Audio
    SFML::Main
    box2d
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    TGUI::TGUI
    magic_enum::magic_enum
    physfs
)