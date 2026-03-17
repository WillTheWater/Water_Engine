// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor.h"

namespace we
{
	class AnimationComponent;
	class PhysicsComponent;
	class CollisionComponent;

	class TestCharacter : public Actor
	{
	public:
		TestCharacter(World& OwningWorld);
		~TestCharacter();

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;

	private:
		void SetupAnimations();

	private:
		shared<AnimationComponent> AnimComp;
		shared<PhysicsComponent> PhysicsComp;
		shared<CollisionComponent> CollisionComp;
	};
}