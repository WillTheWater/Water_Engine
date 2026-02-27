// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <cmath>
#include <limits>

namespace we
{
	// Epsilon comparison for floats
	inline bool FloatEquals(float a, float b, float Epsilon = std::numeric_limits<float>::epsilon())
	{
		return std::abs(a - b) < Epsilon;
	}

	// Linear interpolation
	inline float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	// Vector lerp
	inline vec2f LerpVector(const vec2f& a, const vec2f& b, float t)
	{
		return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t) };
	}

	// Clamp value between min and max
	template<typename T>
	inline T Clamp(T value, T minVal, T maxVal)
	{
		if (value < minVal) return minVal;
		if (value > maxVal) return maxVal;
		return value;
	}
}
