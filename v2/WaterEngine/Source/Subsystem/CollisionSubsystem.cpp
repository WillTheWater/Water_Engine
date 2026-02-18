// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CollisionSubsystem.h"
#include "Interface/Component/TriggerComponent.h"
#include "Interface/Component/BlockingCollisionComponent.h"
#include "Utility/Math.h"

namespace we
{
    // ========================================================================
    // CollisionWorld singleton
    // ========================================================================
    CollisionWorld& CollisionWorld::Get()
    {
        static CollisionWorld Instance;
        return Instance;
    }

    void CollisionWorld::RegisterTrigger(TriggerComponent* Component)
    {
        Subsystem.RegisterTrigger(Component);
    }

    void CollisionWorld::UnregisterTrigger(TriggerComponent* Component)
    {
        Subsystem.UnregisterTrigger(Component);
    }

    void CollisionWorld::RegisterBlocking(BlockingCollisionComponent* Component)
    {
        Subsystem.RegisterBlocking(Component);
    }

    void CollisionWorld::UnregisterBlocking(BlockingCollisionComponent* Component)
    {
        Subsystem.UnregisterBlocking(Component);
    }

    vector<TriggerComponent*> CollisionWorld::QueryTriggerOverlaps(TriggerComponent* QueryComponent)
    {
        return Subsystem.QueryTriggerOverlaps(QueryComponent);
    }

    SweepResult CollisionWorld::SweepAgainstBlocking(
        const vec2f& StartPos, 
        const vec2f& Delta,
        float Radius,
        BlockingCollisionComponent* IgnoreComponent)
    {
        return Subsystem.SweepAgainstBlocking(StartPos, Delta, Radius, IgnoreComponent);
    }

    // ========================================================================
    // CollisionSubsystem implementation
    // ========================================================================
    void CollisionSubsystem::RegisterTrigger(TriggerComponent* Component)
    {
        if (Component && std::find(TriggerComponents.begin(), TriggerComponents.end(), Component) == TriggerComponents.end())
        {
            TriggerComponents.push_back(Component);
        }
    }

    void CollisionSubsystem::UnregisterTrigger(TriggerComponent* Component)
    {
        auto It = std::find(TriggerComponents.begin(), TriggerComponents.end(), Component);
        if (It != TriggerComponents.end())
        {
            TriggerComponents.erase(It);
        }
    }

    void CollisionSubsystem::RegisterBlocking(BlockingCollisionComponent* Component)
    {
        if (Component && std::find(BlockingComponents.begin(), BlockingComponents.end(), Component) == BlockingComponents.end())
        {
            BlockingComponents.push_back(Component);
        }
    }

    void CollisionSubsystem::UnregisterBlocking(BlockingCollisionComponent* Component)
    {
        auto It = std::find(BlockingComponents.begin(), BlockingComponents.end(), Component);
        if (It != BlockingComponents.end())
        {
            BlockingComponents.erase(It);
        }
    }

    vector<TriggerComponent*> CollisionSubsystem::QueryTriggerOverlaps(TriggerComponent* QueryComponent)
    {
        vector<TriggerComponent*> Results;
        
        if (!QueryComponent) return Results;

        for (auto* Other : TriggerComponents)
        {
            if (Other && Other != QueryComponent)
            {
                // Use the component's own overlap test
                if (QueryComponent->TestOverlap(Other))
                {
                    Results.push_back(Other);
                }
            }
        }
        
        return Results;
    }

    SweepResult CollisionSubsystem::SweepAgainstBlocking(
        const vec2f& StartPos, 
        const vec2f& Delta,
        float Radius,
        BlockingCollisionComponent* IgnoreComponent)
    {
        SweepResult Result;
        Result.Time = 1.0f;
        Result.Distance = Length(Delta);

        if (Result.Distance < 0.001f) return Result;

        vec2f Dir = Delta / Result.Distance;

        // Check against all blocking components
        for (auto* Blocker : BlockingComponents)
        {
            if (!Blocker || Blocker == IgnoreComponent) continue;

            // Handle circle or capsule blockers (both approximated as circles for sweep)
            float BlockerRadius = 0.0f;
            if (Blocker->IsCircle())
            {
                BlockerRadius = Blocker->GetRadius();
            }
            else if (Blocker->IsCapsule())
            {
                // Approximate capsule as its radius for sweeping
                BlockerRadius = Blocker->GetCapsule().Radius;
            }
            else
            {
                continue; // Box not supported yet
            }
            
            {
                vec2f BlockerPos = Blocker->GetPosition();
                float CombinedRadius = Radius + BlockerRadius;

                // Ray-circle intersection
                vec2f ToCircle = BlockerPos - StartPos;
                float ProjLength = Dot(ToCircle, Dir);
                
                // Closest point on ray to circle center
                vec2f ClosestPoint = StartPos + Dir * ProjLength;
                float DistSq = DistanceSquared(ClosestPoint, BlockerPos);
                float RadiusSq = CombinedRadius * CombinedRadius;
                
                if (DistSq > RadiusSq) continue; // Miss
                
                float Offset = std::sqrt(RadiusSq - DistSq);
                float HitDist = ProjLength - Offset;
                
                if (HitDist < 0.0f) 
                {
                    // Ray starts inside circle, try other intersection
                    HitDist = ProjLength + Offset;
                }
                
                if (HitDist >= 0.0f && HitDist < Result.Distance * Result.Time)
                {
                    Result.bBlockingHit = true;
                    Result.Time = HitDist / Result.Distance;
                    Result.Distance = HitDist;
                    Result.ImpactPoint = StartPos + Dir * HitDist;
                    Result.ImpactNormal = Normalize(Result.ImpactPoint - BlockerPos);
                }
            }
        }

        return Result;
    }
} // namespace we
