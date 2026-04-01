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

	Actor* World::FindActor(ActorID ID) const
	{
		auto It = ActorByID.find(ID);
		if (It != ActorByID.end())
		{
			return It->second.get();
		}
		return nullptr;
	}

	void World::RegisterActor(ActorID ID, shared<Actor> Actor)
	{
		ActorByID[ID] = Actor;
	}

	void World::UnregisterActor(ActorID ID)
	{
		ActorByID.erase(ID);
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
			RegisterActor(A->GetID(), A);
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
				if (i->get()->HasBegunPlay())
				{
					i->get()->EndPlay();
				}
				UnregisterActor(i->get()->GetID());
				i = Actors.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
}