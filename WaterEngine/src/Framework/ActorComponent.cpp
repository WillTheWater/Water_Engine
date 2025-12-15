#include "Framework/ActorComponent.h"

namespace we
{
	ActorComponent::ActorComponent(Actor* OwningActor)
		: Owner{OwningActor}
	{
	}

	void ActorComponent::BeginPlay()
	{

	}

	void ActorComponent::Tick(float DeltaTime)
	{

	}
}