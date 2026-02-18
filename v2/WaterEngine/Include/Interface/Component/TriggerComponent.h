// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Component/IActorComponent.h"
#include "Framework/World/CollisionTypes.h"

namespace we
{
    class Actor;
    class PhysicsSubsystem;

    // =========================================================================
    // Trigger Component - Pure overlap detection (no physics simulation)
    // Uses spatial queries to detect overlaps each frame
    // =========================================================================
    class TriggerComponent : public IActorComponent
    {
    public:
        explicit TriggerComponent(Actor* InOwner);
        virtual ~TriggerComponent();

        // IActorComponent interface
        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;
        virtual void EndPlay() override;
        virtual Actor* GetOwner() const override { return Owner; }

        // Shape setup
        void SetCircleShape(float Radius);
        void SetBoxShape(const vec2f& HalfExtents);
        void SetCapsuleShape(const CapsuleShape& Capsule);

        // Collision profile
        void SetCollisionProfile(ECollisionProfile Profile);
        ECollisionProfile GetCollisionProfile() const { return Profile; }

        // Overlap callbacks - override in derived classes
        virtual void OnBeginOverlap(TriggerComponent* Other);
        virtual void OnEndOverlap(TriggerComponent* Other);

        // Query overlaps manually
        vector<TriggerComponent*> GetOverlappingTriggers() const;

        // Offset from actor position
        void SetOffset(const vec2f& Offset) { LocalOffset = Offset; }
        vec2f GetOffset() const { return LocalOffset; }

        // Accessors
        vec2f GetPosition() const;
        float GetRadius() const { return bIsCircle ? ShapeRadius : 0.0f; }
        bool IsCircle() const { return bIsCircle; }
        bool IsCapsule() const { return bIsCapsule; }
        const CapsuleShape& GetCapsule() const { return Capsule; }

        // Shape access for collision queries
        float GetCircleRadius() const { return ShapeRadius; }
        const vec2f& GetHalfExtents() const { return ShapeHalfExtents; }
        bool TestOverlap(TriggerComponent* Other) const;

    private:
        void UpdateOverlaps();
        bool CircleOverlap(const vec2f& PosA, float RadiusA, const vec2f& PosB, float RadiusB) const;
        bool BoxOverlap(const vec2f& PosA, const vec2f& HalfA, const vec2f& PosB, const vec2f& HalfB) const;
        bool CapsuleOverlap(const TriggerComponent* Other) const;

        PhysicsSubsystem& GetPhysics() const;

    private:
        Actor* Owner;
        ECollisionProfile Profile = ECollisionProfile::Trigger;

        // Shape data
        bool bIsCircle = true;
        bool bIsCapsule = false;
        float ShapeRadius = 50.0f;
        vec2f ShapeHalfExtents = { 50.0f, 50.0f };
        CapsuleShape Capsule;
        vec2f LocalOffset = { 0.0f, 0.0f };  // Offset from actor position

        // Overlap tracking
        set<TriggerComponent*> CurrentOverlaps;
        set<TriggerComponent*> PreviousOverlaps;
    };
} // namespace we
