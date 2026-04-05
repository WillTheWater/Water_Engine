// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor.h"
#include "Interaction/IInteractable.h"
#include "UI/InteractUI.h"

namespace we
{
	class PhysicsComponent;
	class CollisionComponent;

	class Compass : public Actor, public IInteractable
	{
	public:
		Compass(World& OwningWorld);
		~Compass();

		// IInteractable
		void Interact(Actor* Interactor) override;
		void ShowPrompt(Actor* Interactor) override;
		void HidePrompt(Actor* Interactor) override;

	protected:
		void BeginPlay() override;
		void EndPlay() override;
		void GetDrawables(vector<const drawable*>& OutDrawables) const override;

	private:
		void SetupSprite();

	private:
		InteractUI PromptUI;
		shared<texture> CompassTexture;
		optional<sprite> CompassSprite;
		shared<CollisionComponent> CollComp;
		shared<PhysicsComponent> PhysicsComp;
	};
}