# =============================================================================
# Water Engine v2.0.0
# Copyright (C) 2026 Will The Water
# License: MIT (see LICENSE file for full text)
# =============================================================================

# =============================================================================
# DemoGame/CMakeLists.txt
# =============================================================================

# ========================== DEFINE PATHS ==========================

set(CONTENT_FOLDER_NAME "Content")
set(CONTENT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${CONTENT_FOLDER_NAME}")
set(ASSETS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${CONTENT_FOLDER_NAME}/Assets")
set(ICON_DIR "${ASSETS_SRC_DIR}/Icon")
set(PAK_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Contents.pak")

# ========================== GAME SOURCES ==========================

set(GAME_HEADERS
    Include/Game.h
    Include/GameStateTokens.h

    Include/Levels/MainMenu.h
    Include/Levels/LevelOne.h
    
    Include/UI/MainMenuUI.h
    Include/UI/PauseUI.h
    Include/UI/SettingsMenuUI.h
    
    Include/Input/InputActions.h

    Include/Character/PlayerCharacter.h
    Include/Character/MovementComponent.h
)

set(GAME_SOURCES
    Source/Game.cpp

    Source/Levels/MainMenu.cpp 
    Source/Levels/LevelOne.cpp
    
    Source/UI/MainMenuUI.cpp
    Source/UI/PauseUI.cpp
    Source/UI/SettingsMenuUI.cpp
    
    Source/Character/PlayerCharacter.cpp
    Source/Character/MovementComponent.cpp
)

# =============================================================================
# Executable icon (Windows only)
# =============================================================================

if (WIN32)
    configure_file("${ICON_DIR}/icon.rc" "${CMAKE_CURRENT_BINARY_DIR}/icon.rc" COPYONLY)
    configure_file("${ICON_DIR}/icon.ico" "${CMAKE_CURRENT_BINARY_DIR}/icon.ico" COPYONLY)
    set(GAME_PLATFORM_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/icon.rc")
endif()

# =============================================================================
# Game executable
# =============================================================================

add_executable(
    ${DEMO_GAME}
    ${GAME_SOURCES}
    ${GAME_HEADERS}
    ${GAME_PLATFORM_SOURCES}
)

if (WIN32)
    set_target_properties(${DEMO_GAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
endif()

target_include_directories(${DEMO_GAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(${DEMO_GAME} PUBLIC
    ${WATER_ENGINE}
)

# =============================================================================
# Asset packing
# =============================================================================

# ========================== ASSET PACKING ==========================

add_custom_command(
    OUTPUT ${PAK_OUTPUT}
    # Changes the working directory to the Content folder itself
    # so that 'Content' is at the root of the ZIP.
    COMMAND ${CMAKE_COMMAND} -E tar "cf" "${PAK_OUTPUT}" --format=zip "."
    WORKING_DIRECTORY "${CONTENT_SRC_DIR}"
    DEPENDS "${CONTENT_SRC_DIR}"
    COMMENT "Packing assets into Contents.pak"
)

add_custom_target(PackContents ALL DEPENDS ${PAK_OUTPUT})
add_dependencies(${DEMO_GAME} PackContents)