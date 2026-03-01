// =============================================================================
// DemoGame Configuration
// Simple header-only game settings (no INI parsing needed)
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	// =============================================================================
	// Game Settings - Edit these values directly for your game
	// =============================================================================
	struct GameConfig
	{
		// Basic Game Info
		static constexpr const char* Name = "Demo Game";
		static constexpr const char* Version = "1.0.0";
		static constexpr const char* StartupLevel = "MainMenu";

		// Physics Settings (top-down 2D = no gravity)
		static constexpr vec2f Gravity = { 0.0f, 0.0f };
		
		// Add more game-specific settings here as needed:
		// static constexpr float PlayerMoveSpeed = 200.0f;
		// static constexpr float PlayerJumpForce = 500.0f;
	};
}
