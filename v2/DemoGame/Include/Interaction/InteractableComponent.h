// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"
#include "Interaction/IInteractable.h"

namespace we
{
    class InteractableComponent : public IActorComponent, public IInteractable
    {
    public:
        explicit InteractableComponent(Actor* InOwner);

        // IActorComponent
        void BeginPlay() override {}
        void Tick(float DeltaTime) override {}
        void EndPlay() override {}
        Actor* GetOwner() const override { return Owner; }

        // IInteractable
        bool CanInteract(Actor* Interactor) const override { return bCanInteract; }
        void Interact(Actor* Interactor) override;

        void SetCanInteract(bool bValue) { bCanInteract = bValue; }

    protected:
        Actor* Owner = nullptr;
        bool bCanInteract = true;
    };
}
