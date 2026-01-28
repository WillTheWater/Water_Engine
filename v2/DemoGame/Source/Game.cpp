// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Game.h"

namespace we
{
	unique<Application> GetApplication()
	{
		return make_unique<Game>();
	}


	Game::Game()
		: Application{}
	{
	}
}