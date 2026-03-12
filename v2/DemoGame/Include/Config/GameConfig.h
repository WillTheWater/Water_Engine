// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	// =============================================================================
	// Game Settings
	// =============================================================================
	struct GameConfig
	{
		// Basic Game Info
		static constexpr const char* Name = "Demo Game";

		// Physics Settings (top-down 2D = no gravity)
		static constexpr vec2f Gravity = { 0.0f, 0.0f };	
	};
}