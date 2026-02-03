// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/World.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	World::World(EngineSubsystem& Subsystem)
		: Subsystem{Subsystem}
		, bHasBegunPlay{false}
	{

	}

	World::~World()
	{
		Actors.clear();
	}

	void World::BeginPlay()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
		}
	}

	void World::Tick(float DeltaTime)
	{
		for (auto& Actor : PendingActors)
		{
			Actors.push_back(Actor);
			Actor->BeginPlay();
		}
		PendingActors.clear();
		
		for (auto i = Actors.begin(); i != Actors.end();)
		{
			i->get()->Tick(DeltaTime);
			i++;
		}
		FlushActors();
	}

	void World::Render()
	{
		for (const auto& Actor : Actors)
		{
			Subsystem.Render->Draw(Actor->GetSprite());
		}
	}

	void World::FlushActors()
	{
		for (auto i = Actors.begin(); i != Actors.end();)
		{
			if (i->get()->IsPendingDestroy())
			{
				i = Actors.erase(i);
			}
			else
			{
				i++;
			}
		}
	}
}