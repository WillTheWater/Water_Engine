// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "box2d/b2_math.h"

// =============================================================================
// Used to set global engine configurations
// =============================================================================

namespace we
{
	struct EngineConfig
	{
		EngineConfig();

		string AssetDirectory;
		vec2f RenderResolution;
		vec2f AspectRatio;
		vec2f WindowMinimumSize;
		string WindowIcon;
		string WindowName;
		bool EnableKeyRepeat;
		
		string DefaultCursor;
		vec2f DefaultCursorSize;
		string DefaultFont;

		bool VsyncEnabled;
		float TargetFPS;
		time MaxDeltaTime;
		bool FullscreenMode;
		bool SetRenderSmooth;
		bool DisableSFMLLogs;
		float StartupGlobalVolume;
		ulong MaxSFXStack;

		float JoystickDeadzone;
		float DefaultCursorSpeed;

		b2Vec2 DefaultGravity;
	};

	// Global Accessor
	inline const EngineConfig EC;
}