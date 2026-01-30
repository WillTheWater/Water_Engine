// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

// =============================================================================
// Used to set global engine configurations
// DO NOT EDIT HERE (EDIT: EngineConfig.json)
// =============================================================================

#include <nlohmann/json.hpp>

#include "EngineConfig.h"

namespace we
{
	EngineConfig::EngineConfig()
	{
		filestream File("Content/Config/EngineConfig.json");
		if (File.is_open())
		{
			json Config = json::parse(File);

			WindowSize = { Config["WindowSize"][0], Config["WindowSize"][1] };
			AspectRatio = { Config["AspectRatio"][0], Config["AspectRatio"][1] };
			WindowMinimumSize = { Config["WindowMinimumSize"][0], Config["WindowMinimumSize"][1] };
			FullscreenMode = Config["FullscreenMode"];
			SetRenderSmooth = Config["SetRenderSmooth"];
			WindowName = Config["WindowName"];
			DisableSFMLLogs = Config["DisableSFMLLogs"];
			MaxDeltaTime = sf::seconds(Config["MaxDeltaTime"]);
			WindowIcon = Config["WindowIcon"];
			DefaultBackground = Config["DefaultBackground"];
		}
	}
}