// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include <cassert>
#include "Utility/Log.h"

// VERIFY macro - checks condition, logs message on failure
// In Debug: asserts and logs
// In Release: just logs and continues
#ifdef NDEBUG
#define VERIFY(expr, ...) \
    do { \
        if (!(expr)) { \
            ERROR("VERIFY failed: " __VA_ARGS__); \
        } \
    } while(0)
#else
#define VERIFY(expr, ...) \
    do { \
        if (!(expr)) { \
            ERROR("VERIFY failed: " __VA_ARGS__); \
            assert(expr); \
        } \
    } while(0)
#endif