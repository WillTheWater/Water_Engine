// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/PlayerCharacter.h"
#include "Character/MovementComponent.h"
#include "Interface/Component/AnimationComponent.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"
#include "GameConfig.h"

#include <box2d/b2_body.h>

namespace we
{
	Player::Player(World* OwningWorld, const string& TexturePath)
		: Actor(OwningWorld, TexturePath)
	{
	}

	void Player::BeginPlay()
	{
		InitializeAnimations();
		InitializePhysics();
		Actor::BeginPlay();
	}

	void Player::Tick(float DeltaTime)
	{
		// Tick components manually - THIS IS THE KEY FIX FOR DEBUG DRAW
		if (MoveComp) MoveComp->Tick(DeltaTime);
		if (AnimComp) AnimComp->Tick(DeltaTime);
		if (PhysComp) PhysComp->Tick(DeltaTime);

		UpdateAnimation();
		UpdateFootsteps();

		// Apply movement velocity to physics
		if (MoveComp && PhysComp)
		{
			PhysComp->SetVelocity(MoveComp->GetVelocity());
		}

		// Sync position from physics body to actor
		if (PhysComp && PhysComp->GetPhysicsBody())
		{
			float Scale = GetWorld()->GetSubsystem().Physics->GetPhysicsScale();
			b2Vec2 Pos = PhysComp->GetPhysicsBody()->GetPosition();
			SetPosition({ Pos.x / Scale, Pos.y / Scale });
		}

		Actor::Tick(DeltaTime);
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

	void Player::InitializePhysics()
	{
		PhysComp = make_shared<PhysicsComponent>(this);
		PhysComp->SetBodyType(BodyType::Dynamic);
		PhysComp->SetCircleShape(80.0f);
		PhysComp->SetSensor(true);
		PhysComp->BeginPlay();
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
		if (PhysComp) PhysComp->EndPlay();
		Actor::Destroy();
	}
}
