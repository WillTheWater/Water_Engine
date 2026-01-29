// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Utility/RandomGenerator.h"

namespace we
{
	bool we::RandomGenerator::Random(double Probability)
	{
		double SafeProbablity = std::clamp(Probability, 0.0, 1.0); // Ensures Probablity [0.0, 1.0]
		return std::bernoulli_distribution(SafeProbablity)(Generator);
	}

	vec2f RandomGenerator::Random(vec2f Min, vec2f Max)
	{
		return { Random(Min.x, Max.x), Random(Min.y, Max.y) };
	}

	angle RandomGenerator::Random(angle Min, angle Max)
	{
		return sf::degrees(Random(Min.asDegrees(), Max.asDegrees()));
	}

	color RandomGenerator::Random(color Min, color Max, uint8 Alpha)
	{
		return {
		(uint8)(Random(Min.r, Max.r)),
		(uint8)(Random(Min.g, Max.g)),
		(uint8)(Random(Min.b, Max.b)),
		Alpha
		};
	}
}