// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    // =========================================================================
    // Capsule Shape Definition
    // Box2D doesn't have native capsules, so we define our own
    // A capsule is: 2 half-circles (caps) connected by a rectangle
    // =========================================================================
    struct CapsuleShape
    {
        float HalfHeight = 50.0f;  // Distance from center to cap center
        float Radius = 25.0f;      // Radius of the end caps

        CapsuleShape() = default;
        CapsuleShape(float InHalfHeight, float InRadius)
            : HalfHeight(InHalfHeight), Radius(InRadius) {}

        // Total height of the capsule
        float GetTotalHeight() const { return (HalfHeight + Radius) * 2.0f; }
        
        // Total width of the capsule (diameter)
        float GetTotalWidth() const { return Radius * 2.0f; }

        // Get top cap center position (relative to capsule center)
        vec2f GetTopCapCenter() const { return { 0.0f, -HalfHeight }; }
        
        // Get bottom cap center position (relative to capsule center)
        vec2f GetBottomCapCenter() const { return { 0.0f, HalfHeight }; }
    };

    // =========================================================================
    // Collision Profiles
    // =========================================================================
    enum class ECollisionProfile
    {
        // Triggers - overlap only, no blocking
        NoCollision,
        Trigger,
        
        // Blocking - prevents movement
        WorldStatic,        // Walls, floors (immovable)
        Pawn,               // Characters that block each other
        
        // Physics
        PhysicsObject,      // Full physics simulation
        
        // Custom
        Custom
    };

    struct CollisionProfileData
    {
        bool bIsTrigger = false;
        bool bBlocksMovement = false;
        bool bSimulatePhysics = false;
    };

    inline CollisionProfileData GetCollisionProfileData(ECollisionProfile Profile)
    {
        switch (Profile)
        {
            case ECollisionProfile::NoCollision:
                return { false, false, false };
            case ECollisionProfile::Trigger:
                return { true, false, false };
            case ECollisionProfile::WorldStatic:
                return { false, true, false };
            case ECollisionProfile::Pawn:
                return { false, true, false };
            case ECollisionProfile::PhysicsObject:
                return { false, true, true };
            case ECollisionProfile::Custom:
            default:
                return { false, false, false };
        }
    }

    // =========================================================================
    // Sweep Result
    // =========================================================================
    struct SweepResult
    {
        bool bBlockingHit = false;
        float Time = 1.0f;          // 0-1, how far along the sweep before hit
        vec2f ImpactPoint;          // World position of collision
        vec2f ImpactNormal;         // Normal of the surface hit
        float Distance = 0.0f;      // Distance traveled before hit
    };
} // namespace we
