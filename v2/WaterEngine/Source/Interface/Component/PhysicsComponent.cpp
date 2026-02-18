// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interface/Component/PhysicsComponent.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Assert.h"
#include "Utility/Log.h"
#include "Utility/DebugDraw.h"
#include "Subsystem/PhysicsSubsystem.h"
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

namespace we
{
    PhysicsComponent::PhysicsComponent(Actor* InOwner)
        : Owner{ InOwner }
        , Body{ nullptr }
        , Fixture{ nullptr }
        , Type{ BodyType::Dynamic }
        , ShapeHalfExtents{ 50.0f, 50.0f }
        , ShapeRadius{ 50.0f }
        , bIsCircle{ false }
        , Density{ 1.0f }
        , Friction{ 0.3f }
        , Restitution{ 0.0f }
        , bSensor{ false }
        , bNeedsFixtureUpdate{ false }
    {
    }

    PhysicsComponent::~PhysicsComponent()
    {
        if (Body)
        {
            GetPhysics().DestroyBody(Body);
            Body = nullptr;
        }
    }

    void PhysicsComponent::BeginPlay()
    {
        if (!Owner) return;

        vec2f Pos = Owner->GetPosition();
        float Rot = Owner->GetRotation().asRadians();

        Body = GetPhysics().CreateBody(this, Type, Pos, Rot);
        RecreateFixture();
    }

    void PhysicsComponent::Tick(float DeltaTime)
    {
        if (!Body) return;

        switch (Type)
        {
        case BodyType::Kinematic:
            SyncActorToBody();
            break;

        case BodyType::Dynamic:
            SyncBodyToActor();
            break;

        case BodyType::Static:
        default:
            break;
        }

        if (bNeedsFixtureUpdate)
        {
            RecreateFixture();
            bNeedsFixtureUpdate = false;
        }

        // Debug draw the physics shape
        if (DebugDraw::IsEnabled())
        {
            DrawDebugShape();
        }
    }

    void PhysicsComponent::DrawDebugShape()
    {
        if (!Owner) return;

        vec2f Pos = Owner->GetPosition();
        float Rot = Owner->GetRotation().asRadians();

        // Choose color based on body type
        color DebugColor;
        switch (Type)
        {
        case BodyType::Static:    DebugColor = color{ 255, 255, 0 }; break;  // Yellow
        case BodyType::Kinematic: DebugColor = color{ 0, 255, 255 }; break;  // Cyan
        case BodyType::Dynamic:   DebugColor = color{ 0, 255, 0 }; break;    // Green
        default:                  DebugColor = color::Red; break;
        }

        if (bSensor)
        {
            DebugColor = color{ 255, 0, 255 };  // Magenta for sensors
        }

        if (bIsCircle)
        {
            DebugDraw::Circle(Pos, ShapeRadius, DebugColor, 2.0f);
        }
        else
        {
            DebugDraw::Rect(Pos, ShapeHalfExtents, Rot, DebugColor, 2.0f);
        }
    }

    void PhysicsComponent::EndPlay()
    {
        if (Body)
        {
            GetPhysics().DestroyBody(Body);
            Body = nullptr;
        }
    }

    Actor* PhysicsComponent::GetOwner() const
    {
        return Owner;
    }

    void PhysicsComponent::OnBeginOverlap(IPhysicsComponent* Other)
    {
        // Override in derived classes to handle overlap begin
        (void)Other;
    }

    void PhysicsComponent::OnEndOverlap(IPhysicsComponent* Other)
    {
        // Override in derived classes to handle overlap end
        (void)Other;
    }

    void PhysicsComponent::SetBodyType(BodyType NewType)
    {
        Type = NewType;
        if (Body)
        {
            b2BodyType B2Type = b2_staticBody;
            switch (Type)
            {
            case BodyType::Kinematic: B2Type = b2_kinematicBody; break;
            case BodyType::Dynamic:   B2Type = b2_dynamicBody; break;
            default: break;
            }
            Body->SetType(B2Type);
        }
    }

    void PhysicsComponent::SetBoxShape(const vec2f& HalfExtents)
    {
        ShapeHalfExtents = HalfExtents;
        bIsCircle = false;
        bNeedsFixtureUpdate = true;
    }

    void PhysicsComponent::SetCircleShape(float Radius)
    {
        ShapeRadius = Radius;
        bIsCircle = true;
        bNeedsFixtureUpdate = true;
    }

    void PhysicsComponent::SetDensity(float NewDensity)
    {
        Density = NewDensity;
        bNeedsFixtureUpdate = true;
    }

    void PhysicsComponent::SetFriction(float NewFriction)
    {
        Friction = NewFriction;
        bNeedsFixtureUpdate = true;
    }

    void PhysicsComponent::SetRestitution(float NewRestitution)
    {
        Restitution = NewRestitution;
        bNeedsFixtureUpdate = true;
    }

    void PhysicsComponent::SetSensor(bool bIsSensor)
    {
        bSensor = bIsSensor;
        bNeedsFixtureUpdate = true;
    }

    vec2f PhysicsComponent::GetVelocity() const
    {
        if (!Body) return vec2f{};
        auto Scale = GetPhysics().GetPhysicsScale();
        b2Vec2 V = Body->GetLinearVelocity();
        return { V.x / Scale, V.y / Scale };
    }

    void PhysicsComponent::SetVelocity(const vec2f& Velocity)
    {
        if (!Body) return;
        auto Scale = GetPhysics().GetPhysicsScale();
        Body->SetLinearVelocity(b2Vec2{ Velocity.x * Scale, Velocity.y * Scale });
    }

    void PhysicsComponent::ApplyForce(const vec2f& Force)
    {
        if (!Body) return;
        Body->ApplyForceToCenter(b2Vec2{ Force.x, Force.y }, true);
    }

    void PhysicsComponent::ApplyImpulse(const vec2f& Impulse)
    {
        if (!Body) return;
        Body->ApplyLinearImpulseToCenter(b2Vec2{ Impulse.x, Impulse.y }, true);
    }

    void PhysicsComponent::SyncActorToBody()
    {
        if (!Owner || !Body) return;

        vec2f Pos = Owner->GetPosition();
        float Rot = Owner->GetRotation().asRadians();

        float Scale = GetPhysics().GetPhysicsScale();
        Body->SetTransform(b2Vec2{ Pos.x * Scale, Pos.y * Scale }, Rot);
    }

    void PhysicsComponent::SyncBodyToActor()
    {
        if (!Owner || !Body) return;

        float Scale = GetPhysics().GetPhysicsScale();
        b2Vec2 Pos = Body->GetPosition();
        float Rot = Body->GetAngle();

        Owner->SetPosition({ Pos.x / Scale, Pos.y / Scale });
        Owner->SetRotation(sf::radians(Rot));
    }

    void PhysicsComponent::RecreateFixture()
    {
        if (!Body) return;

        if (Fixture)
        {
            Body->DestroyFixture(Fixture);
            Fixture = nullptr;
        }

        float Scale = GetPhysics().GetPhysicsScale();

        if (bIsCircle)
        {
            b2CircleShape Shape;
            Shape.m_radius = ShapeRadius * Scale;

            b2FixtureDef Def;
            Def.shape = &Shape;
            Def.density = Density;
            Def.friction = Friction;
            Def.restitution = Restitution;
            Def.isSensor = bSensor;

            Fixture = Body->CreateFixture(&Def);
        }
        else
        {
            b2PolygonShape Shape;
            Shape.SetAsBox(ShapeHalfExtents.x * Scale, ShapeHalfExtents.y * Scale);

            b2FixtureDef Def;
            Def.shape = &Shape;
            Def.density = Density;
            Def.friction = Friction;
            Def.restitution = Restitution;
            Def.isSensor = bSensor;

            Fixture = Body->CreateFixture(&Def);
        }
    }

    PhysicsSubsystem& PhysicsComponent::GetPhysics() const
    {
        VERIFY(Owner && "PhysicsComponent has no owner - component may have been destroyed");
        VERIFY(Owner->GetWorld() && "Actor has no world - actor may have been removed from world");
        return *Owner->GetWorld()->GetSubsystem().Physics;
    }
}
