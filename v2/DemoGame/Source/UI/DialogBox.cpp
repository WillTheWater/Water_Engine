// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/DialogBox.h"
#include "UI/Widget/Button.h"
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
	// UI Creation
	// ==========================================================================

	void DialogBox::CreateUI()
	{
		// Create main dialog panel
		DialogPanel = Subsystem.GUI->CreatePanel(
			{ 300.f, 140.f },              // Size
			color{ 0, 0, 0, 200 },         // Dark background with transparency
			color{ 255, 255, 255, 255 },   // White border
			0.f,                           // No border
			WidgetSpace                    // World space
		);
		DialogPanel->SetRenderDepth(200.f);

		// Create title text (NPC name) - at top of panel
		TitleText = Subsystem.GUI->CreateTextBlock("", 280.f, 20, WidgetSpace);
		TitleText->SetRenderDepth(250.f);
		TitleText->SetColor(color{ 255, 220, 100, 255 }); // Gold color for name
		TitleText->SetAlignment(TextAlignment::Center);
		DialogPanel->AddChild(TitleText, Anchor::TopCenter, Anchor::TopCenter, { 0.f, 10.f });

		// Create dialog text - main content area
		DialogText = Subsystem.GUI->CreateTextBlock("", 280.f, 18, WidgetSpace);
		DialogText->SetRenderDepth(250.f);
		DialogText->SetColor(color::White);
		DialogText->SetAlignment(TextAlignment::Left);
		DialogPanel->AddChild(DialogText, Anchor::TopCenter, Anchor::TopCenter, { 0.f, 40.f });

		// Create Continue button - bottom right
		ContinueButton = Subsystem.GUI->CreateButton(
			"Continue", 
			vec2f{ 90.f, 28.f }, 
			color{ 50, 50, 50, 200 }, 
			color::White, 
			1.f, 
			WidgetSpace
		);
		ContinueButton->SetRenderDepth(250.f);
		ContinueButton->SetHoverSound(GC.DefaultButtonHoverSound);
		ContinueButton->SetClickSound(GC.DefaultButtonClickSound);
		ContinueButton->OnClicked.Bind(this, &DialogBox::OnContinueButtonClicked);
		DialogPanel->AddChild(ContinueButton, Anchor::BottomRight, Anchor::BottomRight, { -10.f, -8.f });

		// Create Close button - top right corner (X)
		CloseButton = Subsystem.GUI->CreateButton(
			"X", 
			vec2f{ 28.f, 28.f },
			color{ 100, 50, 50, 200 },
			color::White,
			1.f,
			WidgetSpace
		);
		CloseButton->SetRenderDepth(250.f);
		CloseButton->SetHoverSound(GC.DefaultButtonHoverSound);
		CloseButton->SetClickSound(GC.DefaultButtonClickSound);
		CloseButton->OnClicked.Bind(this, &DialogBox::OnCloseButtonClicked);
		DialogPanel->AddChild(CloseButton, Anchor::TopRight, Anchor::TopRight, { -5.f, 5.f });
	}

	void DialogBox::CreateInteractionHint()
	{
		// Create "!" text block as interaction hint
		InteractionHint = Subsystem.GUI->CreateTextBlock("!", 0.f, 36, WidgetSpace);
		InteractionHint->SetRenderDepth(300.f);
		InteractionHint->SetColor(color::White);
		InteractionHint->SetOutlineColor(color::Black);
		InteractionHint->SetOutlineThickness(1.f);
		InteractionHint->SetAlignment(TextAlignment::Center);
		InteractionHint->Hide();
	}

	// ==========================================================================
	// Content Management
	// ==========================================================================

	void DialogBox::SetDialogText(const string& InText)
	{
		NPCDialog = InText;
		if (DialogText)
		{
			DialogText->SetText(InText);
		}
	}

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
		if (DialogPanel)
		{
			DialogPanel->SetLocalOffset(Position + LocalOffset);
		}
		if (InteractionHint)
		{
			// Position hint above the dialog
			InteractionHint->SetLocalOffset({ Position.x, Position.y - 60.f });
		}
	}

	// ==========================================================================
	// Visibility
	// ==========================================================================

	void DialogBox::Show()
	{
		if (bIsCleaningUp) return;

		if (DialogPanel) DialogPanel->SetVisible(true);
		if (DialogText) DialogText->SetVisible(true);
		if (TitleText) TitleText->SetVisible(true);
		
		bDialogVisible = true;
		
		HideInteractionHint();
	}

	void DialogBox::Hide()
	{
		if (bIsCleaningUp) return;

		if (DialogPanel) DialogPanel->SetVisible(false);
		if (DialogText) DialogText->SetVisible(false);
		if (TitleText) TitleText->SetVisible(false);
		
		bDialogVisible = false;
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

	// ==========================================================================
	// Button Callbacks
	// ==========================================================================

	void DialogBox::OnContinueButtonClicked()
	{
		OnContinueClicked.Broadcast();
		Hide();
	}

	void DialogBox::OnCloseButtonClicked()
	{
		OnCloseClicked.Broadcast();
		Hide();
	}
}
