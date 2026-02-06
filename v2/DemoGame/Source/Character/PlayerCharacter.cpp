// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/PlayerCharacter.h"
#include "Character/MovementComponent.h"
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
		auto MoveComp = CreateComponent<MovementComponent>(this);
		MoveComp->SetSpeed(300.0f);

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