// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/CollisionTypes.h"

namespace we
{
    // =========================================================================
    // Boundary System - Simple line-based level boundaries
    // 
    // No components, no actors - just line segments that block movement
    // Define boundaries as connected chains or individual segments
    // =========================================================================
    struct BoundarySegment
    {
        vec2f Start;
        vec2f End;
        float Thickness;
        
        BoundarySegment() = default;
        BoundarySegment(const vec2f& InStart, const vec2f& InEnd, float InThickness = 10.0f)
            : Start(InStart), End(InEnd), Thickness(InThickness) {}
    };

    class BoundarySystem
    {
    public:
        BoundarySystem() = default;
        
        // Add individual line segment
        void AddSegment(const vec2f& Start, const vec2f& End, float Thickness = 10.0f);
        
        // Add chain of connected points (creates segments between consecutive points)
        void AddChain(const vector<vec2f>& Points, float Thickness = 10.0f);
        
        // Add box/rectangle boundaries (4 segments)
        void AddBox(const vec2f& Min, const vec2f& Max, float Thickness = 10.0f);
        
        // Properties
        void SetDefaultThickness(float Thickness) { DefaultThickness = Thickness; }
        float GetDefaultThickness() const { return DefaultThickness; }
        
        void Clear() { Segments.clear(); }
        size_t GetSegmentCount() const { return Segments.size(); }
        const vector<BoundarySegment>& GetSegments() const { return Segments; }
        
        // Collision queries
        SweepResult SweepCircle(const vec2f& StartPos, const vec2f& Delta, float Radius) const;
        SweepResult SweepCapsule(const vec2f& StartPos, const vec2f& Delta, const CapsuleShape& Capsule) const;
        
        // Test if point is inside any boundary (with radius)
        bool TestPoint(const vec2f& Point, float Radius = 0.0f) const;
        
        // Debug draw all boundaries
        void DebugDraw() const;
        
    private:
        // Single segment sweep
        SweepResult SweepSegment(const BoundarySegment& Seg, 
            const vec2f& StartPos, const vec2f& Delta, float Radius) const;
        
        // Helpers
        vec2f GetClosestPointOnSegment(const vec2f& Point, 
            const vec2f& SegStart, const vec2f& SegEnd) const;
        SweepResult SweepCircleVsCircle(
            const vec2f& RayStart, const vec2f& RayDir, float RayLength,
            const vec2f& CircleCenter, float CombinedRadius) const;
        
    private:
        vector<BoundarySegment> Segments;
        float DefaultThickness = 10.0f;
    };
} // namespace we
