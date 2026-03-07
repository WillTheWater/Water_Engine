// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"
#include "Subsystem/TimerSubsystem.h"

namespace we
{
    class Actor;

    class MainMenu : public World
    {
    public:
        MainMenu(WorldSubsystem& Subsystem);

    protected:
        void BeginPlay() override;
        void Tick(float DeltaTime) override;

    private:
        void DestroyTestActor();

        void SetupInputTests();
        float TestMovementX = 0.0f;
        float TestMovementY = 0.0f;
        
        
        float FireTimer = 0.0f;
        const float FireInterval = 0.1f;

        shared<Actor> TestActor;
        TimerHandle DestroyTimerHandle;
    };
}