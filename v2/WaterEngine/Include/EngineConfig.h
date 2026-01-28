// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

// =============================================================================
// Used to set global engine configurations
// =============================================================================

namespace we
{
	struct EngineConfig
	{
		EngineConfig();

		vec2f WindowSize;
		string WindowName;
	};

	// Global Accessor
	inline const EngineConfig EC;
}