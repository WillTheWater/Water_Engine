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

    // =========================================================================
    // Line Collision Component - Infinite line segment for walls/fences
    // 
    // Used for barriers that stop movement but have no visual representation
    // (the level art shows the fence/wall, this just provides collision)
    // =========================================================================
    class LineCollisionComponent : public IActorComponent
    {
    public:
        explicit LineCollisionComponent(Actor* InOwner);
        virtual ~LineCollisionComponent();

        // IActorComponent interface
        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;
        virtual void EndPlay() override;
        virtual Actor* GetOwner() const override { return Owner; }

        // Line setup - define the line segment in world space (or relative to actor)
        void SetLineStart(const vec2f& Start);
        void SetLineEnd(const vec2f& End);
        void SetLine(const vec2f& Start, const vec2f& End);

        // Thickness for collision (0 = infinite thin line)
        void SetThickness(float Thickness);

        // Getters
        vec2f GetLineStart() const;
        vec2f GetLineEnd() const;
        float GetThickness() const { return Thickness; }

        // Collision queries
        bool TestPoint(const vec2f& Point, float Radius) const;
        SweepResult SweepCircle(const vec2f& StartPos, const vec2f& Delta, float Radius) const;

        // Get the closest point on the line segment to a given point
        vec2f GetClosestPointOnSegment(const vec2f& Point) const;

    private:
        SweepResult SweepCircleVsCircle(
            const vec2f& StartPos, const vec2f& Delta, float Radius, 
            const vec2f& CircleCenter, float CircleRadius) const;
        Actor* Owner;
        vec2f LocalStart;      // Relative to actor position
        vec2f LocalEnd;
        float Thickness;
    };
} // namespace we
