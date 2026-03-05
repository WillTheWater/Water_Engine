// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/World.h"
#include "Framework/World/Actor.h"

namespace we
{
	World::World()
		: bHasBegunPlay{false}
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

	void World::PreConstruct()
	{

	}

	void World::BeginPlay()
	{

	}

	void World::EndPlay()
	{
	}

	void  World::Tick(float DeltaTime)
	{

	}

	void World::GarbageCollection()
	{
		for (auto i = Actors.begin(); i != Actors.end();)
		{
			if (i->get()->IsPendingDestroy())
			{
				i = Actors.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
}