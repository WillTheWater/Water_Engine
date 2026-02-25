// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Character/PlayerCharacter.h"
#include "Character/MovementComponent.h"
#include "Character/NPC.h"
#include "Interface/Component/AnimationComponent.h"
#include "Interface/Component/PhysicsComponent.h"
#include "Interface/IInteractive.h"
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
		// Set up the collision radius for our character sprite
		SetCharacterRadius(40.0f);
		SetCollisionOffset({ 0, 10 });
	}

	void Player::BeginPlay()
	{
		InitializeAnimations();
		Character::BeginPlay();

		// Setup interaction sensor
		SetupInteractionSensor();
		
		// Create camera component (not an Actor - owned by player)
		//LOG("[Player] Spawning at ({:.1f}, {:.1f})", GetPosition().x, GetPosition().y);
		//
		//CamComp = make_shared<CameraComponent>(this);
		//CamComp->AttachTo(this, {0, 0});  // EXACTLY at player position (test)
		//CamComp->SetSmoothTime(0.0f);     // No lag - exact follow
		//CamComp->SetZoom(1.0f);
		//CamComp->SetActive();  // Set as active camera
		//
		//LOG("[Player] Camera initialized at ({:.1f}, {:.1f})", CamComp->CalculateView().Position.x, CamComp->CalculateView().Position.y);
	}

	void Player::Tick(float DeltaTime)
	{
		if (MoveComp) MoveComp->Tick(DeltaTime);
		if (AnimComp) AnimComp->Tick(DeltaTime);
		//if (CamComp) CamComp->Update(DeltaTime);  // Update camera follow

		UpdateAnimation();
		UpdateFootsteps();

		// Apply movement via physics component
		if (MoveComp)
		{
			SetVelocity(MoveComp->GetVelocity());
		}

		Character::Tick(DeltaTime);
	}

	void Player::InitializeAnimations()
	{
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
		if (MoveComp) MoveComp->EndPlay();
		if (AnimComp) AnimComp->EndPlay();
		Character::Destroy();
	}

	void Player::SetupInteractionSensor()
	{
		if (PhysicsComp)
		{
			// Add a sensor shape for interaction detection (separate from collision)
			PhysicsComp->SetSensorShape(true, 100.0f);  // Circle sensor with 100 unit radius
			
			PhysicsComp->OnBeginOverlapEvent.Bind(this, &Player::OnInteractionOverlapBegin);
			PhysicsComp->OnEndOverlapEvent.Bind(this, &Player::OnInteractionOverlapEnd);
			
			LOG("[Player] Interaction sensor set up on existing PhysicsComp");
		}
		else
		{
			LOG("[Player] ERROR: No PhysicsComp available for interaction sensor!");
		}
	}

	void Player::OnInteractionOverlapBegin(IPhysicsComponent* Other)
	{
		if (!Other) return;

		Actor* OtherActor = Other->GetOwner();
		if (!OtherActor) return;

		// Check if the other actor implements IInteractive
		IInteractive* Interactive = dynamic_cast<IInteractive*>(OtherActor);
		if (Interactive)
		{
			NearbyInteractables.insert(Interactive);
			LOG("[Player] Entered interaction range of: {}", typeid(*OtherActor).name());
			
			// Show interaction hint for NPCs and make them idle
			if (auto* NPC = dynamic_cast<class NPC*>(OtherActor))
			{
				NPC->ShowInteractionHint();
				
				// Notify NPC that player is nearby (for Kiyoshi to stop walking)
				NPC->OnPlayerEnteredRange(this);
			}
		}
	}

	void Player::OnInteractionOverlapEnd(IPhysicsComponent* Other)
	{
		if (!Other) return;

		Actor* OtherActor = Other->GetOwner();
		if (!OtherActor) return;

		IInteractive* Interactive = dynamic_cast<IInteractive*>(OtherActor);
		if (Interactive)
		{
			NearbyInteractables.erase(Interactive);
			LOG("[Player] Left interaction range of: {}", typeid(*OtherActor).name());
			
			// Hide interaction hint and dialog for NPCs
			if (auto* NPC = dynamic_cast<class NPC*>(OtherActor))
			{
				NPC->HideInteractionHint();
				NPC->HideDialog();  // Close dialog if player walks away
				
				// Notify NPC that player left (for Kiyoshi to resume patrol)
				NPC->OnPlayerLeftRange(this);
			}
		}
	}

	void Player::TryInteract()
	{
		for (IInteractive* Interactive : NearbyInteractables)
		{
			if (Interactive && Interactive->CanInteract())
			{
				Interactive->OnInteract(this);
				return;
			}
		}
	}
}
