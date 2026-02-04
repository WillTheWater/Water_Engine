// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "EngineConfig.h"

namespace we
{
	inline const font& GetDefaultTitleFont()
	{
		static const font Font(EC.DefaultTitleFont);
		return Font;
	}

	inline const font& GetDefaultFont()
	{
		static const font Font(EC.DefaultFont);
		return Font;
	}

	inline bool IsOutsideWindowLeft(const shape& Bounds)
	{
		return Bounds.getPosition().x < Bounds.getGlobalBounds().size.x / 2.f;
	}

	inline bool IsOutsideWindowRight(const shape& Bounds)
	{
		return Bounds.getPosition().x > EC.WindowSize.x - Bounds.getGlobalBounds().size.x / 2.f;
	}

	inline bool IsOutsideWindowTop(const shape& Bounds)
	{
		return Bounds.getPosition().y < Bounds.getGlobalBounds().size.y / 2.f;
	}

	inline bool IsOutsideWindowBottom(const shape& Bounds)
	{
		return Bounds.getPosition().y > EC.WindowSize.y - Bounds.getGlobalBounds().size.y / 2.f;
	}

	inline bool IsOutsideWindow(vec2f Point)
	{
		return Point.x < 0 || Point.y < 0 ||
			Point.x >= EC.WindowSize.x ||
			Point.y >= EC.WindowSize.y;
	}

	inline optional<sf::FloatRect> Intersects(const shape& ObjectA, const shape& ObjectB)
	{
		return ObjectA.getGlobalBounds().findIntersection(ObjectB.getGlobalBounds());
	}

	inline float DistanceSquared(vec2f PointA, vec2f PointB)
	{
		return (PointA - PointB).lengthSquared();
	}

	inline float Distance(vec2f PointA, vec2f PointB)
	{
		return (PointA - PointB).length();
	}

	inline bool Contains(const shape& Shape, vec2f Point)
	{
		return Shape.getGlobalBounds().contains(Point);
	}

	inline bool Contains(const circle& Circle, vec2f Point)
	{
		return DistanceSquared(Circle.getPosition(), Point) <= std::pow(Circle.getRadius(), 2);
	}

	inline vec2f Lerp(vec2f Start, vec2f End, float Time)
	{
		return { std::lerp(Start.x, End.x, Time), std::lerp(Start.y, End.y, Time) };
	}
}