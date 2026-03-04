// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    // =========================================================================
    // UIScreen - Base class for UI screens
    // Inherit and override Draw() to implement screen content
    // =========================================================================
    class UIScreen
    {
    public:
        virtual ~UIScreen() = default;

        // Called when screen is pushed to stack
        virtual void OnOpen() {}

        // Called when screen is popped from stack
        virtual void OnClose() {}

        // Called when another screen is pushed on top
        virtual void OnPause() {}

        // Called when screen above is popped
        virtual void OnResume() {}

        // Called each frame - put ImGui code here
        virtual void Draw() = 0;

        // If true, game doesn't receive input
        bool bBlockGameInput = true;
    };
}
