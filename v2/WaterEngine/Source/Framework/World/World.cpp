// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/World.h"
#include "Framework/World/Actor.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	World::World(EngineSubsystem& InSubsystem)
		: Subsystem{ InSubsystem }
	{
	}

	World::~World() = default;

	void World::Construct()
	{
	}

	void World::BeginPlay()
	{
	}

	void World::Tick(float DeltaTime)
	{
	}

	void World::BeginPlayGlobal()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
			BeginPlay();
		}
	}

	void World::TickGlobal(float DeltaTime)
	{
		ManageActors(DeltaTime);
		Tick(DeltaTime);
	}

	void World::ManageActors(float DeltaTime)
	{
		// Spawn pending actors
		if (!PendingActors.empty())
		{
			for (auto& NewActor : PendingActors)
			{
				Actors.push_back(NewActor);
				if (!NewActor->HasBegunPlay())
				{
					NewActor->BeginPlay();
				}
			}
			PendingActors.clear();
		}

		// Tick actors and remove destroyed
		for (auto It = Actors.begin(); It != Actors.end();)
		{
			if ((*It)->IsPendingDestroy())
			{
				It = Actors.erase(It);
			}
			else
			{
				if ((*It)->HasBegunPlay())
				{
					(*It)->Tick(DeltaTime);
				}
				++It;
			}
		}
	}
}
