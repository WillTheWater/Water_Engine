// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Utility/Delegate.h"

namespace we
{
    enum class GameState
    {
        None,
        MainMenu,
        Level1
    };

    class GameStateSubsystem
    {
    public:
        void RequestStateChange(GameState NewState);
        void ApplyPendingState();

        GameState GetCurrentState() const { return CurrentState; }
        bool IsTransitionPending() const { return PendingState != GameState::None; }

        Delegate<> OnStateEnter;
        Delegate<> OnStateExit;
        Delegate<> OnQuitRequested;

    private:
        GameState CurrentState = GameState::None;
        GameState PendingState = GameState::None;
    };
}