// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/MainMenuUI.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/Panel.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Utility/Log.h"
#include "GameConfig.h"

namespace we
{
	MainMenuUI::MainMenuUI(EngineSubsystem& InSubsystem)
		: Subsystem{ InSubsystem }
	{
		CreateUI();
	}

	MainMenuUI::~MainMenuUI() = default;

	void MainMenuUI::CreateUI()
	{
		// Create main menu panel - buttons are children
		MenuPanel = Subsystem.GUI->CreatePanel({ 400.f, 400.f }, color::White, color::Black, 4.f);
		MenuPanel->SetAnchorPosition(Anchor::Center, Anchor::Center);
		MenuPanel->SetRenderDepth(50.f);

		// Create Play button
		PlayButton = Subsystem.GUI->CreateButton("Play", vec2f{ 200.f, 50.f });
		PlayButton->SetRenderDepth(100.f);
		PlayButton->SetHoverSound(GC.DefaultButtonHoverSound);
		PlayButton->SetClickSound(GC.DefaultButtonClickSound);
		PlayButton->OnClicked.Bind(this, &MainMenuUI::OnPlayButtonClicked);
		MenuPanel->AddChild(PlayButton, Anchor::TopCenter, Anchor::TopCenter, { 0.f, 20.f });

		// Create Settings button
		SettingsButton = Subsystem.GUI->CreateButton("Settings", vec2f{ 200.f, 50.f });
		SettingsButton->SetRenderDepth(100.f);
		SettingsButton->SetHoverSound(GC.DefaultButtonHoverSound);
		SettingsButton->SetClickSound(GC.DefaultButtonClickSound);
		SettingsButton->OnClicked.Bind(this, &MainMenuUI::OnSettingsButtonClicked);
		MenuPanel->AddChild(SettingsButton, Anchor::TopCenter, Anchor::TopCenter, { 0.f, 90.f });

		// Create Quit button
		QuitButton = Subsystem.GUI->CreateButton("Quit", vec2f{ 200.f, 50.f });
		QuitButton->SetRenderDepth(100.f);
		QuitButton->SetHoverSound(GC.DefaultButtonHoverSound);
		QuitButton->SetClickSound(GC.DefaultButtonClickSound);
		QuitButton->OnClicked.Bind(this, &MainMenuUI::OnQuitButtonClicked);
		MenuPanel->AddChild(QuitButton, Anchor::TopCenter, Anchor::TopCenter, { 0.f, 160.f });
	}

	void MainMenuUI::Hide()
	{
		if (MenuPanel) MenuPanel->SetVisible(false);
	}

	void MainMenuUI::Show()
	{
		if (MenuPanel) MenuPanel->SetVisible(true);
	}

	bool MainMenuUI::IsVisible() const
	{
		return MenuPanel && MenuPanel->IsVisible();
	}

	void MainMenuUI::OnPlayButtonClicked()
	{
		OnPlayClicked.Broadcast();
	}

	void MainMenuUI::OnSettingsButtonClicked()
	{
		OnSettingsClicked.Broadcast();
	}

	void MainMenuUI::OnQuitButtonClicked()
	{
		OnQuitClicked.Broadcast();
	}
}
