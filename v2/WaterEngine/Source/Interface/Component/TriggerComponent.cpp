// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interface/Component/TriggerComponent.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Subsystem/CollisionSubsystem.h"
#include "Utility/Assert.h"
#include "Utility/Log.h"
#include "Utility/Math.h"
#include "Utility/DebugDraw.h"

namespace we
{
    TriggerComponent::TriggerComponent(Actor* InOwner)
        : Owner(InOwner)
    {
    }

    TriggerComponent::~TriggerComponent()
    {
    }

    void TriggerComponent::BeginPlay()
    {
        // Register with global collision world
        CollisionWorld::Get().RegisterTrigger(this);
    }

    void TriggerComponent::Tick(float DeltaTime)
    {
        (void)DeltaTime;
        UpdateOverlaps();

        // Debug draw trigger shape
        if (DebugDraw::IsEnabled() && Owner)
        {
            vec2f Pos = GetPosition();
            color TriggerColor = color{ 0, 255, 255 };  // Cyan for triggers

            if (bIsCircle)
            {
                DebugDraw::Circle(Pos, ShapeRadius, TriggerColor, 2.0f);
            }
            else if (bIsCapsule)
            {
                DebugDraw::Capsule(Pos, Capsule.HalfHeight, Capsule.Radius, TriggerColor, 2.0f);
            }
            else
            {
                DebugDraw::Rect(Pos, ShapeHalfExtents, 0.0f, TriggerColor, 2.0f);
            }
        }
    }

    void TriggerComponent::EndPlay()
    {
        // Fire end overlap for all current overlaps
        for (auto* Other : CurrentOverlaps)
        {
            if (Other)
            {
                OnEndOverlap(Other);
            }
        }
        CurrentOverlaps.clear();
        
        // Unregister from collision world
        CollisionWorld::Get().UnregisterTrigger(this);
    }

    void TriggerComponent::SetCircleShape(float Radius)
    {
        ShapeRadius = Radius;
        bIsCircle = true;
        bIsCapsule = false;
    }

    void TriggerComponent::SetBoxShape(const vec2f& HalfExtents)
    {
        ShapeHalfExtents = HalfExtents;
        bIsCircle = false;
        bIsCapsule = false;
    }

    void TriggerComponent::SetCapsuleShape(const CapsuleShape& InCapsule)
    {
        Capsule = InCapsule;
        bIsCircle = false;
        bIsCapsule = true;
    }

    void TriggerComponent::SetCollisionProfile(ECollisionProfile InProfile)
    {
        Profile = InProfile;
    }

    void TriggerComponent::OnBeginOverlap(TriggerComponent* Other)
    {
        if (Owner && Other && Other->GetOwner())
        {
            LOG("TRIGGER BEGIN: Actor@{:p} -> Actor@{:p}", 
                reinterpret_cast<void*>(Owner), 
                reinterpret_cast<void*>(Other->GetOwner()));
        }
    }

    void TriggerComponent::OnEndOverlap(TriggerComponent* Other)
    {
        if (Owner && Other && Other->GetOwner())
        {
            LOG("TRIGGER END: Actor@{:p} -> Actor@{:p}", 
                reinterpret_cast<void*>(Owner), 
                reinterpret_cast<void*>(Other->GetOwner()));
        }
    }

    vector<TriggerComponent*> TriggerComponent::GetOverlappingTriggers() const
    {
        return vector<TriggerComponent*>(CurrentOverlaps.begin(), CurrentOverlaps.end());
    }

    vec2f TriggerComponent::GetPosition() const
    {
        return Owner ? Owner->GetPosition() + LocalOffset : LocalOffset;
    }

    void TriggerComponent::UpdateOverlaps()
    {
        // Store previous overlaps for comparison
        PreviousOverlaps = CurrentOverlaps;
        CurrentOverlaps.clear();

        if (!Owner) return;

        // Query collision world for all registered triggers
        auto& CollisionWorld = we::CollisionWorld::Get();
        
        for (auto* Other : CollisionWorld.GetAllTriggers())
        {
            if (!Other || Other == this) continue;
            
            if (TestOverlap(Other))
            {
                CurrentOverlaps.insert(Other);
                
                // Check if this is a new overlap
                if (PreviousOverlaps.find(Other) == PreviousOverlaps.end())
                {
                    OnBeginOverlap(Other);
                }
            }
        }
        
        // Check for ended overlaps
        for (auto* Other : PreviousOverlaps)
        {
            if (CurrentOverlaps.find(Other) == CurrentOverlaps.end())
            {
                OnEndOverlap(Other);
            }
        }
    }

    bool TriggerComponent::TestOverlap(TriggerComponent* Other) const
    {
        if (!Other || Other == this) return false;

        vec2f PosA = GetPosition();
        vec2f PosB = Other->GetPosition();

        // Circle vs Circle (fast path for most triggers)
        if (bIsCircle && Other->bIsCircle)
        {
            return CircleOverlap(PosA, ShapeRadius, PosB, Other->ShapeRadius);
        }

        // Circle vs Box
        if (bIsCircle && !Other->bIsCircle && !Other->bIsCapsule)
        {
            // Simple AABB circle test
            vec2f Closest = {
                std::clamp(PosA.x, PosB.x - Other->ShapeHalfExtents.x, PosB.x + Other->ShapeHalfExtents.x),
                std::clamp(PosA.y, PosB.y - Other->ShapeHalfExtents.y, PosB.y + Other->ShapeHalfExtents.y)
            };
            return DistanceSquared(PosA, Closest) <= ShapeRadius * ShapeRadius;
        }

        // Box vs Circle
        if (!bIsCircle && !bIsCapsule && Other->bIsCircle)
        {
            return Other->TestOverlap(const_cast<TriggerComponent*>(this));
        }

        // Box vs Box
        if (!bIsCircle && !bIsCapsule && !Other->bIsCircle && !Other->bIsCapsule)
        {
            return BoxOverlap(PosA, ShapeHalfExtents, PosB, Other->ShapeHalfExtents);
        }

        // Capsule vs anything
        if (bIsCapsule || Other->bIsCapsule)
        {
            return CapsuleOverlap(Other);
        }

        return false;
    }

    bool TriggerComponent::CircleOverlap(const vec2f& PosA, float RadiusA, const vec2f& PosB, float RadiusB) const
    {
        float CombinedRadius = RadiusA + RadiusB;
        return DistanceSquared(PosA, PosB) <= CombinedRadius * CombinedRadius;
    }

    bool TriggerComponent::BoxOverlap(const vec2f& PosA, const vec2f& HalfA, const vec2f& PosB, const vec2f& HalfB) const
    {
        return (std::abs(PosA.x - PosB.x) <= HalfA.x + HalfB.x) &&
               (std::abs(PosA.y - PosB.y) <= HalfA.y + HalfB.y);
    }

    bool TriggerComponent::CapsuleOverlap(const TriggerComponent* Other) const
    {
        // Simplified capsule-capsule or capsule-other tests
        // For now, approximate with circles
        
        const TriggerComponent* CapA = this;
        const TriggerComponent* CapB = Other;

        // If this is not a capsule, swap
        if (!bIsCapsule)
        {
            CapA = Other;
            CapB = this;
        }

        if (!CapA->bIsCapsule) return false; // Neither is capsule

        vec2f PosA = CapA->GetPosition();
        vec2f PosB = CapB->GetPosition();

        if (CapB->bIsCapsule)
        {
            // Capsule vs Capsule - approximate with segment distance
            vec2f TopA = PosA + CapA->Capsule.GetTopCapCenter();
            vec2f BotA = PosA + CapA->Capsule.GetBottomCapCenter();
            vec2f TopB = PosB + CapB->Capsule.GetTopCapCenter();
            vec2f BotB = PosB + CapB->Capsule.GetBottomCapCenter();

            float CombinedRadius = CapA->Capsule.Radius + CapB->Capsule.Radius;
            
            // Segment distance (simplified)
            float DistSq = DistanceSquared(TopA, TopB);
            DistSq = std::min(DistSq, DistanceSquared(TopA, BotB));
            DistSq = std::min(DistSq, DistanceSquared(BotA, TopB));
            DistSq = std::min(DistSq, DistanceSquared(BotA, BotB));
            
            return DistSq <= CombinedRadius * CombinedRadius;
        }
        else if (CapB->bIsCircle)
        {
            // Capsule vs Circle
            vec2f TopA = PosA + CapA->Capsule.GetTopCapCenter();
            vec2f BotA = PosA + CapA->Capsule.GetBottomCapCenter();
            
            float CombinedRadius = CapA->Capsule.Radius + CapB->ShapeRadius;
            
            // Distance from circle center to capsule segment
            // Simplified: check distance to both ends
            float DistSq = std::min(DistanceSquared(PosB, TopA), DistanceSquared(PosB, BotA));
            return DistSq <= CombinedRadius * CombinedRadius;
        }
        else
        {
            // Capsule vs Box - approximate
            vec2f TopA = PosA + CapA->Capsule.GetTopCapCenter();
            vec2f BotA = PosA + CapA->Capsule.GetBottomCapCenter();
            
            // Check if either cap is inside box
            auto PointInBox = [&](const vec2f& Point) {
                return std::abs(Point.x - PosB.x) <= CapB->ShapeHalfExtents.x &&
                       std::abs(Point.y - PosB.y) <= CapB->ShapeHalfExtents.y;
            };
            
            return PointInBox(TopA) || PointInBox(BotA) || PointInBox(PosA);
        }
    }

    PhysicsSubsystem& TriggerComponent::GetPhysics() const
    {
        VERIFY(Owner && "TriggerComponent has no owner");
        VERIFY(Owner->GetWorld() && "Actor has no world");
        return *Owner->GetWorld()->GetSubsystem().Physics;
    }
} // namespace we
