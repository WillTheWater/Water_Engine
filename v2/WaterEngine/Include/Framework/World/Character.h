// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor.h"

namespace we
{
	class AnimationComponent;
	class PhysicsComponent;
	class MovementComponent;
	class CollisionComponent;
	class CameraComponent;

	class Character : public Actor
	{
	public:
		explicit Character(World& InWorld);
		~Character();

		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		void GetDrawables(vector<const drawable*>& OutDrawables) const override;

		shared<CameraComponent> GetCameraComponent() const { return CameraComp; }

	protected:
		shared<AnimationComponent> AnimComp;
		shared<PhysicsComponent> PhysicsComp;
		shared<MovementComponent> MoveComp;
		shared<CollisionComponent> CollComp;
		shared<CameraComponent> CameraComp;
	};
}