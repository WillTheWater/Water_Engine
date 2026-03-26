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

	// =============================================================================
	// Save Keys
	// =============================================================================
	inline constexpr stringView SAVE_TIMES_PLAYED = "DemoGame:TimesPlayed";
	inline constexpr stringView SAVE_LAST_LEVEL = "DemoGame:LastLevel";
	inline constexpr stringView SAVE_PLAYER_POS_X = "DemoGame:PlayerPosX";
	inline constexpr stringView SAVE_PLAYER_POS_Y = "DemoGame:PlayerPosY";
	inline constexpr stringView SAVE_OBSTACLE_POS_X = "DemoGame:ObstaclePosX";
	inline constexpr stringView SAVE_OBSTACLE_POS_Y = "DemoGame:ObstaclePosY";

	// Audio Settings
	inline constexpr stringView SAVE_MASTER_VOLUME = "Settings:Audio:MasterVolume";
	inline constexpr stringView SAVE_MASTER_MUTED = "Settings:Audio:MasterMuted";
	inline constexpr stringView SAVE_MUSIC_VOLUME = "Settings:Audio:MusicVolume";
	inline constexpr stringView SAVE_MUSIC_MUTED = "Settings:Audio:MusicMuted";
	inline constexpr stringView SAVE_AMBIENT_VOLUME = "Settings:Audio:AmbientVolume";
	inline constexpr stringView SAVE_AMBIENT_MUTED = "Settings:Audio:AmbientMuted";
	inline constexpr stringView SAVE_SFX_VOLUME = "Settings:Audio:SFXVolume";
	inline constexpr stringView SAVE_SFX_MUTED = "Settings:Audio:SFXMuted";
	inline constexpr stringView SAVE_VOICE_VOLUME = "Settings:Audio:VoiceVolume";
	inline constexpr stringView SAVE_VOICE_MUTED = "Settings:Audio:VoiceMuted";
	inline constexpr stringView SAVE_UI_VOLUME = "Settings:Audio:UIVolume";
	inline constexpr stringView SAVE_UI_MUTED = "Settings:Audio:UIMuted";
	
	// Video Settings
	inline constexpr stringView SAVE_FULLSCREEN = "Settings:Video:Fullscreen";
}