// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"
#include "Subsystem/InputSubsystem.h"
#include "Interface/Actor/IActorComponent.h"

namespace we
{
    class Actor;
    class PostProcessingComponent;

    class Credits : public World
    {
    public:
        Credits(WorldSubsystem& Subsystem);

    protected:
        void BeginPlay() override;
        void Tick(float DeltaTime) override;
        void EndPlay() override;

    private:
        void Quit();

    private:
        shared<texture> Ground;
        shared<texture> Clouds;
        shared<Actor> GroundImage;
        shared<Actor> CloudImage;
        shared<PostProcessingComponent> CloudsPPC;
    };
}