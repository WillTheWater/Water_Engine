// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor/Actor.h"

namespace we
{
    class PhysicsComponent;

    // =========================================================================
    // Character - Base class for all player and AI characters
    // 
    // Uses Box2D PhysicsComponent for collision:
    // - Main fixture: Physical body that blocks movement
    // - Sensor fixture: Larger area for interaction/trigger detection
    //
    // Movement is kinematic (direct velocity control) or scripted
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
        void SetCharacterRadius(float Radius);
        void SetCollisionOffset(const vec2f& Offset);
        float GetCharacterRadius() const { return CharacterRadius; }
        vec2f GetCollisionOffset() const { return CollisionOffset; }

        // Access the physics component
        PhysicsComponent* GetPhysicsComponent() const { return PhysicsComp.get(); }

        // Movement with collision blocking (uses Box2D)
        void SetVelocity(const vec2f& Velocity);
        vec2f GetVelocity() const;
        void ApplyImpulse(const vec2f& Impulse);

        // Direct movement that respects collision
        bool TryMove(vec2f Delta);

    protected:
        // Called when character collides with something
        virtual void OnCollision(Actor* Other);
        
        // Called when sensor overlaps with something  
        virtual void OnSensorBegin(Actor* Other);
        virtual void OnSensorEnd(Actor* Other);

        // Override to customize collision setup
        virtual void InitializePhysics();

    protected:
        shared<PhysicsComponent> PhysicsComp;

    private:
        float CharacterRadius = 35.0f;
        vec2f CollisionOffset = { 0.0f, 0.0f };
        bool bPhysicsInitialized = false;
    };
} // namespace we
