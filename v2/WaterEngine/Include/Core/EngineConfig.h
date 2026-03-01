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
			vec2u DefaultSize = { 1920, 1080 };
			vec2u MinimumSize = { 1280, 720 };
			bool Fullscreen = false;
			bool VSync = true;
			uint TargetFPS = 120;
		} Window;

		// Render Settings
		struct RenderConfig
		{
			uint ResolutionX = 1920;
			uint ResolutionY = 1080;
			bool WorldSmooth = true;
			bool WorldUISmooth = false;
			bool ScreenUISmooth = false;
			bool CursorSmooth = false;
		} Render;

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
			vec2f Gravity = { 0.f, 9.8f };
			float PhysicsScale = 0.01f;
			uint VelocityIterations = 8;
			uint PositionIterations = 3;
			float TimeStep = 0.016f;
		} Physics;

		// Audio Settings
		struct AudioConfig
		{
			float MasterVolume = 0.4f;
			float MusicVolume = 0.8f;
			float AmbientVolume = 0.8f;
			float SFXVolume = 0.8f;
			float VoiceVolume = 1.0f;
			float UIVolume = 1.0f;
			uint MaxSFXInstances = 32;
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
