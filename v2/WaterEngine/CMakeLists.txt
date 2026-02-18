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

# magic_enum
FetchContent_Declare(enum URL https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.7.tar.gz)

# PhysicsFS
set(PHYSFS_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_TEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(physicsfs URL https://github.com/icculus/physfs/archive/refs/tags/release-3.2.0.tar.gz)

# Populate dependencies
FetchContent_MakeAvailable(sfml box2d json logs enum physicsfs)
# =============================================================================

# =============================================================================
# WaterEngine Library
# =============================================================================

set(WATER_ENGINE_HEADERS
    Include/Entry.h
    Include/EngineConfig.h

    Include/Framework/WaterEngine.h
    Include/Framework/EngineSubsystem.h
    
    Include/Framework/EventHandling/GameWindowEventHandler.h
    
    Include/Core/CoreMinimal.h
    Include/Core/JsonTypes.h
    Include/Core/AudioTypes.h

    Include/Framework/World/World.h    
    Include/Framework/World/Object/Object.h
    Include/Framework/World/Actor/Actor.h
    Include/Framework/World/RenderTypes.h

    Include/Subsystem/WindowSubsystem.h
    Include/Subsystem/ResourceSubsystem.h
    Include/Subsystem/TimeSubsystem.h
    Include/Subsystem/RenderSubsystem.h
    Include/Subsystem/SaveLoadSubsystem.h    
    Include/Subsystem/AudioSubsystem.h
    Include/Subsystem/InputSubsystem.h
    Include/Subsystem/PhysicsSubsystem.h
    Include/Subsystem/WorldSubsystem.h
    Include/Subsystem/GameStateSubsystem.h
    Include/Subsystem/GUISubsystem.h
    Include/Subsystem/CursorSubsystem.h
    
    Include/PostProcess/IPostProcess.h
    Include/PostProcess/EmbeddedShaders.h
    Include/PostProcess/PPE/TestPPE.h
    Include/PostProcess/PPE/EmbeddedPPETest.h
    Include/PostProcess/PPE/TimePPETest.h
    Include/PostProcess/PPE/BloomPPE.h
    
    Include/UI/Clipboard/Clipboard.h

    Include/UI/Widget/Panel.h
    Include/UI/Widget/AutoPanel.h
    Include/UI/Widget/Spacer.h
    Include/UI/Widget/CheckBox.h
    Include/UI/Widget/Widget.h
    Include/UI/Widget/Button.h
    Include/UI/Widget/HorizontalBox.h
    Include/UI/Widget/VerticalBox.h
    Include/UI/Widget/GridBox.h
    Include/UI/Widget/Slider.h
    Include/UI/Widget/CheckBox.h
    Include/UI/Widget/ProgressBar.h
    Include/UI/Widget/TextBlock.h

    Include/Input/InputBinding.h
    Include/Input/InputEventHandler.h

    Include/AssetDirectory/PakDirectory.h

    Include/Interface/IAssetDirector.h
    Include/Interface/IGameStateToken.h
    Include/Interface/Component/IActorComponent.h
    Include/Interface/Component/IPhysicsComponent.h
    Include/Interface/Component/AnimationComponent.h
    Include/Interface/Component/PhysicsComponent.h
    
    Include/UI/UIUtility.h
    Include/Utility/Log.h
    Include/Utility/RandomGenerator.h
    Include/Utility/Assert.h
    Include/Utility/Delegate.h
    Include/Utility/Timer.h
    Include/Utility/DebugDraw.h
)

set(WATER_ENGINE_SOURCES
    Source/Entry.cpp
    Source/EngineConfig.cpp
	
    Source/Framework/WaterEngine.cpp
    
    Source/Framework/EventHandling/GameWindowEventHandler.cpp

    Source/Framework/World/World.cpp
    
    Source/Framework/World/Object/Object.cpp
    Source/Framework/World/Actor/Actor.cpp

    Source/Subsystem/WindowSubsystem.cpp
    Source/Subsystem/ResourceSubsystem.cpp
    Source/Subsystem/TimeSubsystem.cpp
    Source/Subsystem/RenderSubsystem.cpp
    Source/Subsystem/SaveLoadSubsystem.cpp
    
    Source/Subsystem/AudioSubsystem.cpp
    Source/Subsystem/InputSubsystem.cpp    
    Source/Subsystem/PhysicsSubsystem.cpp
    Source/Subsystem/WorldSubsystem.cpp
    Source/Subsystem/GameStateSubsystem.cpp
    Source/Subsystem/GUISubsystem.cpp
    Source/Subsystem/CursorSubsystem.cpp

    Source/PostProcess/PPE/TestPPE.cpp
    Source/PostProcess/PPE/EmbeddedPPETest.cpp
    Source/PostProcess/PPE/TimePPETest.cpp
    Source/PostProcess/PPE/BloomPPE.cpp

    Source/UI/Clipboard/Clipboard.cpp

    Source/UI/Widget/Panel.cpp
    Source/UI/Widget/AutoPanel.cpp
    Source/UI/Widget/CheckBox.cpp
    Source/UI/Widget/Widget.cpp
    Source/UI/Widget/Button.cpp
    Source/UI/Widget/HorizontalBox.cpp
    Source/UI/Widget/VerticalBox.cpp
    Source/UI/Widget/GridBox.cpp
    Source/UI/Widget/Slider.cpp
    Source/UI/Widget/CheckBox.cpp
    Source/UI/Widget/ProgressBar.cpp
    Source/UI/Widget/TextBlock.cpp

    Source/Input/InputBinding.cpp    
    Source/Input/InputEventHandler.cpp
    
    Source/AssetDirectory/PakDirectory.cpp

    Source/Interface/Component/AnimationComponent.cpp
    Source/Interface/Component/PhysicsComponent.cpp

    Source/Utility/RandomGenerator.cpp 
    Source/Utility/Timer.cpp
    Source/Utility/DebugDraw.cpp
)

add_library(${WATER_ENGINE} STATIC
    ${WATER_ENGINE_SOURCES}
    ${WATER_ENGINE_HEADERS}
)

# =============================================================================
# Precompiled Headers (PCH)
# =============================================================================
# This speeds up build times by pre-compiling CoreMinimal.h once
target_precompile_headers(${PROJECT_NAME} PRIVATE 
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/Include/Core/CoreMinimal.h>"
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
    magic_enum::magic_enum
    physfs-static
)