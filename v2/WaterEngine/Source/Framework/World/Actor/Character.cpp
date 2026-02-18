// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Character.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Log.h"
#include "Utility/Math.h"

namespace we
{
    Character::Character(World* OwningWorld, const string& TexturePath)
        : Actor(OwningWorld, TexturePath)
        , CharacterRadius{ 35.0f }
    {
    }

    Character::~Character()
    {
    }

    void Character::BeginPlay()
    {
        if (!bPhysicsInitialized)
        {
            InitializePhysics();
        }

        if (PhysicsComp) 
        {
            PhysicsComp->BeginPlay();
            // High damping = stops quickly when not moving (no sliding)
            PhysicsComp->SetLinearDamping(20.0f);
            // Fixed rotation = sprite stays upright
            PhysicsComp->SetFixedRotation(true);
        }

        Actor::BeginPlay();
    }

    void Character::Tick(float DeltaTime)
    {
        if (PhysicsComp) PhysicsComp->Tick(DeltaTime);

        Actor::Tick(DeltaTime);
    }

    void Character::Destroy()
    {
        if (PhysicsComp) PhysicsComp->EndPlay();

        Actor::Destroy();
    }

    void Character::InitializePhysics()
    {
        bPhysicsInitialized = true;

        PhysicsComp = make_shared<PhysicsComponent>(this);
        
        // Dynamic body - gets collision response from static bodies (walls)
        // High damping prevents sliding, fixed rotation keeps sprite upright
        PhysicsComp->SetBodyType(BodyType::Dynamic);
        PhysicsComp->SetDensity(1.0f);
        PhysicsComp->SetFriction(0.0f);
        PhysicsComp->SetRestitution(0.0f);
        
        // Main collision shape (physical body)
        PhysicsComp->SetCircleShape(CharacterRadius);

        // Sensor for interaction/trigger detection (2x radius)
        PhysicsComp->SetSensorShape(true, CharacterRadius * 2.0f);
        PhysicsComp->SetSensorOffset(CollisionOffset);
    }

    void Character::SetCharacterRadius(float Radius)
    {
        CharacterRadius = Radius;
        if (PhysicsComp)
        {
            PhysicsComp->SetCircleShape(Radius);
            PhysicsComp->SetSensorShape(true, Radius * 2.0f);
        }
    }

    void Character::SetCollisionOffset(const vec2f& Offset)
    {
        CollisionOffset = Offset;
        if (PhysicsComp)
        {
            PhysicsComp->SetSensorOffset(Offset);
        }
    }

    void Character::SetVelocity(const vec2f& Velocity)
    {
        if (PhysicsComp)
        {
            PhysicsComp->SetVelocity(Velocity);
        }
    }

    vec2f Character::GetVelocity() const
    {
        if (PhysicsComp)
        {
            return PhysicsComp->GetVelocity();
        }
        return vec2f{};
    }

    void Character::ApplyImpulse(const vec2f& Impulse)
    {
        if (PhysicsComp)
        {
            PhysicsComp->ApplyImpulse(Impulse);
        }
    }

    bool Character::TryMove(vec2f Delta)
    {
        if (!PhysicsComp || LengthSquared(Delta) < 0.001f)
        {
            SetPosition(GetPosition() + Delta);
            return true;
        }

        // Set velocity to achieve movement. Box2D handles collision blocking.
        float Dt = GetWorld()->GetSubsystem().Time->GetDeltaTime();
        if (Dt > 0.0f)
        {
            vec2f TargetVelocity = Delta / Dt;
            PhysicsComp->SetVelocity(TargetVelocity);
        }

        // Position will be synced from body in Tick via SyncBodyToActor
        return true;
    }
}
