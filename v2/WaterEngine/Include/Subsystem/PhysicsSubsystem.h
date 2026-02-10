// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>

namespace we
{
    class PhysicsComponent;
    class RenderSubsystem;

    enum class BodyType : uint8
    {
        Static,     // Default, never moves
        Kinematic,  // Moved by code, affects dynamic
        Dynamic     // Moved by physics, gravity, collisions
    };

    class PhysicsContactListener : public b2ContactListener
    {
    public:
        void BeginContact(b2Contact* Contact) override;
        void EndContact(b2Contact* Contact) override;
    };

    class PhysicsSubsystem
    {
    public:
        static PhysicsSubsystem& Get();

        void Initialize();
        void Shutdown();

        void Step(float DeltaTime);

        // Body management
        b2Body* CreateBody(PhysicsComponent* Owner, BodyType Type, const vec2f& Position, float Rotation);
        void DestroyBody(b2Body* Body);

        // Component Registration
        void RegisterComponent(PhysicsComponent* Comp);
        void UnregisterComponent(PhysicsComponent* Comp);
        void DebugDraw(RenderSubsystem* Render);

        // World properties
        void SetGravity(const vec2f& Gravity);
        vec2f GetGravity() const;

        float GetPhysicsScale() const { return PhysicsScale; }

    private:
        PhysicsSubsystem();
        ~PhysicsSubsystem();

        void ProcessPendingRemovals();

    private:
        b2World World;
        float PhysicsScale;
        float FixedTimeStep;
        float Accumulator;
        int VelocityIterations;
        int PositionIterations;

        PhysicsContactListener ContactListener;
        set<b2Body*> PendingRemovals;
        set<PhysicsComponent*> Components;
    };

    inline PhysicsSubsystem& Physics() { return PhysicsSubsystem::Get(); }
}