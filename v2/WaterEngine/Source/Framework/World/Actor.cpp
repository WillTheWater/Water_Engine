// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor.h"
#include "Framework/World/World.h"

namespace we
{
	Actor::Actor(World* InWorld)
		: OwnerWorld{ InWorld }
	{
	}

	Actor::~Actor() = default;

	void Actor::BeginPlay()
	{
		bHasBegunPlay = true;
	}

	void Actor::Tick(float DeltaTime)
	{
	}

	void Actor::Destroy()
	{
		bPendingDestroy = true;
	}

	void Actor::SetPosition(const vec2f& Pos)
	{
		Position = Pos;
	}
}
