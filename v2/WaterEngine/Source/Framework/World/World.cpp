// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/World.h"
#include "Framework/World/Actor.h"

namespace we
{
	World::World(WorldSubsystem& Subsystem)
		: Subsystem{Subsystem}
		, bHasBegunPlay{false}
		, PendingActors{}
		, Actors{}
	{

	}

	World::~World()
	{
	}

	void World::StartPlay()
	{
		if (!bHasBegunPlay)
		{
			BeginPlay();
			bHasBegunPlay = true;
		}
	}


	void World::StartTick(float DeltaTime)
	{
		for (auto& A : PendingActors)
		{
			Actors.push_back(A);
			A->StartPlay();
		}

		PendingActors.clear();

		for (auto i = Actors.begin(); i != Actors.end();)
		{
			i->get()->StartTick(DeltaTime);
			++i;
		}

		if (!IsPendingDestroy())
		{
			Tick(DeltaTime);
		}
	}

	void World::EndingPlay()
	{
		// Call EndPlay on all actors before world ends
		for (auto& Actor : Actors)
		{
			if (Actor->HasBegunPlay())
			{
				Actor->EndPlay();
			}
		}
		EndPlay();
	}

	void World::GarbageCollection()
	{
		for (auto i = Actors.begin(); i != Actors.end();)
		{
			if (i->get()->IsPendingDestroy())
			{
				// Call EndPlay before destroying actor
				if (i->get()->HasBegunPlay())
				{
					i->get()->EndPlay();
				}
				i = Actors.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
}
