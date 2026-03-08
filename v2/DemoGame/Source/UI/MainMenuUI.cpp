// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/MainMenuUI.h"
#include "Subsystem/GuiSubsystem.h"
#include "Utility/Log.h"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>

namespace we
{
	MainMenuUI::MainMenuUI()
	{
	}
	
	void MainMenuUI::Initialize()
	{
		if (bInitialized)
			return;
		
		SetupLayout();
		bInitialized = true;
		
		LOG("[MainMenuUI] Initialized");
	}
	
	void MainMenuUI::SetupLayout()
	{
		auto& GUI = MakeGUI().GetScreenUI();
		
		// Button dimensions and spacing
		const float ButtonWidth = 250.0f;
		const float ButtonHeight = 50.0f;
		const float ButtonSpacing = 20.0f;
		const float CenterX = 810.0f - (ButtonWidth / 2.0f);
		const float StartY = 450.0f; // Lower third area
		
		// Title
		auto Title = tgui::Label::create("WATER ENGINE");
		Title->setTextSize(48);
		Title->setPosition({CenterX - 75.0f, 100.0f});
		GUI.add(Title, "Title");
		
		// Play Button
		auto Play = tgui::Button::create("PLAY");
		Play->setSize({ButtonWidth, ButtonHeight});
		Play->setPosition({CenterX, StartY});
		Play->setFocusable(false);
		Play->onPress([this]() { OnPlayClicked(); });
		GUI.add(Play, "PlayButton");
		
		// Settings Button
		auto Settings = tgui::Button::create("SETTINGS");
		Settings->setSize({ButtonWidth, ButtonHeight});
		Settings->setPosition({CenterX, StartY + ButtonHeight + ButtonSpacing});
		Settings->setFocusable(false);
		Settings->onPress([this]() { OnSettingsClicked(); });
		GUI.add(Settings, "SettingsButton");
		
		// Quit Button
		auto Quit = tgui::Button::create("QUIT");
		Quit->setSize({ButtonWidth, ButtonHeight});
		Quit->setPosition({CenterX, StartY + (ButtonHeight + ButtonSpacing) * 2.0f});
		Quit->setFocusable(false);
		Quit->onPress([this]() { OnQuitClicked(); });
		GUI.add(Quit, "QuitButton");
	}
	
	void MainMenuUI::Show()
	{
		if (!bInitialized)
			Initialize();
		
		auto& GUI = MakeGUI().GetScreenUI();
		GUI.get("Title")->setVisible(true);
		GUI.get("PlayButton")->setVisible(true);
		GUI.get("SettingsButton")->setVisible(true);
		GUI.get("QuitButton")->setVisible(true);
		
		bVisible = true;
	}
	
	void MainMenuUI::Hide()
	{
		if (!bInitialized)
			return;
		
		auto& GUI = MakeGUI().GetScreenUI();
		GUI.get("Title")->setVisible(false);
		GUI.get("PlayButton")->setVisible(false);
		GUI.get("SettingsButton")->setVisible(false);
		GUI.get("QuitButton")->setVisible(false);
		
		bVisible = false;
	}
	
	void MainMenuUI::OnPlayClicked()
	{
		LOG("[MainMenuUI] Play clicked");
	}
	
	void MainMenuUI::OnSettingsClicked()
	{
		LOG("[MainMenuUI] Settings clicked");
	}
	
	void MainMenuUI::OnQuitClicked()
	{
		LOG("[MainMenuUI] Quit clicked");
	}
}
