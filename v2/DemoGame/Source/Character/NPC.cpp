// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/NPC.h"
#include "Interface/Component/AnimationComponent.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	NPC::NPC(World* OwningWorld, const string& TexturePath)
		: Character(OwningWorld, TexturePath)
	{
		SetCharacterRadius(40.0f);
		SetCollisionOffset({ 0, 10 });
	}

	void NPC::BeginPlay()
	{
		InitializeAnimations();
		Character::BeginPlay();
		if (PhysicsComp)
		{
			PhysicsComp->SetBodyType(BodyType::Static);
		}
	}

	void NPC::Tick(float DeltaTime)
	{
		if (AnimComp)
		{
			AnimComp->Tick(DeltaTime);
		}

		Character::Tick(DeltaTime);
	}

	void NPC::InitializeAnimations()
	{
		AnimComp = make_shared<AnimationComponent>(this);
		AnimComp->BeginPlay();

		// Load animation sheets
		AnimComp->AddSpriteSheet(0, SpriteSheet{ GC.NPCSheetIdle, {256, 256}, 8 });

		// IDLE animations
		AnimComp->AddAnimation({ (uint8)NPCAnimState::Idle, 0, {0, 0}, {0, 7}, 0.15f, true });

		AnimComp->Transition((uint8)NPCAnimState::Idle);
	}

	void NPC::Destroy()
	{
		if (AnimComp) AnimComp->EndPlay();
		Character::Destroy();
	}
}
