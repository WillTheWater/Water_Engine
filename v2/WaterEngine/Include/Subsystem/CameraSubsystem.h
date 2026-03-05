// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    class CameraSubsystem
    {
    public:
        void Update(vec2u WindowSize);
        const view& GetView() const;

    private:
        view CameraView;
        vec2u RenderResolution = { 1920, 1080 };
    };
}