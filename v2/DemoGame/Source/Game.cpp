// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"
#include "Entry.h"
#include "Levels/MainMenu.h"
#include "Config/GameConfig.h"
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
		LOG("Game: {} v{}", GameConfig::Name, GameConfig::Version);
		
		// Set physics gravity from game config (top-down = no gravity)
		if (Subsystem.Physics)
		{
			Subsystem.Physics->SetGravity(GameConfig::Gravity);
			LOG("Game::Construct - Set gravity to [{}, {}]", 
				GameConfig::Gravity.x, 
				GameConfig::Gravity.y);
		}
		
		// Load MainMenu world
		Subsystem.World->LoadWorld<MainMenu>();
		LOG("Game::Construct - END");
	}
}
