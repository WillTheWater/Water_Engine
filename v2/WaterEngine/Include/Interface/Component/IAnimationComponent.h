// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"
#include "Interface/Component/IActorComponent.h"

namespace we
{
    // Pure virtual interface for animation components
    // Allows querying and controlling animation without depending on concrete implementation
    class IAnimationComponent : public IActorComponent
    {
    public:
        virtual ~IAnimationComponent() = default;

        // State Management
        virtual void Transition(uint8 StateID) = 0;
        virtual uint8 GetCurrentState() const = 0;
        virtual bool IsPlaying(uint8 StateID) const = 0;

        // Playback Control
        virtual void SetGlobalPlaybackSpeed(float Multiplier) = 0;
        virtual float GetGlobalPlaybackSpeed() const = 0;

        // Facing/Direction
        virtual void SetFacing(bool bInFaceLeft) = 0;
        virtual bool IsFacingLeft() const = 0;

        // Sheet Management
        virtual void SetActiveSpriteSheet(uint8 SheetID) = 0;
        virtual uint8 GetActiveSpriteSheet() const = 0;
    };
}
