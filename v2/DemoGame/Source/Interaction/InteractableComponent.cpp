// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interaction/InteractableComponent.h"
#include "Framework/World/Actor.h"
#include "Utility/Log.h"

namespace we
{
    InteractableComponent::InteractableComponent(Actor* InOwner)
        : Owner(InOwner)
    {
    }

    void InteractableComponent::Interact(Actor* Interactor)
    {
        if (!CanInteract(Interactor))
            return;

        LOG("[InteractableComponent] {} interacted with {}", 
            Interactor ? Interactor->GetID() : 0,
            Owner ? Owner->GetID() : 0);
    }
}
