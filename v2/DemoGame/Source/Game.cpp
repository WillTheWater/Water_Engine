// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"
#include "Framework/World/World.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/GameStateSubsystem.h"
#include "Input/InputActions.h"
#include "GameStateTokens.h"
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
		, bPaused{ false }
	{
		Construct();
	}

	void Game::Construct()
	{
		RegisterLevels();

		Subsystem.GameState->OnStateEnter.Bind(this, &Game::OnStateEnter);
		Subsystem.GameState->OnStateExit.Bind(this, &Game::OnStateExit);
	}

	void Game::BeginPlay()
	{
		Subsystem.GameState->RequestStateChange(MakeState(EGameState::MainMenu));
		BindInput();
	}

	void Game::Tick(float DeltaTime)
	{
		
	}

	void Game::RegisterLevels()
	{
		RegisterLevel(MainMenu);
		RegisterLevel(LevelOne);
	}

	void Game::OnStateEnter(shared<IGameStateToken> NewState)
	{
		Subsystem.World->LoadWorldForState(NewState.get());
		LOG("Entered state: {}", NewState->GetDebugName());
	}

	void Game::OnStateExit(shared<IGameStateToken> OldState)
	{
		if (OldState)
		{
			LOG("Exited state: {}", OldState->GetDebugName());
		}
	}

	void Game::BindInput()
	{
	
	}

	void Game::TogglePause()
	{
		bPaused = !Subsystem.Time->IsPaused();
		Subsystem.Time->SetPaused(bPaused);
	}
}