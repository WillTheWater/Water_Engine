// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"

namespace we
{
    class IAnimationComponent : public IActorComponent
    {
    public:
        virtual ~IAnimationComponent() = default;

        virtual void Transition(uint8 StateID) = 0;
        virtual uint8 GetCurrentState() const = 0;
        virtual bool IsPlaying(uint8 StateID) const = 0;

        virtual void SetGlobalPlaybackSpeed(float Multiplier) = 0;
        virtual float GetGlobalPlaybackSpeed() const = 0;

        virtual void SetFacing(bool bInFaceLeft) = 0;
        virtual bool IsFacingLeft() const = 0;
    };
}