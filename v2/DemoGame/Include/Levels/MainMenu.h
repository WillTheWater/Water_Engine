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
    class PostProcessingComponent;

    class MainMenu : public World
    {
    public:
        MainMenu(WorldSubsystem& Subsystem);

    protected:
        void BeginPlay() override;
        void Tick(float DeltaTime) override;
        void EndPlay() override;

    private:
		unique<MainMenuUI> MenuUI;
        shared<texture> BG;
        shared<Actor> BGImage;
        shared<PostProcessingComponent> PPCGrayscale;

    private:
        void Play();
        void Settings();
        void Quit();
    };
}