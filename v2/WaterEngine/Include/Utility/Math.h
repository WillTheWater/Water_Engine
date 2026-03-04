// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <random>

namespace we
{
    // =========================================================================
    // Float Comparison
    // =========================================================================
    inline bool FloatEquals(float A, float B, float Epsilon = EPSILON)
    {
        return std::abs(A - B) < Epsilon;
    }

    inline bool FloatIsZero(float Value, float Epsilon = EPSILON)
    {
        return std::abs(Value) < Epsilon;
    }

    // =========================================================================
    // Basic Math
    // =========================================================================
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

    template<typename T>
    inline T Wrap(T Value, T Max)
    {
        T Result = std::fmod(Value, Max);
        return (Result < 0) ? Result + Max : Result;
    }

    // =========================================================================
    // Interpolation
    // =========================================================================
    inline float Lerp(float Start, float Target, float Alpha)
    {
        return Start + (Target - Start) * Alpha;
    }

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

    inline vec2f LerpVector(const vec2f& Start, const vec2f& Target, float Alpha)
    {
        return vec2f{
            Lerp(Start.x, Target.x, Alpha),
            Lerp(Start.y, Target.y, Alpha)
        };
    }

    inline float SmoothStep(float Start, float Target, float Alpha)
    {
        Alpha = Clamp01(Alpha);
        Alpha = Alpha * Alpha * (3.0f - 2.0f * Alpha);
        return Lerp(Start, Target, Alpha);
    }

    inline float SmootherStep(float Start, float Target, float Alpha)
    {
        Alpha = Clamp01(Alpha);
        Alpha = Alpha * Alpha * Alpha * (Alpha * (Alpha * 6.0f - 15.0f) + 10.0f);
        return Lerp(Start, Target, Alpha);
    }

    inline float EaseIn(float Start, float Target, float Alpha)
    {
        Alpha = Clamp01(Alpha);
        return Lerp(Start, Target, Alpha * Alpha);
    }

    inline float EaseOut(float Start, float Target, float Alpha)
    {
        Alpha = Clamp01(Alpha);
        return Lerp(Start, Target, 1.0f - (1.0f - Alpha) * (1.0f - Alpha));
    }

    // =========================================================================
    // Vector Math
    // =========================================================================
    inline float LengthSquared(const vec2f& V) { return V.x * V.x + V.y * V.y; }
    inline float Length(const vec2f& V) { return std::sqrt(LengthSquared(V)); }

    inline vec2f Normalize(const vec2f& V)
    {
        float Len = Length(V);
        return (Len > EPSILON) ? V / Len : vec2f{ 0.0f, 0.0f };
    }

    inline vec2f NormalizeSafe(const vec2f& V, const vec2f& Fallback)
    {
        float Len = Length(V);
        return (Len > EPSILON) ? V / Len : Fallback;
    }

    inline float Dot(const vec2f& A, const vec2f& B) { return A.x * B.x + A.y * B.y; }
    inline float Distance(const vec2f& A, const vec2f& B) { return Length(B - A); }
    inline float DistanceSquared(const vec2f& A, const vec2f& B) { return LengthSquared(B - A); }
    inline vec2f Project(const vec2f& A, const vec2f& B) { return B * Dot(A, B); }
    inline vec2f Reflect(const vec2f& V, const vec2f& N) { return V - 2.0f * Dot(V, N) * N; }
    inline vec2f Perpendicular(const vec2f& V) { return vec2f{ -V.y, V.x }; }

    inline vec2f MoveTowards(const vec2f& Current, const vec2f& Target, float MaxDistance)
    {
        vec2f Delta = Target - Current;
        float Dist = Length(Delta);
        return (Dist <= MaxDistance || Dist < EPSILON) ? Target : Current + (Delta / Dist) * MaxDistance;
    }

    inline float SmoothDamp(float Current, float Target, float& Velocity, float SmoothTime, float MaxSpeed, float DeltaTime)
    {
        if (SmoothTime < EPSILON) return Target;

        float Omega = 2.0f / SmoothTime;
        float X = Omega * DeltaTime;
        float Exp = 1.0f / (1.0f + X + 0.48f * X * X + 0.235f * X * X * X);

        float Change = Current - Target;
        float OriginalChange = Change;

        if (MaxSpeed > EPSILON)
        {
            float MaxChange = MaxSpeed * SmoothTime;
            Change = Clamp(Change, -MaxChange, MaxChange);
        }

        float NewTarget = Current - Change;
        float Temp = (Velocity + Omega * Change) * DeltaTime;
        Velocity = (Velocity - Omega * Temp) * Exp;
        float Output = NewTarget + (Change + Temp) * Exp;

        if ((Target - Current > 0.0f) == (Output > Target))
        {
            Output = Target;
            Velocity = (Output - Target) / DeltaTime;
        }

        return Output;
    }

    inline vec2f SmoothDamp(const vec2f& Current, const vec2f& Target, vec2f& Velocity, float SmoothTime, float MaxSpeed, float DeltaTime)
    {
        return vec2f{
            SmoothDamp(Current.x, Target.x, Velocity.x, SmoothTime, MaxSpeed, DeltaTime),
            SmoothDamp(Current.y, Target.y, Velocity.y, SmoothTime, MaxSpeed, DeltaTime)
        };
    }

    // =========================================================================
    // Angle Math
    // =========================================================================
    inline float DegreesToRadians(float Degrees) { return Degrees * DEG_TO_RAD; }
    inline float RadiansToDegrees(float Radians) { return Radians * RAD_TO_DEG; }
    inline float Angle(const vec2f& V) { return std::atan2(V.y, V.x); }

    inline float AngleBetween(const vec2f& A, const vec2f& B)
    {
        float DotProduct = Dot(Normalize(A), Normalize(B));
        return std::acos(Clamp(DotProduct, -1.0f, 1.0f));
    }

    inline vec2f Rotate(const vec2f& V, float Radians)
    {
        float Cos = std::cos(Radians);
        float Sin = std::sin(Radians);
        return vec2f{
            V.x * Cos - V.y * Sin,
            V.x * Sin + V.y * Cos
        };
    }

    inline vec2f DirectionFromAngle(float Radians) { return vec2f{ std::cos(Radians), std::sin(Radians) }; }

    // =========================================================================
    // Random Number Generator
    // =========================================================================
    class RandomGenerator
    {
    public:
        static RandomGenerator& Get()
        {
            static RandomGenerator Instance;
            return Instance;
        }

        // Bool Probability (0.0 to 1.0)
        inline bool Random(double Probability)
        {
            std::uniform_real_distribution<double> Dist(0.0, 1.0);
            return Dist(RandomEngine) < Probability;
        }

        // Random Float [Min, Max]
        inline float Random(float Min, float Max)
        {
            std::uniform_real_distribution<float> Dist(Min, Max);
            return Dist(RandomEngine);
        }

        // Random Int [Min, Max]
        inline int Random(int Min, int Max)
        {
            std::uniform_int_distribution<int> Dist(Min, Max);
            return Dist(RandomEngine);
        }

        // Random Vector
        inline vec2f Random(vec2f Min, vec2f Max)
        {
            return vec2f{
                Random(Min.x, Max.x),
                Random(Min.y, Max.y)
            };
        }

        // Random Angle
        inline angle Random(angle Min, angle Max)
        {
            return sf::degrees(Random(Min.asRadians(), Max.asRadians()));
        }

        // Random Color
        inline color Random(color Min, color Max, uint8_t Alpha = 255)
        {
            return color{
                static_cast<uint8_t>(Random(static_cast<float>(Min.r), static_cast<float>(Max.r))),
                static_cast<uint8_t>(Random(static_cast<float>(Min.g), static_cast<float>(Max.g))),
                static_cast<uint8_t>(Random(static_cast<float>(Min.b), static_cast<float>(Max.b))),
                Alpha
            };
        }

        template<typename T>
        inline T Random(T Min, T Max)
        {
            return static_cast<T>(Random(static_cast<float>(Min), static_cast<float>(Max)));
        }

    private:
        std::mt19937 RandomEngine = std::mt19937{ std::random_device{}() };
    };

    inline RandomGenerator& RNG() { return RandomGenerator::Get(); }
}
