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
    class BlockingCollisionComponent;
    class PhysicsSubsystem;

    // =========================================================================
    // Blocking Collision Component - Prevents movement through solid objects
    // Uses swept queries to prevent tunneling
    // 
    // Unlike PhysicsComponent:
    // - No b2Body (actor position drives collision, not physics)
    // - Swept collision prevents tunneling at high speeds
    // - No bounce/forces - just blocking
    // =========================================================================
    class BlockingCollisionComponent : public IActorComponent
    {
    public:
        explicit BlockingCollisionComponent(Actor* InOwner);
        virtual ~BlockingCollisionComponent();

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

        // =========================================================================
        // Swept Collision Queries (Prevent Tunneling)
        // =========================================================================
        
        // Sweeps shape along Delta and returns hit result
        // This is the main function MovementComponent will use
        SweepResult Sweep(vec2f Delta) const;
        
        // Sweeps from a specific start position
        SweepResult SweepFrom(vec2f StartPosition, vec2f Delta) const;
        
        // Sweeps to a specific position
        SweepResult SweepTo(vec2f TargetPosition) const;

        // Simple overlap test (for current position only)
        bool IsOverlapping() const;

        // Offset from actor position
        void SetOffset(const vec2f& Offset) { LocalOffset = Offset; }
        vec2f GetOffset() const { return LocalOffset; }

        // Accessors
        vec2f GetPosition() const;
        bool IsCircle() const { return bIsCircle; }
        bool IsCapsule() const { return bIsCapsule; }
        float GetRadius() const { return ShapeRadius; }
        const vec2f& GetHalfExtents() const { return ShapeHalfExtents; }
        const CapsuleShape& GetCapsule() const { return Capsule; }

    private:
        // Internal sweep implementations
        SweepResult SweepCircle(vec2f Delta) const;
        SweepResult SweepCircleFrom(vec2f StartPos, vec2f Delta) const;
        SweepResult SweepBox(vec2f Delta) const;
        SweepResult SweepCapsule(vec2f Delta) const;
        SweepResult SweepCapsuleFrom(vec2f StartPos, vec2f Delta) const;

        // Test against another blocking component
        bool TestOverlap(const BlockingCollisionComponent* Other) const;
        SweepResult SweepAgainst(const BlockingCollisionComponent* Other, vec2f Delta) const;

        // Circle vs Circle sweep
        SweepResult SweepCircleVsCircle(vec2f Start, float RadiusA, vec2f End, 
            vec2f OtherPos, float RadiusB) const;

        // Utilities
        static float RayIntersectCircle(vec2f RayStart, vec2f RayDir, 
            vec2f CircleCenter, float CircleRadius);

    private:
        Actor* Owner;
        ECollisionProfile Profile = ECollisionProfile::Pawn;

        // Shape data
        bool bIsCircle = true;
        bool bIsCapsule = false;
        float ShapeRadius = 50.0f;
        vec2f ShapeHalfExtents = { 50.0f, 50.0f };
        CapsuleShape Capsule;
        vec2f LocalOffset = { 0.0f, 0.0f };  // Offset from actor position
    };
}
