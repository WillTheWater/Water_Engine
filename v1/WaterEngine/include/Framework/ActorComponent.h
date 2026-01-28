#pragma once
#include "Framework/Object.h"

namespace we
{
    class Actor;

    class ActorComponent : public Object
    {
    public:
        explicit ActorComponent(Actor* OwningActor);

        virtual ~ActorComponent() = default;

        virtual void BeginPlay();
        virtual void Tick(float DeltaTime);

        Actor* GetOwner() const { return Owner; }

    protected:
        Actor* Owner;
    };
}