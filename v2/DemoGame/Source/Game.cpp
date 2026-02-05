// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"
#include "Framework/World/World.h"
#include "Utility/Log.h"

// ========================= LEVELS =========================
#include "Levels/MainMenu.h"
#include "Levels/LevelOne.h"

namespace we
{
	unique<WaterEngine> GetEngine()
	{
		return make_unique<Game>();
	}

	Game::Game()
		: WaterEngine{}
	{
		Subsystem.GameState->OnStateEnter.Bind(this, &Game::OnStateEnter);
		Subsystem.GameState->RequestStateChange(GameState::MainMenu);
	}

	void Game::OnStateEnter()
	{
		Subsystem.World->UnloadWorld();

		switch (Subsystem.GameState->GetCurrentState())
		{
		case GameState::MainMenu:
			LOG("Main Menu");
			Subsystem.World->LoadWorld<MainMenu>();
			break;

		case GameState::Level1:
			LOG("Level 1");
			Subsystem.World->LoadWorld<LevelOne>();
			break;

		default:
			break;
		}
	}
}