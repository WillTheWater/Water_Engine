#pragma once

#include <SFML/Graphics.hpp>

namespace we
{
	sf::Vector2f RotationToVector(const sf::Angle Rotation);

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
	sf::Vector2<T>& Normalize(sf::Vector2<T>& Vector)
	{
		float VectorLength = GetVectorLength(Vector);
		if (VectorLength == 0) { return sf::Vector2<T>{}; }
		ScaleVector(Vector, 1.0/VectorLength);
		return Vector;
	}
}