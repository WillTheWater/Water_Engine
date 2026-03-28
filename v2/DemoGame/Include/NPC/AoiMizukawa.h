// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Character.h"
#include "Interaction/IInteractable.h"

namespace we
{
	class AoiMizukawa : public Character, public IInteractable
	{
	public:
		AoiMizukawa(World& OwningWorld);
		~AoiMizukawa();

		// IInteractable
		void Interact(Actor* Interactor) override;

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;

	private:
		void SetupAnimation();
		void OnBeginOverlap(Actor* Other);
		void OnEndOverlap(Actor* Other);
	};
}