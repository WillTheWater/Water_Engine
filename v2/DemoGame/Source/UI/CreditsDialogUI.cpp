// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/CreditsDialogUI.h"
#include "Subsystem/GuiSubsystem.h"
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
		Background = tgui::Panel::create({ Width, Height });
		Background->getRenderer()->setBackgroundColor({ 0, 0, 0, 220 });
		Background->getRenderer()->setBorderColor({ 255, 255, 255, 100 });
		Background->getRenderer()->setBorders({ 2 });

		// Text label for dialog content
		TextLabel = tgui::Label::create();
		TextLabel->setSize({ Width - 80, Height - 100 });
		TextLabel->setPosition({ 40, 40 });
		TextLabel->setTextSize(22);
		TextLabel->getRenderer()->setTextColor(sf::Color::White);
		TextLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		TextLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		Background->add(TextLabel);

		// Continue arrow indicator (down triangle to match DialogUI)
		ContinueArrow = tgui::Label::create("▼");
		ContinueArrow->setTextSize(18);
		ContinueArrow->getRenderer()->setTextColor({ 255, 255, 255, 255 });
		ContinueArrow->setPosition({ Width - 35, Height - 35 });
		ContinueArrow->setVisible(true);
		Background->add(ContinueArrow);

		// Buttons layout (horizontal, centered at bottom)
		ButtonsLayout = tgui::HorizontalLayout::create();
		ButtonsLayout->setSize({ 360, 45 });
		ButtonsLayout->setPosition({ (Width - 360) / 2, Height - 70 });
		ButtonsLayout->setVisible(false);
		Background->add(ButtonsLayout);

		// Main Menu button - styled to match MainMenuUI exactly
		MainMenuButton = tgui::Button::create("Main Menu");
		MainMenuButton->setTextSize(24);
		MainMenuButton->setFocusable(false);
		MainMenuButton->getRenderer()->setBackgroundColor(tgui::Color{ 0, 16, 31 });
		MainMenuButton->getRenderer()->setBackgroundColorHover(tgui::Color{ 47, 121, 142 });
		MainMenuButton->getRenderer()->setBackgroundColorDown(tgui::Color{ 133, 120, 81 });
		MainMenuButton->getRenderer()->setTextColor(tgui::Color::White);
		MainMenuButton->getRenderer()->setTextColorHover(tgui::Color::White);
		MainMenuButton->getRenderer()->setTextColorDown(tgui::Color::White);
		MainMenuButton->getRenderer()->setBorderColor(tgui::Color::White);
		MainMenuButton->getRenderer()->setBorders(tgui::Outline(2));
		MainMenuButton->onPress([this]() {
			if (OnMainMenu) OnMainMenu();
		});
		ButtonsLayout->add(MainMenuButton);

		// Spacer between buttons
		ButtonsLayout->addSpace(0.2f);

		// Quit button - styled to match MainMenuUI exactly
		QuitButton = tgui::Button::create("Quit");
		QuitButton->setTextSize(24);
		QuitButton->setFocusable(false);
		QuitButton->getRenderer()->setBackgroundColor(tgui::Color{ 0, 16, 31 });
		QuitButton->getRenderer()->setBackgroundColorHover(tgui::Color{ 47, 121, 142 });
		QuitButton->getRenderer()->setBackgroundColorDown(tgui::Color{ 133, 120, 81 });
		QuitButton->getRenderer()->setTextColor(tgui::Color::White);
		QuitButton->getRenderer()->setTextColorHover(tgui::Color::White);
		QuitButton->getRenderer()->setTextColorDown(tgui::Color::White);
		QuitButton->getRenderer()->setBorderColor(tgui::Color::White);
		QuitButton->getRenderer()->setBorders(tgui::Outline(2));
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
	}

	void CreditsDialogUI::Hide()
	{
		if (Background && bInitialized)
		{
			Background->setVisible(false);
		}
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
	}

	void CreditsDialogUI::HideEndButtons()
	{
		ContinueArrow->setVisible(true);
		ButtonsLayout->setVisible(false);
	}
}
