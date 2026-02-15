// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Color.hpp>
#include <random>

namespace we
{
    class RandomGenerator
    {
    public:

        static RandomGenerator& Get()
        {
            static RandomGenerator Instance;
            return Instance;
        }

        template<typename T>
        T Random(T Min, T Max);

        template<typename Container>
        auto& Random(const Container& C);        

        bool Random(double Probability = 0.5);
        vec2f Random(vec2f Min, vec2f Max);
        angle Random(angle Min, angle Max);
        color Random(color Min, color Max, uint8 Alpha = 255);

    private:
        std::mt19937 Generator{ std::random_device{}() };
    };

    template<typename T>
    T RandomGenerator::Random(T Min, T Max)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return std::uniform_real_distribution<T>(Min, Max)(Generator);
        }
        else if constexpr (sizeof(T) == 1)
        {
            return static_cast<T>(std::uniform_int_distribution<int>(Min, Max)(Generator));
        }
        else
        {
            return std::uniform_int_distribution<T>(Min, Max)(Generator);
        }
    }
    inline RandomGenerator& RNG() { return RandomGenerator::Get(); }

    template<typename Container>
    inline auto& RandomGenerator::Random(const Container& C)
    {
        assert(!C.empty());
        return C[Random<size_t>(0, C.size() - 1)];
    }
}