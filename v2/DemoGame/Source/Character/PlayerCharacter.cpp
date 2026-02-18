// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/PlayerCharacter.h"
#include "Character/MovementComponent.h"
#include "Interface/Component/AnimationComponent.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	Player::Player(World* OwningWorld, const string& TexturePath)
		: Character(OwningWorld, TexturePath)
	{
		// Character base class creates Trigger and Blocking components
		// We just need to set up the capsule size for our character sprite
		SetCapsuleRadius(40.0f);      // Slightly smaller than the 128px half-sprite
		SetCapsuleHalfHeight(50.0f);  // Tall enough for the character
		SetCapsuleOffset({ 0,10 });
	}

	void Player::BeginPlay()
	{
		InitializeAnimations();
		Character::BeginPlay();
	}

	void Player::Tick(float DeltaTime)
	{
		// Tick components manually
		if (MoveComp) MoveComp->Tick(DeltaTime);
		if (AnimComp) AnimComp->Tick(DeltaTime);

		UpdateAnimation();
		UpdateFootsteps();

		// Apply movement with collision blocking
		if (MoveComp)
		{
			vec2f Velocity = MoveComp->GetVelocity();
			float DeltaTime = GetWorld()->GetSubsystem().Time->GetDeltaTime();
			vec2f Delta = Velocity * DeltaTime;
			
			// TryMove respects blocking collision (prevents walking through walls)
			TryMove(Delta);
		}

		Character::Tick(DeltaTime);
	}

	void Player::InitializeAnimations()
	{
		// Create components directly with make_shared
		MoveComp = make_shared<MovementComponent>(this);
		MoveComp->SetSpeed(300.0f);
		MoveComp->BeginPlay();

		AnimComp = make_shared<AnimationComponent>(this);
		AnimComp->BeginPlay();

		// Load animation sheets
		AnimComp->AddSpriteSheet(0, SpriteSheet{ GC.CharacterSheetIdle, {256, 256}, 8 });
		AnimComp->AddSpriteSheet(1, SpriteSheet{ GC.CharacterSheetWalk, {256, 256}, 8 });

		// IDLE animations
		AnimComp->AddAnimation({ (uint8)AnimState::IdleDown,       0, {0, 0}, {0, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleDownRight,  0, {1, 0}, {1, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleRight,      0, {2, 0}, {2, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleUpRight,    0, {3, 0}, {3, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleUp,         0, {4, 0}, {4, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleUpLeft,     0, {5, 0}, {5, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleLeft,       0, {6, 0}, {6, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleDownLeft,   0, {7, 0}, {7, 7}, 0.15f, true });

		// RUN animations
		AnimComp->AddAnimation({ (uint8)AnimState::RunDown,        1, {0, 0}, {0, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunDownRight,   1, {1, 0}, {1, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunRight,       1, {2, 0}, {2, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunUpRight,     1, {3, 0}, {3, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunUp,          1, {4, 0}, {4, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunUpLeft,      1, {5, 0}, {5, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunLeft,        1, {6, 0}, {6, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunDownLeft,    1, {7, 0}, {7, 7}, 0.10f, true });

		AnimComp->Transition((uint8)AnimState::IdleDown);
	}

	void Player::UpdateAnimation()
	{
		if (!MoveComp || !AnimComp) return;

		vec2f Forward = MoveComp->GetForwardVector();
		bool bIsMoving = MoveComp->IsMoving();

		int DirIndex = 0;
		float Angle = std::atan2(Forward.y, Forward.x);
		DirIndex = int(std::round(Angle / (PI / 4.0f))) % 8;
		if (DirIndex < 0) DirIndex += 8;

		uint8 BaseState = bIsMoving ? 9 : 1;
		AnimState TargetState = static_cast<AnimState>(BaseState + DirIndex);

		AnimComp->Transition((uint8)TargetState);
	}

	void Player::UpdateFootsteps()
	{
		if (!MoveComp || !AnimComp) return;

		bool bIsMoving = MoveComp->IsMoving();
		uint CurrentCol = AnimComp->GetCurrentFrame().y;

		if (bIsMoving && (CurrentCol == 1 || CurrentCol == 5))
		{
			if (LastFootstepFrame != CurrentCol)
			{
				PlayFootstep();
				LastFootstepFrame = CurrentCol;
			}
		}
	}

	void Player::PlayFootstep()
	{
		static const array<string, 4> Sounds = {
			GC.Footstep1,
			GC.Footstep2,
			GC.Footstep3,
			GC.Footstep4
		};
		
		if (GetWorld() && GetWorld()->GetSubsystem().Audio)
		{
			GetWorld()->GetSubsystem().Audio->PlaySFX(RNG().RandomElement(Sounds));
		}
	}

	void Player::Destroy()
	{
		LOG("Player destroyed");
		if (MoveComp) MoveComp->EndPlay();
		if (AnimComp) AnimComp->EndPlay();
		Character::Destroy();
	}
}
