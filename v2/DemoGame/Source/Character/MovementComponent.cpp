// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/MovementComponent.h"
#include "Framework/World/Actor/Actor.h"
#include "Utility/Log.h"

namespace we
{
	MovementComponent::MovementComponent(Actor* InOwner)
		: Owner(InOwner)
	{
	}

	void MovementComponent::BeginPlay()
	{
		LOG("MovementComponent BeginPlay");
	}

	void MovementComponent::Tick(float DeltaTime)
	{
		if (!Owner) return;

		Velocity = InputVector * Speed;
		Owner->SetPosition(Owner->GetPosition() + Velocity * DeltaTime);

		InputVector = {}; // Reset for next frame
	}

	void MovementComponent::EndPlay()
	{
		LOG("MovementComponent EndPlay");
	}

	Actor* MovementComponent::GetOwner() const
	{
		return Owner;
	}

	void MovementComponent::AddInputVector(const vec2f& Direction)
	{
		InputVector += Direction;
	}

	void MovementComponent::SetSpeed(float InSpeed)
	{
		Speed = InSpeed;
	}
}