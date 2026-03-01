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
		
		Subsystem.Physics->SetGravity(GameConfig::Gravity);
		
		Subsystem.World->LoadWorld<MainMenu>();
	}
}
