// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/Widget/Widget.h"
#include "Utility/Delegate.h"

namespace we
{
	class Button;
	class Panel;
	class TextBlock;
	struct EngineSubsystem;

	// ==========================================================================
	// DialogBox - UI class for NPC dialog interactions
	// ==========================================================================

	class DialogBox
	{
	public:
		DialogBox(EngineSubsystem& InSubsystem, EWidgetSpace InSpace = EWidgetSpace::World);
		~DialogBox();

		// Dialog content
		void SetDialogText(const string& InText);
		const string& GetDialogText() const { return NPCDialog; }
		void SetTitleText(const string& InTitle);

		// Visibility
		void Show();
		void Hide();
		bool IsVisible() const;
		void Toggle();

		// World-space positioning (for NPC-attached dialogs)
		void SetWorldPosition(const vec2f& Position);
		void SetLocalOffset(const vec2f& Offset) { LocalOffset = Offset; }
		const vec2f& GetLocalOffset() const { return LocalOffset; }

		// Interaction hint (the "!" above NPC's head)
		void ShowInteractionHint();
		void HideInteractionHint();
		bool IsInteractionHintVisible() const;

		Delegate<> OnContinueClicked;
		Delegate<> OnCloseClicked;

	private:
		void CreateUI();
		void CreateInteractionHint();
		void OnContinueButtonClicked();
		void OnCloseButtonClicked();

	private:
		EngineSubsystem& Subsystem;
		EWidgetSpace WidgetSpace;

		// Main dialog UI
		shared<Panel> DialogPanel;
		shared<TextBlock> TitleText;
		shared<TextBlock> DialogText;
		shared<Button> ContinueButton;
		shared<Button> CloseButton;

		// Interaction hint
		shared<TextBlock> InteractionHint;

		// State
		string NPCDialog;
		string NPCTitle;
		vec2f LocalOffset{ 0.f, 0.f };
		bool bDialogVisible = false;
		bool bIsCleaningUp = false;
	};
}
