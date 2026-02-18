// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GameStateSubsystem.h"

namespace we
{
    void GameStateSubsystem::RequestStateChange(shared<IGameStateToken> NewState, bool bInForce)
    {
        if (!NewState) return;

        bool bIsDifferent = !CurrentState || (NewState->GetStateID() != CurrentState->GetStateID());

        if ((bIsDifferent || bInForce) &&
            (!PendingState.has_value() || (*PendingState)->GetStateID() != NewState->GetStateID()))
        {
            PendingState = NewState;
        }
    }

    void GameStateSubsystem::ApplyPendingState()
    {
        if (!PendingState.has_value()) return;

        auto OldState = CurrentState;
        CurrentState = *PendingState;
        PendingState.reset();

        if (OldState)
        {
            OnStateExit.Broadcast(OldState);
        }

        OnStateEnter.Broadcast(CurrentState);
    }
}