// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    class Actor;

    class IInteractable
    {
    public:
        virtual ~IInteractable() = default;
        virtual bool CanInteract(Actor* Interactor) const { return true; }
        virtual void Interact(Actor* Interactor) = 0;
    };
}
