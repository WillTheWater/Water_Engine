// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
	class World
	{
	public:
		void ChangeLevel(stringView LevelName);
		void RestartLevel();

	private:
		string CurrentLevel;
		optional<string> NextLevel;

	private:
		friend class WaterEngine;

		optional<string> GetNextLevel();
	};
}