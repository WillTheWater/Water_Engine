// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/PhysicsSubsystem.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Utility/Log.h"
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>

namespace we
{
    PhysicsSubsystem& PhysicsSubsystem::Get()
    {
        static PhysicsSubsystem Instance;
        return Instance;
    }

    PhysicsSubsystem::PhysicsSubsystem()
        : World{ b2Vec2{0.0f, 9.8f} }
        , PhysicsScale{ 0.01f }
        , FixedTimeStep{ 1.0f / 60.0f }
        , Accumulator{ 0.0f }
        , VelocityIterations{ 8 }
        , PositionIterations{ 3 }
    {
        World.SetContactListener(&ContactListener);
    }

    PhysicsSubsystem::~PhysicsSubsystem()
    {
        Shutdown();
    }

    void PhysicsSubsystem::Initialize()
    {
        LOG("Physics initialized");
    }

    void PhysicsSubsystem::Shutdown()
    {
        ProcessPendingRemovals();
        LOG("Physics shutdown");
    }

    void PhysicsSubsystem::Step(float DeltaTime)
    {
        ProcessPendingRemovals();

        Accumulator += DeltaTime;
        while (Accumulator >= FixedTimeStep)
        {
            World.Step(FixedTimeStep, VelocityIterations, PositionIterations);
            Accumulator -= FixedTimeStep;
        }
    }

    void PhysicsSubsystem::ProcessPendingRemovals()
    {
        for (b2Body* Body : PendingRemovals)
        {
            World.DestroyBody(Body);
        }
        PendingRemovals.clear();
    }

    b2Body* PhysicsSubsystem::CreateBody(PhysicsComponent* Owner, BodyType Type, const vec2f& Position, float Rotation)
    {
        b2BodyDef BodyDef;

        switch (Type)
        {
        case BodyType::Static:    BodyDef.type = b2_staticBody; break;
        case BodyType::Kinematic: BodyDef.type = b2_kinematicBody; break;
        case BodyType::Dynamic:   BodyDef.type = b2_dynamicBody; break;
        }

        BodyDef.position.Set(Position.x * PhysicsScale, Position.y * PhysicsScale);
        BodyDef.angle = Rotation;
        BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(Owner);

        return World.CreateBody(&BodyDef);
    }

    void PhysicsSubsystem::DestroyBody(b2Body* Body)
    {
        if (Body)
        {
            PendingRemovals.insert(Body);
        }
    }

    void PhysicsSubsystem::SetGravity(const vec2f& Gravity)
    {
        World.SetGravity(b2Vec2{ Gravity.x, Gravity.y });
    }

    vec2f PhysicsSubsystem::GetGravity() const
    {
        b2Vec2 G = World.GetGravity();
        return { G.x, G.y };
    }

    void PhysicsContactListener::BeginContact(b2Contact* Contact)
    {
        auto* CompA = reinterpret_cast<PhysicsComponent*>(Contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        auto* CompB = reinterpret_cast<PhysicsComponent*>(Contact->GetFixtureB()->GetBody()->GetUserData().pointer);

        if (CompA) CompA->OnBeginOverlap(CompB);
        if (CompB) CompB->OnBeginOverlap(CompA);
    }

    void PhysicsContactListener::EndContact(b2Contact* Contact)
    {
        auto* CompA = reinterpret_cast<PhysicsComponent*>(Contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        auto* CompB = reinterpret_cast<PhysicsComponent*>(Contact->GetFixtureB()->GetBody()->GetUserData().pointer);

        if (CompA) CompA->OnEndOverlap(CompB);
        if (CompB) CompB->OnEndOverlap(CompA);
    }
}