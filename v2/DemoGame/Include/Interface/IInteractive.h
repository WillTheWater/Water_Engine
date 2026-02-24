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

        // Called when a player interacts with this object
        virtual void OnInteract(Actor* Interactor) = 0;

        // Return true if this object can currently be interacted with
        virtual bool CanInteract() const { return true; }

        // Get interaction prompt text (e.g., "Talk", "Open", "Pick up")
        virtual string GetInteractionPrompt() const { return "Interact"; }
    };
}
