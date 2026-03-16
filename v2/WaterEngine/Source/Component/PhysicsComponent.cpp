// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/PhysicsComponent.h"

namespace we
{
	PhysicsComponent::PhysicsComponent(Actor* InOwner)
		: Owner(InOwner)
	{
	}

	void PhysicsComponent::BeginPlay()
	{
	}

	void PhysicsComponent::Tick(float DeltaTime)
	{
	}

	void PhysicsComponent::EndPlay()
	{
	}

	Actor* PhysicsComponent::GetOwner() const
	{
		return Owner;
	}
}
