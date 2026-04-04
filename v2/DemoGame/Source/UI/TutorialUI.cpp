// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/TutorialUI.h"
#include "Subsystem/GuiSubsystem.h"
#include "Subsystem/CursorSubsystem.h"

namespace we
{
	TutorialUI::~TutorialUI()
	{
		if (Background)
		{
			MakeGUI().GetScreenUI().remove(Background);
		}
	}

	void TutorialUI::Initialize()
	{
		if (bInitialized)
			return;

		SetupLayout();
		bInitialized = true;
		Hide();
	}

	void TutorialUI::ClearWidgets()
	{
		if (!bInitialized)
			return;

		auto& GUI = MakeGUI().GetScreenUI();
		GUI.remove(Background);

		bInitialized = false;
		bVisible = false;
	}

	void TutorialUI::SetupLayout()
	{
		auto& GUI = MakeGUI().GetScreenUI();

		// Main background panel - 90% width and height, centered
		Background = tgui::Panel::create({ "90%", "90%" });
		Background->getRenderer()->setBackgroundColor(tgui::Color{ 0, 0, 0, 150 });
		Background->getRenderer()->setBorderColor(tgui::Color::White);
		Background->getRenderer()->setBorders(tgui::Outline(2));
		Background->setOrigin(0.5f, 0.5f);
		Background->setPosition("50%", "50%");

		// Title
		auto Title = tgui::Label::create("HOW TO PLAY");
		Title->setTextSize(32);
		Title->getRenderer()->setTextColor(tgui::Color::White);
		Title->setPosition("50%", "5%");
		Title->setOrigin(0.5f, 0.0f);
		Background->add(Title);

		// Movement Section
		auto MoveLabel = tgui::Label::create("MOVEMENT");
		MoveLabel->setTextSize(32);
		MoveLabel->getRenderer()->setTextColor(tgui::Color{ 200, 200, 200 });
		MoveLabel->setPosition("10%", "15%");
		Background->add(MoveLabel);

		auto MoveDesc = tgui::Label::create("WASD or Arrow Keys to move your character");
		MoveDesc->setTextSize(18);
		MoveDesc->getRenderer()->setTextColor(tgui::Color::White);
		MoveDesc->setPosition("15%", "23%");
		Background->add(MoveDesc);

		auto MoveController = tgui::Label::create("Controller: Use D-Pad or Left Stick to move");
		MoveController->setTextSize(18);
		MoveController->getRenderer()->setTextColor(tgui::Color::White);
		MoveController->setPosition("15%", "30%");
		Background->add(MoveController);

		// Interact Section
		auto InteractLabel = tgui::Label::create("INTERACT");
		InteractLabel->setTextSize(32);
		InteractLabel->getRenderer()->setTextColor(tgui::Color{ 200, 200, 200 });
		InteractLabel->setPosition("10%", "40%");
		Background->add(InteractLabel);

		auto InteractDesc = tgui::Label::create("E Key to interact with objects and NPCs");
		InteractDesc->setTextSize(18);
		InteractDesc->getRenderer()->setTextColor(tgui::Color::White);
		InteractDesc->setPosition("15%", "48%");
		Background->add(InteractDesc);

		auto InteractController = tgui::Label::create("Controller: South Button (A/X) to interact");
		InteractController->setTextSize(18);
		InteractController->getRenderer()->setTextColor(tgui::Color::White);
		InteractController->setPosition("15%", "55%");
		Background->add(InteractController);

		// Menu Section
		auto MenuLabel = tgui::Label::create("MENU");
		MenuLabel->setTextSize(32);
		MenuLabel->getRenderer()->setTextColor(tgui::Color{ 200, 200, 200 });
		MenuLabel->setPosition("10%", "65%");
		Background->add(MenuLabel);

		auto MenuDesc = tgui::Label::create("ESC Key to open Pause Menu");
		MenuDesc->setTextSize(18);
		MenuDesc->getRenderer()->setTextColor(tgui::Color::White);
		MenuDesc->setPosition("15%", "73%");
		Background->add(MenuDesc);

		auto MenuController = tgui::Label::create("Controller: Start Button to open Pause Menu");
		MenuController->setTextSize(18);
		MenuController->getRenderer()->setTextColor(tgui::Color::White);
		MenuController->setPosition("15%", "80%");
		Background->add(MenuController);

		// Continue button - bottom right
		ContinueButton = CreateButton("CONTINUE");
		ContinueButton->onPress([this]() { OnContinuePressed(); });
		ContinueButton->setPosition("75%", "85%");
		Background->add(ContinueButton);

		GUI.add(Background, "TutorialPanel");
	}

	tgui::Button::Ptr TutorialUI::CreateButton(const std::string& Text)
	{
		auto Button = tgui::Button::create(Text);
		Button->setTextSize(24);

		auto Renderer = Button->getRenderer();
		Renderer->setBackgroundColor(tgui::Color{ 0, 16, 31 });
		Renderer->setBackgroundColorHover(tgui::Color{ 47, 121, 142 });
		Renderer->setBackgroundColorDown(tgui::Color{ 133, 120, 81 });
		Renderer->setTextColor(tgui::Color::White);
		Renderer->setTextColorHover(tgui::Color::White);
		Renderer->setTextColorDown(tgui::Color::White);
		Renderer->setBorderColor(tgui::Color::White);
		Renderer->setBorders(tgui::Outline(2));

		return Button;
	}

	void TutorialUI::Show()
	{
		if (!bInitialized)
			Initialize();

		Background->setVisible(true);
		bVisible = true;

		if (ContinueButton)
			ContinueButton->setFocused(true);

		GetCursor().SetVisibility(true);
	}

	void TutorialUI::Hide()
	{
		if (!bInitialized)
			return;

		Background->setVisible(false);
		bVisible = false;

		// Hide cursor when closing (like PauseMenuUI)
		GetCursor().SetVisibility(false);
	}

	void TutorialUI::OnContinuePressed()
	{
		OnContinueClicked.Broadcast();
	}
}
