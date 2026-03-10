// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/MainMenuUI.h"
#include "Subsystem/GuiSubsystem.h"
#include "Utility/Log.h"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>

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
		
		// Button layout
		auto Layout = tgui::VerticalLayout::create();
		Layout->setSize({ "16%", "20%" });
		Layout->setOrigin(0.5f, 0.5f);
		Layout->setPosition("50%", "80%");
		Layout->getRenderer()->setSpaceBetweenWidgets(10);
		Layout->getRenderer()->setPadding(10);
		
		// Play Button
		auto Play = CreateButton("PLAY", tgui::Color::Green, tgui::Color::Yellow);
		Play->onPress([this]() { OnPlayClicked(); });
		Layout->add(Play, "PlayButton");
		
		// Settings Button
		auto Settings = CreateButton("SETTINGS", tgui::Color::Red, tgui::Color::Magenta);
		Settings->onPress([this]() { OnSettingsClicked(); });
		Layout->add(Settings, "SettingsButton");
		
		// Quit Button
		auto Quit = CreateButton("QUIT", tgui::Color::Blue, tgui::Color::Cyan);
		Quit->onPress([this]() { OnQuitClicked(); });
		Layout->add(Quit, "QuitButton");
		
		GUI.add(Layout, "ButtonLayout");
		
		LOG("[MainMenuUI] Layout setup complete");
	}
	
	tgui::Button::Ptr MainMenuUI::CreateButton(const std::string& Text, 
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
	
	void MainMenuUI::Show()
	{
		if (!bInitialized)
			Initialize();
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("ButtonLayout"))
			Layout->setVisible(true);
		
		bVisible = true;
	}
	
	void MainMenuUI::Hide()
	{
		if (!bInitialized)
			return;
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("ButtonLayout"))
			Layout->setVisible(false);
		
		bVisible = false;
	}
	
	void MainMenuUI::OnPlayClicked()
	{
		LOG("[MainMenuUI] Play clicked");
		OnPlayButtonClicked.Broadcast();
	}
	
	void MainMenuUI::OnSettingsClicked()
	{
		LOG("[MainMenuUI] Settings clicked");
		OnSettingsButtonClicked.Broadcast();
	}
	
	void MainMenuUI::OnQuitClicked()
	{
		LOG("[MainMenuUI] Quit clicked");
		OnQuitButtonClicked.Broadcast();
	}
}
