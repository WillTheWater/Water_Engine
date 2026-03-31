// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"
#include "Subsystem/InputSubsystem.h"

namespace we
{
    class Actor;

    class Credits : public World
    {
    public:
        Credits(WorldSubsystem& Subsystem);

    protected:
        void BeginPlay() override;
        void Tick(float DeltaTime) override;
        void EndPlay() override;

    private:
        void OnInput();
        void Quit();

    private:
        shared<texture> BG;
        shared<Actor> BGImage;
        BindingHandle ExitBinding;
    };
}