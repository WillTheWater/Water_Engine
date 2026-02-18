// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interface/Component/BlockingCollisionComponent.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Subsystem/CollisionSubsystem.h"
#include "Utility/Math.h"
#include "Utility/DebugDraw.h"

namespace we
{
    BlockingCollisionComponent::BlockingCollisionComponent(Actor* InOwner)
        : Owner(InOwner)
    {
    }

    BlockingCollisionComponent::~BlockingCollisionComponent()
    {
    }

    void BlockingCollisionComponent::BeginPlay()
    {
        // Register with global collision world
        CollisionWorld::Get().RegisterBlocking(this);
    }

    void BlockingCollisionComponent::Tick(float DeltaTime)
    {
        (void)DeltaTime;
        // Blocking collision is reactive - we check on movement attempt

        // Debug draw blocking shape
        if (DebugDraw::IsEnabled() && Owner)
        {
            vec2f Pos = GetPosition();
            color BlockColor = color{ 255, 165, 0 };  // Orange for blocking collision

            if (bIsCircle)
            {
                DebugDraw::Circle(Pos, ShapeRadius, BlockColor, 2.0f);
            }
            else if (bIsCapsule)
            {
                DebugDraw::Capsule(Pos, Capsule.HalfHeight, Capsule.Radius, BlockColor, 2.0f);
            }
            else
            {
                DebugDraw::Rect(Pos, ShapeHalfExtents, 0.0f, BlockColor, 2.0f);
            }
        }
    }

    void BlockingCollisionComponent::EndPlay()
    {
        // Unregister from collision world
        CollisionWorld::Get().UnregisterBlocking(this);
    }

    void BlockingCollisionComponent::SetCircleShape(float Radius)
    {
        ShapeRadius = Radius;
        bIsCircle = true;
        bIsCapsule = false;
    }

    void BlockingCollisionComponent::SetBoxShape(const vec2f& HalfExtents)
    {
        ShapeHalfExtents = HalfExtents;
        bIsCircle = false;
        bIsCapsule = false;
    }

    void BlockingCollisionComponent::SetCapsuleShape(const CapsuleShape& InCapsule)
    {
        Capsule = InCapsule;
        bIsCircle = false;
        bIsCapsule = true;
    }

    void BlockingCollisionComponent::SetCollisionProfile(ECollisionProfile InProfile)
    {
        Profile = InProfile;
    }

    vec2f BlockingCollisionComponent::GetPosition() const
    {
        return Owner ? Owner->GetPosition() + LocalOffset : LocalOffset;
    }

    // =========================================================================
    // Swept Collision - The Anti-Tunneling Solution
    // =========================================================================

    SweepResult BlockingCollisionComponent::Sweep(vec2f Delta) const
    {
        if (bIsCircle)
        {
            return SweepCircle(Delta);
        }
        else if (bIsCapsule)
        {
            return SweepCapsule(Delta);
        }
        else
        {
            return SweepBox(Delta);
        }
    }

    SweepResult BlockingCollisionComponent::SweepFrom(vec2f StartPosition, vec2f Delta) const
    {
        if (bIsCircle)
        {
            return SweepCircleFrom(StartPosition, Delta);
        }
        else if (bIsCapsule)
        {
            return SweepCapsuleFrom(StartPosition, Delta);
        }
        else
        {
            return SweepBox(Delta);
        }
    }

    SweepResult BlockingCollisionComponent::SweepTo(vec2f TargetPosition) const
    {
        vec2f CurrentPos = GetPosition();
        return Sweep(TargetPosition - CurrentPos);
    }

    SweepResult BlockingCollisionComponent::SweepCircle(vec2f Delta) const
    {
        return SweepCircleFrom(GetPosition(), Delta);
    }

    SweepResult BlockingCollisionComponent::SweepCircleFrom(vec2f StartPos, vec2f Delta) const
    {
        SweepResult Result;
        Result.Time = 1.0f;
        Result.Distance = Length(Delta);

        if (Result.Distance < 0.001f)
        {
            return Result; // No movement
        }
        
        // Use collision world to sweep against all blocking components
        return CollisionWorld::Get().SweepAgainstBlocking(
            StartPos, Delta, ShapeRadius, 
            const_cast<BlockingCollisionComponent*>(this));
    }

    SweepResult BlockingCollisionComponent::SweepBox(vec2f Delta) const
    {
        // Stub - box sweeping is more complex
        SweepResult Result;
        Result.Time = 1.0f;
        Result.Distance = Length(Delta);
        return Result;
    }

    SweepResult BlockingCollisionComponent::SweepCapsule(vec2f Delta) const
    {
        return SweepCapsuleFrom(GetPosition(), Delta);
    }

    SweepResult BlockingCollisionComponent::SweepCapsuleFrom(vec2f StartPos, vec2f Delta) const
    {
        // For sweeping, approximate capsule as a circle with the capsule's radius
        // This is simpler and usually sufficient for top-down games
        
        SweepResult Result;
        Result.Time = 1.0f;
        Result.Distance = Length(Delta);

        if (Result.Distance < 0.001f)
        {
            return Result;
        }
        
        // Use the capsule radius for the sweep (approximation)
        return CollisionWorld::Get().SweepAgainstBlocking(
            StartPos, Delta, Capsule.Radius, 
            const_cast<BlockingCollisionComponent*>(this));
    }

    SweepResult BlockingCollisionComponent::SweepAgainst(const BlockingCollisionComponent* Other, vec2f Delta) const
    {
        SweepResult Result;
        Result.Time = 1.0f;
        Result.Distance = Length(Delta);

        if (!Other || Other == this) return Result;

        vec2f OtherPos = Other->GetPosition();

        // Circle vs Circle sweep
        if (bIsCircle && Other->bIsCircle)
        {
            return SweepCircleVsCircle(GetPosition(), ShapeRadius, 
                GetPosition() + Delta, OtherPos, Other->ShapeRadius);
        }

        // TODO: Other shape combinations
        return Result;
    }

    SweepResult BlockingCollisionComponent::SweepCircleVsCircle(
        vec2f Start, float RadiusA, vec2f End, vec2f OtherPos, float RadiusB) const
    {
        SweepResult Result;
        Result.Time = 1.0f;
        
        vec2f Delta = End - Start;
        float Dist = Length(Delta);
        
        if (Dist < 0.001f) return Result;

        vec2f Dir = Delta / Dist;
        float CombinedRadius = RadiusA + RadiusB;

        // Ray-circle intersection
        float HitTime = RayIntersectCircle(Start, Dir, OtherPos, CombinedRadius);
        
        if (HitTime >= 0.0f && HitTime <= Dist)
        {
            Result.bBlockingHit = true;
            Result.Time = HitTime / Dist;
            Result.Distance = HitTime;
            Result.ImpactPoint = Start + Dir * HitTime;
            Result.ImpactNormal = Normalize(Result.ImpactPoint - OtherPos);
        }

        return Result;
    }

    float BlockingCollisionComponent::RayIntersectCircle(
        vec2f RayStart, vec2f RayDir, vec2f CircleCenter, float CircleRadius)
    {
        vec2f ToCircle = CircleCenter - RayStart;
        float ProjLength = Dot(ToCircle, RayDir);
        
        // Closest point on ray to circle center
        vec2f ClosestPoint = RayStart + RayDir * ProjLength;
        float DistSq = DistanceSquared(ClosestPoint, CircleCenter);
        float RadiusSq = CircleRadius * CircleRadius;
        
        if (DistSq > RadiusSq) return -1.0f; // Miss
        
        float Offset = std::sqrt(RadiusSq - DistSq);
        float HitDist = ProjLength - Offset;
        
        if (HitDist < 0.0f) 
        {
            // Ray starts inside circle, try other intersection
            HitDist = ProjLength + Offset;
        }
        
        return HitDist;
    }

    bool BlockingCollisionComponent::IsOverlapping() const
    {
        // Check current position for overlaps
        if (!Owner || !Owner->GetWorld()) return false;
        
        // TODO: Query world for overlaps
        return false;
    }

    bool BlockingCollisionComponent::TestOverlap(const BlockingCollisionComponent* Other) const
    {
        if (!Other || Other == this) return false;

        vec2f PosA = GetPosition();
        vec2f PosB = Other->GetPosition();

        // Circle vs Circle
        if (bIsCircle && Other->bIsCircle)
        {
            float CombinedRadius = ShapeRadius + Other->ShapeRadius;
            return DistanceSquared(PosA, PosB) <= CombinedRadius * CombinedRadius;
        }

        // TODO: Other shape combinations
        return false;
    }
} // namespace we
