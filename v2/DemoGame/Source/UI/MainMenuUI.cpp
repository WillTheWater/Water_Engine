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
		MenuPanel = make_shared<Panel>(Subsystem, EC.DefaultPanel);
		MenuPanel->SetLocalOffset({ EC.WindowSize.x * 0.5f, EC.WindowSize.y * 0.83f });

		PlayButton = make_shared<Button>(Subsystem, "Play", EC.DefaultButton);
		PlayButton->SetLocalOffset({ 0, -40 });
		PlayButton->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);

		QuitButton = make_shared<Button>(Subsystem, "Quit", EC.DefaultButton);
		QuitButton->SetLocalOffset({ 0, 60 });
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
		Subsystem.GameState->OnQuitRequested.Broadcast();
	}
}