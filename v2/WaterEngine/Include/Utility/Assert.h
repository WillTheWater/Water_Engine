// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include <cassert>

#ifdef NDEBUG
#define VERIFY(expr) void(expr)
#else
#define VERIFY(expr) assert(expr)
#endif