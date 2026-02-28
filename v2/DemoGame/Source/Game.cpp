// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"
#include "Entry.h"
#include "Levels/MainMenu.h"
#include "Utility/Log.h"

namespace we
{
	unique<WaterEngine> GetEngine()
	{
		return make_unique<Game>();
	}

	Game::Game()
		: WaterEngine{}
	{
		Construct();
	}

	void Game::Construct()
	{
		LOG("Game::Construct - START");
		// Load MainMenu world
		Subsystem.World->LoadWorld<MainMenu>();
		LOG("Game::Construct - END");
	}
}
