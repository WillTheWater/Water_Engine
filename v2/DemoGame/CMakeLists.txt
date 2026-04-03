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

# Configure the icon.rc with absolute path to icon
configure_file(
    "${CMAKE_SOURCE_DIR}/Content/Assets/Icon/icon.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/icon.rc"
)

# Include the generated RC in the executable sources
set(ICON_RC "${CMAKE_CURRENT_BINARY_DIR}/generated/icon.rc")

add_executable(${DEMO_GAME}
    ${GAME_SOURCES}
    ${GAME_HEADERS}
    ${ICON_RC}
)

target_include_directories(${DEMO_GAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(${DEMO_GAME} PUBLIC
    ${WATER_ENGINE}
)