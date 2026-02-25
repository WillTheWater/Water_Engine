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
	class HorizontalBox;
	struct EngineSubsystem;

	// ==========================================================================
	// DialogBox - UI class for NPC dialog interactions
	// ==========================================================================
	// Buttonless dialog - uses Enter key or Controller South button to advance.
	// Emits delegates for cursor visibility control.
	// ==========================================================================

	class DialogBox
	{
	public:
		DialogBox(EngineSubsystem& InSubsystem, EWidgetSpace InSpace = EWidgetSpace::World);
		~DialogBox();

		// Dialog content - single string or multiple pages
		void SetDialogText(const string& InText);
		void SetDialogPages(const vector<string>& InPages);
		void AddDialogPage(const string& InPage);
		void ClearDialogPages();
		
		const string& GetCurrentDialogText() const;
		int GetCurrentPageIndex() const { return CurrentPageIndex; }
		int GetTotalPages() const { return static_cast<int>(DialogPages.size()); }
		bool IsOnLastPage() const;

		// Title
		void SetTitleText(const string& InTitle);
		const string& GetTitleText() const { return NPCTitle; }

		// Visibility
		void Show();
		void Hide();
		bool IsVisible() const;
		void Toggle();

		// Input handling - call when Enter or South button is pressed
		// Returns true if dialog handled the input (was open), false otherwise
		bool HandleConfirm();

		// World-space positioning (for NPC-attached dialogs)
		void SetWorldPosition(const vec2f& Position);
		void SetLocalOffset(const vec2f& Offset) { LocalOffset = Offset; }
		const vec2f& GetLocalOffset() const { return LocalOffset; }

		// Interaction hint (the "!" above NPC's head)
		void ShowInteractionHint();
		void HideInteractionHint();
		bool IsInteractionHintVisible() const;

		// Delegates for cursor visibility control
		Delegate<> OnShown;      // Dialog was shown (show cursor)
		Delegate<> OnHidden;     // Dialog was hidden (hide cursor)

		// Dialog finished delegate (all pages viewed)
		Delegate<> OnDialogFinished;

	private:
		void CreateUI();
		void CreateInteractionHint();
		void UpdateDialogText();
		void AdvancePage();
		void ResetToFirstPage();

	private:
		EngineSubsystem& Subsystem;
		EWidgetSpace WidgetSpace;

		// Layout containers
		shared<Panel> DialogPanel;
		
		// Content widgets
		shared<TextBlock> TitleText;
		shared<TextBlock> DialogText;

		// Interaction hint
		shared<TextBlock> InteractionHint;

		// State
		vector<string> DialogPages;
		int CurrentPageIndex = 0;
		string NPCTitle;
		vec2f LocalOffset{ 0.f, 0.f };
		bool bDialogVisible = false;
		bool bIsCleaningUp = false;
	};
}
