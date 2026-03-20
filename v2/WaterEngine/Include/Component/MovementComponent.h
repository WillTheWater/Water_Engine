// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"
#include "Utility/Delegate.h"

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
		void SetSpeed(float InSpeed) { CurrentSpeed = InSpeed; }
		float GetSpeed() const { return CurrentSpeed; }
		void SetAutoOrient(bool bEnabled) { bAutoOrient = bEnabled; }
		bool IsAutoOrient() const { return bAutoOrient; }

		// Movement queries
		vec2f GetVelocity() const { return Velocity; }
		vec2f GetInputDirection() const;
		bool IsMoving() const { return bIsMoving; }

		// Orientation
		vec2f GetForwardVector() const { return ForwardVector; }
		vec2f GetRightVector() const { return RightVector; }
		void SetFacingDirection(const vec2f& Direction);
		
		// Last movement direction
		vec2f GetLastMoveDirection() const { return LastMoveDir; }
		void SetLastMoveDirection(const vec2f& Dir);

		const drawable* DrawDebug();

		vec2f GetLocalInput() const;
		
		// Velocity broadcast delegate - bind PhysicsComponent::SetVelocity here
		Delegate<vec2f> OnVelocityCalculated;

	private:
		void UpdateOrientation();
		void ApplyMovement(float DeltaTime);
		vec2f CalculateVelocity();

	private:
		Actor* Owner;
		float CurrentSpeed = 200.0f;
		vec2f Velocity;
		vec2f InputVector;

		vec2f ForwardVector{ 0, 1 };
		vec2f RightVector{ 1, 0 };
		
		// Movement state (for animation/controller queries)
		vec2f LastMoveDir{ 0.0f, 1.0f };  // Default facing down/forward
		bool bIsMoving = false;

		// Movement type flags
		bool bIsWalking = true;
		bool bIsRunning = false;
		bool bIsSwimming = false;

		bool bAutoOrient = true;
		optional<sf::VertexArray> DebugArrow;
	};
}