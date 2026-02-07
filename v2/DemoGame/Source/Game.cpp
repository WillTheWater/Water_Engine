// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"
#include "Framework/World/World.h"
#include "Input/InputActions.h"
#include "GameStates.h"
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
		, bPaused{false}
	{
		Subsystem.GameState->OnStateEnter.Bind(this, &Game::OnStateEnter);
		Subsystem.GameState->OnStateExit.Bind(this, &Game::OnStateExit);

		Subsystem.GameState->RequestStateChange(MakeState(EGameState::MainMenu));

		BindInput();

		PauseMenu = make_unique<PauseUI>(Subsystem);
		PauseMenu->OnResume.Bind(this, &Game::TogglePause);
	}

	void Game::Tick(float DeltaTime)
	{
	}

	void Game::TogglePause()
	{
		bPaused = !Subsystem.Time->IsPaused();
		Subsystem.Time->SetPaused(bPaused);

		if (bPaused)
		{
			PauseMenu->Show();

			auto Current = Subsystem.GameState->GetCurrentState();
			if (auto GameState = Current->As<GameStateToken>())
			{
				if (GameState->Is(EGameState::LevelOne))
				{
					Subsystem.Cursor->SetVisibility(true);
				}
			}
		}
		else
		{
			PauseMenu->Hide();

			auto Current = Subsystem.GameState->GetCurrentState();
			if (auto GameState = Current->As<GameStateToken>())
			{
				if (GameState->Is(EGameState::LevelOne))
				{
					Subsystem.Cursor->SetVisibility(false);
				}
			}
		}
	}

	void Game::OnStateEnter(shared<IGameStateToken> NewState)
	{
		auto GameState = NewState->As<GameStateToken>();
		if (!GameState)
		{
			ERROR("Invalid state token received");
			return;
		}

		LOG("Entering state: {}", GameState->GetDebugName());

		Subsystem.World->UnloadWorld();

		switch (GameState->GetState())
		{
		case EGameState::MainMenu:
			Subsystem.World->LoadWorld<MainMenu>();
			break;

		case EGameState::LevelOne:
			Subsystem.World->LoadWorld<LevelOne>();
			break;

		default:
			break;
		}
	}

	void Game::OnStateExit(shared<IGameStateToken> OldState)
	{
		auto GameState = OldState->As<GameStateToken>();
		if (GameState)
		{
			LOG("Exiting state: {}", GameState->GetDebugName());
		}
	}

	void Game::BindInput()
	{
		Subsystem.Input->Bind(ACTION_TOGGLE_PAUSE, Input::Keyboard{ sf::Keyboard::Scan::Escape });
		Subsystem.Input->Bind(ACTION_TOGGLE_PAUSE, Input::Gamepad{GamepadButton::Start});

		Subsystem.Input->OnPressed(ACTION_TOGGLE_PAUSE, [this](){ TogglePause(); });
	}
}