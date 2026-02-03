// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GameStateSubsystem.h"

namespace we
{
    void GameStateSubsystem::RequestStateChange(GameState NewState)
    {
        if (NewState != CurrentState && NewState != GameState::None)
            PendingState = NewState;
    }

    void GameStateSubsystem::ApplyPendingState()
    {
        if (PendingState == GameState::None) return;

        OnStateExit.Broadcast();
        CurrentState = PendingState;
        PendingState = GameState::None;
        OnStateEnter.Broadcast();
    }
}