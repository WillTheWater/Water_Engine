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

	void Aoi::InitializeAnimations()
	{
		// Aoi is a static NPC - no movement
		if (PhysicsComp)
		{
			PhysicsComp->SetBodyType(BodyType::Static);
		}
		
		AnimComp = make_shared<AnimationComponent>(this);
		AnimComp->BeginPlay();

		// Load animation sheet (girlidle format - 256x256 frames, 8 per row)
		AnimComp->AddSpriteSheet(0, SpriteSheet{ GC.NPCSheetIdle, {256, 256}, 8 });

		// Single idle animation facing down (row 0)
		AnimComp->AddAnimation({ 1, 0, {0, 0}, {0, 7}, 0.15f, true });

		// Start with idle
		AnimComp->Transition(1);
		
		LOG("[Aoi] Initialized 1-way idle animation");
	}
}
