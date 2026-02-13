// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/MainMenuUI.h"
#include "UI/Widget/Panel.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/VerticalBox.h"
#include "UI/Widget/HorizontalBox.h"
#include "UI/Widget/GridBox.h"
#include "UI/Widget/Slider.h"
#include "UI/Widget/CheckBox.h"
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

		Btn1 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", vec2f{ 200, 40 });
		Btn2 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", vec2f{ 200, 40 });
		Btn3 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", vec2f{ 200, 40 });
		Btn4 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", vec2f{ 200, 40 });

		//Btn1->SetAnchorPosition(Anchor::Center, Anchor::TopCenter, { 0, 0 });
		//Btn2->SetAnchorPosition(Anchor::Center, Anchor::TopCenter, { 0, 0 });

		/*auto VBox = Subsystem.GUI->CreateWidget<VerticalBox>(
			Subsystem,
			list<shared<Widget>>{ Btn1, Btn2 },
			20.f,
			Anchor::Center,
			Anchor::TopRight
		);

		auto VBox2 = Subsystem.GUI->CreateWidget<VerticalBox>(
			Subsystem,
			list<shared<Widget>>{ Btn3, Btn4 },
			20.f,
			Anchor::Center,
			Anchor::TopCenter
		);

		auto HBox = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem,
			list<shared<Widget>>{ VBox, VBox2},
			20.f,
			Anchor::Center,
			Anchor::Center);*/

		/*auto Grid = Subsystem.GUI->CreateWidget<GridBox>(Subsystem, 
			list<shared<Widget>>{Btn1, Btn2, Btn3, Btn4}, 
			2.f);*/

		auto Slid = Subsystem.GUI->CreateWidget<Slider>(Subsystem);

		Btn1->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);
		Btn3->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);
		Btn2->OnClicked.Bind(this, &MainMenuUI::OnQuitClicked);
		Btn4->OnClicked.Bind(this, &MainMenuUI::OnQuitClicked);

		// Create a settings checkbox
		auto SoundCheckbox = Subsystem.GUI->CreateWidget<Checkbox>(
			Subsystem,
			"Enable Sound Effects",
			true  // Initially checked
		);

		SoundCheckbox->SetAnchorPosition(
			Anchor::Center,
			Anchor::BottomLeft,
			{ 0.f, -20.f }
		);

		SoundCheckbox->SetChecked(On);
		SoundCheckbox->OnToggled.Bind(this, &MainMenuUI::Test);
		auto TestPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem,
			list<shared<Widget>>{Slid, SoundCheckbox}, rectf{ {50,50},{50,50} }, Anchor::Center, Anchor::Center, vec2f{-100.f,0.f});

	}

	void MainMenuUI::OnPlayClicked()
	{
		//OnPlayButtonClicked.Broadcast();
		LOG("Play")
	}

	void MainMenuUI::OnQuitClicked()
	{
		//Subsystem.GameState->OnQuitRequested.Broadcast();
		LOG("QUIT")
	}

	void MainMenuUI::Test(bool newbool)
	{
		On = !On;
		LOG("Sound: {}", On)
	}
}