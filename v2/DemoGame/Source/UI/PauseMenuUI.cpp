// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/PauseMenuUI.h"
#include "UI/UIStyle.h"
#include "Subsystem/GuiSubsystem.h"
#include "Subsystem/SaveSubsystem.h"
#include "Subsystem/CursorSubsystem.h"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>

namespace we
{
	PauseMenuUI::PauseMenuUI()
	{
	}
	
	void PauseMenuUI::Initialize(SaveSubsystem& InSave)
	{
		if (bInitialized)
			return;
		
		Save = &InSave;
		SetupLayout();
		
		// Settings UI
		SettingsMenu = make_unique<SettingsUI>();
		SettingsMenu->Initialize(InSave);
		SettingsMenu->OnBackClicked.Bind(this, &PauseMenuUI::OnSettingsClosed);
		
		bInitialized = true;
		Hide();
	}
	
	void PauseMenuUI::ClearWidgets()
	{
		if (!bInitialized)
			return;
		
		if (SettingsMenu)
			SettingsMenu->ClearWidgets();
		
		auto& GUI = MakeGUI().GetScreenUI();
		GUI.remove(GUI.get("PauseLayout"));
		
		bInitialized = false;
		bVisible = false;
	}
	
	void PauseMenuUI::SetupLayout()
	{
		auto& GUI = MakeGUI().GetScreenUI();
		
		// Button layout
		auto Layout = UIStyle::CreateVerticalLayout();
		Layout->setSize({ "16%", "20%" });
		Layout->setOrigin(0.5f, 0.5f);
		Layout->setPosition("50%", "50%");  // Center of screen
		
		// Resume Button
		auto Resume = UIStyle::CreateButton("RESUME");
		Resume->onPress([this]() { OnResume(); });
		Layout->add(Resume, "ResumeButton");
		
		// Settings Button
		auto Settings = UIStyle::CreateButton("SETTINGS");
		Settings->onPress([this]() { OnSettings(); });
		Layout->add(Settings, "SettingsButton");
		
		// Main Menu Button
		auto MainMenu = UIStyle::CreateButton("MAIN MENU");
		MainMenu->onPress([this]() { OnMainMenu(); });
		Layout->add(MainMenu, "MainMenuButton");
		
		// Save & Quit Button
		auto SaveQuit = UIStyle::CreateButton("SAVE & QUIT");
		SaveQuit->onPress([this]() { OnSaveAndQuit(); });
		Layout->add(SaveQuit, "SaveQuitButton");
		
		GUI.add(Layout, "PauseLayout");
	}
	
	void PauseMenuUI::Show()
	{
		if (!bInitialized)
			return;
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("PauseLayout"))
			Layout->setVisible(true);
		
		bVisible = true;
		bInSettings = false;
		GetCursor().SetVisibility(true);
		Controller.Enable();
	}
	
	void PauseMenuUI::Hide()
	{
		if (!bInitialized)
			return;
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("PauseLayout"))
			Layout->setVisible(false);
		
		if (SettingsMenu)
			SettingsMenu->Hide();
		
		bVisible = false;
		bInSettings = false;
		GetCursor().SetVisibility(false);
		Controller.Disable();
	}
	
	void PauseMenuUI::Tick(float DeltaTime)
	{
		if (!bVisible)
			return;
		
		Controller.Tick(DeltaTime);
	}
	
	void PauseMenuUI::Toggle()
	{
		if (bVisible)
		{
			if (bInSettings && SettingsMenu)
			{
				SettingsMenu->Hide();
				bInSettings = false;
				Show();
			}
			else
			{
				Hide();
			}
		}
		else
		{
			Show();
		}
	}
	
	void PauseMenuUI::OnResume()
	{
		OnResumeClicked.Broadcast();
	}
	
	void PauseMenuUI::OnSettings()
	{
		bInSettings = true;
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("PauseLayout"))
			Layout->setVisible(false);
		
		if (SettingsMenu)
			SettingsMenu->Show();
		
		OnSettingsClicked.Broadcast();
	}
	
	void PauseMenuUI::OnSettingsClosed()
	{
		bInSettings = false;
		
		if (SettingsMenu)
			SettingsMenu->Hide();
		
		auto& GUI = MakeGUI().GetScreenUI();
		if (auto Layout = GUI.get("PauseLayout"))
			Layout->setVisible(true);
	}
	
	void PauseMenuUI::OnMainMenu()
	{
		OnMainMenuClicked.Broadcast();
	}
	
	void PauseMenuUI::OnSaveAndQuit()
	{
		OnSaveAndQuitClicked.Broadcast();
	}
}
