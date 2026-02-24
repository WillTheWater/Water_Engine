// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Component/IPhysicsComponent.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Utility/Delegate.h"

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
        virtual void OnBeginOverlap(IPhysicsComponent* Other) override;
        virtual void OnEndOverlap(IPhysicsComponent* Other) override;
        virtual b2Body* GetPhysicsBody() const override { return Body; }

        // Delegates for overlap events (composition-friendly alternative to overriding)
        Delegate<IPhysicsComponent*> OnBeginOverlapEvent;
        Delegate<IPhysicsComponent*> OnEndOverlapEvent;

        // Shape setup
        void SetBodyType(BodyType Type);
        void SetBoxShape(const vec2f& HalfExtents);
        void SetCircleShape(float Radius);

        // Properties
        void SetDensity(float Density);
        void SetFriction(float Friction);
        void SetRestitution(float Restitution);
        void SetSensor(bool bInSensor);
        void SetLinearDamping(float Damping);
        void SetFixedRotation(bool bFixed);

        // Physics queries
        vec2f GetVelocity() const;
        void SetVelocity(const vec2f& Velocity);
        void ApplyForce(const vec2f& Force);
        void ApplyImpulse(const vec2f& Impulse);

        // Sensor (trigger) support - for overlap detection without blocking
        void SetSensorShape(bool bCircle, float RadiusOrHalfExtent);
        void SetSensorBoxShape(const vec2f& HalfExtents);
        void SetSensorOffset(const vec2f& Offset);
        bool HasSensor() const { return SensorFixture != nullptr; }

    private:
        void SyncActorToBody();   // Kinematic: actor -> body
        void SyncBodyToActor();   // Dynamic: body -> actor
        void RecreateFixture();
        void RecreateSensorFixture();
        void DrawDebugShape();    // Draw debug visualization

        PhysicsSubsystem& GetPhysics() const;

    private:
        Actor* Owner;
        b2Body* Body;
        b2Fixture* Fixture;
        b2Fixture* SensorFixture;

        BodyType Type;
        vec2f ShapeHalfExtents;
        float ShapeRadius;
        bool bIsCircle;
        float Density;
        float Friction;
        float Restitution;
        bool bSensor;
        bool bNeedsFixtureUpdate;

        // Sensor data
        bool bSensorIsCircle = true;
        float SensorRadius = 0.0f;
        vec2f SensorHalfExtents = { 0.0f, 0.0f };
        vec2f SensorOffset = { 0.0f, 0.0f };
        bool bNeedsSensorUpdate = false;
    };
}
