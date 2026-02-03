// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/World.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	World::World(EngineSubsystem& Subsystem)
		: Subsystem{ Subsystem }
		, Actors{}
		, PendingActors{}
		, bHasBegunPlay{false}
	{

	}

	World::~World()
	{
		Actors.clear();
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
		for (auto& Actor : PendingActors)
		{
			Actors.push_back(Actor);
			Actor->BeginPlayGlobal();
		}
		PendingActors.clear();

		for (auto i = Actors.begin(); i != Actors.end();)
		{
			if (i->get()->IsPendingDestroy())
			{
				i = Actors.erase(i);
				LOG("Actor Erased")
			}
			else
			{
				i->get()->Tick(DeltaTime);
				++i;
			}
		}
	}

	void World::RenderActors()
	{
		for (auto& A : Actors)
		{
			if (A->HasSprite())
			{
				Subsystem.Render->Draw(A->GetSprite());
			}
		}
	}

	void World::BeginPlay()
	{
	}

	void World::Tick(float DeltaTime)
	{
	}

	void World::Render()
	{
		RenderActors();
	}
}