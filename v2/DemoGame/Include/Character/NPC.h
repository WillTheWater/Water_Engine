// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor/Character.h"
#include "Interface/Component/CameraComponent.h"
#include "Interface/Component/IMovementComponent.h"
#include "GameConfig.h"

namespace we
{
	class AnimationComponent;

	enum class NPCAnimState : uint8
	{
		None = 0,
		Idle
	};

	class NPC : public Character
	{
	public:
		NPC(World* OwningWorld, const string& TexturePath = GC.NPCSheetIdle);

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Destroy() override;

	public:
		void InitializeAnimations();

	private:
		shared<AnimationComponent> AnimComp;
	};
}
