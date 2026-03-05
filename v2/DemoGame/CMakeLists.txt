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

target_include_directories(${DEMO_GAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(${DEMO_GAME} PUBLIC
    ${WATER_ENGINE}
)