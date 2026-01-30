// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

#include <SFML/System/Time.hpp>

// =============================================================================
// Used to set global engine configurations
// =============================================================================

namespace we
{
	struct EngineConfig
	{
		EngineConfig();

		vec2f WindowSize;
		vec2f AspectRatio;
		vec2f WindowMinimumSize;
		bool FullscreenMode;
		bool SetRenderSmooth;
		string WindowName;
		bool DisableSFMLLogs;
		sf::Time MaxDeltaTime;
		string WindowIcon;
		string DefaultBackground;
	};

	// Global Accessor
	inline const EngineConfig EC;
}