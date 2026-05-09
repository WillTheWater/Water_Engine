// =============================================================================
// Water Engine v2.1.1
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#ifdef NDEBUG
namespace we
{
    // Shows the release splash screen and blocks until it finishes.
    // Call once from main() before the engine is started.
    void ShowSplash();
}
#endif // NDEBUG
