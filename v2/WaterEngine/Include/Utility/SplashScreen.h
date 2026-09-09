// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

// Guard must match SplashScreen.cpp and the call site in Entry.cpp
#if defined(NDEBUG) && defined(WIN32)
namespace we
{
    // Release-only pre-launch splash. Reads SplashConfig::TexturePath from
    // Content.pak and animates it in a layered Win32 popup with per-pixel alpha.
    // Mounts PhysicsFS itself; ResourceSubsystem skips re-init if already mounted.
    // Call once from main() before the engine starts. Blocks until it finishes.
    void ShowSplash();
}
#endif
