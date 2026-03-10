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

        virtual void StartPlay() {}

        bool IsRunning() const;
        bool HasFocus() const;
        void ProcessEvents();
        void Update();
        void Render();
        void ProcessQuit();

    protected:
        EngineSubsystem Subsystem;

    private:
        void Initialize();
        void BindDelegates();
    };
}