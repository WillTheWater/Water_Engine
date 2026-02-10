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
		PhysComp->SetVelocity(GetVelocity());
		Actor::Tick(DeltaTime);
	}

	void Player::InitializeAnimations()
	{
		MoveComp = CreateComponent<MovementComponent>(this);
		MoveComp->SetSpeed(300.0f);

		AnimComp = CreateComponent<AnimationComponent>(this, vec2u{ 256, 256 }, 8);

		// IDLE ANIMATIONS
		AnimComp->AddAnimation({ (uint8)AnimState::IdleDown,       {0, 0}, {0, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleDownRight,  {1, 0}, {1, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleRight,      {2, 0}, {2, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleUpRight,    {3, 0}, {3, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleUp,         {4, 0}, {4, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleUpLeft,     {5, 0}, {5, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleLeft,       {6, 0}, {5, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::IdleDownLeft,   {7, 0}, {7, 7}, 0.15f, true });

		// RUN ANIMATIONS
		AnimComp->AddAnimation({ (uint8)AnimState::RunDown,         {0, 0}, {0, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunDownRight,	{1, 0}, {1, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunRight,		{2, 0}, {2, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunUpRight,		{3, 0}, {3, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunUp,			{4, 0}, {4, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunUpLeft,		{5, 0}, {5, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunLeft,			{6, 0}, {6, 7}, 0.15f, true });
		AnimComp->AddAnimation({ (uint8)AnimState::RunDownLeft,		{7, 0}, {7, 7}, 0.15f, true });

		AnimComp->Transition((uint8)AnimState::IdleDown);
	}

	void Player::InitializePhysics()
	{
		PhysComp = CreateComponent<PhysicsComponent>(this);
		PhysComp->SetBodyType(BodyType::Dynamic);
		PhysComp->SetCircleShape(80.0f);
		PhysComp->SetDebugDraw(true);
		Physics().RegisterComponent(PhysComp.get());
		PhysComp->SetSensor(true);
	}

	void Player::UpdateAnimation()
	{
		if (!MoveComp || !AnimComp) return;

		vec2f Forward = MoveComp->GetForwardVector();
		bool bIsMoving = MoveComp->GetVelocity().lengthSquared() > 1.0f;

		// Determine direction index from ForwardVector (0-7)
		// 0=Right, 1=DownRight, 2=Down, 3=DownLeft, 4=Left, 5=UpLeft, 6=Up, 7=UpRight
		int DirIndex = 0;
		float x = Forward.x;
		float y = Forward.y;

		// Use angle for reliable direction mapping
		float Angle = std::atan2(y, x);
		DirIndex = int(std::round(Angle / (PI / 4.0f))) % 8;
		if (DirIndex < 0) DirIndex += 8;

		// Map direction index to animation state
		// Idle: 1-8, Run: 9-16
		uint8 BaseState = bIsMoving ? 9 : 1; // RunRight or IdleRight
		AnimState TargetState = static_cast<AnimState>(BaseState + DirIndex);

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