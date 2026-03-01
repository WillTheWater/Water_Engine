// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>

namespace we
{
    class Actor;

    class PhysicsContactListener : public b2ContactListener
    {
    public:
        void BeginContact(b2Contact* Contact) override;
        void EndContact(b2Contact* Contact) override;

    };

    class PhysicsSubsystem
    {
    public:
        explicit PhysicsSubsystem(const EngineConfig::PhysicsConfig& Config);

        void Step(float DeltaTime);
        b2Body* AddListener(Actor* Listener);
        void RemoveListener(b2Body* LisenterBody);
        void UpdateBodyCollision(Actor* Listener);
        float GetPhysicsScale() const { return PhysicsScale; }
        
        // Set world gravity (for top-down games use {0, 0})
        void SetGravity(const vec2f& Gravity);

    private:
        void RemovePendingListeners();

    private:
        b2World PhysicsWorld;
        float PhysicsScale;
        uint VelocityIterations;
        uint PositionIterations;

        PhysicsContactListener ContactListener;
        set<b2Body*> PendingRemovalListeners;
    };
}
