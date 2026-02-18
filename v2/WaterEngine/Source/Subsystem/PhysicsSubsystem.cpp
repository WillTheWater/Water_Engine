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
    PhysicsContactListener::PhysicsContactListener(PhysicsSubsystem& InPhysics)
        : Physics{ InPhysics }
    {
    }

    void PhysicsContactListener::BeginContact(b2Contact* Contact)
    {
        b2Body* BodyA = Contact->GetFixtureA()->GetBody();
        b2Body* BodyB = Contact->GetFixtureB()->GetBody();

        if (BodyA && BodyB)
        {
            auto* CompA = reinterpret_cast<PhysicsComponent*>(BodyA->GetUserData().pointer);
            auto* CompB = reinterpret_cast<PhysicsComponent*>(BodyB->GetUserData().pointer);
            if (CompA && CompB)
            {
                LOG("PHYSICS CONTACT BEGIN: {:p} <-> {:p}", reinterpret_cast<void*>(CompA), reinterpret_cast<void*>(CompB));
            }
            // Only trigger overlap callbacks for sensor contacts
            bool bIsSensorContact = Contact->GetFixtureA()->IsSensor() || Contact->GetFixtureB()->IsSensor();
            Physics.QueueContactEvent({ ContactEvent::Type::Begin, BodyA, BodyB, bIsSensorContact });
        }
    }

    void PhysicsContactListener::EndContact(b2Contact* Contact)
    {
        b2Body* BodyA = Contact->GetFixtureA()->GetBody();
        b2Body* BodyB = Contact->GetFixtureB()->GetBody();

        if (BodyA && BodyB)
        {
            // Only trigger overlap callbacks for sensor contacts
            bool bIsSensorContact = Contact->GetFixtureA()->IsSensor() || Contact->GetFixtureB()->IsSensor();
            Physics.QueueContactEvent({ ContactEvent::Type::End, BodyA, BodyB, bIsSensorContact });
        }
    }

    PhysicsSubsystem::PhysicsSubsystem(const PhysicsConfig& InConfig)
        : World{ b2Vec2{ InConfig.DefaultGravity.x, InConfig.DefaultGravity.y } }
        , PhysicsScale{ InConfig.PhysicsScale }
        , FixedTimeStep{ InConfig.FixedTimeStep }
        , Accumulator{ 0.0f }
        , VelocityIterations{ InConfig.VelocityIterations }
        , PositionIterations{ InConfig.PositionIterations }
        , ContactListener{ *this }
        , Config{ InConfig }
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

        ProcessContactEvents();
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

    void PhysicsSubsystem::QueueContactEvent(const ContactEvent& Event)
    {
        ContactEvents.push_back(Event);
    }

    void PhysicsSubsystem::ProcessContactEvents()
    {
        for (const ContactEvent& Event : ContactEvents)
        {
            if (PendingRemovals.count(Event.BodyA) > 0 || PendingRemovals.count(Event.BodyB) > 0)
            {
                continue;
            }

            auto* CompA = reinterpret_cast<PhysicsComponent*>(Event.BodyA->GetUserData().pointer);
            auto* CompB = reinterpret_cast<PhysicsComponent*>(Event.BodyB->GetUserData().pointer);

            if (!CompA || !CompB)
            {
                continue;
            }

            // Only trigger overlap callbacks for sensor contacts (not physical collisions)
            if (!Event.bSensorContact)
            {
                continue;
            }

            if (Event.EventType == ContactEvent::Type::Begin)
            {
                CompA->OnBeginOverlap(CompB);
                CompB->OnBeginOverlap(CompA);
            }
            else
            {
                CompA->OnEndOverlap(CompB);
                CompB->OnEndOverlap(CompA);
            }
        }
        ContactEvents.clear();
    }
}
