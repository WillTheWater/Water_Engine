// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Interface/Component/LineCollisionComponent.h"
#include "Framework/World/Actor/Actor.h"
#include "Subsystem/CollisionSubsystem.h"
#include "Utility/Math.h"
#include "Utility/DebugDraw.h"

namespace we
{
    LineCollisionComponent::LineCollisionComponent(Actor* InOwner)
        : Owner(InOwner)
        , LocalStart{ -100.0f, 0.0f }
        , LocalEnd{ 100.0f, 0.0f }
        , Thickness{ 10.0f }
    {
    }

    LineCollisionComponent::~LineCollisionComponent()
    {
    }

    void LineCollisionComponent::BeginPlay()
    {
        // Register with collision world if we add line collision support there
    }

    void LineCollisionComponent::Tick(float DeltaTime)
    {
        (void)DeltaTime;

        // Debug draw the line
        if (DebugDraw::IsEnabled() && Owner)
        {
            vec2f WorldStart = GetLineStart();
            vec2f WorldEnd = GetLineEnd();
            
            // Draw the line segment
            DebugDraw::Line(WorldStart, WorldEnd, color{ 255, 128, 0 }, 3.0f);
            
            // Draw endpoints
            DebugDraw::Circle(WorldStart, 5.0f, color{ 255, 128, 0 }, 2.0f);
            DebugDraw::Circle(WorldEnd, 5.0f, color{ 255, 128, 0 }, 2.0f);
        }
    }

    void LineCollisionComponent::EndPlay()
    {
    }

    void LineCollisionComponent::SetLineStart(const vec2f& Start)
    {
        LocalStart = Start;
    }

    void LineCollisionComponent::SetLineEnd(const vec2f& End)
    {
        LocalEnd = End;
    }

    void LineCollisionComponent::SetLine(const vec2f& Start, const vec2f& End)
    {
        LocalStart = Start;
        LocalEnd = End;
    }

    void LineCollisionComponent::SetThickness(float InThickness)
    {
        Thickness = InThickness;
    }

    vec2f LineCollisionComponent::GetLineStart() const
    {
        return Owner ? Owner->GetPosition() + LocalStart : LocalStart;
    }

    vec2f LineCollisionComponent::GetLineEnd() const
    {
        return Owner ? Owner->GetPosition() + LocalEnd : LocalEnd;
    }

    vec2f LineCollisionComponent::GetClosestPointOnSegment(const vec2f& Point) const
    {
        vec2f Start = GetLineStart();
        vec2f End = GetLineEnd();
        
        vec2f Segment = End - Start;
        vec2f ToPoint = Point - Start;
        
        float SegmentLengthSq = LengthSquared(Segment);
        
        if (SegmentLengthSq < 0.0001f)
        {
            // Degenerate line, return start point
            return Start;
        }
        
        // Project point onto line, clamped to segment
        float T = std::clamp(Dot(ToPoint, Segment) / SegmentLengthSq, 0.0f, 1.0f);
        
        return Start + Segment * T;
    }

    bool LineCollisionComponent::TestPoint(const vec2f& Point, float Radius) const
    {
        vec2f Closest = GetClosestPointOnSegment(Point);
        float DistSq = DistanceSquared(Point, Closest);
        float CombinedRadius = Thickness + Radius;
        
        return DistSq <= CombinedRadius * CombinedRadius;
    }

    SweepResult LineCollisionComponent::SweepCircle(const vec2f& StartPos, const vec2f& Delta, float Radius) const
    {
        SweepResult Result;
        Result.Time = 1.0f;
        Result.Distance = Length(Delta);
        
        if (Result.Distance < 0.001f) return Result;
        
        vec2f LineStart = GetLineStart();
        vec2f LineEnd = GetLineEnd();
        vec2f LineDir = LineEnd - LineStart;
        float LineLength = Length(LineDir);
        
        if (LineLength < 0.001f) return Result;
        
        LineDir = LineDir / LineLength;
        vec2f LineNormal = Perpendicular(LineDir); // Perpendicular to line
        
        // Treat as infinite line first, then check segment bounds
        vec2f RayDir = Delta / Result.Distance;
        float CombinedRadius = Thickness + Radius;
        
        // Ray vs infinite line intersection
        // Line: (Point - LineStart) . LineNormal = 0
        // Ray: StartPos + RayDir * t
        // Solve: ((StartPos + RayDir * t) - LineStart) . LineNormal = 0
        // (StartPos - LineStart) . LineNormal + t * (RayDir . LineNormal) = 0
        
        float Denom = Dot(RayDir, LineNormal);
        
        if (std::abs(Denom) < 0.0001f)
        {
            // Ray is parallel to line
            return Result;
        }
        
        float T = -Dot(StartPos - LineStart, LineNormal) / Denom;
        
        // Check if intersection is in front of ray and within segment
        if (T < 0.0f || T > Result.Distance)
        {
            return Result;
        }
        
        vec2f HitPoint = StartPos + RayDir * T;
        
        // Check if hit point is within line segment bounds
        float LineT = Dot(HitPoint - LineStart, LineDir);
        
        if (LineT < 0.0f || LineT > LineLength)
        {
            // Hit is beyond segment endpoints, check endpoint circles
            SweepResult StartCapResult = SweepCircleVsCircle(StartPos, Delta, Radius, LineStart, Thickness);
            SweepResult EndCapResult = SweepCircleVsCircle(StartPos, Delta, Radius, LineEnd, Thickness);
            
            // Return closest hit
            if (StartCapResult.bBlockingHit && StartCapResult.Time < Result.Time)
            {
                Result = StartCapResult;
            }
            if (EndCapResult.bBlockingHit && EndCapResult.Time < Result.Time)
            {
                Result = EndCapResult;
            }
            
            return Result;
        }
        
        // Valid hit on line segment
        Result.bBlockingHit = true;
        Result.Time = T / Result.Distance;
        Result.Distance = T;
        Result.ImpactPoint = HitPoint;
        
        // Normal points away from line (oppose movement direction)
        Result.ImpactNormal = (Denom > 0.0f) ? -LineNormal : LineNormal;
        
        return Result;
    }
    
    SweepResult LineCollisionComponent::SweepCircleVsCircle(
        const vec2f& StartPos, const vec2f& Delta, float Radius, 
        const vec2f& CircleCenter, float CircleRadius) const
    {
        SweepResult Result;
        Result.Time = 1.0f;
        Result.Distance = Length(Delta);
        
        if (Result.Distance < 0.001f) return Result;
        
        vec2f Dir = Delta / Result.Distance;
        float CombinedRadius = Radius + CircleRadius;
        
        vec2f ToCircle = CircleCenter - StartPos;
        float ProjLength = Dot(ToCircle, Dir);
        
        vec2f ClosestPoint = StartPos + Dir * ProjLength;
        float DistSq = DistanceSquared(ClosestPoint, CircleCenter);
        float RadiusSq = CombinedRadius * CombinedRadius;
        
        if (DistSq > RadiusSq) return Result;
        
        float Offset = std::sqrt(RadiusSq - DistSq);
        float HitDist = ProjLength - Offset;
        
        if (HitDist < 0.0f)
        {
            HitDist = ProjLength + Offset;
        }
        
        if (HitDist >= 0.0f && HitDist <= Result.Distance)
        {
            Result.bBlockingHit = true;
            Result.Time = HitDist / Result.Distance;
            Result.Distance = HitDist;
            Result.ImpactPoint = StartPos + Dir * HitDist;
            Result.ImpactNormal = Normalize(Result.ImpactPoint - CircleCenter);
        }
        
        return Result;
    }
} // namespace we
