// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Component/IPhysicsComponent.h"
#include "Subsystem/PhysicsSubsystem.h"

class b2Body;
class b2Fixture;

namespace we
{
    class Actor;

    class PhysicsComponent : public IPhysicsComponent
    {
    public:
        explicit PhysicsComponent(Actor* InOwner);
        virtual ~PhysicsComponent();

        // IActorComponent
        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;
        virtual void EndPlay() override;
        virtual Actor* GetOwner() const override;

        // IPhysicsComponent
        virtual void OnBeginOverlap(PhysicsComponent* Other) override;
        virtual void OnEndOverlap(PhysicsComponent* Other) override;
        void SetOverlapping(bool bOverlapping) { bIsOverlapping = bOverlapping; }
        bool IsOverlapping() const { return bIsOverlapping; }
        virtual b2Body* GetPhysicsBody() const override { return Body; }

        // Shape setup
        void SetBodyType(BodyType Type);
        void SetBoxShape(const vec2f& HalfExtents);
        void SetCircleShape(float Radius);

        // Properties
        void SetDensity(float Density);
        void SetFriction(float Friction);
        void SetRestitution(float Restitution);
        void SetSensor(bool bIsSensor);

        // Physics queries
        vec2f GetVelocity() const;
        void SetVelocity(const vec2f& Velocity);
        void ApplyForce(const vec2f& Force);
        void ApplyImpulse(const vec2f& Impulse);

        // Debug
        void SetDebugDraw(bool bEnable) { bDebugDraw = bEnable; }
        void DebugDraw(RenderSubsystem* Render);

    private:
        void SyncActorToBody();   // Kinematic: actor -> body
        void SyncBodyToActor();   // Dynamic: body -> actor
        void RecreateFixture();

    private:
        Actor* Owner;
        b2Body* Body;
        b2Fixture* Fixture;

        BodyType Type;
        vec2f ShapeHalfExtents;
        float ShapeRadius;
        bool bIsCircle;
        float Density;
        float Friction;
        float Restitution;
        bool bSensor;
        bool bIsOverlapping;
        int OverlapCount;
        bool bNeedsFixtureUpdate;
        bool bDebugDraw;
    };
}