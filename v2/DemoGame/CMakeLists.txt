# =============================================================================
# Water Engine v2.0.0 - DemoGame
# Copyright (C) 2026 Will The Water
# License: MIT (see LICENSE file for full text)
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
set(CONFIG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Config")
set(ENGINE_CONFIG_SOURCE "${CMAKE_SOURCE_DIR}/WaterEngine/Config/EngineConfig.ini")

# Debug: Copy everything loose for hot reload
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Copy Content folder
    if(EXISTS ${CONTENT_SRC_DIR})
        add_custom_command(
            TARGET ${DEMO_GAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONTENT_SRC_DIR} ${CONTENT_DEST_DIR}
            COMMENT "Copying Content folder"
        )
    endif()
    
    # Copy game configs to Content/Config
    add_custom_command(
        TARGET ${DEMO_GAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CONTENT_DEST_DIR}/Config
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONFIG_SRC_DIR} ${CONTENT_DEST_DIR}/Config
        COMMENT "Copying Config to Content/Config"
    )
    
    # Copy engine config to Content/Config
    add_custom_command(
        TARGET ${DEMO_GAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ENGINE_CONFIG_SOURCE} ${CONTENT_DEST_DIR}/Config/EngineConfig.ini
        COMMENT "Copying EngineConfig.ini to Content/Config"
    )
endif()

# Release: Pack Content into Content.pak
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(PAK_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Content.pak")
    set(STAGING_DIR "${CMAKE_CURRENT_BINARY_DIR}/ContentStaging")
    
    # Create a cmake script to handle the pak creation
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/PackContent.cmake" "
# Clean staging
file(REMOVE_RECURSE \"${STAGING_DIR}\")
file(MAKE_DIRECTORY \"${STAGING_DIR}\")

# Copy content
file(COPY \"${CONTENT_SRC_DIR}/\" DESTINATION \"${STAGING_DIR}\")

# Copy configs
file(MAKE_DIRECTORY \"${STAGING_DIR}/Config\")
file(COPY \"${CONFIG_SRC_DIR}/\" DESTINATION \"${STAGING_DIR}/Config\")
file(COPY \"${ENGINE_CONFIG_SOURCE}\" DESTINATION \"${STAGING_DIR}/Config\")

# Create pak (Assets/ will be at root)
execute_process(
    COMMAND \${CMAKE_COMMAND} -E tar cf \"${PAK_OUTPUT}\" --format=zip .
    WORKING_DIRECTORY \"${STAGING_DIR}\"
)
")
    
    add_custom_command(
        OUTPUT ${PAK_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/PackContent.cmake"
        DEPENDS ${CONTENT_SRC_DIR} ${CONFIG_SRC_DIR} ${ENGINE_CONFIG_SOURCE}
        COMMENT "Packing Content.pak"
    )
    
    add_custom_target(PackContents ALL DEPENDS ${PAK_OUTPUT})
    add_dependencies(${DEMO_GAME} PackContents)
endif()
