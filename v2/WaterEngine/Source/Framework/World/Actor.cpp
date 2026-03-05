// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor.h"
#include "Utility/Log.h"

namespace we
{
	Actor::Actor(World* OwningWorld)
		: OwningWorld{OwningWorld}
		, bIsVisible{true}
		, bHasBegunPlay{false}
	{
	}

	Actor::~Actor()
	{
		LOG("Actor Destroyed")
	}

	void Actor::StartPlay()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
			BeginPlay();
		}
	}

	void Actor::StartTick(float DeltaTime)
	{
		if (!IsPendingDestroy())
		{
			Tick(DeltaTime);
		}
	}

	void Actor::BeginPlay()
	{
	}

	void Actor::Tick(float DeltaTime)
	{
	}
}