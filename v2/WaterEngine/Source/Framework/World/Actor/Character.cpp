// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Character.h"
#include "Interface/Component/TriggerComponent.h"
#include "Interface/Component/BlockingCollisionComponent.h"
#include "Framework/World/World.h"
#include "Utility/Log.h"
#include "Utility/Math.h"

namespace we
{
    Character::Character(World* OwningWorld, const string& TexturePath)
        : Actor(OwningWorld, TexturePath)
        , CapsuleConfig{ 40.0f, 35.0f }  // Default: 80px tall, 35px radius
    {
    }

    Character::~Character()
    {
    }

    void Character::BeginPlay()
    {
        if (!bCollisionInitialized)
        {
            InitializeCollision();
        }

        // Initialize components if they exist
        if (TriggerComp) TriggerComp->BeginPlay();
        if (BlockingComp) BlockingComp->BeginPlay();

        Actor::BeginPlay();
    }

    void Character::Tick(float DeltaTime)
    {
        // Tick components
        if (TriggerComp) TriggerComp->Tick(DeltaTime);
        if (BlockingComp) BlockingComp->Tick(DeltaTime);

        Actor::Tick(DeltaTime);
    }

    void Character::Destroy()
    {
        if (TriggerComp) TriggerComp->EndPlay();
        if (BlockingComp) BlockingComp->EndPlay();

        Actor::Destroy();
    }

    void Character::InitializeCollision()
    {
        bCollisionInitialized = true;

        // Create trigger component for interactions (CIRCLE - larger interaction radius)
        TriggerComp = make_shared<TriggerComponent>(this);
        TriggerComp->SetCircleShape(CapsuleConfig.Radius * 2.0f);  // 2x radius for interaction
        TriggerComp->SetCollisionProfile(ECollisionProfile::Trigger);
        TriggerComp->SetOffset(CapsuleOffset);

        // Create blocking component for collision (CAPSULE - physical body)
        BlockingComp = make_shared<BlockingCollisionComponent>(this);
        BlockingComp->SetCapsuleShape(CapsuleConfig);
        BlockingComp->SetCollisionProfile(ECollisionProfile::Pawn);
        BlockingComp->SetOffset(CapsuleOffset);
    }

    void Character::SetCapsuleRadius(float Radius)
    {
        CapsuleConfig.Radius = Radius;
        // Trigger = larger circle for interaction
        if (TriggerComp) TriggerComp->SetCircleShape(Radius * 2.0f);
        // Blocking = capsule for physical collision
        if (BlockingComp) BlockingComp->SetCapsuleShape(CapsuleConfig);
    }

    void Character::SetCapsuleHalfHeight(float HalfHeight)
    {
        CapsuleConfig.HalfHeight = HalfHeight;
        // Only blocking component uses capsule shape
        if (BlockingComp) BlockingComp->SetCapsuleShape(CapsuleConfig);
    }

    void Character::SetCapsuleOffset(const vec2f& Offset)
    {
        CapsuleOffset = Offset;
        if (BlockingComp) BlockingComp->SetOffset(Offset);
        if (TriggerComp) TriggerComp->SetOffset(Offset);
    }

    bool Character::TryMove(vec2f Delta)
    {
        if (!BlockingComp || LengthSquared(Delta) < 0.001f)
        {
            // No blocking component or no movement - just move
            SetPosition(GetPosition() + Delta);
            return true;
        }

        // Sweep from component's position (which includes offset)
        vec2f SweepStart = BlockingComp->GetPosition();
        SweepResult Hit = BlockingComp->SweepFrom(SweepStart, Delta);

        // Also check against world boundaries
        if (GetWorld())
        {
            SweepResult BoundaryHit = GetWorld()->GetBoundaries().SweepCapsule(
                SweepStart, Delta, CapsuleConfig);
            
            if (BoundaryHit.bBlockingHit && BoundaryHit.Time < Hit.Time)
            {
                Hit = BoundaryHit;
            }
        }

        if (Hit.bBlockingHit)
        {
            // SLIDING: Remove normal component, keep tangent component
            vec2f Normal = Hit.ImpactNormal;
            
            // Decompose remaining movement into normal and tangent
            float NormalDot = Dot(Delta * (1.0f - Hit.Time), Normal);
            vec2f Remaining = Delta * (1.0f - Hit.Time);
            
            // Slide: keep only the tangent (perpendicular) component
            vec2f SlideDelta = Remaining - Normal * NormalDot;
            
            // Move up to the hit point + small push away from wall
            const float SkinDepth = 0.5f;
            vec2f AllowedDelta = Delta * Hit.Time;
            
            if (Hit.Time > 0.001f)
            {
                // Move back slightly from the hit to avoid penetration
                AllowedDelta = AllowedDelta - Normalize(Delta) * SkinDepth;
            }
            else
            {
                AllowedDelta = { 0.0f, 0.0f };
            }

            SetPosition(GetPosition() + AllowedDelta);
            
            // Optionally: do a second sweep with the slide delta for corner cases
            // For now, just allow the slide
            if (LengthSquared(SlideDelta) > 1.0f)
            {
                // Small slide - apply it
                SetPosition(GetPosition() + SlideDelta * 0.5f);
            }
            
            OnBlocked(Hit);
            return false; // Movement was blocked (but we slid)
        }
        else
        {
            // No collision - full movement allowed
            SetPosition(GetPosition() + Delta);
            return true;
        }
    }

    void Character::SetPositionWithCollision(const vec2f& NewPosition)
    {
        if (!BlockingComp)
        {
            SetPosition(NewPosition);
            return;
        }

        vec2f Delta = NewPosition - GetPosition();
        TryMove(Delta);
    }

    void Character::OnBlocked(const SweepResult& Hit)
    {
        // Log the collision
        LOG("COLLISION: Character blocked at {:.1f}, {:.1f}", 
            Hit.ImpactPoint.x, Hit.ImpactPoint.y);
    }
} // namespace we
