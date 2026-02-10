// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Framework/World/Actor/Actor.h"
#include "EngineConfig.h"

namespace we
{
	class MovementComponent;
	class AnimationComponent;
	class PhysicsComponent;

	enum class AnimState : uint8
	{
		None = 0,
		// Idle states (8 directions)
		IdleRight,      // 1
		IdleDownRight,  // 2
		IdleDown,       // 3
		IdleDownLeft,   // 4
		IdleLeft,       // 5
		IdleUpLeft,     // 6
		IdleUp,         // 7
		IdleUpRight,    // 8
		// Run states (8 directions)
		RunRight,       // 9
		RunDownRight,   // 10
		RunDown,        // 11
		RunDownLeft,    // 12
		RunLeft,        // 13
		RunUpLeft,      // 14
		RunUp,          // 15
		RunUpRight      // 16
	};

	class Player : public Actor
	{
	public:
		Player(World* OwningWorld, const string& TexturePath = EC.CharacterSheetIdle);

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Destroy() override;

	public:
		void InitializeAnimations();
		void InitializePhysics();
		void UpdateAnimation();
		void UpdateFootsteps();
		void PlayFootstep();

	private:
		shared<MovementComponent> MoveComp;
		shared<AnimationComponent> AnimComp;
		shared<PhysicsComponent> PhysComp;
		uint LastFootstepFrame = 255;
	};
}