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

	enum class AnimState : uint8
	{
		None = 0,
		Idle,
		Run
	};

	class Player : public Actor
	{
	public:
		Player(World* OwningWorld, const string& TexturePath = EC.CharacterSheet);

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Destroy() override;

	private:
		shared<MovementComponent> MoveComp;
		shared<AnimationComponent> AnimComp;
	};
}