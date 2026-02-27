// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	// Render depth entry for Y-sorting (2.5D effect)
	struct RenderDepth
	{
		const drawable* Drawable = nullptr;
		float Depth = 0.0f;

		bool operator<(const RenderDepth& Other) const
		{
			return Depth < Other.Depth;  // Lower depth = further back
		}
	};
}
