# =============================================================================
# Water Engine v2.0.0 - DemoGame
# Copyright (C) 2026 Will The Water
# License: MIT (see LICENSE file for full text)
# =============================================================================

# =============================================================================
# DemoGame/CMakesLists.txt
# =============================================================================

file(GLOB_RECURSE GAME_HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h"
)

file(GLOB_RECURSE GAME_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)

add_executable(${DEMO_GAME}
    ${GAME_SOURCES}
    ${GAME_HEADERS}
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
# Asset and Config Handling - Debug vs Release
# =============================================================================

set(CONTENT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Content")
set(CONTENT_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/Content")
set(GAME_CONFIG_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Config/GameConfig.ini")
set(ENGINE_CONFIG_SOURCE "${CMAKE_SOURCE_DIR}/WaterEngine/Config/EngineConfig.ini")

# Debug: Copy everything loose for hot reload
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "")
    # Copy Content folder
    if(EXISTS ${CONTENT_SRC_DIR})
        add_custom_command(
            TARGET ${DEMO_GAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONTENT_SRC_DIR} ${CONTENT_DEST_DIR}
            COMMENT "Copying Content folder to build directory for hot reload"
        )
    endif()
    
    # Copy config files loose
    if(EXISTS ${GAME_CONFIG_SOURCE})
        add_custom_command(
            TARGET ${DEMO_GAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GAME_CONFIG_SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/GameConfig.ini
            COMMENT "Copying GameConfig.ini to build directory"
        )
    endif()
    
    add_custom_command(
        TARGET ${DEMO_GAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ENGINE_CONFIG_SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/EngineConfig.ini
        COMMENT "Copying EngineConfig.ini to build directory"
    )
endif()

# Release: Pack Content + Configs into Content.pak
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(PAK_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Content.pak")
    
    # Create a temp staging directory for packing
    set(STAGING_DIR "${CMAKE_CURRENT_BINARY_DIR}/ContentStaging")
    
    add_custom_command(
        TARGET ${DEMO_GAME} POST_BUILD
        # Clean staging dir
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${STAGING_DIR}
        # Make staging dir with Content subfolder
        COMMAND ${CMAKE_COMMAND} -E make_directory ${STAGING_DIR}/Content
        # Copy Content assets to staging/Content
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONTENT_SRC_DIR} ${STAGING_DIR}/Content
        # Copy configs into staging/Content/Config folder
        COMMAND ${CMAKE_COMMAND} -E make_directory ${STAGING_DIR}/Content/Config
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ENGINE_CONFIG_SOURCE} ${STAGING_DIR}/Content/Config/EngineConfig.ini
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GAME_CONFIG_SOURCE} ${STAGING_DIR}/Content/Config/GameConfig.ini
        # Create pak from staging (so Content/ is at root of pak)
        COMMAND ${CMAKE_COMMAND} -E chdir ${STAGING_DIR} ${CMAKE_COMMAND} -E tar cf ${PAK_OUTPUT} --format=zip Content
        COMMENT "Packing Content + Configs into Content.pak"
    )
endif()
