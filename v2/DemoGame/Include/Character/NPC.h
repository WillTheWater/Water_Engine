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
#include "UI/DialogBox.h"

namespace we
{
	class AnimationComponent;

	// =============================================================================
	// Base NPC Class
	// =============================================================================
	// Abstract base class for all NPCs. Provides interaction system and dialog
	// functionality. UI creation is delegated to DialogBox class.
	// Supports paginated dialogs (multiple pages of text).
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
		
		// Dialog input handling - returns true if dialog is visible and handled input
		bool HandleDialogConfirm();

		// Player proximity notifications
		virtual void OnPlayerEnteredRange(Actor* Player);
		virtual void OnPlayerLeftRange(Actor* Player);

		// Dialog content - single text or multiple pages
		const string& GetName() const { return Name; }
		
		// Set a single dialog (creates one page)
		void SetDialog(const string& InDialog);
		
		// Set multiple dialog pages (for longer conversations)
		void SetDialogPages(const vector<string>& InPages);
		void AddDialogPage(const string& InPage);
		void ClearDialogPages();

		// Check if this NPC has any dialog set
		bool HasDialog() const { return !PendingDialogPages.empty(); }
		int GetDialogPageCount() const { return static_cast<int>(PendingDialogPages.size()); }

		// Dialog delegates - forward from DialogBox
		Delegate<>& OnDialogShown() { return DialogUI ? DialogUI->OnShown : EmptyDelegate; }
		Delegate<>& OnDialogHidden() { return DialogUI ? DialogUI->OnHidden : EmptyDelegate; }

	protected:
		// Override this in derived classes to set up animations
		virtual void InitializeAnimations() = 0;
		void SetupInteractionSensor();

	protected:
		shared<AnimationComponent> AnimComp;
		string Name;
		unique<DialogBox> DialogUI;
		
		// Dialog pages stored here until DialogBox is created in BeginPlay
		vector<string> PendingDialogPages;
		string PendingDialogTitle;
		
	private:
		static Delegate<> EmptyDelegate;
	};
}
