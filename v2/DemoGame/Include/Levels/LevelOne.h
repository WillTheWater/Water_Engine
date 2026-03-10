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
    class PauseMenuUI;

    class LevelOne : public World
    {
    public:
        LevelOne(WorldSubsystem& Subsystem);
        ~LevelOne();

    protected:
        void BeginPlay() override;
        void Tick(float DeltaTime) override;
        void EndPlay() override;

    private:
        void TogglePauseMenu();
        void PauseGame();
        void ResumeGame();
        void ReturnToMainMenu();
        void OnSettings();

    private:
        shared<texture> BG;
        shared<Actor> BGImage;
        unique<PauseMenuUI> PauseUI;
        bool bWasPausePressed = false;
    };
}