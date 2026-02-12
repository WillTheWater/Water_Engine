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
		/*MenuPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem, EC.DefaultPanel);
		MenuPanel->SetAnchorPosition(Anchor::Center, Anchor::TopCenter, {0,200});
		MenuPanel->SetZOrder(LAYER_GAME_UI);

		PlayButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", EC.DefaultButton);
		PlayButton->SetLocalOffset({ 0, -40 });
		PlayButton->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);

		QuitButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", EC.DefaultButton);
		QuitButton->SetLocalOffset({ 0, 60 });
		QuitButton->OnClicked.Bind(this, &MainMenuUI::OnQuitClicked);

		MenuPanel->AddChild(PlayButton, Anchor::Center, Anchor::BottomCenter);
		MenuPanel->AddChild(QuitButton, Anchor::Center, Anchor::TopCenter);*/

		auto MainPanel2 = Subsystem.GUI->CreateWidget<Panel>(Subsystem, vec2f{ 400, 200 });
		MainPanel2->SetAnchorPosition(Anchor::Center, Anchor::Center);
		MainPanel2->SetLocalScale({ 2,1.3 });
		MainPanel2->SetLocalOffset({ 0, -200 });

		auto SaveBtn = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Save");

		MainPanel2->AddChild(SaveBtn, Anchor::Center, Anchor::Center);
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