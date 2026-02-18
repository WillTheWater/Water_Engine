// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Component/IActorComponent.h"

class b2Body;
class b2Fixture;

namespace we
{
    enum class BodyType
    {
        Static,
        Kinematic,
        Dynamic
    };

    class IPhysicsComponent : public IActorComponent
    {
    public:
        virtual ~IPhysicsComponent() = default;

        // Overlap callbacks
        virtual void OnBeginOverlap(IPhysicsComponent* Other) = 0;
        virtual void OnEndOverlap(IPhysicsComponent* Other) = 0;

        // Physics body access
        virtual b2Body* GetPhysicsBody() const = 0;
    };
}
