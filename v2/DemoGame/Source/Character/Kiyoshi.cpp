// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/Kiyoshi.h"
#include "Interface/Component/AnimationComponent.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	Kiyoshi::Kiyoshi(World* OwningWorld)
		: NPC(OwningWorld, GC.OldNPCSheetIdle, "Kiyoshi")
	{
		SetDialog("Greetings! I'm Kiyoshi. I sell potions and items.");
	}

	void Kiyoshi::InitializeAnimations()
	{
		AnimComp = make_shared<AnimationComponent>(this);
		AnimComp->BeginPlay();

		// Use the oldIdle sprite sheet (8x8 grid, 256x256 frames)
		// Row 0 = Idle Down, Row 1 = Idle DownRight, etc.
		AnimComp->AddSpriteSheet(0, SpriteSheet{ GC.OldNPCSheetIdle, {256, 256}, 8 });

		// IDLE animations for all 8 directions
		AnimComp->AddAnimation({ 1, 0, {0, 0}, {0, 7}, 0.15f, true });  // Down
		AnimComp->AddAnimation({ 2, 0, {1, 0}, {1, 7}, 0.15f, true });  // DownRight
		AnimComp->AddAnimation({ 3, 0, {2, 0}, {2, 7}, 0.15f, true });  // Right
		AnimComp->AddAnimation({ 4, 0, {3, 0}, {3, 7}, 0.15f, true });  // UpRight
		AnimComp->AddAnimation({ 5, 0, {4, 0}, {4, 7}, 0.15f, true });  // Up
		AnimComp->AddAnimation({ 6, 0, {5, 0}, {5, 7}, 0.15f, true });  // UpLeft
		AnimComp->AddAnimation({ 7, 0, {6, 0}, {6, 7}, 0.15f, true });  // Left
		AnimComp->AddAnimation({ 8, 0, {7, 0}, {7, 7}, 0.15f, true });  // DownLeft

		// Start facing down
		AnimComp->Transition(1);
		
		LOG("[Kiyoshi] Initialized 8-way idle animations");
	}
}
