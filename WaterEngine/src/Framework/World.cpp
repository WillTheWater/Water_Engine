#include "Framework/World.h"
#include "Framework/Core.h"
#include "Framework/Actor.h"
#include "Framework/Application.h"
#include "Framework/Renderer.h"
#include "GameMode/Level.h"

namespace we
{
	World::World(Application* OwningApp)
		: OwningApp{ OwningApp }
		, Actors{}
		, PendingActors{}
		, bHasBegunPlay{ false }
		, Levels{}
		, CurrentLevelIndex{-1}
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
			InitLevels();
			LoadNextLevel();
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
			i->get()->Tick(DeltaTime);
			i++;
		}

		if (CurrentLevelIndex >= 0 && CurrentLevelIndex < Levels.size())
		{
			Levels[CurrentLevelIndex]->TickLevel(DeltaTime);
		}

		Tick(DeltaTime);
	}

	void World::BeginPlay()
	{
	}

	void World::Tick(float DeltaTime)
	{

	}

	void World::InitLevels()
	{
	}

	void World::EndLevels()
	{
	}

	void World::LoadNextLevel()
	{
		CurrentLevelIndex++;
		if (CurrentLevelIndex >= 0 && CurrentLevelIndex < Levels.size())
		{
			Levels[CurrentLevelIndex]->OnLevelEnd.Bind(GetObject(), &World::LoadNextLevel);
			Levels[CurrentLevelIndex]->BeginLevel();
		}
		else
		{
			EndLevels();
			LOG("Next Level Failed To Load")
		}
	}

	void World::Render(Renderer& GameRenderer)
	{
		for (auto& Actor : Actors)
		{
			Actor->Render(GameRenderer);
		}
	}

	void World::GarbageCollectionCycle()
	{
		for (auto i = Actors.begin(); i != Actors.end(); )
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

		for (auto i = Levels.begin(); i != Levels.end();)
		{
			if (i->get()->IsLevelFinished())
			{
				i = Levels.erase(i);
			}
			else
			{
				i++;
			}
		}
	}

	void World::AddLevel(const shared<Level>& NewLevel)
	{
		Levels.push_back(NewLevel);
	}

	sf::Vector2u World::GetWindowSize() const
	{
		return OwningApp->GetWindowSize();
	}
}