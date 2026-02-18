// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"
#include "Interface/Component/IMovementComponent.h"

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

	// MovementComponent calculates movement intent from input
	// Does NOT apply movement - external code (e.g., PlayerCharacter) must apply velocity to physics or position
	class MovementComponent : public IMovementComponent
	{
	public:
		MovementComponent(Actor* InOwner);

		// IActorComponent
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void EndPlay() override;
		virtual Actor* GetOwner() const override;

		// IMovementComponent
		virtual vec2f GetVelocity() const override { return Velocity; }
		virtual vec2f GetForwardVector() const override { return ForwardVector; }
		virtual vec2f GetRightVector() const override { return RightVector; }
		virtual bool IsMoving() const override { return Velocity.lengthSquared() > 1.0f; }
		virtual void SetSpeed(float InSpeed) override { Speed = InSpeed; }
		virtual float GetSpeed() const override { return Speed; }

		// Additional input control
		void AddInputVector(const vec2f& Direction);

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
