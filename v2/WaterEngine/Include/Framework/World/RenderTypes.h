// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    // Render Depth for sorted drawing
    // Used by World to provide drawables to the engine with depth info
    struct RenderDepth
    {
        const sf::Drawable* Drawable;
        float Depth;  // Y-position for top-down sorting (lower on screen = higher value = in front)

        // For std::sort - lower depth drawn first (back to front)
        bool operator<(const RenderDepth& Other) const
        {
            return Depth < Other.Depth;
        }
    };
}