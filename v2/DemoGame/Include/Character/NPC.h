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
#include "UI/Widget/Panel.h"
#include "UI/Widget/TextBlock.h"

namespace we
{
	class AnimationComponent;

	// =============================================================================
	// Base NPC Class
	// =============================================================================
	// Abstract base class for all NPCs. Provides interaction system, UI hints,
	// and dialog boxes. Derived classes implement their own animation setups.
	// =============================================================================

	class NPC : public Character, public IInteractive
	{
	public:
		NPC(World* OwningWorld, const string& TexturePath, const string& InName);
		virtual ~NPC() = default;

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Destroy() override;

		// IInteractive interface
		virtual void OnInteract(Actor* Interactor) override;
		virtual bool CanInteract() const override { return true; }
		virtual string GetInteractionPrompt() const override { return "Talk to " + Name; }

		// UI Management
		void ShowInteractionHint();      // Show "!" above head
		void HideInteractionHint();      // Hide "!"
		void ShowDialog();               // Show dialog panel
		void HideDialog();               // Hide dialog panel
		bool IsDialogVisible() const;

		// Accessors
		const string& GetName() const { return Name; }
		void SetDialog(const string& InDialog) { Dialog = InDialog; }

	protected:
		// Override this in derived classes to set up animations
		virtual void InitializeAnimations() = 0;

		// Helper to set up interaction sensor
		void SetupInteractionSensor();

		// UI Creation
		void CreateInteractionHint();
		void CreateDialogPanel();
		void UpdateUIPositions();  // Keep UI positioned above NPC

	protected:
		shared<AnimationComponent> AnimComp;
		string Name;
		string Dialog;

		// UI Elements
		shared<TextBlock> InteractionHint;  // The "!" indicator
		shared<Panel> DialogPanel;          // Dialog box panel
		shared<TextBlock> DialogText;       // Dialog text
		bool bDialogVisible = false;
	};
}
