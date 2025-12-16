#include "Framework/MathUtility.h"

namespace we
{
    sf::Vector2f RotationToVector(const sf::Angle Rotation)
    {
        const float radians = Rotation.asRadians();
        return sf::Vector2f(std::cos(radians), std::sin(radians));
    }

    sf::Angle VectorToRotation(const sf::Vector2f Vector)
    {
        return sf::radians(std::atan2(Vector.y, Vector.x));
    }

	float Lerp(float Start, float Target, float Alpha)
	{
		if (Alpha > 1) Alpha = 1;
		if (Alpha < 0) Alpha = 0;
		return Start + (Target - Start) * Alpha;
	}

	float RandomRange(float Min, float Max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_real_distribution<float> distribution{ Min, Max };

		return distribution(gen);
	}

	sf::Vector2f RandomUnitVector()
	{
		float randomX = RandomRange(-1, 1);
		float randomY = RandomRange(-1, 1);
		sf::Vector2f randVec{ randomX, randomY };
		Normalize(randVec);
		return randVec;
	}

	sf::Color LerpColor(const sf::Color& Start, const sf::Color& Target, float Alpha)
	{
		int lerpR = Lerp(Start.r, Start.r, Alpha);
		int lerpG = Lerp(Start.g, Target.g, Alpha);
		int lerpB = Lerp(Start.b, Target.b, Alpha);
		int lerpA = Lerp(Start.a, Target.a, Alpha);

		return sf::Color(lerpR, lerpG, lerpB, lerpA);
	}

	sf::Vector2f LerpVector(const sf::Vector2f& Start, const sf::Vector2f& Target, float Alpha)
	{
		float lerpX = Lerp(Start.x, Target.x, Alpha);
		float lerpY = Lerp(Start.y, Target.y, Alpha);

		return sf::Vector2f(lerpX, lerpY);
	}
}
