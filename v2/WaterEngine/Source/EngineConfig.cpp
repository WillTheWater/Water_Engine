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
			WindowName = Config["WindowName"];
		}
	}
}