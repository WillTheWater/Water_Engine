// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/MainMenuUI.h"
#include "UI/UIStyle.h"
#include "Subsystem/GuiSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
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
	}
	
	void MainMenuUI::ClearWidgets()
	{
		if (!bInitialized)
			return;
		
		auto& GUI = MakeGUI().GetScreenUI();
		GUI.remove(GUI.get("ButtonLayout"));
		
		bInitialized = false;
		bVisible = false;
	}
	
	void MainMenuUI::SetupLayout()
	{
		auto& GUI = MakeGUI().GetScreenUI();
		
		// Button layout
		auto Layout = UIStyle::CreateVerticalLayout();
		Layout->setSize({ "16%", "20%" });
		Layout->setOrigin(0.5f, 0.5f);
		Layout->setPosition("50%", "80%");
		
		// Play Button
		auto Play = UIStyle::CreateButton("PLAY");
		Play->onPress([this]() { OnPlayClicked(); });
		Layout->add(Play, "PlayButton");
		
		// Settings Button
		auto Settings = UIStyle::CreateButton("SETTINGS");
		Settings->onPress([this]() { OnSettingsClicked(); });
		Layout->add(Settings, "SettingsButton");
		
		// Quit Button
		auto Quit = UIStyle::CreateButton("QUIT");
		Quit->onPress([this]() { OnQuitClicked(); });
		Layout->add(Quit, "QuitButton");
		
		GUI.add(Layout, "ButtonLayout");
	}
	
	void MainMenuUI::Show()
	{
		if (!bInitialized)
			Initialize();
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("ButtonLayout"))
			Layout->setVisible(true);
		
		bVisible = true;
		Controller.Enable();
	}
	
	void MainMenuUI::Hide()
	{
		if (!bInitialized)
			return;
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("ButtonLayout"))
			Layout->setVisible(false);
		
		bVisible = false;
		Controller.Disable();
	}
	
	void MainMenuUI::Tick(float DeltaTime)
	{
		if (!bVisible)
			return;
		
		Controller.Tick(DeltaTime);
	}
	
	void MainMenuUI::OnPlayClicked()
	{
		OnPlayButtonClicked.Broadcast();
	}
	
	void MainMenuUI::OnSettingsClicked()
	{
		OnSettingsButtonClicked.Broadcast();
	}
	
	void MainMenuUI::OnQuitClicked()
	{
		OnQuitButtonClicked.Broadcast();
	}
}
