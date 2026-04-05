// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor.h"
#include "Component/CollisionComponent.h"
#include "Utility/Delegate.h"

namespace we
{
	class ForestExit : public Actor
	{
	public:
		ForestExit(World& OwningWorld);
		~ForestExit();

		Delegate<> OnExitTriggered;

	protected:
		void BeginPlay() override;
		void EndPlay() override;

	private:
		void OnBeginOverlap(Actor* Other);

	private:
		shared<CollisionComponent> CollComp;
	};
}