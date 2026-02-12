// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/MainMenuUI.h"
#include "UI/Widget/Panel.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/VerticalBox.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"
#include "UI/UILayerOrder.h"
#include "Utility/Log.h"

namespace we
{
	MainMenuUI::MainMenuUI(EngineSubsystem& Subsystem)
		: Subsystem{ Subsystem }
	{
		CreateUI();
	}

	MainMenuUI::~MainMenuUI()
	{
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

		Btn1 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", vec2f{ 220, 40 });
		Btn2 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", vec2f{ 120, 40 });

		Btn1->SetAnchorPosition(Anchor::Center, Anchor::Center, { 0, 0 });
		Btn2->SetAnchorPosition(Anchor::Center, Anchor::Center, { 0, 50 });

		auto TestPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem,
			list<shared<Widget>>{Btn1, Btn2}, rectf{ {100,200},{10,10} }, Anchor::Center, Anchor::Center);
		Btn1->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);
	}

	void MainMenuUI::OnPlayClicked()
	{
		//OnPlayButtonClicked.Broadcast();
	}

	void MainMenuUI::OnQuitClicked()
	{
		Subsystem.GameState->OnQuitRequested.Broadcast();
	}
}