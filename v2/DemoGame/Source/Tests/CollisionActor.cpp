// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Tests/CollisionActor.h"
#include "Component/CollisionComponent.h"

namespace we
{
	CollisionActor::CollisionActor(World& OwningWorld)
		: Actor(OwningWorld)
	{
	}

	CollisionActor::~CollisionActor() = default;

	void CollisionActor::BeginPlay()
	{
		Actor::BeginPlay();

		CollisionComp = make_shared<CollisionComponent>(this);
		CollisionComp->SetRadius(64.0f);
		CollisionComp->BeginPlay();
		CollisionComp->DrawDebug();
	}

	void CollisionActor::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);

		if (CollisionComp)
		{
			CollisionComp->Tick(DeltaTime);
		}
	}

	void CollisionActor::EndPlay()
	{
		if (CollisionComp)
		{
			CollisionComp->EndPlay();
			CollisionComp.reset();
		}

		Actor::EndPlay();
	}

	void CollisionActor::GetDrawables(vector<const drawable*>& OutDrawables) const
	{
		Actor::GetDrawables(OutDrawables);

		if (CollisionComp)
		{
			if (const auto* Debug = CollisionComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}
	}
}
