// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	// =========================================================================
	// IPostProcessEffect - Interface for post-processing effects
	// =========================================================================
	class IPostProcessEffect
	{
	public:
		virtual ~IPostProcessEffect() = default;

		// Apply effect from input texture to output target
		// Input: source texture (already displayed)
		// Output: render target to draw to
		virtual void Apply(const texture& Input, renderTarget& Output) = 0;
	};
}
