// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/MainMenuUI.h"
#include "UI/Widget/Panel.h"
#include "UI/Widget/Button.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"

namespace we
{
	MainMenuUI::MainMenuUI(EngineSubsystem& Subsystem)
		: Subsystem{ Subsystem }
	{
		CreateUI();
	}

	MainMenuUI::~MainMenuUI()
	{
		if (MenuPanel)
		{
			Subsystem.GUI->RemoveWidget(MenuPanel.get());
		}
	}

	void MainMenuUI::CreateUI()
	{
		// Create panel - size from texture, origin centered
		MenuPanel = make_shared<Panel>(EC.DefaultPanel);
		MenuPanel->SetPosition({ EC.WindowSize.x / 2.f, EC.WindowSize.y * 0.83f });

		// Play button - size from texture, origin centered
		PlayButton = make_shared<Button>("Play", EC.DefaultButton);
		PlayButton->SetPosition({ 0, -80 });
		PlayButton->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);

		// Quit button
		QuitButton = make_shared<Button>("Quit", EC.DefaultButton);
		QuitButton->SetPosition({ 0, 80 });
		QuitButton->OnClicked.Bind(this, &MainMenuUI::OnQuitClicked);

		MenuPanel->AddChild(PlayButton);
		MenuPanel->AddChild(QuitButton);
		Subsystem.GUI->AddWidget(MenuPanel);
	}

	void MainMenuUI::OnPlayClicked()
	{
		Subsystem.GameState->RequestStateChange(GameState::Level1);
	}

	void MainMenuUI::OnQuitClicked()
	{
		// TODO: Quit request to engine
	}
}