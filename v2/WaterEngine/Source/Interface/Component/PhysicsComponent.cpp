// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interface/Component/PhysicsComponent.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Utility/Log.h"
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
        , bIsOverlapping{ false }
        , OverlapCount{ 0 }
        , bNeedsFixtureUpdate{ false }
        , bDebugDraw{ false }
    {
    }

    PhysicsComponent::~PhysicsComponent()
    {
        if (Body)
        {
            Physics().DestroyBody(Body);
            Body = nullptr;
        }
    }

    void PhysicsComponent::BeginPlay()
    {
        if (!Owner) return;

        vec2f Pos = Owner->GetPosition();
        float Rot = Owner->GetRotation().asRadians();

        Body = Physics().CreateBody(this, Type, Pos, Rot);
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
    }

    void PhysicsComponent::EndPlay()
    {
        if (Body)
        {
            Physics().DestroyBody(Body);
            Body = nullptr;
        }
    }

    Actor* PhysicsComponent::GetOwner() const
    {
        return Owner;
    }

    void PhysicsComponent::OnBeginOverlap(PhysicsComponent* Other)
    {
        LOG("Begin overlap: {} with {}",
            Owner ? "Actor" : "None",
            Other && Other->GetOwner() ? "Other" : "None");
        bIsOverlapping = true;
        OverlapCount++;
    }

    void PhysicsComponent::OnEndOverlap(PhysicsComponent* Other)
    {
        LOG("End overlap");
        bIsOverlapping = false;
        OverlapCount = std::max(0, OverlapCount - 1);
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
        auto Scale = Physics().GetPhysicsScale();
        b2Vec2 V = Body->GetLinearVelocity();
        return { V.x / Scale, V.y / Scale };
    }

    void PhysicsComponent::SetVelocity(const vec2f& Velocity)
    {
        if (!Body) return;
        auto Scale = Physics().GetPhysicsScale();
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

        float Scale = Physics().GetPhysicsScale();
        Body->SetTransform(b2Vec2{ Pos.x * Scale, Pos.y * Scale }, Rot);
    }

    void PhysicsComponent::SyncBodyToActor()
    {
        if (!Owner || !Body) return;

        float Scale = Physics().GetPhysicsScale();
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

        float Scale = Physics().GetPhysicsScale();

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

    void PhysicsComponent::DebugDraw(RenderSubsystem* Render)
    {
        if (!bDebugDraw || !Owner || !Render) return;

        vec2f Pos = Owner->GetPosition();
        angle Rot = Owner->GetRotation();

        color DebugColor = (OverlapCount > 0) ? color::Green : color::Red;

        if (bIsCircle)
        {
            circle Circle(ShapeRadius);
            Circle.setPosition(Pos);
            Circle.setRotation(Rot);
            Circle.setOrigin({ ShapeRadius, ShapeRadius });
            Circle.setFillColor(color::Transparent);
            Circle.setOutlineColor(DebugColor);
            Circle.setOutlineThickness(2.0f);
            Render->Draw(Circle);
        }
        else
        {
            rectangle Rect({ ShapeHalfExtents.x * 2, ShapeHalfExtents.y * 2 });
            Rect.setPosition(Pos);
            Rect.setRotation(Rot);
            Rect.setOrigin({ ShapeHalfExtents.x, ShapeHalfExtents.y });
            Rect.setFillColor(color::Transparent);
            Rect.setOutlineColor(DebugColor);
            Rect.setOutlineThickness(2.0f);
            Render->Draw(Rect);
        }
    }
}