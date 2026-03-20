// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Character.h"
#include "Component/AnimationComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/CameraComponent.h"
#include "Utility/Log.h"

namespace we
{
	Character::Character(World& InWorld)
		: Actor(InWorld)
	{
		AnimComp = make_shared<AnimationComponent>(this);
		PhysicsComp = make_shared<PhysicsComponent>(this);
		MoveComp = make_shared<MovementComponent>(this);
		CollComp = make_shared<CollisionComponent>(this);
		CameraComp = make_shared<CameraComponent>(this);
	}

	Character::~Character() = default;

	void Character::BeginPlay()
	{
		Actor::BeginPlay();

		LOG("[Character] BeginPlay on Actor {}", GetID());

		// Bind movement velocity to physics body
		MoveComp->OnVelocityCalculated.Bind(PhysicsComp.get(), &PhysicsComponent::SetVelocity);

		// Initialize all components
		AnimComp->BeginPlay();
		PhysicsComp->BeginPlay();
		MoveComp->BeginPlay();
		CollComp->BeginPlay();
		CollComp->DrawDebug();
		CameraComp->BeginPlay();
	}

	void Character::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);

		if (MoveComp) MoveComp->Tick(DeltaTime);
		if (PhysicsComp) PhysicsComp->Tick(DeltaTime);
		if (CollComp) CollComp->Tick(DeltaTime);
		if (AnimComp) AnimComp->Tick(DeltaTime);
		if (CameraComp) CameraComp->Tick(DeltaTime);
	}

	void Character::EndPlay()
	{
		if (AnimComp)
		{
			AnimComp->EndPlay();
			AnimComp.reset();
		}

		if (CollComp)
		{
			CollComp->EndPlay();
			CollComp.reset();
		}

		if (MoveComp)
		{
			MoveComp->EndPlay();
			MoveComp.reset();
		}

		if (PhysicsComp)
		{
			PhysicsComp->EndPlay();
			PhysicsComp.reset();
		}

		if (CameraComp)
		{
			CameraComp->EndPlay();
			CameraComp.reset();
		}

		Actor::EndPlay();
	}

	void Character::GetDrawables(vector<const drawable*>& OutDrawables) const
	{
		Actor::GetDrawables(OutDrawables);

		if (PhysicsComp)
		{
			if (const auto* Debug = PhysicsComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}

		if (CollComp)
		{
			if (const auto* Debug = CollComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}

		if (MoveComp)
		{
			if (const auto* Debug = MoveComp->DrawDebug())
				OutDrawables.push_back(Debug);
		}
	}
}