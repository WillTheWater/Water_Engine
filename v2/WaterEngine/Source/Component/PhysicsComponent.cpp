// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/PhysicsComponent.h"
#include "Framework/World/Actor.h"
#include "Framework/World/World.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Core/EngineConfig.h"
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

    PhysicsComponent::~PhysicsComponent()
    {
        if (Body)
        {
            Body->GetUserData().pointer = 0;

            if (Owner)
            {
                auto& Physics = Owner->GetWorld().GetPhysics();
                Physics.MarkForDestruction(Body);
            }
        }
    }

    void PhysicsComponent::BeginPlay()
    {
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
        
        // Apply shape offset to initial position
        vec2f SpawnPos = Owner->GetPosition() + ShapeOffset;
        BodyDef.position = b2Vec2(
            Physics.PixelsToMeters(SpawnPos.x),
            Physics.PixelsToMeters(SpawnPos.y)
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
            // Note: Circle shapes can't be locally offset, so we use body position offset in CreateBody
            
            b2FixtureDef FixtureDef;
            FixtureDef.shape = &CircleShape;
            FixtureDef.density = (BodyType == b2_dynamicBody) ? 1.0f : 0.0f;
            FixtureDef.friction = 0.3f;
            FixtureDef.restitution = 0.0f;
            FixtureDef.isSensor = false;
            
            // Set collision filter - Physics channel collides with World and Physics
            uint16 ChannelBits = static_cast<uint16>(CollisionChannel);
            FixtureDef.filter.categoryBits = ChannelBits;
            FixtureDef.filter.maskBits = static_cast<uint16>(ECollisionChannel::World) | ChannelBits;

            Body->CreateFixture(&FixtureDef);
        }
        else  // Rectangle
        {
            b2PolygonShape BoxShape{};
            // For rectangles, we CAN offset the shape locally
            // Convert offset to meters (negated because body is already offset)
            b2Vec2 LocalOffset(
                Physics.PixelsToMeters(-ShapeOffset.x),
                Physics.PixelsToMeters(-ShapeOffset.y)
            );
            
            BoxShape.SetAsBox(
                Physics.PixelsToMeters(ShapeSize.x),
                Physics.PixelsToMeters(ShapeSize.y),
                LocalOffset,  // Local position offset
                0.0f          // No rotation
            );
            
            b2FixtureDef FixtureDef;
            FixtureDef.shape = &BoxShape;
            FixtureDef.density = (BodyType == b2_dynamicBody) ? 1.0f : 0.0f;
            FixtureDef.friction = 0.3f;
            FixtureDef.restitution = 0.0f;
            FixtureDef.isSensor = false;
            
            // Set collision filter - Physics channel collides with World and Physics
            uint16 ChannelBits = static_cast<uint16>(CollisionChannel);
            FixtureDef.filter.categoryBits = ChannelBits;
            FixtureDef.filter.maskBits = static_cast<uint16>(ECollisionChannel::World) | ChannelBits;

            Body->CreateFixture(&FixtureDef);
        }
    }

    void PhysicsComponent::SetCollisionChannel(ECollisionChannel Channel)
    {
        CollisionChannel = Channel;
        
        if (!Body)
            return;
        
        // Update existing fixture filter
        uint16 ChannelBits = static_cast<uint16>(Channel);
        uint16 MaskBits = static_cast<uint16>(ECollisionChannel::World) | ChannelBits;
        
        for (b2Fixture* Fixture = Body->GetFixtureList(); Fixture; Fixture = Fixture->GetNext())
        {
            b2Filter Filter = Fixture->GetFilterData();
            Filter.categoryBits = ChannelBits;
            Filter.maskBits = MaskBits;
            Fixture->SetFilterData(Filter);
        }
    }

    void PhysicsComponent::Tick(float DeltaTime)
    {
        if (!Body || !Owner) return;

        SyncActorToBody();
    }

    void PhysicsComponent::EndPlay()
    {
        DestroyBody();
    }

    void PhysicsComponent::DestroyBody()
    {
        if (!Body) return;
        Body->GetUserData().pointer = 0;

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

    void PhysicsComponent::SetShapeOffset(vec2f Offset)
    {
        ShapeOffset = Offset;
        
        // If body exists, we need to recreate it to apply offset
        // (simpler than trying to move existing body/fixture)
        if (Body)
        {
            DestroyBody();
            CreateBody();
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
        if (!Body || !Owner) return;

        auto& Physics = Owner->GetWorld().GetPhysics();
        // Apply offset when syncing actor position to body
        vec2f BodyPos = Owner->GetPosition() + ShapeOffset;
        
        Body->SetTransform(
            b2Vec2(Physics.PixelsToMeters(BodyPos.x), Physics.PixelsToMeters(BodyPos.y)),
            0.0f  // No rotation
        );
    }

    void PhysicsComponent::SyncActorToBody()
    {
        if (!Body || !Owner) return;

        auto& Physics = Owner->GetWorld().GetPhysics();
        b2Vec2 Pos = Body->GetPosition();
        
        // Subtract offset when syncing body position back to actor
        vec2f ActorPos(
            Physics.MetersToPixels(Pos.x) - ShapeOffset.x,
            Physics.MetersToPixels(Pos.y) - ShapeOffset.y
        );
        
        Owner->SetPosition(ActorPos);
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
        bDebugDrawEnabled = true;
        
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

            // Debug visual at actor position (body is offset, but debug shows relative to actor)
            DebugCircle->setPosition(Owner->GetPosition() + ShapeOffset);

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

            // Debug visual at actor position (body is offset, but debug shows relative to actor)
            DebugRect->setPosition(Owner->GetPosition() + ShapeOffset);

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