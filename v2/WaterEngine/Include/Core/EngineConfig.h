// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Interface/IAssetDirector.h"

namespace we
{
	// =============================================================================
	// Engine Configuration - Loaded from EngineConfig.ini
	// =============================================================================
	struct EngineConfig
	{
		// Window Settings
		struct WindowConfig
		{
			string Title = "Water Engine";
			uint Width = 1920;
			uint Height = 1080;
			bool Fullscreen = false;
			bool VSync = true;
			float TargetFPS = 60.0f;
		} Window;

		// Path Settings
		struct PathConfig
		{
			string AssetDirectory = "Content";
			string DefaultFont = "Fonts/Default.ttf";
		} Paths;

		// Debug Settings
		struct DebugConfig
		{
			bool EnableHotReload = true;
			bool EnableConsole = true;
			string LogLevel = "debug";
		} Debug;

		// Physics Settings
		struct PhysicsConfig
		{
			float GravityX = 0.0f;
			float GravityY = 9.8f;
			float TimeStep = 0.016f;
		} Physics;

		// Audio Settings
		struct AudioConfig
		{
			float GlobalVolume = 1.0f;
			float MusicVolume = 1.0f;
			float SFXVolume = 1.0f;
		} Audio;
	};

	// =============================================================================
	// Engine Config Manager - Loads and provides access to EngineConfig.ini
	// =============================================================================
	class EngineConfigManager
	{
	public:
		// Load config from EngineConfig.ini
		static bool LoadFromFile(const string& FilePath);
		
		// Load config from IAssetDirector (for Release mode with pak files)
		static bool LoadFromAssetDirector(const IAssetDirector& AssetDir, const string& FilePath);
		
		// Reload config (for hot reload)
		static bool Reload();
		
		// Access current config
		static const EngineConfig& Get() { return CurrentConfig; }
		
		static bool GetIsLoaded() { return IsLoaded; }
		static const string& GetLoadedPath() { return LoadedPath; }

	private:
		static EngineConfig CurrentConfig;
		static string LoadedPath;
		static bool IsLoaded;
	};
}
