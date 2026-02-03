// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"
#include "Framework/World/World.h"
#include "Framework/World/Actor/Actor.h"
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
        BindInputs();

        Subsystem.GameState->OnStateEnter.Bind(this, &Game::OnStateEnterHandler);
        Subsystem.GameState->RequestStateChange(GameState::MainMenu);
    }

    void Game::BindInputs()
    {
        Subsystem.Input->Bind(static_cast<int>(GameAction::MainMenu),
            Input::Keyboard{ sf::Keyboard::Scan::Num1 });
        Subsystem.Input->Bind(static_cast<int>(GameAction::Level1),
            Input::Keyboard{ sf::Keyboard::Scan::Num2 });
    }

    void Game::Tick(float DeltaTime)
    {
        // Request state changes
        if (Subsystem.Input->IsJustPressed(static_cast<int>(GameAction::MainMenu)))
        {
            Subsystem.GameState->RequestStateChange(GameState::MainMenu);
        }
        else if (Subsystem.Input->IsJustPressed(static_cast<int>(GameAction::Level1)))
        {
            Subsystem.GameState->RequestStateChange(GameState::Level1);
        }
    }

    void Game::OnStateEnterHandler()
    {
        auto State = Subsystem.GameState->GetCurrentState();

        Subsystem.World->UnloadWorld();
        TestActor.reset();

        switch (State)
        {
        case GameState::MainMenu:
            LOG("Main Menu");
            Subsystem.World->LoadWorld<World>();
            break;
        case GameState::Level1:
            LOG("Level 1");
            {
                auto NewWorld = Subsystem.World->LoadWorld<World>();
                TestActor = NewWorld.lock()->SpawnActor<Actor>();
                TestActor.lock()->CenterOrigin();
                TestActor.lock()->SetPosition({ EC.WindowSize.x / 2, EC.WindowSize.y / 2 });
            }
            break;
        default:
            break;
        }
    }
}