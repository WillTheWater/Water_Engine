// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor/Character.h"
#include "Interface/Component/CameraComponent.h"
#include "Interface/Component/IMovementComponent.h"
#include "Interface/IInteractive.h"

namespace we
{
	class AnimationComponent;
	class DialogBox;

	// =============================================================================
	// Base NPC Class
	// =============================================================================
	// Abstract base class for all NPCs. Provides interaction system and dialog
	// functionality. UI creation is delegated to DialogBox class.
	// Derived classes implement their own animation setups.
	// =============================================================================

	class NPC : public Character, public IInteractive
	{
	public:
		NPC(World* OwningWorld, const string& TexturePath, const string& InName);
		virtual ~NPC();

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Destroy() override;

		// IInteractive interface
		virtual void OnInteract(Actor* Interactor) override;
		virtual bool CanInteract() const override { return true; }
		virtual string GetInteractionPrompt() const override { return "Talk to " + Name; }

		// UI Management - delegated to DialogBox
		void ShowInteractionHint();      // Show "!" above head
		void HideInteractionHint();      // Hide "!"
		void ShowDialog();               // Show dialog panel
		void HideDialog();               // Hide dialog panel
		bool IsDialogVisible() const;
		
		// Player proximity notifications (for autonomous behavior)
		virtual void OnPlayerEnteredRange(Actor* Player);
		virtual void OnPlayerLeftRange(Actor* Player);

		// Accessors
		const string& GetName() const { return Name; }
		void SetDialog(const string& InDialog) { Dialog = InDialog; }
		void SetDialogTitle(const string& InTitle) { DialogTitle = InTitle; }

	protected:
		// Override this in derived classes to set up animations
		virtual void InitializeAnimations() = 0;

		// Helper to set up interaction sensor
		void SetupInteractionSensor();

	protected:
		shared<AnimationComponent> AnimComp;
		string Name;
		string Dialog;
		string DialogTitle;

		// UI is now managed by DialogBox
		unique<DialogBox> DialogUI;
	};
}
