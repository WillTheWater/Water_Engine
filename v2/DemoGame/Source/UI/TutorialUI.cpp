// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/TutorialUI.h"
#include "UI/UIStyle.h"
#include "Subsystem/GuiSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/WorldSubsystem.h"

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
		Background = UIStyle::CreatePanel({ "90%", "90%" });
		Background->setOrigin(0.5f, 0.5f);
		Background->setPosition("50%", "50%");

		// Title
		auto Title = UIStyle::CreateLabel("HOW TO PLAY", UILabelStyle::Title);
		Title->setPosition("50%", "5%");
		Title->setOrigin(0.5f, 0.0f);
		Background->add(Title);

		// Movement Section
		auto MoveLabel = UIStyle::CreateLabel("MOVE", UILabelStyle::Section);
		MoveLabel->setPosition("10%", "15%");
		Background->add(MoveLabel);

		auto MoveDesc = UIStyle::CreateLabel("WASD or Arrow Keys", UILabelStyle::Body);
		MoveDesc->setPosition("15%", "23%");
		Background->add(MoveDesc);

		auto MoveController = UIStyle::CreateLabel("Controller: Left Stick", UILabelStyle::Body);
		MoveController->setPosition("15%", "30%");
		Background->add(MoveController);

		// Interact Section
		auto InteractLabel = UIStyle::CreateLabel("INTERACT", UILabelStyle::Section);
		InteractLabel->setPosition("10%", "40%");
		Background->add(InteractLabel);

		auto InteractDesc = UIStyle::CreateLabel("E", UILabelStyle::Body);
		InteractDesc->setPosition("15%", "48%");
		Background->add(InteractDesc);

		auto InteractController = UIStyle::CreateLabel("Controller: Facebutton South", UILabelStyle::Body);
		InteractController->setPosition("15%", "55%");
		Background->add(InteractController);

		// Menu Section
		auto MenuLabel = UIStyle::CreateLabel("PAUSE", UILabelStyle::Section);
		MenuLabel->setPosition("10%", "65%");
		Background->add(MenuLabel);

		auto MenuDesc = UIStyle::CreateLabel("ESC", UILabelStyle::Body);
		MenuDesc->setPosition("15%", "73%");
		Background->add(MenuDesc);

		auto MenuController = UIStyle::CreateLabel("Controller: Start", UILabelStyle::Body);
		MenuController->setPosition("15%", "80%");
		Background->add(MenuController);

		// Continue button - bottom right
		ContinueButton = UIStyle::CreateButton("CONTINUE");
		ContinueButton->onPress([this]() { OnContinuePressed(); });
		ContinueButton->setPosition("75%", "85%");
		Background->add(ContinueButton);

		GUI.add(Background, "TutorialPanel");
	}

	void TutorialUI::Show()
	{
		if (!bInitialized)
			Initialize();

		Background->setVisible(true);
		bVisible = true;

		GetCursor().SetVisibility(true);
		Controller.Enable();
	}

	void TutorialUI::Hide()
	{
		if (!bInitialized)
			return;

		Background->setVisible(false);
		bVisible = false;

		GetCursor().SetVisibility(false);
		Controller.Disable();
	}

	void TutorialUI::Tick(float DeltaTime)
	{
		if (!bVisible)
			return;

		Controller.Tick(DeltaTime);
	}

	void TutorialUI::OnContinuePressed()
	{
		OnContinueClicked.Broadcast();
	}
}
