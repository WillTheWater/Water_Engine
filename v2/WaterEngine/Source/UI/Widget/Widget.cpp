// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Widget.h"

namespace we
{
	bool Widget::Contains(const vec2f& Point) const
	{
		float HalfWidth = Size.x / 2.f;
		float HalfHeight = Size.y / 2.f;

		return Point.x >= Position.x - HalfWidth &&
			Point.x <= Position.x + HalfWidth &&
			Point.y >= Position.y - HalfHeight &&
			Point.y <= Position.y + HalfHeight;
	}
}