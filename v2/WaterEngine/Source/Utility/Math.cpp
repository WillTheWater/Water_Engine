// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Utility/Math.h"
#include <random>

namespace we
{
    // =============================================================================
    // Global Random Functions (Simple API)
    // =============================================================================
    namespace
    {
        thread_local std::mt19937 ThreadRandomEngine{ std::random_device{}() };
    }

    float Random01()
    {
        std::uniform_real_distribution<float> Dist(0.0f, 1.0f);
        return Dist(ThreadRandomEngine);
    }

    float RandomRange(float Min, float Max)
    {
        std::uniform_real_distribution<float> Dist(Min, Max);
        return Dist(ThreadRandomEngine);
    }

    int RandomRangeInt(int Min, int Max)
    {
        std::uniform_int_distribution<int> Dist(Min, Max);
        return Dist(ThreadRandomEngine);
    }

    vec2f RandomPointInCircle()
    {
        // Rejection sampling for uniform distribution
        while (true)
        {
            float X = RandomRange(-1.0f, 1.0f);
            float Y = RandomRange(-1.0f, 1.0f);
            if (X * X + Y * Y <= 1.0f)
            {
                return vec2f{ X, Y };
            }
        }
    }

    vec2f RandomDirection()
    {
        float Angle = RandomRange(0.0f, TWO_PI);
        return DirectionFromAngle(Angle);
    }

    // =============================================================================
    // RandomGenerator Implementation (Full-Featured API)
    // =============================================================================
    bool RandomGenerator::Random(double Probability)
    {
        double SafeProbability = std::clamp(Probability, 0.0, 1.0);
        return std::bernoulli_distribution(SafeProbability)(RandomEngine);
    }

    float RandomGenerator::Random(float Min, float Max)
    {
        std::uniform_real_distribution<float> Dist(Min, Max);
        return Dist(RandomEngine);
    }

    int RandomGenerator::Random(int Min, int Max)
    {
        std::uniform_int_distribution<int> Dist(Min, Max);
        return Dist(RandomEngine);
    }

    vec2f RandomGenerator::Random(vec2f Min, vec2f Max)
    {
        return { Random(Min.x, Max.x), Random(Min.y, Max.y) };
    }

    angle RandomGenerator::Random(angle Min, angle Max)
    {
        return sf::degrees(Random(Min.asDegrees(), Max.asDegrees()));
    }

    color RandomGenerator::Random(color Min, color Max, uint8_t Alpha)
    {
        return {
            static_cast<uint8_t>(Random(Min.r, Max.r)),
            static_cast<uint8_t>(Random(Min.g, Max.g)),
            static_cast<uint8_t>(Random(Min.b, Max.b)),
            Alpha
        };
    }
}
