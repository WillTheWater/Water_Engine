// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WorldSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	void World::ChangeLevel(stringView LevelName)
	{
		if (!NextLevel) { NextLevel = LevelName; }
	}

	void World::RestartLevel()
	{
		ChangeLevel(CurrentLevel);
	}

	optional<string> World::GetNextLevel()
	{
		if (NextLevel)
		{
			if (CurrentLevel == *NextLevel)
			{
				LOG("Reloading Level: {}", CurrentLevel)
			}
			else
			{
				LOG("Loading Next Level: {}", *NextLevel)
			}
			CurrentLevel = *NextLevel;
			return std::exchange(NextLevel, std::nullopt);
		}
		return std::nullopt;
	}
}