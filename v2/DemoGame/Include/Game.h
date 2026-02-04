// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include <Framework/WaterEngine.h>

namespace we
{
    class Actor;

    enum class GameAction
    {
        MainMenu = 1,
        Level1 = 2
    };

    class Game : public WaterEngine
    {
    public:
        Game();
        virtual void Tick(float DeltaTime) override;

        void HandleStartGame();
        void HandleExitGame();

    private:
        void BindInputs();
        void OnStateEnterHandler();

        weak<Actor> TestActor;
    };
}