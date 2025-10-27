#include "GameFramework/Game.h"
#include "Framework/World.h"

we::Application* GetApplication()
{
	return new we::Game{};
}

namespace we
{
	Game::Game()
	{
		LoadWorld<World>();
	}
}