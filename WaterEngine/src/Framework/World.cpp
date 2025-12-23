#include "Framework/World.h"
#include "Framework/Core.h"
#include "Framework/Actor.h"
#include "Framework/Application.h"
#include "Framework/Renderer.h"
#include "GameMode/Level.h"
#include "UI/HUD.h"

namespace we
{
	World::World(Application* OwningApp)
		: OwningApp{ OwningApp }
		, Actors{}
		, PendingActors{}
		, bHasBegunPlay{ false }
		, Levels{}
		, CurrentLevel{Levels.end()}
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
			StartLevels();
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

		if (CurrentLevel != Levels.end())
		{
			CurrentLevel->get()->TickLevel(DeltaTime);
		}

		if (GameHUD->IsInitialized())
		{
			GameHUD->Tick(DeltaTime);
		}

		Tick(DeltaTime);
	}

	void World::BeginPlay()
	{
	}

	void World::Tick(float DeltaTime)
	{

	}

	void World::Render(Renderer& GameRenderer)
	{
		if (GameHUD && !GameHUD->IsInitialized())
		{
			GameHUD->NativeInitialize(GameRenderer);
		}

		for (auto& Actor : Actors)
		{
			Actor->Render(GameRenderer);
		}

		RenderHUD(GameRenderer);
	}

	void World::RenderHUD(Renderer& GameRenderer)
	{
		if (GameHUD)
		{
			GameHUD->Render(GameRenderer);
		}
	}

	void World::InitLevels()
	{
	}

	void World::EndLevels()
	{
		LOG("All Levels Complete!")
	}

	void World::LoadNextLevel()
	{
		CurrentLevel = Levels.erase(CurrentLevel);
		if (CurrentLevel != Levels.end())
		{
			CurrentLevel->get()->BeginLevel();
			CurrentLevel->get()->OnLevelEnd.Bind(GetObject(), &World::LoadNextLevel);
		}
		else
		{
			EndLevels();
		}
	}

	void World::StartLevels()
	{
		CurrentLevel = Levels.begin();
		if (CurrentLevel != Levels.end())
		{
			CurrentLevel->get()->BeginLevel();
			CurrentLevel->get()->OnLevelEnd.Bind(GetObject(), &World::LoadNextLevel);
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
	}

	void World::AddLevel(const shared<Level>& NewLevel)
	{
		Levels.push_back(NewLevel);
	}

	bool World::DispatchEvent(const optional<sf::Event> Event)
	{
		if (GameHUD)
		{
			return GameHUD->HandleEvent(Event);
		}
		return false;
	}

	sf::Vector2u World::GetWindowSize() const
	{
		return OwningApp->GetWindowSize();
	}
}