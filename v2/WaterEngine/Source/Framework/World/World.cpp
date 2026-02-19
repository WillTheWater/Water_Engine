// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/World.h"
#include "Framework/World/Actor/Actor.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Log.h"
#include "Utility/Math.h"

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

	void World::ManageActors(float DeltaTime)
	{
		// Spawn pending
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

		// Tick and cleanup destroyed
		for (auto It = Actors.begin(); It != Actors.end();)
		{
			if ((*It)->IsPendingDestroy())
			{
				It = Actors.erase(It);
				bRenderOrderDirty = true;
			}
			else
			{
				// Only tick actors that have begun play
				if (!(*It)->HasBegunPlay())
				{
					++It;
					continue;
				}
				
				vec2f OldPos = (*It)->GetPosition();
				(*It)->Tick(DeltaTime);
				// Mark dirty if actor moved in Y (affects depth sorting)
				// Use epsilon comparison for float equality
				if (!FloatEquals((*It)->GetPosition().y, OldPos.y))
				{
					bRenderOrderDirty = true;
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

			// Add manual drawables first (usually backgrounds)
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

			// Sort by depth (Y position) - back to front
			std::sort(CachedRenderDepths.begin(), CachedRenderDepths.end());
			bRenderOrderDirty = false;
		}

		OutDepths = CachedRenderDepths;
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