// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"
#include "Interface/Component/IActorComponent.h"

namespace we
{
    // Pure virtual interface for movement components
    // Allows querying movement intent without depending on physics implementation
    class IMovementComponent : public IActorComponent
    {
    public:
        virtual ~IMovementComponent() = default;

        // Get the current calculated velocity from input
        virtual vec2f GetVelocity() const = 0;

        // Get the facing direction (normalized)
        virtual vec2f GetForwardVector() const = 0;

        // Get the right vector relative to facing (normalized)
        virtual vec2f GetRightVector() const = 0;

        // Check if currently moving (velocity above threshold)
        virtual bool IsMoving() const = 0;

        // Speed configuration
        virtual void SetSpeed(float InSpeed) = 0;
        virtual float GetSpeed() const = 0;
    };
}
