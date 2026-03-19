// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/PhysicsComponent.h"
#include "Framework/World/Actor.h"
#include "Framework/World/World.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "box2d/b2_body.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "Utility/Log.h"

namespace we
{
    PhysicsComponent::PhysicsComponent(Actor* InOwner)
        : Owner(InOwner)
    {
    }

    void PhysicsComponent::BeginPlay()
    {
        LOG("[PhysicsComponent] BeginPlay on Actor {}", Owner ? Owner->GetID() : 0);
        CreateBody();
    }

    void PhysicsComponent::CreateBody()
    {
        if (!Owner)
        {
            ERROR("[PhysicsComponent] Cannot create body - no owner");
            return;
        }

        auto& Physics = Owner->GetWorld().GetPhysics();

        b2BodyDef BodyDef;
        BodyDef.type = BodyType;
        BodyDef.linearDamping = LinearDamping;  // Prevents sliding
        
        vec2f ActorPos = Owner->GetPosition();
        BodyDef.position = b2Vec2(
            Physics.PixelsToMeters(ActorPos.x),
            Physics.PixelsToMeters(ActorPos.y)
        );
        BodyDef.angle = 0.0f;

        Body = Physics.CreateBody(BodyDef);
        if (!Body)
        {
            ERROR("[PhysicsComponent] Failed to create body");
            return;
        }

        Body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

        CreateFixture();
    }

    void PhysicsComponent::CreateFixture()
    {
        if (!Body) return;

        auto& Physics = Owner->GetWorld().GetPhysics();

        // Create shape based on type
        if (ShapeType == EShapeType::Circle)
        {
            b2CircleShape CircleShape{};
            CircleShape.m_radius = Physics.PixelsToMeters(ShapeSize.x);
            
            b2FixtureDef FixtureDef;
            FixtureDef.shape = &CircleShape;
            FixtureDef.density = (BodyType == b2_dynamicBody) ? 1.0f : 0.0f;
            FixtureDef.friction = 0.3f;
            FixtureDef.restitution = 0.0f;
            FixtureDef.isSensor = false;

            Body->CreateFixture(&FixtureDef);
        }
        else  // Rectangle
        {
            b2PolygonShape BoxShape{};
            BoxShape.SetAsBox(
                Physics.PixelsToMeters(ShapeSize.x),
                Physics.PixelsToMeters(ShapeSize.y)
            );
            
            b2FixtureDef FixtureDef;
            FixtureDef.shape = &BoxShape;
            FixtureDef.density = (BodyType == b2_dynamicBody) ? 1.0f : 0.0f;
            FixtureDef.friction = 0.3f;
            FixtureDef.restitution = 0.0f;
            FixtureDef.isSensor = false;

            Body->CreateFixture(&FixtureDef);
            LOG("[PhysicsComponent] Rectangle fixture created, halfExtents={}m, {}m",
                Physics.PixelsToMeters(ShapeSize.x),
                Physics.PixelsToMeters(ShapeSize.y));
        }
    }

    void PhysicsComponent::Tick(float DeltaTime)
    {
        if (!Body || !Owner) return;

        // Sync actor position FROM physics body
        // (Physics body is the source of truth when it exists)
        SyncActorToBody();
    }

    void PhysicsComponent::EndPlay()
    {
        LOG("[PhysicsComponent] EndPlay on Actor {}", Owner ? Owner->GetID() : 0);
        DestroyBody();
    }

    void PhysicsComponent::DestroyBody()
    {
        if (!Body) return;

        LOG("[PhysicsComponent] Destroying body");

        if (Owner)
        {
            auto& Physics = Owner->GetWorld().GetPhysics();
            Physics.MarkForDestruction(Body);
        }

        Body = nullptr;
    }

    Actor* PhysicsComponent::GetOwner() const
    {
        return Owner;
    }

    void PhysicsComponent::SetBodyType(b2BodyType Type)
    {
        if (BodyType == Type) return;
        
        BodyType = Type;
        
        if (Body)
        {
            DestroyBody();
            CreateBody();
        }
    }

    void PhysicsComponent::SetShapeType(EShapeType Type)
    {
        if (ShapeType == Type) return;
        
        ShapeType = Type;
        
        if (Body)
        {
            DestroyBody();
            CreateBody();
        }
    }

    void PhysicsComponent::SetShapeSize(vec2f Size)
    {
        if (ShapeSize == Size) return;
        
        ShapeSize = Size;
        
        if (Body)
        {
            DestroyBody();
            CreateBody();
        }
    }

    void PhysicsComponent::SetLinearDamping(float Damping)
    {
        LinearDamping = Damping;
        
        if (Body)
        {
            Body->SetLinearDamping(Damping);
        }
    }

    void PhysicsComponent::SetVelocity(vec2f Velocity)
    {
        if (!Body) return;

        auto& Physics = Owner->GetWorld().GetPhysics();
        Body->SetLinearVelocity(b2Vec2(
            Physics.PixelsToMeters(Velocity.x),
            Physics.PixelsToMeters(Velocity.y)
        ));
    }

    vec2f PhysicsComponent::GetVelocity() const
    {
        if (!Body) return vec2f{};

        auto& Physics = Owner->GetWorld().GetPhysics();
        b2Vec2 Vel = Body->GetLinearVelocity();
        return vec2f(
            Physics.MetersToPixels(Vel.x),
            Physics.MetersToPixels(Vel.y)
        );
    }

    void PhysicsComponent::SyncBodyToActor()
    {
        // Call when actor is teleported/moved manually
        // Updates physics body to match actor position
        if (!Body || !Owner) return;

        auto& Physics = Owner->GetWorld().GetPhysics();
        vec2f ActorPos = Owner->GetPosition();
        
        Body->SetTransform(
            b2Vec2(Physics.PixelsToMeters(ActorPos.x), Physics.PixelsToMeters(ActorPos.y)),
            0.0f  // No rotation
        );
    }

    void PhysicsComponent::SyncActorToBody()
    {
        if (!Body || !Owner) return;

        auto& Physics = Owner->GetWorld().GetPhysics();
        b2Vec2 Pos = Body->GetPosition();
        
        Owner->SetPosition(vec2f(
            Physics.MetersToPixels(Pos.x),
            Physics.MetersToPixels(Pos.y)
        ));
    }

    void PhysicsComponent::SetMass(float Mass)
    {
        if (!Body || Mass <= 0.0f) return;

        b2MassData MassData;
        MassData.mass = Mass;
        MassData.center = b2Vec2(0.0f, 0.0f);
        MassData.I = 0.0f;
        
        Body->SetMassData(&MassData);
    }

    float PhysicsComponent::GetMass() const
    {
        if (!Body) return 0.0f;
        return Body->GetMass();
    }

    const drawable* PhysicsComponent::DrawDebug()
    {
        if (!Body || !Owner)
            return nullptr;

        if (ShapeType == EShapeType::Circle)
        {
            if (!DebugCircle.has_value())
            {
                float Radius = ShapeSize.x;
                DebugCircle = circle(Radius);
                DebugCircle->setOrigin({ Radius, Radius });
                DebugCircle->setFillColor(color::Transparent);
                DebugCircle->setOutlineThickness(2.0f);
            }

            DebugCircle->setPosition(Owner->GetPosition());

            // Color based on body type
            if (BodyType == b2_kinematicBody)
                DebugCircle->setOutlineColor(color::Cyan);      // Kinematic - player control
            else if (BodyType == b2_dynamicBody)
                DebugCircle->setOutlineColor(color::Magenta);   // Dynamic - physics driven
            else
                DebugCircle->setOutlineColor(color::White);     // Static - immovable

            return &DebugCircle.value();
        }
        else  // Rectangle
        {
            if (!DebugRect.has_value())
            {
                vec2f HalfExtents = ShapeSize;
                DebugRect = rectangle({ HalfExtents.x * 2.0f, HalfExtents.y * 2.0f });
                DebugRect->setOrigin(HalfExtents);
                DebugRect->setFillColor(color::Transparent);
                DebugRect->setOutlineThickness(2.0f);
            }

            DebugRect->setPosition(Owner->GetPosition());

            // Color based on body type
            if (BodyType == b2_kinematicBody)
                DebugRect->setOutlineColor(color::Cyan);
            else if (BodyType == b2_dynamicBody)
                DebugRect->setOutlineColor(color::Magenta);
            else
                DebugRect->setOutlineColor(color::White);

            return &DebugRect.value();
        }
    }
}