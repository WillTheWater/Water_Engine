// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"
#include "EngineConfig.h"

namespace we
{
	// -------------------------------------------------------------------------
	// Cursor / Mouse Position (Pixel-Accurate)
	// -------------------------------------------------------------------------
	
	// Get mouse position in screen pixels (0,0 to RenderResolution)
	inline vec2f GetMousePixelPosition()
	{
		// Use raw mouse position mapped to render resolution
		vec2i PixelPos = sf::Mouse::getPosition();
		return vec2f(static_cast<float>(PixelPos.x), static_cast<float>(PixelPos.y));
	}

	// Get mouse position relative to render resolution
	// This accounts for window scaling and gives accurate UI coordinates
	inline vec2f GetUICursorPosition(const renderTarget& Target)
	{
		vec2i PixelPos = sf::Mouse::getPosition();
		return Target.mapPixelToCoords(PixelPos);
	}

	// -------------------------------------------------------------------------
	// Hit Testing
	// -------------------------------------------------------------------------
	
	// Check if point is inside a rectangle (using top-left origin)
	inline bool PointInRect(vec2f Point, vec2f RectTopLeft, vec2f RectSize)
	{
		return Point.x >= RectTopLeft.x &&
		       Point.x < RectTopLeft.x + RectSize.x &&
		       Point.y >= RectTopLeft.y &&
		       Point.y < RectTopLeft.y + RectSize.y;
	}

	// Check if point is inside a centered rectangle
	inline bool PointInCenteredRect(vec2f Point, vec2f RectCenter, vec2f RectSize)
	{
		vec2f HalfSize = RectSize.componentWiseDiv({ 2.f, 2.f });
		return Point.x >= RectCenter.x - HalfSize.x &&
		       Point.x < RectCenter.x + HalfSize.x &&
		       Point.y >= RectCenter.y - HalfSize.y &&
		       Point.y < RectCenter.y + HalfSize.y;
	}

	// Check if point is inside a circle
	inline bool PointInCircle(vec2f Point, vec2f CircleCenter, float Radius)
	{
		vec2f Delta = Point - CircleCenter;
		return Delta.lengthSquared() <= Radius * Radius;
	}

	// -------------------------------------------------------------------------
	// Window / Screen Helpers
	// -------------------------------------------------------------------------
	
	inline bool IsOutsideWindow(vec2f Point)
	{
		return Point.x < 0 || Point.y < 0 ||
		       Point.x >= EC.RenderResolution.x ||
		       Point.y >= EC.RenderResolution.y;
	}

	inline bool IsOutsideWindowLeft(const shape& Bounds)
	{
		return Bounds.getPosition().x < Bounds.getGlobalBounds().size.x / 2.f;
	}

	inline bool IsOutsideWindowRight(const shape& Bounds)
	{
		return Bounds.getPosition().x > EC.RenderResolution.x - Bounds.getGlobalBounds().size.x / 2.f;
	}

	inline bool IsOutsideWindowTop(const shape& Bounds)
	{
		return Bounds.getPosition().y < Bounds.getGlobalBounds().size.y / 2.f;
	}

	inline bool IsOutsideWindowBottom(const shape& Bounds)
	{
		return Bounds.getPosition().y > EC.RenderResolution.y - Bounds.getGlobalBounds().size.y / 2.f;
	}

	// -------------------------------------------------------------------------
	// Shape Intersection
	// -------------------------------------------------------------------------
	
	inline optional<sf::FloatRect> Intersects(const shape& ObjectA, const shape& ObjectB)
	{
		return ObjectA.getGlobalBounds().findIntersection(ObjectB.getGlobalBounds());
	}

	inline bool Contains(const shape& Shape, vec2f Point)
	{
		return Shape.getGlobalBounds().contains(Point);
	}

	inline bool Contains(const circle& Circle, vec2f Point)
	{
		vec2f Delta = Circle.getPosition() - Point;
		return Delta.lengthSquared() <= Circle.getRadius() * Circle.getRadius();
	}

	// -------------------------------------------------------------------------
	// Distance
	// -------------------------------------------------------------------------
	
	inline float DistanceSquared(vec2f PointA, vec2f PointB)
	{
		return (PointA - PointB).lengthSquared();
	}

	inline float Distance(vec2f PointA, vec2f PointB)
	{
		return (PointA - PointB).length();
	}
}
