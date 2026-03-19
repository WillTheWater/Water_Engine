// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"

#include "box2d/b2_body.h"

class b2Body;

namespace we
{
    class PhysicsComponent : public IActorComponent
    {
    public:
        enum class EShapeType
        {
            Circle,
            Rectangle
        };

        explicit PhysicsComponent(Actor* InOwner);

        // IActorComponent
        void BeginPlay() override;
        void Tick(float DeltaTime) override;
        void EndPlay() override;
        Actor* GetOwner() const override;

        void SetBodyType(b2BodyType Type);
        void SetShapeType(EShapeType Type);
        void SetShapeSize(vec2f Size);
        void SetLinearDamping(float Damping);  // 0 = no damping, higher = more friction

        // Queries
        b2BodyType GetBodyType() const { return BodyType; }
        EShapeType GetShapeType() const { return ShapeType; }
        vec2f GetShapeSize() const { return ShapeSize; }
        b2Body* GetBody() const { return Body; }
        bool HasBody() const { return Body != nullptr; }

        void SetVelocity(vec2f Velocity);
        vec2f GetVelocity() const;

        // Position (syncs body to actor position)
        void SyncBodyToActor();
        void SyncActorToBody();

        // Mass (only meaningful for dynamic bodies)
        void SetMass(float Mass);
        float GetMass() const;

        // Debug visualization
        const drawable* DrawDebug();

    private:
        void CreateBody();
        void DestroyBody();
        void CreateFixture();

    private:
        Actor* Owner;
        b2Body* Body = nullptr;

        // Configuration
        b2BodyType BodyType = b2_kinematicBody;
        EShapeType ShapeType = EShapeType::Circle;
        vec2f ShapeSize{32.0f, 32.0f};
        float LinearDamping = 10.0f;
        
        optional<circle> DebugCircle;
        optional<rectangle> DebugRect;
    };
}