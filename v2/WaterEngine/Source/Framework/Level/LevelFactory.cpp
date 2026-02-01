// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/Level/LevelFactory.h"

namespace we
{
	namespace LF
	{
		Levels CreateLevels(EngineSubsystem& Subsystem)
		{
			Levels LevelsDictionary;

			// List of Levels
			// LevelsDictionary.emplace("MainMenu", make_unique<MainMenu::Level>(Subsystem));

			return LevelsDictionary;
		}
	}
}