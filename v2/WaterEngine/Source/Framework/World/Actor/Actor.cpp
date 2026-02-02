// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Actor.h"

namespace we
{
	Actor::Actor(World* OwningWorld, const string& TexturePath)
		: OwnerWorld{OwningWorld}
		, Texture{}
		, Sprite{Texture}
		, bHasBegunPlay{false}
	{
		//SetTexture(TexturePath);
	}

	void Actor::BeginPlay()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
		}
	}

	void Actor::Tick(float DeltaTime)
	{
	}
}