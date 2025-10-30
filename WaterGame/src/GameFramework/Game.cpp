﻿// Copyright (C) 2025 Stoic Ronin Studio. All Rights Reserved.

#include "GameFramework/Game.h"
#include "Framework/World.h"
#include "Framework/Actor.h"

we::Application* GetApplication()
{
	return new we::Game{};
}

namespace we
{
	Game::Game()
	{
		weak<World> NewWorld = LoadWorld<World>();
		NewWorld.lock()->SpawnActor<Actor>();
	}
}