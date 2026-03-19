// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Character.h"

namespace we
{
	enum class ETestCharAnim : uint8
	{
		// Idle animations
		IdleForward,
		IdleForwardRight,
		IdleRight,
		IdleBackRight,
		IdleBack,
		IdleBackLeft,
		IdleLeft,
		IdleForwardLeft,
		
		// Walk animations
		WalkForward,
		WalkForwardRight,
		WalkRight,
		WalkBackRight,
		WalkBack,
		WalkBackLeft,
		WalkLeft,
		WalkForwardLeft
	};

	enum class ETestCharSheet : uint8
	{
		Idle,
		Walk
	};

	class TestCharacter : public Character
	{
	public:
		TestCharacter(World& OwningWorld);
		~TestCharacter();

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;

	private:
		void SetupAnimations();
		void BindInput();
		void HandleInput();
		void UpdateDirectionalAnimation();
		ETestCharAnim DirectionToIdleAnim(const vec2f& Dir) const;
		ETestCharAnim DirectionToWalkAnim(const vec2f& Dir) const;
	};
}
