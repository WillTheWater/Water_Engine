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
# Disable Network Module
set(SFML_BUILD_NETWORK OFF) 
# Use std::format for spdlog
set(SPDLOG_USE_STD_FORMAT ON) 
# TGUI Backend
set(TGUI_BACKEND SFML_GRAPHICS) 

# SFML 3.0.2
set(SFML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(sfml URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.tar.gz)

# Box2D 2.4.2
set(BOX2D_BUILD_SHARED OFF CACHE BOOL "" FORCE)
FetchContent_Declare(box2d URL https://github.com/erincatto/box2d/archive/refs/tags/v2.4.2.tar.gz)

# JSON
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)

# spdlog
FetchContent_Declare(logs URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz)

# TGUI
FetchContent_Declare(tgui URL https://github.com/texus/TGUI/archive/refs/tags/v1.11.0.tar.gz)

# magic_enum
FetchContent_Declare(enum URL https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.7.tar.gz)

# PhysicsFS
set(PHYSFS_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_TEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(physicsfs URL https://github.com/icculus/physfs/archive/refs/tags/release-3.2.0.tar.gz)

# Populate dependencies
FetchContent_MakeAvailable(sfml box2d json logs tgui enum physicsfs)
# =============================================================================

# =============================================================================
# WaterEngine Library
# =============================================================================

set(WATER_ENGINE_HEADERS
    Include/Entry.h
    Include/EngineConfig.h

    Include/Framework/WaterEngine.h
    Include/Framework/EngineSubsystem.h
    Include/Framework/GameWindow.h
    Include/Framework/GameWindowEventHandler.h

    Include/Framework/World/World.h    
    Include/Framework/World/Object/Object.h
    Include/Framework/World/Actor/Actor.h

    Include/Subsystem/TimerSubsystem.h
    Include/Subsystem/RenderSubsystem.h
    Include/Subsystem/SaveLoadSubsystem.h
    Include/Subsystem/ResourceSubsystem.h
    Include/Subsystem/AudioSubsystem.h
    Include/Subsystem/InputSubsystem.h

    Include/UI/Cursor/Cursor.h
    Include/UI/Clipboard/Clipboard.h
    Include/UI/Widget/WidgetUtility.h

    Include/Input/InputBinding.h
    Include/Input/InputEventHandler.h

    Include/Interface/IAssetDirector.h
    Include/AssetDirectory/PakDirectory.h
    
    Include/Utility/CoreMinimal.h
    Include/Utility/Log.h
    Include/Utility/RandomGenerator.h
    Include/Utility/Assert.h
    Include/Utility/Delegate.h
)

set(WATER_ENGINE_SOURCES
    Source/Entry.cpp
    Source/EngineConfig.cpp
	
    Source/Framework/WaterEngine.cpp
    Source/Framework/GameWindow.cpp
    Source/Framework/GameWindowEventHandler.cpp

    Source/Framework/World/World.cpp
    
    Source/Framework/World/Object/Object.cpp
    Source/Framework/World/Actor/Actor.cpp

    Source/Subsystem/TimerSubsystem.cpp
    Source/Subsystem/RenderSubsystem.cpp
    Source/Subsystem/SaveLoadSubsystem.cpp
    Source/Subsystem/ResourceSubsystem.cpp
    Source/Subsystem/AudioSubsystem.cpp
    Source/Subsystem/InputSubsystem.cpp    

    Source/UI/Cursor/Cursor.cpp
    Source/UI/Clipboard/Clipboard.cpp

    Source/Input/InputBinding.cpp    
    Source/Input/InputEventHandler.cpp

    Source/AssetDirectory/PakDirectory.cpp

    Source/Utility/RandomGenerator.cpp 
)

add_library(${WATER_ENGINE} STATIC
    ${WATER_ENGINE_SOURCES}
    ${WATER_ENGINE_HEADERS}
)

# =============================================================================
# Generate Embedded Config Header
# =============================================================================
set(CONFIG_JSON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Config/EngineConfig.json")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/Generated")
set(CONFIG_HEADER_PATH "${GENERATED_DIR}/EngineConfigData.h")

# Read JSON and escape it for C++ string literal at configure time
file(READ "${CONFIG_JSON_PATH}" CONFIG_JSON_RAW)
string(REPLACE "\\" "\\\\" CONFIG_JSON_ESCAPED "${CONFIG_JSON_RAW}")
string(REPLACE "\"" "\\\"" CONFIG_JSON_ESCAPED "${CONFIG_JSON_ESCAPED}")
string(REPLACE "\n" "\\n\"\n\"" CONFIG_JSON_ESCAPED "${CONFIG_JSON_ESCAPED}")

# Generated directory
file(MAKE_DIRECTORY "${GENERATED_DIR}")

# Create header file
file(WRITE "${CONFIG_HEADER_PATH}" 
"// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

namespace we 
{
    constexpr char EMBEDDED_ENGINE_CONFIG[] = \"${CONFIG_JSON_ESCAPED}\\n\";
}
")

target_include_directories(${WATER_ENGINE} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${GENERATED_DIR}
    ${physicsfs_SOURCE_DIR}/src
)

target_compile_definitions(${WATER_ENGINE} PUBLIC 
    SFML_STATIC
    PHYSFS_STATIC
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
    physfs-static
)