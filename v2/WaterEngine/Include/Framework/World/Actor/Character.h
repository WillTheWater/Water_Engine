// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/World/CollisionTypes.h"

namespace we
{
    class TriggerComponent;
    class BlockingCollisionComponent;
    class MovementComponent;
    class AnimationComponent;

    // =========================================================================
    // Character - Base class for all player and AI characters
    // 
    // Comes with built-in components:
    // - TriggerComponent: For interaction/proximity detection
    // - BlockingCollisionComponent: Prevents walking through walls (no physics bounce)
    // - MovementComponent: Optional (can be added by derived class)
    // - AnimationComponent: Optional (can be added by derived class)
    //
    // Unlike regular Actors:
    // - Uses swept capsule collision (prevents tunneling)
    // - Position drives collision (not physics-driven)
    // - Designed for top-down/side-scroller character movement
    // =========================================================================
    class Character : public Actor
    {
    public:
        Character(World* OwningWorld, const string& TexturePath = "");
        virtual ~Character();

        // Lifecycle
        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;
        virtual void Destroy() override;

        // Collision setup
        void SetCapsuleRadius(float Radius);
        void SetCapsuleHalfHeight(float HalfHeight);
        void SetCapsuleOffset(const vec2f& Offset);  // Offset from actor position
        const CapsuleShape& GetCapsule() const { return CapsuleConfig; }
        vec2f GetCapsuleOffset() const { return CapsuleOffset; }

        // Component access
        TriggerComponent* GetTriggerComponent() const { return TriggerComp.get(); }
        BlockingCollisionComponent* GetBlockingComponent() const { return BlockingComp.get(); }

        // Movement helper
        // Attempts to move by Delta, respects blocking collision
        bool TryMove(vec2f Delta);
        
        // Set position with collision check
        void SetPositionWithCollision(const vec2f& NewPosition);

    protected:
        // Called when character is blocked by collision
        virtual void OnBlocked(const SweepResult& Hit);

        // Override to customize collision profile
        virtual void InitializeCollision();

    protected:
        shared<TriggerComponent> TriggerComp;
        shared<BlockingCollisionComponent> BlockingComp;

    private:
        CapsuleShape CapsuleConfig;
        vec2f CapsuleOffset = { 0.0f, 0.0f };  // Offset from actor position
        bool bCollisionInitialized = false;
    };
} // namespace we
