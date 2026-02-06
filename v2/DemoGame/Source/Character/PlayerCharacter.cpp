// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/PlayerCharacter.h"
#include "Character/MovementComponent.h"
#include "Interface/Component/IAnimationComponent.h"
#include "Utility/Log.h"

namespace we
{
	Player::Player(World* OwningWorld, const string& TexturePath)
		: Actor(OwningWorld, TexturePath)
	{
	}

	void Player::BeginPlay()
	{
		LOG("Player spawned");
		CenterOrigin();
		MoveComp = CreateComponent<MovementComponent>(this);
		MoveComp->SetSpeed(300.0f);

		AnimComp = CreateComponent<AnimationComponent>(this, vec2u{ 96, 80 }, 8);

		// Adding an animation
		AnimComp->AddAnimation({
			(uint8)AnimState::Idle,
			{0, 0},      // Start frame (col 0, row 0)
			{7, 0},      // End frame (col 7, row 0) - 8 frames total
			0.15f,       // Frame duration
			true         // bLoop = true
			});

		// Start playing it
		AnimComp->Transition((uint8)AnimState::Idle);

		Actor::BeginPlay();
	}

	void Player::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);
	}

	void Player::Destroy()
	{
		LOG("Player destroyed");
		Actor::Destroy();
	}
}