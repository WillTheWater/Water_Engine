// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"

namespace we
{
    class Actor;

    class LevelOne : public World
    {
    public:
        LevelOne(WorldSubsystem& Subsystem);

    protected:
        void BeginPlay() override;
        void Tick(float DeltaTime) override;

    private:
        shared<texture> BG;
        shared<Actor> BGImage;
    };
}