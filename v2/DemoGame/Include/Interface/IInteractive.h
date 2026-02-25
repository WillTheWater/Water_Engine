// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    class Actor;

    // =============================================================================
    // IInteractive Interface
    // =============================================================================
    // Implemented by actors that can be interacted with (NPCs, chests, doors, etc.)
    // =============================================================================

    class IInteractive
    {
    public:
        virtual ~IInteractive() = default;

        virtual void OnInteract(Actor* Interactor) = 0;
        virtual bool CanInteract() const { return true; }
        virtual string GetInteractionPrompt() const { return "Interact"; }
    };
}
