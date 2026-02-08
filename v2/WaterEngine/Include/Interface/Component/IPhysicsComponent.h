// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Interface/Component/IActorComponent.h"

class b2Body;
class b2Fixture;

namespace we
{
    enum class BodyType : uint8;
    class PhysicsComponent;

    class IPhysicsComponent : public IActorComponent
    {
    public:
        virtual ~IPhysicsComponent() = default;

        virtual void OnBeginOverlap(PhysicsComponent* Other) = 0;
        virtual void OnEndOverlap(PhysicsComponent* Other) = 0;

        virtual b2Body* GetPhysicsBody() const = 0;
    };
}