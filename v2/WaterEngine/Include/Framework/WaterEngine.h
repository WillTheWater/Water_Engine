// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
    class WaterEngine
    {
    public:
        WaterEngine();

        bool IsRunning() const;
        bool HasFocus() const;
        void ProcessEvents();
        void Update();
        void Render();

    private:
        void Initialize();
        void BindDelegates();
        void SetCameraView(vec2u WindowSize);

    private:
        EngineSubsystem Subsystem;
    };
}
