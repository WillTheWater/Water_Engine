// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/World.h"
#include "Framework/World/Actor.h"
#include "Utility/Math.h"

namespace we
{
	World::World(EngineSubsystem& InSubsystem)
		: Object()
		, Subsystem{ InSubsystem }
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

	void World::ManageActors(float DeltaTime)
	{
		// Spawn pending actors
		if (!PendingActors.empty())
		{
			bRenderOrderDirty = true;
			for (auto& NewActor : PendingActors)
			{
				Actors.push_back(NewActor);
				if (!NewActor->HasBegunPlay())
				{
					NewActor->BeginPlay();
				}
			}
			PendingActors.clear();
		}

		// Tick actors and remove destroyed
		for (auto It = Actors.begin(); It != Actors.end();)
		{
			if ((*It)->IsPendingDestroy())
			{
				It = Actors.erase(It);
				bRenderOrderDirty = true;
			}
			else
			{
				if ((*It)->HasBegunPlay())
				{
					vec2f OldPos = (*It)->GetPosition();
					(*It)->Tick(DeltaTime);
					
					// Mark dirty if actor moved in Y
					if (!FloatEquals((*It)->GetPosition().y, OldPos.y))
					{
						bRenderOrderDirty = true;
					}
				}
				++It;
			}
		}
	}

	void World::CollectRenderDepths(vector<RenderDepth>& OutDepths)
	{
		if (bRenderOrderDirty)
		{
			CachedRenderDepths.clear();

			// Add manual drawables first
			for (const auto& RD : ManualRenderDepths)
			{
				CachedRenderDepths.push_back(RD);
			}

			// Add actors
			for (const auto& Actor : Actors)
			{
				if (Actor && !Actor->IsPendingDestroy() && Actor->IsVisible())
				{
					if (const auto* Drawable = Actor->GetDrawable())
					{
						CachedRenderDepths.push_back({ Drawable, Actor->GetRenderDepth() });
					}
				}
			}

			// Sort by depth (back to front)
			std::sort(CachedRenderDepths.begin(), CachedRenderDepths.end());
			bRenderOrderDirty = false;
		}

		OutDepths = CachedRenderDepths;
	}

	void World::AddRenderDepth(const drawable* Drawable, float Depth)
	{
		ManualRenderDepths.push_back({ Drawable, Depth });
		bRenderOrderDirty = true;
	}

	void World::ClearManualRenderDepths()
	{
		if (!ManualRenderDepths.empty())
		{
			ManualRenderDepths.clear();
			bRenderOrderDirty = true;
		}
	}

	void World::Construct() {}
	void World::BeginPlay() {}
	void World::Tick(float DeltaTime) {}
}
