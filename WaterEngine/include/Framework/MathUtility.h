#pragma once

#include <SFML/Graphics.hpp>
#include <random>

namespace we
{
	sf::Vector2f RotationToVector(const sf::Angle Rotation);
	sf::Angle VectorToRotation(const sf::Vector2f Vector);

	float Lerp(float Start, float Target, float Alpha);
	sf::Color LerpColor(const sf::Color& Start, const sf::Color& Target, float Alpha);
	sf::Vector2f LerpVector(const sf::Vector2f& Start, const sf::Vector2f& Target, float Alpha);

	float RandomRange(float Min, float Max);
	sf::Vector2f RandomUnitVector();

	template<typename T>
	float GetVectorLength(const sf::Vector2<T>& Vector)
	{
		return std::sqrt(Vector.x * Vector.x + Vector.y * Vector.y);
	}

	template<typename T>
	sf::Vector2<T>& ScaleVector(sf::Vector2<T>& Vector, float Scale)
	{
		Vector.x *= Scale;
		Vector.y *= Scale;
		return Vector;
	}

	template<typename T>
	sf::Vector2<T>& Normalize(sf::Vector2<T>& vector)
	{
		float vectorLength = GetVectorLength<T>(vector);
		if (vectorLength == 0.f) return vector;

		ScaleVector(vector, 1.0 / vectorLength);

		return vector;
	}

}