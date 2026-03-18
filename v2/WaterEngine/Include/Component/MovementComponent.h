// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"

namespace we
{
	class MovementComponent : public IActorComponent
	{
	public:
		explicit MovementComponent(Actor* InOwner);

		// IActorComponent
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		Actor* GetOwner() const override;

		// Input control (call from your input handling)
		void AddInputVector(const vec2f& Direction);
		void ClearInput();

		// Configuration
		void SetSpeed(float InSpeed) { Speed = InSpeed; }
		float GetSpeed() const { return Speed; }
		void SetAutoOrient(bool bEnabled) { bAutoOrient = bEnabled; }
		bool IsAutoOrient() const { return bAutoOrient; }

		// Movement queries
		vec2f GetVelocity() const { return Velocity; }
		vec2f GetInputDirection() const;
		bool IsMoving() const { return Velocity.lengthSquared() > 1.0f; }

		// Orientation
		vec2f GetForwardVector() const { return ForwardVector; }
		vec2f GetRightVector() const { return RightVector; }
		void SetFacingDirection(const vec2f& Direction);

		// Debug visualization - returns combined arrow drawable
		const drawable* DrawDebug();

		// Convert world input to local space (for strafing)
		vec2f GetLocalInput() const;

	private:
		void UpdateOrientation();
		void ApplyMovement(float DeltaTime);

	private:
		Actor* Owner;
		float Speed = 200.0f;
		vec2f Velocity;
		vec2f InputVector;

		vec2f ForwardVector{ 0, 1 };
		vec2f RightVector{ 1, 0 };

		bool bAutoOrient = true;
		optional<sf::VertexArray> DebugArrow;
	};
}