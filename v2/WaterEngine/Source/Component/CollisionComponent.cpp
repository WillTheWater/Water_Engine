// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/CollisionComponent.h"

namespace we
{
	CollisionComponent::CollisionComponent(Actor* InOwner)
		: Owner(InOwner)
	{
	}

	void CollisionComponent::BeginPlay()
	{
	}

	void CollisionComponent::Tick(float DeltaTime)
	{
	}

	void CollisionComponent::EndPlay()
	{
	}

	Actor* CollisionComponent::GetOwner() const
	{
		return Owner;
	}
}
