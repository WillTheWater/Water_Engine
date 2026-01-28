# =============================================================================
# Water Engine v2.0.0
# Copyright (C) 2026 Will The Water
# License: MIT (see LICENSE file for full text)
# =============================================================================
cmake_minimum_required(VERSION 3.30.3)

project(WaterEngine
    VERSION 2.0.0 ## Iteration.Features.BugFixes
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS       OFF)

set(WATER_ENGINE WaterEngine)
set(DEMO_GAME DemoGame)

add_subdirectory(WaterEngine)
add_subdirectory(DemoGame)