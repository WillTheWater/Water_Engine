
#include "Framework/Actor.h"
#include "Framework/Core.h"

namespace we
{
	Actor::Actor(World* OwningWorld)
		: OwningWorld{OwningWorld},
		bHasBegunPlay{false}
	{

	}

	Actor::~Actor()
	{
		LOG("Actor Destroyed")
	}

	void Actor::BeginPlayGlobal()
	{
		if (!bHasBegunPlay)
		{
			bHasBegunPlay = true;
			BeginPlay();
		}
	}

	void Actor::BeginPlay()
	{
		LOG("Actor BeginPlay Called!")
	}

	void Actor::Tick(float DeltaTime)
	{
	}
}