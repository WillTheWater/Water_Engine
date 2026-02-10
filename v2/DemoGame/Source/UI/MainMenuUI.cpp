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
#include "UI/UILayerOrder.h"

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
			Subsystem.GUI->DestroyWidget(MenuPanel.get());
		}
	}

	void MainMenuUI::CreateUI()
	{
		MenuPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem, EC.DefaultPanel);
		MenuPanel->SetLocalOffset({ EC.WindowSize.x * 0.5f, EC.WindowSize.y * 0.83f });
		MenuPanel->SetZOrder(LAYER_GAME_UI);

		PlayButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", EC.DefaultButton);
		PlayButton->SetLocalOffset({ 0, -40 });
		PlayButton->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);

		QuitButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", EC.DefaultButton);
		QuitButton->SetLocalOffset({ 0, 60 });
		QuitButton->OnClicked.Bind(this, &MainMenuUI::OnQuitClicked);

		MenuPanel->AddChild(PlayButton);
		MenuPanel->AddChild(QuitButton);
	}

	void MainMenuUI::OnPlayClicked()
	{
		OnPlayButtonClicked.Broadcast();
	}

	void MainMenuUI::OnQuitClicked()
	{
		Subsystem.GameState->OnQuitRequested.Broadcast();
	}
}