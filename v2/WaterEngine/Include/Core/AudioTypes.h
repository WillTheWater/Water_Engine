// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    // Audio volume (0.0 = silent, 1.0 = full)
    using Volume = float;

    // Audio channel identifiers
    enum class AudioChannel : uint8
    {
        Master,
        Music,
        Ambient,
        SFX,
        Voice,
        UI,
        Count
    };

    // Audio playback properties
    struct AudioPlaybackConfig
    {
        bool bLoop = false;
        Volume Volume = 1.0f;
        float Pitch = 1.0f;  // 1.0 = normal, 0.5 = half speed, 2.0 = double speed
        
        // 3D audio (only for SFX)
        vec2f Position = { 0.0f, 0.0f };
        bool bSpatial = false;  // If true, uses 3D positioning
        float MinDistance = 0.0f;
        float MaxDistance = 1000.0f;
    };
}
