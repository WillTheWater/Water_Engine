#include "Framework/ActorComponent.h"
#include <cassert>

namespace we
{
	ActorComponent::ActorComponent(Actor* OwningActor)
		: Owner{OwningActor}
	{
		assert(Owner && "ActorComponent must have an owning Actor");
	}

	void ActorComponent::BeginPlay()
	{

	}

	void ActorComponent::Tick(float DeltaTime)
	{

	}
}