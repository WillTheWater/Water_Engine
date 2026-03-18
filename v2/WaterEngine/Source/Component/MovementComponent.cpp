// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Component/MovementComponent.h"
#include "Framework/World/Actor.h"
#include "Utility/Math.h"

namespace we
{
	MovementComponent::MovementComponent(Actor* InOwner)
		: Owner(InOwner)
	{
	}

	void MovementComponent::BeginPlay()
	{
	}

	void MovementComponent::Tick(float DeltaTime)
	{
		if (!Owner)
			return;

		// Calculate velocity from input
		if (LengthSquared(InputVector) > 0.001f)
		{
			InputVector = Normalize(InputVector);
			Velocity = InputVector * Speed;
		}
		else
		{
			Velocity = {};
		}

		// Update orientation
		if (bAutoOrient && LengthSquared(Velocity) > 0.001f)
		{
			UpdateOrientation();
		}

		ApplyMovement(DeltaTime);
		InputVector = {};
	}

	void MovementComponent::EndPlay()
	{
	}

	Actor* MovementComponent::GetOwner() const
	{
		return Owner;
	}

	void MovementComponent::AddInputVector(const vec2f& Direction)
	{
		InputVector += Direction;
	}

	void MovementComponent::ClearInput()
	{
		InputVector = {};
	}

	vec2f MovementComponent::GetInputDirection() const
	{
		return LengthSquared(InputVector) > 0.001f ? Normalize(InputVector) : vec2f{};
	}

	void MovementComponent::SetFacingDirection(const vec2f& Direction)
	{
		if (LengthSquared(Direction) > 0.001f)
		{
			ForwardVector = Normalize(Direction);
			RightVector = Perpendicular(ForwardVector);
		}
	}

	vec2f MovementComponent::GetLocalInput() const
	{
		if (LengthSquared(InputVector) < 0.001f)
			return {};

		vec2f NormalizedInput = Normalize(InputVector);
		float ForwardDot = Dot(NormalizedInput, ForwardVector);
		float RightDot = Dot(NormalizedInput, RightVector);

		return { RightDot, ForwardDot };
	}

	void MovementComponent::UpdateOrientation()
	{
		if (LengthSquared(Velocity) > 0.001f)
		{
			ForwardVector = Normalize(Velocity);
			RightVector = Perpendicular(ForwardVector);
		}
	}

	void MovementComponent::ApplyMovement(float DeltaTime)
	{
		vec2f NewPosition = Owner->GetPosition() + (Velocity * DeltaTime);
		Owner->SetPosition(NewPosition);
	}
}