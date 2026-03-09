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
	}
	
	void MainMenuUI::SetupLayout()
	{
		auto& GUI = MakeGUI().GetScreenUI();
		
		// Render target is 1920x1080
		const float ScreenWidth = 1920.0f;
		const float ScreenHeight = 1080.0f;
		
		// Button size
		const float ButtonWidth = 250.0f;
		const float ButtonHeight = 50.0f;
		
		// Percentage positions
		const float TitleY = ScreenHeight * 0.10f;   
		const float PlayY = ScreenHeight * 0.65f;    
		const float SettingsY = ScreenHeight * 0.75f;
		const float QuitY = ScreenHeight * 0.85f;    
		const float CenterX = ScreenWidth * 0.50f;   
				
		// Play Button
		auto Play = tgui::Button::create("PLAY");
		Play->setSize({ButtonWidth, ButtonHeight});
		Play->setPosition({CenterX - ButtonWidth / 2.0f, PlayY});
		Play->setFocusable(false);
		Play->setTextSize(24);
		
		// Colors for Play button
		auto playRenderer = Play->getRenderer();
		playRenderer->setBackgroundColor(tgui::Color::Black);
		playRenderer->setBackgroundColorHover(tgui::Color::Green);
		playRenderer->setBackgroundColorDown(tgui::Color::Yellow);
		playRenderer->setTextColor(tgui::Color::White);
		playRenderer->setTextColorHover(tgui::Color::Black);
		playRenderer->setTextColorDown(tgui::Color::Black);
		playRenderer->setBorderColor(tgui::Color::White);
		playRenderer->setBorders(tgui::Outline(2));
		
		Play->onPress([this]() { OnPlayClicked(); });
		GUI.add(Play, "PlayButton");
		
		// Settings Button
		auto Settings = tgui::Button::create("SETTINGS");
		Settings->setSize({ButtonWidth, ButtonHeight});
		Settings->setPosition({CenterX - ButtonWidth / 2.0f, SettingsY});
		Settings->setFocusable(false);
		Settings->setTextSize(24);
		
		// Colors for Settings button
		auto settingsRenderer = Settings->getRenderer();
		settingsRenderer->setBackgroundColor(tgui::Color::Black);
		settingsRenderer->setBackgroundColorHover(tgui::Color::Red);
		settingsRenderer->setBackgroundColorDown(tgui::Color::Magenta);
		settingsRenderer->setTextColor(tgui::Color::White);
		settingsRenderer->setTextColorHover(tgui::Color::White);
		settingsRenderer->setTextColorDown(tgui::Color::White);
		settingsRenderer->setBorderColor(tgui::Color::White);
		settingsRenderer->setBorders(tgui::Outline(2));
		
		Settings->onPress([this]() { OnSettingsClicked(); });
		GUI.add(Settings, "SettingsButton");
		
		// Quit Button
		auto Quit = tgui::Button::create("QUIT");
		Quit->setSize({ButtonWidth, ButtonHeight});
		Quit->setPosition({CenterX - ButtonWidth / 2.0f, QuitY});
		Quit->setFocusable(false);
		Quit->setTextSize(24);
		
		// Colors for Quit button
		auto quitRenderer = Quit->getRenderer();
		quitRenderer->setBackgroundColor(tgui::Color::Black);
		quitRenderer->setBackgroundColorHover(tgui::Color::Blue);
		quitRenderer->setBackgroundColorDown(tgui::Color::Cyan);
		quitRenderer->setTextColor(tgui::Color::White);
		quitRenderer->setTextColorHover(tgui::Color::White);
		quitRenderer->setTextColorDown(tgui::Color::Black);
		quitRenderer->setBorderColor(tgui::Color::White);
		quitRenderer->setBorders(tgui::Outline(2));
		
		Quit->onPress([this]() { OnQuitClicked(); });
		GUI.add(Quit, "QuitButton");
	}
	
	void MainMenuUI::Show()
	{
		if (!bInitialized)
			Initialize();
		
		auto& GUI = MakeGUI().GetScreenUI();
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
