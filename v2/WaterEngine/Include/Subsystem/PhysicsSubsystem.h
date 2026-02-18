// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Component/IPhysicsComponent.h"
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>

namespace we
{
    class PhysicsComponent;
    class EngineSubsystem;
    class PhysicsSubsystem;

    struct ContactEvent
    {
        enum class Type { Begin, End };
        Type EventType;
        b2Body* BodyA;
        b2Body* BodyB;
    };

    class PhysicsContactListener : public b2ContactListener
    {
    public:
        PhysicsContactListener(PhysicsSubsystem& InPhysics);
        void BeginContact(b2Contact* Contact) override;
        void EndContact(b2Contact* Contact) override;

    private:
        PhysicsSubsystem& Physics;
    };

    class PhysicsSubsystem
    {
    public:
        PhysicsSubsystem(EngineSubsystem& InSubsystem);
        ~PhysicsSubsystem();

        void Initialize();
        void Shutdown();

        void Step(float DeltaTime);

        b2Body* CreateBody(PhysicsComponent* Owner, BodyType Type, const vec2f& Position, float Rotation);
        void DestroyBody(b2Body* Body);

        void SetGravity(const vec2f& Gravity);
        vec2f GetGravity() const;

        float GetPhysicsScale() const { return PhysicsScale; }

        void QueueContactEvent(const ContactEvent& Event);
        void ProcessContactEvents();

    private:
        void ProcessPendingRemovals();

    private:
        EngineSubsystem& Subsystem;
        b2World World;
        float PhysicsScale;
        float FixedTimeStep;
        float Accumulator;
        int32 VelocityIterations;
        int32 PositionIterations;
        PhysicsContactListener ContactListener;

        set<b2Body*> PendingRemovals;
        vector<ContactEvent> ContactEvents;
    };
}
