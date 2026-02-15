//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#include "UI/MainMenuUI.h"
//#include "UI/Widget/Panel.h"
//#include "UI/Widget/Button.h"
//#include "UI/Widget/VerticalBox.h"
//#include "UI/Widget/HorizontalBox.h"
//#include "UI/Widget/GridBox.h"
//#include "UI/Widget/Slider.h"
//#include "UI/Widget/CheckBox.h"
//#include "UI/Widget/ProgressBar.h"
//#include "UI/Widget/TextBlock.h"
//#include "Framework/EngineSubsystem.h"
//#include "Subsystem/ResourceSubsystem.h"
//#include "EngineConfig.h"
//#include "UI/UILayerOrder.h"
//#include "Utility/Log.h"
//
//namespace we
//{
//	MainMenuUI::MainMenuUI(EngineSubsystem& Subsystem)
//		: Subsystem{ Subsystem }
//	{
//		CreateUI();
//	}
//
//	MainMenuUI::~MainMenuUI()
//	{
//	}
//
//	void MainMenuUI::CreateUI()
//	{
//		PlayButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", EC.DefaultButton);
//		PlayButton->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);
//
//		SettingsButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Settings", EC.DefaultButton);
//		SettingsButton->OnClicked.Bind(this, &MainMenuUI::OnSettingsClicked);
//
//		QuitButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", EC.DefaultButton);
//		QuitButton->OnClicked.Bind(this, &MainMenuUI::OnQuitClicked);
//
//		auto VBox = Subsystem.GUI->CreateWidget<VerticalBox>(Subsystem
//			, vector<shared<Widget>>{PlayButton, SettingsButton, QuitButton});
//
//		MenuPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem
//				, vector<shared<Widget>>{VBox}
//				, rectf{{10,10},{10,10}}
//				, Anchor::Center
//				, Anchor::TopCenter
//				, vec2f{0,200}
//			);
//
//		/*Btn1 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", vec2f{ 200, 40 });
//		Btn2 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", vec2f{ 200, 40 });
//		Btn3 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", vec2f{ 200, 40 });
//		Btn4 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", vec2f{ 200, 40 });*/
//
//		//Btn1->SetAnchorPosition(Anchor::Center, Anchor::TopCenter, { 0, 0 });
//		//Btn2->SetAnchorPosition(Anchor::Center, Anchor::TopCenter, { 0, 0 });
//
//		/*auto VBox = Subsystem.GUI->CreateWidget<VerticalBox>(
//			Subsystem,
//			vector<shared<Widget>>{ Btn1, Btn2 },
//			20.f,
//			Anchor::Center,
//			Anchor::TopRight
//		);
//
//		auto VBox2 = Subsystem.GUI->CreateWidget<VerticalBox>(
//			Subsystem,
//			vector<shared<Widget>>{ Btn3, Btn4 },
//			20.f,
//			Anchor::Center,
//			Anchor::TopCenter
//		);
//
//		auto HBox = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem,
//			vector<shared<Widget>>{ VBox, VBox2},
//			20.f,
//			Anchor::Center,
//			Anchor::Center);*/
//
//		/*auto Grid = Subsystem.GUI->CreateWidget<GridBox>(Subsystem, 
//			vector<shared<Widget>>{Btn1, Btn2, Btn3, Btn4}, 
//			2.f);*/
//			// Create a health bar
//		//auto HealthBar = Subsystem.GUI->CreateWidget<ProgressBar>(
//		//	Subsystem,
//		//	vec2f{ 260.f, 24.f },
//		//	50.f
//		//);
//
//		//HealthBar->SetAnchorPosition(Anchor::Center, Anchor::BottomCenter, { 20.f, 20.f });
//
//		//HealthBar->SetFillColor(color{ 220, 50, 50 });
//		//HealthBar->SetBackgroundColor(color{ 60, 20, 20 });
//
//		//HealthBar->SetValue(.5);
//
//		//// Vertical mana bar (BottomToTop fills from bottom)
//		//auto ManaBar = Subsystem.GUI->CreateWidget<ProgressBar>(
//		//	Subsystem,
//		//	vec2f{ 240.f, 150.f },
//		//	.2f
//		//);
//		//ManaBar->SetAnchorPosition(Anchor::Center, Anchor::TopCenter, { 20.f, 60.f });
//		//ManaBar->SetDirection(ProgressDirection::RightToLeft);
//		//ManaBar->SetFillColor(color{ 50, 100, 220 });
//
//
//		//auto Text = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem
//		//	, "This is random text as a test to see if it work, odds are it doesn't work! This is random text as a test to see if it work, odds are it doesn't work! This is random text as a test to see if it work, odds are it doesn't work!"
//		//	, 400);
//
//		//auto TextPanel = Subsystem.GUI->CreateWidget<Panel>(
//		//	Subsystem,
//		//	vector<shared<Widget>>{Text},
//		//	rectf{ {10.f, 10.f}, {10.f, 10.f} },
//		//	Anchor::Center,
//		//	Anchor::BottomCenter,
//		//	vec2f{30,0}
//		//);
//
//		//// In a panel
//		//auto TestPanel = Subsystem.GUI->CreateWidget<Panel>(
//		//	Subsystem,
//		//	vector<shared<Widget>>{HealthBar, ManaBar, TextPanel},
//		//	rectf{ {50.f, 50.f}, {50.f, 50.f} },
//		//	Anchor::Center,
//		//	Anchor::Center
//		//);
//		//auto Slid = Subsystem.GUI->CreateWidget<Slider>(Subsystem);
//
//		//Btn1->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);
//		//Btn3->OnClicked.Bind(this, &MainMenuUI::OnPlayClicked);
//		//Btn2->OnClicked.Bind(this, &MainMenuUI::OnQuitClicked);
//		//Btn4->OnClicked.Bind(this, &MainMenuUI::OnQuitClicked);
//
//		//// Create a settings checkbox
//		//auto SoundCheckbox = Subsystem.GUI->CreateWidget<Checkbox>(
//		//	Subsystem,
//		//	"Enable Sound Effects",
//		//	true  // Initially checked
//		//);
//
//		//SoundCheckbox->SetAnchorPosition(
//		//	Anchor::Center,
//		//	Anchor::BottomLeft,
//		//	{ 0.f, -20.f }
//		//);
//
//		//SoundCheckbox->SetChecked(On);
//		//SoundCheckbox->OnToggled.Bind(this, &MainMenuUI::Test);
//		//auto TestPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem,
//		//	vector<shared<Widget>>{Slid, SoundCheckbox, HealthBar, ManaBar}, rectf{ {50,50},{50,50} }, Anchor::Center, Anchor::Center, vec2f{-100.f,0.f});
//
//	}
//
//	void MainMenuUI::OnPlayClicked()
//	{
//		OnPlayButtonClicked.Broadcast();
//	}
//
//	void MainMenuUI::OnSettingsClicked()
//	{
//		// Open Settings Menu
//		OnSettingsButtonClicked.Broadcast();
//	}
//
//	void MainMenuUI::OnQuitClicked()
//	{
//		Subsystem.GameState->OnQuitRequested.Broadcast();
//	}
//}