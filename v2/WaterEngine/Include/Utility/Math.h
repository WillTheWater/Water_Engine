// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <random>
#include <cassert>

namespace we
{
    // =============================================================================
    // Constants (PI is defined in CoreMinimal.h)
    // =============================================================================
    inline constexpr float TWO_PI = 2.0f * PI;
    inline constexpr float HALF_PI = PI / 2.0f;
    inline constexpr float DEG_TO_RAD = PI / 180.0f;
    inline constexpr float RAD_TO_DEG = 180.0f / PI;
    inline constexpr float EPSILON = 1e-6f;

    // =============================================================================
    // Basic Math
    // =============================================================================
    template<typename T>
    inline T Abs(T Value) { return std::abs(Value); }

    template<typename T>
    inline T Min(T A, T B) { return (A < B) ? A : B; }

    template<typename T>
    inline T Max(T A, T B) { return (A > B) ? A : B; }

    template<typename T>
    inline T Clamp(T Value, T MinVal, T MaxVal)
    {
        return (Value < MinVal) ? MinVal : (Value > MaxVal) ? MaxVal : Value;
    }

    template<typename T>
    inline T Clamp01(T Value) { return Clamp(Value, T(0), T(1)); }

    // Wrap value to range [0, Max)
    template<typename T>
    inline T Wrap(T Value, T Max)
    {
        T Result = std::fmod(Value, Max);
        return (Result < 0) ? Result + Max : Result;
    }

    // =============================================================================
    // Interpolation
    // =============================================================================
    
    // Linear interpolation: Lerp(A, B, 0) = A, Lerp(A, B, 1) = B
    inline float Lerp(float Start, float Target, float Alpha)
    {
        return Start + (Target - Start) * Alpha;
    }

    // Color interpolation
    inline color LerpColor(const color& Start, const color& Target, float Alpha)
    {
        Alpha = Clamp01(Alpha);
        return color{
            static_cast<uint8_t>(Lerp(static_cast<float>(Start.r), static_cast<float>(Target.r), Alpha)),
            static_cast<uint8_t>(Lerp(static_cast<float>(Start.g), static_cast<float>(Target.g), Alpha)),
            static_cast<uint8_t>(Lerp(static_cast<float>(Start.b), static_cast<float>(Target.b), Alpha)),
            static_cast<uint8_t>(Lerp(static_cast<float>(Start.a), static_cast<float>(Target.a), Alpha))
        };
    }

    // Vector2f interpolation
    inline vec2f LerpVector(const vec2f& Start, const vec2f& Target, float Alpha)
    {
        return vec2f{
            Lerp(Start.x, Target.x, Alpha),
            Lerp(Start.y, Target.y, Alpha)
        };
    }

    // Smooth interpolation (ease in/out) - 3t^2 - 2t^3
    inline float SmoothStep(float Start, float Target, float Alpha)
    {
        Alpha = Clamp01(Alpha);
        Alpha = Alpha * Alpha * (3.0f - 2.0f * Alpha);
        return Lerp(Start, Target, Alpha);
    }

    // Smoother interpolation - 6t^5 - 15t^4 + 10t^3
    inline float SmootherStep(float Start, float Target, float Alpha)
    {
        Alpha = Clamp01(Alpha);
        Alpha = Alpha * Alpha * Alpha * (Alpha * (Alpha * 6.0f - 15.0f) + 10.0f);
        return Lerp(Start, Target, Alpha);
    }

    // Ease in (quadratic)
    inline float EaseIn(float Start, float Target, float Alpha)
    {
        Alpha = Clamp01(Alpha);
        return Lerp(Start, Target, Alpha * Alpha);
    }

    // Ease out (quadratic)
    inline float EaseOut(float Start, float Target, float Alpha)
    {
        Alpha = Clamp01(Alpha);
        return Lerp(Start, Target, 1.0f - (1.0f - Alpha) * (1.0f - Alpha));
    }

    // =============================================================================
    // Vector Math
    // =============================================================================
    
    // Vector length squared (faster than Length, use when comparing distances)
    inline float LengthSquared(const vec2f& V) { return V.x * V.x + V.y * V.y; }

    // Vector length
    inline float Length(const vec2f& V) { return std::sqrt(LengthSquared(V)); }

    // Normalize vector (returns zero vector if input is zero)
    inline vec2f Normalize(const vec2f& V)
    {
        float Len = Length(V);
        if (Len > EPSILON)
        {
            return V / Len;
        }
        return vec2f{ 0.0f, 0.0f };
    }

    // Safe normalize with fallback direction
    inline vec2f NormalizeSafe(const vec2f& V, const vec2f& Fallback)
    {
        float Len = Length(V);
        if (Len > EPSILON)
        {
            return V / Len;
        }
        return Fallback;
    }

    // Dot product
    inline float Dot(const vec2f& A, const vec2f& B) { return A.x * B.x + A.y * B.y; }

    // Distance between two points
    inline float Distance(const vec2f& A, const vec2f& B) { return Length(B - A); }

    // Distance squared (faster, use for comparisons)
    inline float DistanceSquared(const vec2f& A, const vec2f& B) { return LengthSquared(B - A); }

    // Project A onto B (B should be normalized)
    inline vec2f Project(const vec2f& A, const vec2f& B) { return B * Dot(A, B); }

    // Reflect vector V across normal N (N should be normalized)
    inline vec2f Reflect(const vec2f& V, const vec2f& N) { return V - 2.0f * Dot(V, N) * N; }

    // Perpendicular vector (rotated 90 degrees CCW)
    inline vec2f Perpendicular(const vec2f& V) { return vec2f{ -V.y, V.x }; }

    // Linear interpolation between vectors by distance
    inline vec2f MoveTowards(const vec2f& Current, const vec2f& Target, float MaxDistance)
    {
        vec2f Delta = Target - Current;
        float Dist = Length(Delta);
        if (Dist <= MaxDistance || Dist < EPSILON)
        {
            return Target;
        }
        return Current + (Delta / Dist) * MaxDistance;
    }

    // =============================================================================
    // Angle Math
    // =============================================================================
    
    // Convert degrees to radians
    inline float DegreesToRadians(float Degrees) { return Degrees * DEG_TO_RAD; }

    // Convert radians to degrees  
    inline float RadiansToDegrees(float Radians) { return Radians * RAD_TO_DEG; }

    // Get angle of vector in radians (-PI to PI)
    inline float Angle(const vec2f& V) { return std::atan2(V.y, V.x); }

    // Get angle between two vectors in radians
    inline float AngleBetween(const vec2f& A, const vec2f& B)
    {
        float DotProduct = Dot(Normalize(A), Normalize(B));
        DotProduct = Clamp(DotProduct, -1.0f, 1.0f);  // Prevent NaN from floating point error
        return std::acos(DotProduct);
    }

    // Rotate vector by angle (radians)
    inline vec2f Rotate(const vec2f& V, float Radians)
    {
        float Cos = std::cos(Radians);
        float Sin = std::sin(Radians);
        return vec2f{
            V.x * Cos - V.y * Sin,
            V.x * Sin + V.y * Cos
        };
    }

    // Direction vector from angle (radians), 0 = right
    inline vec2f DirectionFromAngle(float Radians) { return vec2f{ std::cos(Radians), std::sin(Radians) }; }

    // =============================================================================
    // Random Number Generator
    // 
    // Simple global functions for common cases:
    //   float Value = RandomRange(0.0f, 100.0f);
    //   int Roll = RandomRangeInt(1, 20);
    //   vec2f Pos = RandomPointInCircle() * Radius;
    //
    // Or use RNG() for more options:
    //   RNG().Random(0.0f, 100.0f);
    //   RNG().Random(MyVector);  // Random element
    //   RNG().Random(0.75);  // 75% chance
    // =============================================================================

    // Random float in range [0, 1)
    float Random01();

    // Random float in range [Min, Max)
    float RandomRange(float Min, float Max);

    // Random int in range [Min, Max]
    int RandomRangeInt(int Min, int Max);

    // Random point in unit circle (uniform distribution)
    vec2f RandomPointInCircle();

    // Random direction (normalized vector)
    vec2f RandomDirection();

    // Full-featured random generator singleton
    class RandomGenerator
    {
    public:
        static RandomGenerator& Get()
        {
            static RandomGenerator Instance;
            return Instance;
        }

        // Non-template overloads implemented in Math.cpp
        bool Random(double Probability);
        float Random(float Min, float Max);
        int Random(int Min, int Max);
        vec2f Random(vec2f Min, vec2f Max);
        angle Random(angle Min, angle Max);
        color Random(color Min, color Max, uint8_t Alpha = 255);

        // Template for integral/floating types
        template<typename T>
        inline T Random(T Min, T Max)
        {
            return static_cast<T>(Random(static_cast<float>(Min), static_cast<float>(Max)));
        }

        // Random element from container
        template<typename Container>
        inline auto& RandomElement(const Container& C)
        {
            assert(!C.empty());
            return C[Random<size_t>(0u, C.size() - 1)];
        }

    private:
        std::mt19937 RandomEngine = std::mt19937{ std::random_device{}() };
    };

    // Global shortcut: RNG().Random(...)
    inline RandomGenerator& RNG() { return RandomGenerator::Get(); }

    // Legacy compatibility: RandomBool(Probability)
    inline bool RandomBool(double Probability = 0.5) { return RNG().Random(Probability); }
}
