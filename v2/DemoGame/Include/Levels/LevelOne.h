// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"
#include "Subsystem/InputSubsystem.h"

namespace we
{
    class Actor;
    class Character;
    class CollisionActor;
    class PauseMenuUI;
    class PostProcessingComponent;
    class LevelObject;

    class LevelOne : public World
    {
    public:
        LevelOne(WorldSubsystem& Subsystem);
        ~LevelOne();

    protected:
        void BeginPlay() override;
        void Tick(float DeltaTime);
        void EndPlay() override;

    private:
        void TogglePauseMenu();
        void PauseGame();
        void ResumeGame();
        void ReturnToMainMenu();
        void SaveAndQuit();
        void OnExitGame();

    private:
        shared<texture> BG;
        shared<texture> Water;
        shared<Actor> BGImage;
        shared<Actor> WaterImage;
        shared<texture> Hut1Tex;
        shared<texture> Hut1TexShadow;
        shared<Actor> Hut1;
        shared<Actor> Hut1Shadow;
        shared<texture> Hut2Tex;
        shared<texture> Hut2TexShadow;
        shared<Actor> Hut2;
        shared<Actor> Hut2Shadow;
        shared<texture> Hut3Tex;
        shared<texture> Hut3TexShadow;
        shared<Actor> Hut3;
        shared<Actor> Hut3Shadow;
        shared<PostProcessingComponent> WaterPPC;
        shared<Character> Character;
        unique<PauseMenuUI> PauseUI;
        
        unique<LevelObject> Tree1;

        BindingHandle PauseBinding;
    };
}