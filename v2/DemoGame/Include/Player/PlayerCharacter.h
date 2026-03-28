// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Character.h"
#include "Subsystem/InputSubsystem.h"
#include "Interaction/IInteractor.h"

namespace we
{
	enum class EPlayerAnim : uint8
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

	enum class EPlayerState : uint8
	{
		Idle,
		Walk
	};

	class PlayerCharacter : public Character, public IInteractor
	{
	public:
		PlayerCharacter(World& OwningWorld);
		~PlayerCharacter();

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;

	private:
		void SetupAnimations();
		void BindInput();
		void HandleInput();
		void UpdateDirectionalAnimation();
		void TryInteract();
		EPlayerAnim DirectionToIdleAnim(const vec2f& Dir) const;
		EPlayerAnim DirectionToWalkAnim(const vec2f& Dir) const;
		void SetCameraBounds();

		BindingHandle InteractBinding;
	};
}