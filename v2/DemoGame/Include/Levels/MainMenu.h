// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"
#include "UI/MainMenuUI.h"

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
		unique<MainMenuUI> MenuUI;
    };
}