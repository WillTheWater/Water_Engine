#include "Framework/World.h"
#include "Framework/Core.h"
#include "Framework/Actor.h"
#include "Framework/Application.h"

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

		for (auto i = Actors.begin(); i != Actors.end(); )
		{
			if (i->get()->IsPendingDestroy())
			{
				i = Actors.erase(i);
			}
			else
			{
				i->get()->Tick(DeltaTime);
				i++;
			}
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

	void World::Render(sf::RenderWindow& Window)
	{
		for (auto& Actor : Actors)
		{
			Actor->Render(Window);
		}
	}

	sf::Vector2u World::GetWindowSize() const
	{
		return OwningApp->GetWindowSize();
	}
}