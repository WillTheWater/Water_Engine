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
    class PhysicsSubsystem;

    struct PhysicsConfig
    {
        vec2f DefaultGravity;
        float PhysicsScale = 0.01f;
        float FixedTimeStep = 1.0f / 60.0f;
        int32 VelocityIterations = 8;
        int32 PositionIterations = 3;
    };

    struct ContactEvent
    {
        enum class Type { Begin, End };
        Type EventType;
        b2Body* BodyA;
        b2Body* BodyB;
        bool bSensorContact;  // True if at least one fixture is a sensor
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
        explicit PhysicsSubsystem(const PhysicsConfig& Config);
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
        b2World World;
        PhysicsConfig Config;
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
