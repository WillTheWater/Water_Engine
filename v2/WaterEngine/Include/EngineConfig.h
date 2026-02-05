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

		string AssetDirectory;
		vec2f WindowSize;
		vec2f AspectRatio;
		vec2f WindowMinimumSize;
		float TargetFPS;
		bool VsyncEnabled;
		bool FullscreenMode;
		bool SetRenderSmooth;
		string WindowName;
		bool DisableSFMLLogs;
		sf::Time MaxDeltaTime;
		string WindowIcon;
		string DefaultBackground;
		string DefaultCursor;
		float StartupGlobalVolume;
		bool EnableKeyRepeat;
		vec2f DefaultCursorSize;
		float DefaultCursorSpeed;
		float JoystickDeadzone;
		string DefaultTitleFont;
		string DefaultFont;
		string DefaultPanel;
		string DefaultButton;
		string DefaultMusic;
		string DefaultAmbient;
		string DefaultButtonHoverSound;
		string DefaultButtonClickSound;
		size_t MaxSFXStack;
	};

	// Global Accessor
	inline const EngineConfig EC;
}