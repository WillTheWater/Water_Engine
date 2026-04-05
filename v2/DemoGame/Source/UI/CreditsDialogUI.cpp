// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/CreditsDialogUI.h"
#include "UI/UIStyle.h"
#include "Subsystem/GuiSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	CreditsDialogUI::CreditsDialogUI() = default;

	CreditsDialogUI::~CreditsDialogUI()
	{
		if (Background)
		{
			MakeGUI().GetWorldUI().remove(Background);
		}
	}

	void CreditsDialogUI::Initialize()
	{
		if (bInitialized) return;

		// Main background panel
		Background = UIStyle::CreatePanel({ Width, Height });
		Background->getRenderer()->setBorderColor({ 255, 255, 255, 100 });

		// Text label for dialog content
		TextLabel = UIStyle::CreateLabel("", UILabelStyle::Body);
		TextLabel->setSize({ Width - 80, Height - 100 });
		TextLabel->setPosition({ 40, 40 });
		TextLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		TextLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		Background->add(TextLabel);

		// Continue arrow indicator (down triangle to match DialogUI)
		ContinueArrow = UIStyle::CreateLabel("▼", UILabelStyle::Tiny);
		ContinueArrow->setPosition({ Width - 35, Height - 35 });
		ContinueArrow->setVisible(true);
		Background->add(ContinueArrow);

		// Buttons layout (horizontal, centered at bottom)
		ButtonsLayout = UIStyle::CreateHorizontalLayout();
		ButtonsLayout->setSize({ 360, 45 });
		ButtonsLayout->setPosition({ (Width - 360) / 2, Height - 70 });
		ButtonsLayout->setVisible(false);
		Background->add(ButtonsLayout);

		// Main Menu button
		MainMenuButton = UIStyle::CreateButton("Main Menu");
		MainMenuButton->onPress([this]() {
			if (OnMainMenu) OnMainMenu();
		});
		ButtonsLayout->add(MainMenuButton);

		// Spacer between buttons
		ButtonsLayout->addSpace(0.2f);

		// Quit button
		QuitButton = UIStyle::CreateButton("Quit");
		QuitButton->onPress([this]() {
			if (OnQuit) OnQuit();
		});
		ButtonsLayout->add(QuitButton);

		// Center the dialog on screen
		Background->setOrigin(0.5f, 0.5f);
		auto WindowSize = MakeGUI().GetWorldUI().getView().getSize();
		Background->setPosition({ WindowSize.x / 2, WindowSize.y / 2 + 100 });

		MakeGUI().GetWorldUI().add(Background);
		Background->setVisible(false);

		bInitialized = true;
	}

	void CreditsDialogUI::Show()
	{
		if (Background && bInitialized)
		{
			Background->setVisible(true);
		}
		Controller.Enable();
	}

	void CreditsDialogUI::Hide()
	{
		if (Background && bInitialized)
		{
			Background->setVisible(false);
		}
		GetCursor().SetVisibility(false);
		Controller.Disable();
	}

	void CreditsDialogUI::Tick(float DeltaTime)
	{
		if (!IsVisible())
			return;
		
		Controller.Tick(DeltaTime);
	}

	bool CreditsDialogUI::IsVisible() const
	{
		return Background && Background->isVisible();
	}

	void CreditsDialogUI::SetDialog(const vector<string>& Lines)
	{
		DialogLines = Lines;
		CurrentLine = 0;
		bComplete = false;
		HideEndButtons();
		DisplayCurrentLine();
	}

	bool CreditsDialogUI::Advance()
	{
		if (bComplete)
		{
			return false;
		}

		CurrentLine++;

		if (CurrentLine >= DialogLines.size())
		{
			// Dialog complete - show buttons
			bComplete = true;
			ShowEndButtons();
			return false;
		}

		DisplayCurrentLine();
		return true;
	}

	bool CreditsDialogUI::IsComplete() const
	{
		return bComplete;
	}

	void CreditsDialogUI::SetOnMainMenu(std::function<void()> Callback)
	{
		OnMainMenu = Callback;
	}

	void CreditsDialogUI::SetOnQuit(std::function<void()> Callback)
	{
		OnQuit = Callback;
	}

	void CreditsDialogUI::DisplayCurrentLine()
	{
		if (TextLabel && CurrentLine < DialogLines.size())
		{
			TextLabel->setText(DialogLines[CurrentLine]);
		}
	}

	void CreditsDialogUI::ShowEndButtons()
	{
		// Hide continue arrow
		ContinueArrow->setVisible(false);

		// Show buttons
		ButtonsLayout->setVisible(true);

		// Show cursor when buttons appear
		GetCursor().SetVisibility(true);
	}

	void CreditsDialogUI::HideEndButtons()
	{
		ContinueArrow->setVisible(true);
		ButtonsLayout->setVisible(false);

		// Hide cursor in dialog mode
		GetCursor().SetVisibility(false);
	}
}
