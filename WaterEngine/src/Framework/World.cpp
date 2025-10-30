#include "Framework/World.h"
#include "Framework/Core.h"
#include "Framework/Actor.h"

namespace we
{
	World::World(Application* OwningApp)
		: OwningApp{OwningApp},
		Actors{},
		PendingActors{},
		bHasBegunPlay{false}
	{
	}

	World::~World()
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
		for (auto Actor : PendingActors)
		{
			Actors.push_back(Actor);
			Actor->BeginPlayGlobal();
		}

		PendingActors.clear();

		for (auto Actor : Actors)
		{
			Actor->Tick(DeltaTime);
		}
		Tick(DeltaTime);
	}

	void World::BeginPlay()
	{
		LOG("World BeginPlay Called")
	}

	void World::Tick(float DeltaTime)
	{
	
	}
}