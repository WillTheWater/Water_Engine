// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"

namespace we
{
	class CollisionComponent : public IActorComponent
	{
	public:
		explicit CollisionComponent(Actor* InOwner);

		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;
		Actor* GetOwner() const override;

	private:
		Actor* Owner;
	};
}
