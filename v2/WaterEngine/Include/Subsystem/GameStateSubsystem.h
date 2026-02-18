// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Utility/Delegate.h"
#include "Interface/IGameStateToken.h"

namespace we
{
    class GameStateSubsystem
    {
    public:
        void RequestStateChange(shared<IGameStateToken> NewState, bool bInForce = false);
        void ApplyPendingState();

        shared<IGameStateToken> GetCurrentState() const { return CurrentState; }
        bool IsTransitionPending() const { return PendingState.has_value(); }

        Delegate<shared<IGameStateToken>> OnStateEnter;
        Delegate<shared<IGameStateToken>> OnStateExit;
        Delegate<> OnQuitRequested;

    private:
        shared<IGameStateToken> CurrentState;
        optional<shared<IGameStateToken>> PendingState;
    };
}