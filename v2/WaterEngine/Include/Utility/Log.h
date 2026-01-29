// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include <spdlog/spdlog.h>

#define LOG(...) SPDLOG_INFO(__VA_ARGS__);
#define WARNING(...) SPDLOG_WARN(__VA_ARGS__);
#define ERROR(...) SPDLOG_ERROR(__VA_ARGS__);