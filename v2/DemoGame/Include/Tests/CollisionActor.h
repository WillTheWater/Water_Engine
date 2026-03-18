// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor.h"

namespace we
{
	class CollisionComponent;

	class CollisionActor : public Actor
	{
	public:
		CollisionActor(World& OwningWorld);
		~CollisionActor();

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		void GetDrawables(vector<const drawable*>& OutDrawables) const override;

	private:
		shared<CollisionComponent> CollisionComp;
	};
}
