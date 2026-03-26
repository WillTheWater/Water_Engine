// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/EngineSubsystem.h"
#include "Core/EngineConfig.h"

namespace we
{
    class WaterEngine
    {
    public:
        WaterEngine();
        virtual ~WaterEngine();

        virtual void StartPlay();
        void Shutdown();

        bool IsRunning() const;
        bool HasFocus() const;
        void ProcessEvents();
        void Update();
        void Render();
        void ProcessQuit();

    protected:
        virtual shared<GameInstance> CreateGameInstance() { return make_shared<GameInstance>(); }
        
        EngineSubsystem Subsystem;

    private:
        void Initialize();
        void BindDelegates();
    };
}