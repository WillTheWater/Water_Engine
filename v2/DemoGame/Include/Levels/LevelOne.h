// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h
#include "Framework/World/World.h"
#include "Utility/Timer.h"

namespace we
{
    class LevelOne : public World
    {
    public:
        explicit LevelOne(EngineSubsystem& Subsystem);

        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;
        virtual void Render() override;

        void OnTimerFinished();

    private:
        shared<texture> BgTexture;
        optional<sprite> Background;
        TimerHandle TestTimer;
    };
}