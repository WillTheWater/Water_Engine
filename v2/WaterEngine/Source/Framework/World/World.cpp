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
	World::World(EngineSubsystem& InSubsystem)
		: Object()
		, Subsystem{ InSubsystem }
		, bHasBegunPlay{ false }
	{
	}

	World::~World()
	{
		Actors.clear();
		PendingActors.clear();
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

	void World::AddRenderDepth(const drawable* Drawable, float Depth)
	{
		ManualRenderDepths.push_back({ Drawable, Depth });
	}

	void World::ClearManualRenderDepths()
	{
		ManualRenderDepths.clear();
	}

	void World::ManageActors(float DeltaTime)
	{
		// Spawn pending
		for (auto& NewActor : PendingActors)
		{
			Actors.push_back(NewActor);
			NewActor->BeginPlay();
		}
		PendingActors.clear();

		// Tick and cleanup destroyed
		for (auto It = Actors.begin(); It != Actors.end();)
		{
			if ((*It)->IsPendingDestroy())
			{
				It = Actors.erase(It);
			}
			else
			{
				(*It)->Tick(DeltaTime);
				++It;
			}
		}
	}

	void World::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		// Add manual drawables first (usually backgrounds)
		for (const auto& RD : ManualRenderDepths)
		{
			OutDepths.push_back(RD);
		}

		// Add actors
		for (const auto& Actor : Actors)
		{
			if (Actor && !Actor->IsPendingDestroy() && Actor->IsVisible())
			{
				if (const auto* Drawable = Actor->GetDrawable())
				{
					OutDepths.push_back({ Drawable, Actor->GetRenderDepth() });
				}
			}
		}
	}

	void World::Construct()
	{
	}

	void World::BeginPlay()
	{
	}

	void World::Tick(float DeltaTime)
	{
	}
}