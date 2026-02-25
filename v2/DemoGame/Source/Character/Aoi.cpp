// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/Aoi.h"
#include "Interface/Component/AnimationComponent.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	Aoi::Aoi(World* OwningWorld)
		: NPC(OwningWorld, GC.NPCSheetIdle, "Aoi")
	{
		SetDialog("Hello! I'm Aoi. Welcome to our village!");
	}

	void Aoi::BeginPlay()
	{
		NPC::BeginPlay();
		auto PhyBody = GetPhysicsComponent();
		PhyBody->SetBodyType(BodyType::Static);
		InitializeAnimations();
	}

	void Aoi::InitializeAnimations()
	{		
		AnimComp = make_shared<AnimationComponent>(this);
		AnimComp->BeginPlay();

		AnimComp->AddSpriteSheet(0, SpriteSheet{ GC.NPCSheetIdle, {256, 256}, 8 });

		AnimComp->AddAnimation({ 1, 0, {0, 0}, {0, 7}, 0.15f, true });

		AnimComp->Transition(1);
		
		LOG("[Aoi] Initialized 1-way idle animation");
	}
}
