// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h
#include "Interface/Component/IActorComponent.h"

namespace we
{
	enum InputActions
	{
		MOVE_UP = 1,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		INTERACT
	};

	class MovementComponent : public IActorComponent
	{
	public:
		MovementComponent(Actor* InOwner);

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void EndPlay() override;
		virtual Actor* GetOwner() const override;

		void AddInputVector(const vec2f& Direction);
		void SetSpeed(float InSpeed);

		vec2f GetVelocity() const { return Velocity; }
		vec2f GetForwardVector() const { return ForwardVector; }
		vec2f GetRightVector() const { return RightVector; }

	private:
		void BindInput();
		void UpdateFacingDirection();

		Actor* Owner;
		float Speed = 200.0f;
		vec2f Velocity;
		vec2f InputVector;

		// Default facing down (0,1), right is (-1,0) for 2D top-down
		vec2f ForwardVector{ 0, 1 };
		vec2f RightVector{ -1, 0 };
	};
}