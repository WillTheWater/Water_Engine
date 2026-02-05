// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"
#include "Framework/World/World.h"
#include "Input/InputActions.h"
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
		BindInput();

		// Create pause menu (starts hidden)
		PauseMenu = make_unique<PauseUI>(Subsystem);
		PauseMenu->OnResume.Bind(this, &Game::TogglePause);
	}

	void Game::Tick(float DeltaTime)
	{
		if (Subsystem.Input->IsJustPressed(ACTION_TOGGLE_PAUSE))
		{
			TogglePause();
		}
	}

	void Game::TogglePause()
	{
		SetPaused(!IsPaused());

		if (IsPaused())
		{
			PauseMenu->Show();
		}
		else
		{
			PauseMenu->Hide();
		}
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

	void Game::BindInput()
	{
		Subsystem.Input->Bind(ACTION_TOGGLE_PAUSE, Input::Keyboard{ sf::Keyboard::Scan::Escape });
	}
}