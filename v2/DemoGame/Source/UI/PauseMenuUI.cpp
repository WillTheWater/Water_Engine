// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/PauseMenuUI.h"
#include "Subsystem/GuiSubsystem.h"
#include "Utility/Log.h"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>

namespace we
{
	PauseMenuUI::PauseMenuUI()
	{
	}
	
	void PauseMenuUI::Initialize()
	{
		if (bInitialized)
			return;
		
		SetupLayout();
		bInitialized = true;  // Set BEFORE calling Hide()
		Hide();  // Start hidden
		
		LOG("[PauseMenuUI] Initialized");
	}
	
	void PauseMenuUI::ClearWidgets()
	{
		if (!bInitialized)
			return;
		
		auto& GUI = MakeGUI().GetScreenUI();
		GUI.remove(GUI.get("PauseLayout"));
		
		bInitialized = false;
		bVisible = false;
		
		LOG("[PauseMenuUI] Widgets cleared");
	}
	
	void PauseMenuUI::SetupLayout()
	{
		auto& GUI = MakeGUI().GetScreenUI();
		
		// Button layout
		auto Layout = tgui::VerticalLayout::create();
		Layout->setSize({ "16%", "20%" });
		Layout->setOrigin(0.5f, 0.5f);
		Layout->setPosition("50%", "50%");  // Center of screen
		Layout->getRenderer()->setSpaceBetweenWidgets(10);
		Layout->getRenderer()->setPadding(10);
		
		// Resume Button
		auto Resume = CreateButton("RESUME", tgui::Color::Green, tgui::Color::Yellow);
		Resume->onPress([this]() { OnResume(); });
		Layout->add(Resume, "ResumeButton");
		
		// Settings Button
		auto Settings = CreateButton("SETTINGS", tgui::Color::Red, tgui::Color::Magenta);
		Settings->onPress([this]() { OnSettings(); });
		Layout->add(Settings, "SettingsButton");
		
		// Main Menu Button
		auto MainMenu = CreateButton("MAIN MENU", tgui::Color::Blue, tgui::Color::Cyan);
		MainMenu->onPress([this]() { OnMainMenu(); });
		Layout->add(MainMenu, "MainMenuButton");
		
		// Save & Quit Button
		auto SaveQuit = CreateButton("SAVE & QUIT", tgui::Color::Red, tgui::Color::Magenta);
		SaveQuit->onPress([this]() { OnSaveAndQuit(); });
		Layout->add(SaveQuit, "SaveQuitButton");
		
		GUI.add(Layout, "PauseLayout");
		
		LOG("[PauseMenuUI] Layout setup complete");
	}
	
	tgui::Button::Ptr PauseMenuUI::CreateButton(const std::string& Text, 
		tgui::Color HoverColor, tgui::Color DownColor)
	{
		auto Button = tgui::Button::create(Text);
		Button->setTextSize(24);
		Button->setFocusable(false);
		
		auto Renderer = Button->getRenderer();
		Renderer->setBackgroundColor(tgui::Color::Black);
		Renderer->setBackgroundColorHover(HoverColor);
		Renderer->setBackgroundColorDown(DownColor);
		Renderer->setTextColor(tgui::Color::White);
		Renderer->setTextColorHover(tgui::Color::White);
		Renderer->setTextColorDown(tgui::Color::White);
		Renderer->setBorderColor(tgui::Color::White);
		Renderer->setBorders(tgui::Outline(2));
		
		return Button;
	}
	
	void PauseMenuUI::Show()
	{
		if (!bInitialized)
			Initialize();
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("PauseLayout"))
			Layout->setVisible(true);
		
		bVisible = true;
	}
	
	void PauseMenuUI::Hide()
	{
		if (!bInitialized)
			return;
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("PauseLayout"))
			Layout->setVisible(false);
		
		bVisible = false;
	}
	
	void PauseMenuUI::Toggle()
	{
		if (bVisible)
			Hide();
		else
			Show();
	}
	
	void PauseMenuUI::OnResume()
	{
		LOG("[PauseMenuUI] Resume clicked");
		OnResumeClicked.Broadcast();
	}
	
	void PauseMenuUI::OnSettings()
	{
		LOG("[PauseMenuUI] Settings clicked");
		OnSettingsClicked.Broadcast();
	}
	
	void PauseMenuUI::OnMainMenu()
	{
		LOG("[PauseMenuUI] Main Menu clicked");
		OnMainMenuClicked.Broadcast();
	}
	
	void PauseMenuUI::OnSaveAndQuit()
	{
		LOG("[PauseMenuUI] Save & Quit clicked");
		OnSaveAndQuitClicked.Broadcast();
	}
}
