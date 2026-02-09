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
#include "Utility/RandomGenerator.h"
#include "Utility/Log.h"

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
		UpdateAnimation();
		UpdateFootsteps();
		Actor::Tick(DeltaTime);
	}

	void Player::InitializeAnimations()
	{
		MoveComp = CreateComponent<MovementComponent>(this);
		MoveComp->SetSpeed(300.0f);

		AnimComp = CreateComponent<AnimationComponent>(this, vec2u{ 96, 80 }, 8);

		// Idle animations
		AnimComp->AddAnimation({ (uint8)AnimState::IdleDown,  {0, 0}, {0, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleLeft,  {1, 0}, {1, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleRight, {2, 0}, {2, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleUp,    {3, 0}, {3, 7}, 0.15f, true });

		// Run animations
		AnimComp->AddAnimation({ (uint8)AnimState::RunDown,   {4, 0}, {4, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunLeft,   {5, 0}, {5, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunRight,  {6, 0}, {6, 7}, 0.10f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunUp,     {7, 0}, {7, 7}, 0.10f, true });

		AnimComp->Transition((uint8)AnimState::IdleDown);
	}

	void Player::InitializePhysics()
	{
		PhysComp = CreateComponent<PhysicsComponent>(this);
		PhysComp->SetBodyType(BodyType::Kinematic);
		PhysComp->SetCircleShape(80.0f);
		Physics().RegisterComponent(PhysComp.get());
		PhysComp->SetDebugDraw(true);
	}

	void Player::UpdateAnimation()
	{
		if (!MoveComp || !AnimComp) return;

		vec2f Forward = MoveComp->GetForwardVector();
		bool bIsMoving = MoveComp->GetVelocity().lengthSquared() > 1.0f;
		AnimState TargetState;

		if (std::abs(Forward.y) > std::abs(Forward.x))
		{
			TargetState = (Forward.y > 0) ?
				(bIsMoving ? AnimState::RunDown : AnimState::IdleDown) :
				(bIsMoving ? AnimState::RunUp : AnimState::IdleUp);
		}
		else
		{
			TargetState = (Forward.x > 0) ?
				(bIsMoving ? AnimState::RunRight : AnimState::IdleRight) :
				(bIsMoving ? AnimState::RunLeft : AnimState::IdleLeft);
		}
		AnimComp->Transition((uint8)TargetState);
	}

	void Player::UpdateFootsteps()
	{
		if (!MoveComp || !AnimComp) return;

		bool bIsMoving = MoveComp->GetVelocity().lengthSquared() > 1.0f;
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
		static const arr<string, 4> Sounds = {
			EC.Footstep1,
			EC.Footstep2,
			EC.Footstep3,
			EC.Footstep4
		};
		OwnerWorld->GetSubsystem().Audio->PlaySFX(RNG().Random(Sounds));
	}

	void Player::Destroy()
	{
		LOG("Player destroyed");
		Actor::Destroy();
	}
}