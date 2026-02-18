// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/CollisionTypes.h"

namespace we
{
    class TriggerComponent;
    class BlockingCollisionComponent;

    // =========================================================================
    // Collision Subsystem - Manages trigger and blocking collision queries
    // 
    // Components register themselves on BeginPlay and unregister on EndPlay
    // This allows efficient querying for overlaps and sweep tests
    // =========================================================================
    class CollisionSubsystem
    {
    public:
        void RegisterTrigger(TriggerComponent* Component);
        void UnregisterTrigger(TriggerComponent* Component);
        
        void RegisterBlocking(BlockingCollisionComponent* Component);
        void UnregisterBlocking(BlockingCollisionComponent* Component);

        // Query all overlapping triggers for a given trigger
        vector<TriggerComponent*> QueryTriggerOverlaps(TriggerComponent* QueryComponent);
        
        // Sweep against all blocking components
        SweepResult SweepAgainstBlocking(
            const vec2f& StartPos, 
            const vec2f& Delta,
            float Radius,  // For circle sweeps (most common)
            BlockingCollisionComponent* IgnoreComponent = nullptr);

        // Get all blocking components
        const vector<BlockingCollisionComponent*>& GetAllBlocking() const { return BlockingComponents; }
        const vector<TriggerComponent*>& GetAllTriggers() const { return TriggerComponents; }

    private:
        vector<TriggerComponent*> TriggerComponents;
        vector<BlockingCollisionComponent*> BlockingComponents;
    };

    // Global access to collision subsystem (similar to how ResourceSubsystem works)
    // This is a simple approach - in a full engine this would be accessed through World
    class CollisionWorld
    {
    public:
        static CollisionWorld& Get();
        
        void RegisterTrigger(TriggerComponent* Component);
        void UnregisterTrigger(TriggerComponent* Component);
        void RegisterBlocking(BlockingCollisionComponent* Component);
        void UnregisterBlocking(BlockingCollisionComponent* Component);
        
        vector<TriggerComponent*> QueryTriggerOverlaps(TriggerComponent* QueryComponent);
        SweepResult SweepAgainstBlocking(
            const vec2f& StartPos, 
            const vec2f& Delta,
            float Radius,
            BlockingCollisionComponent* IgnoreComponent = nullptr);

        // Direct access for iteration
        const vector<TriggerComponent*>& GetAllTriggers() const { return Subsystem.GetAllTriggers(); }
        const vector<BlockingCollisionComponent*>& GetAllBlocking() const { return Subsystem.GetAllBlocking(); }

    private:
        CollisionWorld() = default;
        CollisionSubsystem Subsystem;
    };
} // namespace we
