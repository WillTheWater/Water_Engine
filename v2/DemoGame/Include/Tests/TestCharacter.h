// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor.h"

namespace we
{
	class AnimationComponent;
	class PhysicsComponent;
	class CollisionComponent;
	class MovementComponent;

	enum class ETestCharAnim : uint8
	{
		IdleForward,
		IdleForwardRight,
		IdleRight,
		IdleBackRight,
		IdleBack,
		IdleBackLeft,
		IdleLeft,
		IdleForwardLeft
	};

	enum class ETestCharSheet : uint8
	{
		Idle
	};

	class TestCharacter : public Actor
	{
	public:
		TestCharacter(World& OwningWorld);
		~TestCharacter();

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		void GetDrawables(vector<const drawable*>& OutDrawables) const override;

	private:
		void SetupAnimations();
		
		// Input handling (separated for easy removal)
		void BindInput();
		void HandleInput();

	private:
		shared<AnimationComponent> AnimComp;
		shared<PhysicsComponent> PhysicsComp;
		shared<CollisionComponent> CollisionComp;
		shared<MovementComponent> MoveComp;
	};
}