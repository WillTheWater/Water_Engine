// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"
#include "Entry.h"
#include "Config/GameConfig.h"
#include "Utility/Log.h"
#include "DemoGameInstance.h"

#include "Levels/MainMenu.h"

namespace we
{
	unique<WaterEngine> GetEngine()
	{
		return make_unique<Game>();
	}

	shared<GameInstance> Game::CreateGameInstance()
	{
		return make_shared<DemoGameInstance>();
	}

	Game::Game()
		: WaterEngine{}
	{
	}

	void Game::StartPlay()
	{
		WaterEngine::StartPlay();
		Subsystem.World->CreateWorld<MainMenu>();
	}

}