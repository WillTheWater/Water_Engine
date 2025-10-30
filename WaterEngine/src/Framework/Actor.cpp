﻿// Copyright (C) 2025 Stoic Ronin Studio. All Rights Reserved.

#include "Framework/Actor.h"
#include "Framework/Core.h"

namespace we
{
	Actor::Actor(World* OwningWorld)
		: OwningWorld{OwningWorld},
		bHasBegunPlay{false}
	{

	}

	void Actor::BeginPlayGlobal()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
			BeginPlay();
		}
	}

	void Actor::BeginPlay();
	{
		LOG("Actor BeginPlay Called!")
	}

	void Actor::Tick(float DeltaTime)
	{
		LOG("Actor Ticking")
	}
}