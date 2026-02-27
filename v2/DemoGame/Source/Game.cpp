// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"
#include "Entry.h"
#include "Levels/MainMenu.h"

namespace we
{
	unique<WaterEngine> GetEngine()
	{
		return make_unique<Game>();
	}

	Game::Game()
		: WaterEngine{}
	{
	}

	void Game::Initialize()
	{
		// Initialize base engine
		WaterEngine::Initialize();
		
		// Load MainMenu world
		LoadWorld<MainMenu>();
	}
}
