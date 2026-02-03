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
		auto NewWorld = LoadWorld<World>();
		auto TestWorld = NewWorld.lock();
		TestActor = TestWorld->SpawnActor<Actor>();
	}

	void Game::Tick(float DeltaTime)
	{
		static float Time = 3.f;
		if (Subsystem.Time->GetElapsedTime() > Time)
		{
			if (!TestActor.expired())
			{
				LOG("Calling Actor Destruction!");
				TestActor.lock()->Destroy();
			}
		}
	}
}