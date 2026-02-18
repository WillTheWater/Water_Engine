// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/BoundarySystem.h"
#include "Utility/Math.h"
#include "Utility/DebugDraw.h"

namespace we
{
    void BoundarySystem::AddSegment(const vec2f& Start, const vec2f& End, float Thickness)
    {
        Segments.emplace_back(Start, End, Thickness);
    }
    
    void BoundarySystem::AddChain(const vector<vec2f>& Points, float Thickness)
    {
        if (Points.size() < 2) return;
        
        for (size_t i = 0; i < Points.size() - 1; ++i)
        {
            Segments.emplace_back(Points[i], Points[i + 1], Thickness);
        }
    }
    
    void BoundarySystem::AddBox(const vec2f& Min, const vec2f& Max, float Thickness)
    {
        // Top
        Segments.emplace_back(vec2f{ Min.x, Min.y }, vec2f{ Max.x, Min.y }, Thickness);
        // Right
        Segments.emplace_back(vec2f{ Max.x, Min.y }, vec2f{ Max.x, Max.y }, Thickness);
        // Bottom
        Segments.emplace_back(vec2f{ Max.x, Max.y }, vec2f{ Min.x, Max.y }, Thickness);
        // Left
        Segments.emplace_back(vec2f{ Min.x, Max.y }, vec2f{ Min.x, Min.y }, Thickness);
    }
    
    SweepResult BoundarySystem::SweepCircle(const vec2f& StartPos, const vec2f& Delta, float Radius) const
    {
        SweepResult BestResult;
        BestResult.Time = 1.0f;
        BestResult.Distance = Length(Delta);
        
        if (BestResult.Distance < 0.001f) return BestResult;
        
        for (const auto& Seg : Segments)
        {
            SweepResult Hit = SweepSegment(Seg, StartPos, Delta, Radius);
            
            if (Hit.bBlockingHit && Hit.Time < BestResult.Time)
            {
                BestResult = Hit;
            }
        }
        
        return BestResult;
    }
    
    SweepResult BoundarySystem::SweepCapsule(const vec2f& StartPos, const vec2f& Delta, const CapsuleShape& Capsule) const
    {
        // Approximate capsule as circle with capsule radius
        return SweepCircle(StartPos, Delta, Capsule.Radius);
    }
    
    bool BoundarySystem::TestPoint(const vec2f& Point, float Radius) const
    {
        for (const auto& Seg : Segments)
        {
            vec2f Closest = GetClosestPointOnSegment(Point, Seg.Start, Seg.End);
            float DistSq = DistanceSquared(Point, Closest);
            float CombinedRadius = Seg.Thickness + Radius;
            
            if (DistSq <= CombinedRadius * CombinedRadius)
            {
                return true;
            }
        }
        return false;
    }
    
    void BoundarySystem::DebugDraw() const
    {
        if (!DebugDraw::IsEnabled()) return;
        
        color BoundaryColor = color{ 255, 0, 255 };  // Magenta for boundaries
        
        for (const auto& Seg : Segments)
        {
            // Draw the line segment
            DebugDraw::Line(Seg.Start, Seg.End, BoundaryColor, 3.0f);
            
            // Draw endpoints
            DebugDraw::Circle(Seg.Start, 5.0f, BoundaryColor, 2.0f);
            DebugDraw::Circle(Seg.End, 5.0f, BoundaryColor, 2.0f);
        }
    }
    
    SweepResult BoundarySystem::SweepSegment(const BoundarySegment& Seg, 
        const vec2f& StartPos, const vec2f& Delta, float Radius) const
    {
        SweepResult Result;
        Result.Time = 1.0f;
        Result.Distance = Length(Delta);
        
        if (Result.Distance < 0.001f) return Result;
        
        vec2f RayDir = Delta / Result.Distance;
        vec2f LineDir = Seg.End - Seg.Start;
        float LineLength = Length(LineDir);
        
        if (LineLength < 0.001f) return Result;
        
        LineDir = LineDir / LineLength;
        vec2f LineNormal = Perpendicular(LineDir);
        
        float CombinedRadius = Seg.Thickness + Radius;
        
        // Ray vs infinite line intersection
        float Denom = Dot(RayDir, LineNormal);
        
        if (std::abs(Denom) < 0.0001f)
        {
            return Result;  // Parallel
        }
        
        float T = -Dot(StartPos - Seg.Start, LineNormal) / Denom;
        
        if (T < 0.0f || T > Result.Distance)
        {
            return Result;
        }
        
        vec2f HitPoint = StartPos + RayDir * T;
        float LineT = Dot(HitPoint - Seg.Start, LineDir);
        
        if (LineT < 0.0f || LineT > LineLength)
        {
            // Beyond segment endpoints - check endpoint circles
            SweepResult StartCap = SweepCircleVsCircle(
                StartPos, RayDir, Result.Distance, Seg.Start, CombinedRadius);
            SweepResult EndCap = SweepCircleVsCircle(
                StartPos, RayDir, Result.Distance, Seg.End, CombinedRadius);
            
            if (StartCap.bBlockingHit && StartCap.Time < Result.Time)
            {
                Result = StartCap;
            }
            if (EndCap.bBlockingHit && EndCap.Time < Result.Time)
            {
                Result = EndCap;
            }
            
            return Result;
        }
        
        // Valid hit on line segment
        Result.bBlockingHit = true;
        Result.Time = T / Result.Distance;
        Result.Distance = T;
        Result.ImpactPoint = HitPoint;
        Result.ImpactNormal = (Denom > 0.0f) ? -LineNormal : LineNormal;
        
        return Result;
    }
    
    vec2f BoundarySystem::GetClosestPointOnSegment(const vec2f& Point, 
        const vec2f& SegStart, const vec2f& SegEnd) const
    {
        vec2f Segment = SegEnd - SegStart;
        vec2f ToPoint = Point - SegStart;
        
        float SegmentLengthSq = LengthSquared(Segment);
        
        if (SegmentLengthSq < 0.0001f)
        {
            return SegStart;
        }
        
        float T = std::clamp(Dot(ToPoint, Segment) / SegmentLengthSq, 0.0f, 1.0f);
        return SegStart + Segment * T;
    }
    
    SweepResult BoundarySystem::SweepCircleVsCircle(
        const vec2f& RayStart, const vec2f& RayDir, float RayLength,
        const vec2f& CircleCenter, float CombinedRadius) const
    {
        SweepResult Result;
        Result.Time = 1.0f;
        Result.Distance = RayLength;
        
        vec2f ToCircle = CircleCenter - RayStart;
        float ProjLength = Dot(ToCircle, RayDir);
        
        vec2f ClosestPoint = RayStart + RayDir * ProjLength;
        float DistSq = DistanceSquared(ClosestPoint, CircleCenter);
        float RadiusSq = CombinedRadius * CombinedRadius;
        
        if (DistSq > RadiusSq) return Result;
        
        float Offset = std::sqrt(RadiusSq - DistSq);
        float HitDist = ProjLength - Offset;
        
        if (HitDist < 0.0f)
        {
            HitDist = ProjLength + Offset;
        }
        
        if (HitDist >= 0.0f && HitDist <= RayLength)
        {
            Result.bBlockingHit = true;
            Result.Time = HitDist / RayLength;
            Result.Distance = HitDist;
            Result.ImpactPoint = RayStart + RayDir * HitDist;
            Result.ImpactNormal = Normalize(Result.ImpactPoint - CircleCenter);
        }
        
        return Result;
    }
} // namespace we
