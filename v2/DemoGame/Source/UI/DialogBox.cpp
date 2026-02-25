// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/DialogBox.h"
#include "UI/Widget/Panel.h"
#include "UI/Widget/TextBlock.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	// ==========================================================================
	// Constructor / Destructor
	// ==========================================================================

	DialogBox::DialogBox(EngineSubsystem& InSubsystem, EWidgetSpace InSpace)
		: Subsystem{ InSubsystem }
		, WidgetSpace{ InSpace }
	{
		CreateUI();
		CreateInteractionHint();
		Hide();
	}

	DialogBox::~DialogBox()
	{
		bIsCleaningUp = true;
	}

	// ==========================================================================
	// UI Creation - Buttonless Design
	// ==========================================================================
	// Dialog uses Enter key or Controller South button to advance.
	// No on-screen buttons - cleaner look for keyboard/controller users.
	// ==========================================================================

	void DialogBox::CreateUI()
	{
		// Create main dialog panel
		DialogPanel = Subsystem.GUI->CreatePanel(
			{ 320.f, 140.f },              // Compact size without buttons
			color{ 20, 20, 30, 230 },      // Dark blue-gray background
			color{ 180, 160, 120, 255 },   // Gold/bronze border
			3.f,
			WidgetSpace
		);
		DialogPanel->SetRenderDepth(200.f);

		// =============================================================
		// TITLE - Centered at top
		// =============================================================
		TitleText = Subsystem.GUI->CreateTextBlock("NPC Name", 0.f, 18, WidgetSpace);
		TitleText->SetRenderDepth(250.f);
		TitleText->SetColor(color{ 255, 220, 100, 255 }); // Gold
		TitleText->SetAlignment(TextAlignment::Center);
		DialogPanel->AddChild(TitleText, Anchor::TopCenter, Anchor::TopCenter, { 0.f, 6.f });

		// =============================================================
		// DIALOG TEXT - Left aligned below title
		// =============================================================
		DialogText = Subsystem.GUI->CreateTextBlock("", 290.f, 16, WidgetSpace);
		DialogText->SetRenderDepth(250.f);
		DialogText->SetColor(color::White);
		DialogText->SetAlignment(TextAlignment::Left);
		DialogText->SetWrapWidth(290.f);
		DialogPanel->AddChild(DialogText, Anchor::TopLeft, Anchor::TopLeft, { 15.f, 30.f });
	}

	void DialogBox::CreateInteractionHint()
	{
		InteractionHint = Subsystem.GUI->CreateTextBlock("!", 0.f, 32, WidgetSpace);
		InteractionHint->SetRenderDepth(300.f);
		InteractionHint->SetColor(color::White);
		InteractionHint->SetOutlineColor(color::Black);
		InteractionHint->SetOutlineThickness(1.f);
		InteractionHint->SetAlignment(TextAlignment::Center);
		InteractionHint->Hide();
	}

	// ==========================================================================
	// Dialog Content Management
	// ==========================================================================

	void DialogBox::SetDialogText(const string& InText)
	{
		DialogPages.clear();
		DialogPages.push_back(InText);
		CurrentPageIndex = 0;
		UpdateDialogText();
	}

	void DialogBox::SetDialogPages(const vector<string>& InPages)
	{
		DialogPages = InPages;
		CurrentPageIndex = 0;
		UpdateDialogText();
	}

	void DialogBox::AddDialogPage(const string& InPage)
	{
		bool wasEmpty = DialogPages.empty();
		DialogPages.push_back(InPage);
		if (wasEmpty)
		{
			CurrentPageIndex = 0;
			UpdateDialogText();
		}
	}

	void DialogBox::ClearDialogPages()
	{
		DialogPages.clear();
		CurrentPageIndex = 0;
		if (DialogText)
		{
			DialogText->SetText("");
		}
	}

	const string& DialogBox::GetCurrentDialogText() const
	{
		if (CurrentPageIndex >= 0 && CurrentPageIndex < static_cast<int>(DialogPages.size()))
		{
			return DialogPages[CurrentPageIndex];
		}
		static const string EmptyString;
		return EmptyString;
	}

	bool DialogBox::IsOnLastPage() const
	{
		return DialogPages.empty() || CurrentPageIndex >= static_cast<int>(DialogPages.size()) - 1;
	}

	void DialogBox::UpdateDialogText()
	{
		if (DialogText)
		{
			DialogText->SetText(GetCurrentDialogText());
		}
	}

	void DialogBox::AdvancePage()
	{
		if (!IsOnLastPage())
		{
			CurrentPageIndex++;
			UpdateDialogText();
		}
	}

	void DialogBox::ResetToFirstPage()
	{
		CurrentPageIndex = 0;
		UpdateDialogText();
	}

	// ==========================================================================
	// Input Handling
	// ==========================================================================

	bool DialogBox::HandleConfirm()
	{
		if (!IsVisible())
		{
			return false;
		}

		if (IsOnLastPage())
		{
			// On last page, close the dialog
			Hide();
			OnDialogFinished.Broadcast();
		}
		else
		{
			// Advance to next page
			AdvancePage();
		}

		return true;
	}

	// ==========================================================================
	// Title
	// ==========================================================================

	void DialogBox::SetTitleText(const string& InTitle)
	{
		NPCTitle = InTitle;
		if (TitleText)
		{
			TitleText->SetText(InTitle);
		}
	}

	// ==========================================================================
	// Positioning (for world-space dialogs)
	// ==========================================================================

	void DialogBox::SetWorldPosition(const vec2f& Position)
	{
		vec2f FinalPosition = Position + LocalOffset;
		
		if (DialogPanel)
		{
			DialogPanel->SetLocalOffset(FinalPosition);
		}
		if (InteractionHint)
		{
			InteractionHint->SetLocalOffset({ Position.x, Position.y - 70.f });
		}
	}

	// ==========================================================================
	// Visibility
	// ==========================================================================

	void DialogBox::Show()
	{
		if (bIsCleaningUp) return;

		ResetToFirstPage();

		if (DialogPanel) DialogPanel->SetVisible(true);
		bDialogVisible = true;
		
		HideInteractionHint();
		OnShown.Broadcast();
	}

	void DialogBox::Hide()
	{
		if (bIsCleaningUp) return;

		if (DialogPanel) DialogPanel->SetVisible(false);
		bDialogVisible = false;
		OnHidden.Broadcast();
	}

	bool DialogBox::IsVisible() const
	{
		return bDialogVisible;
	}

	void DialogBox::Toggle()
	{
		if (IsVisible())
		{
			Hide();
		}
		else
		{
			Show();
		}
	}

	// ==========================================================================
	// Interaction Hint
	// ==========================================================================

	void DialogBox::ShowInteractionHint()
	{
		if (InteractionHint && !bDialogVisible)
		{
			InteractionHint->Show();
		}
	}

	void DialogBox::HideInteractionHint()
	{
		if (InteractionHint)
		{
			InteractionHint->Hide();
		}
	}

	bool DialogBox::IsInteractionHintVisible() const
	{
		return InteractionHint && InteractionHint->IsVisible();
	}
}
