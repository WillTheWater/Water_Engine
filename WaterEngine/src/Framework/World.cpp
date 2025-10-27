#include "Framework/World.h"
#include "Framework/Core.h"

namespace we
{
	World::World(Application* OwningApp)
		: OwningApp {OwningApp},
		bHasBegunPlay {false}
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
		Tick(DeltaTime);
	}

	void World::BeginPlay()
	{
		LOG("BeginPlay Called")
	}

	void World::Tick(float DeltaTime)
	{
		LOG("Ticking: %f", 1.f/DeltaTime)
	}
}