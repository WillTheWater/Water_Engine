// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/SettingsMenuUI.h"
#include "UI/Widget/Panel.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/VerticalBox.h"
#include "UI/Widget/HorizontalBox.h"
#include "UI/Widget/GridBox.h"
#include "UI/Widget/Slider.h"
#include "UI/Widget/CheckBox.h"
#include "UI/Widget/ProgressBar.h"
#include "UI/Widget/TextBlock.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"
#include "UI/UILayerOrder.h"
#include "Utility/Log.h"

namespace we
{
	SettingsMenuUI::SettingsMenuUI(EngineSubsystem& Subsystem)
		: Subsystem{ Subsystem }
	{
		CreateUI();
		Hide();
	}

	void SettingsMenuUI::Show()
	{
		if (SettingsPanel)
		{
			SettingsPanel->SetVisible(true);
			bVisible = true;
		}
	}

	void SettingsMenuUI::Hide()
	{
		if (SettingsPanel)
		{
			SettingsPanel->SetVisible(false);
			bVisible = false;
		}
	}

	void SettingsMenuUI::CreateUI()
	{
		// TODO
		// * Vsync			[X]
		// * Framerate		[ ]
		// * Full Screen	[ ]
		// * Mute			[X]
		// * Master Volume	[ ]
		// * Music Volume	[ ]
		// * Ambient Volume	[ ]
		// * SFX Volume		[ ]
		// * Controls		[ ]
		// * Exit Settings	[X]

		auto VsyncText = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem, "Enable Vsync", 200);
		auto VsyncToggle = Subsystem.GUI->CreateWidget<Checkbox>(Subsystem, "", EC.VsyncEnabled, 30.f);
		auto HBox1 = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem, list<shared<Widget>>{VsyncText, VsyncToggle});

		auto MuteText = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem, "Mute Audio", 200);
		auto MuteToggle = Subsystem.GUI->CreateWidget<Checkbox>(Subsystem, "", false, 30.f);
		auto HBox2 = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem, list<shared<Widget>>{MuteText, MuteToggle});



		ExitButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Exit", EC.DefaultButton);
		ExitButton->OnClicked.Bind(this, &SettingsMenuUI::OnExitButtonClicked);

		auto VBox = Subsystem.GUI->CreateWidget<VerticalBox>(Subsystem
			, list<shared<Widget>>{HBox1, HBox2, ExitButton});

		SettingsPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem
			, list<shared<Widget>>{VBox}
			, rectf{ {10,10},{10,10} }
			, Anchor::Center
			, Anchor::Center
			);

		/////*Btn1 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", vec2f{ 200, 40 });
		////Btn2 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", vec2f{ 200, 40 });
		////Btn3 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Play", vec2f{ 200, 40 });
		////Btn4 = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Quit", vec2f{ 200, 40 });*/

		////Btn1->SetAnchorPosition(Anchor::Center, Anchor::TopCenter, { 0, 0 });
		////Btn2->SetAnchorPosition(Anchor::Center, Anchor::TopCenter, { 0, 0 });

		/////*auto VBox = Subsystem.GUI->CreateWidget<VerticalBox>(
		////	Subsystem,
		////	list<shared<Widget>>{ Btn1, Btn2 },
		////	20.f,
		////	Anchor::Center,
		////	Anchor::TopRight
		////);

		////auto VBox2 = Subsystem.GUI->CreateWidget<VerticalBox>(
		////	Subsystem,
		////	list<shared<Widget>>{ Btn3, Btn4 },
		////	20.f,
		////	Anchor::Center,
		////	Anchor::TopCenter
		////);

		////auto HBox = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem,
		////	list<shared<Widget>>{ VBox, VBox2},
		////	20.f,
		////	Anchor::Center,
		////	Anchor::Center);*/

		////	/*auto Grid = Subsystem.GUI->CreateWidget<GridBox>(Subsystem,
		////		list<shared<Widget>>{Btn1, Btn2, Btn3, Btn4},
		////		2.f);*/
		////		// Create a health bar
		////	//auto HealthBar = Subsystem.GUI->CreateWidget<ProgressBar>(
		////	//	Subsystem,
		////	//	vec2f{ 260.f, 24.f },
		////	//	50.f
		////	//);

		//	//HealthBar->SetAnchorPosition(Anchor::Center, Anchor::BottomCenter, { 20.f, 20.f });

		//	//HealthBar->SetFillColor(color{ 220, 50, 50 });
		//	//HealthBar->SetBackgroundColor(color{ 60, 20, 20 });

		//	//HealthBar->SetValue(.5);

		//	//// Vertical mana bar (BottomToTop fills from bottom)
		//	//auto ManaBar = Subsystem.GUI->CreateWidget<ProgressBar>(
		//	//	Subsystem,
		//	//	vec2f{ 240.f, 150.f },
		//	//	.2f
		//	//);
		//	//ManaBar->SetAnchorPosition(Anchor::Center, Anchor::TopCenter, { 20.f, 60.f });
		//	//ManaBar->SetDirection(ProgressDirection::RightToLeft);
		//	//ManaBar->SetFillColor(color{ 50, 100, 220 });


		//	//auto Text = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem
		//	//	, "This is random text as a test to see if it work, odds are it doesn't work! This is random text as a test to see if it work, odds are it doesn't work! This is random text as a test to see if it work, odds are it doesn't work!"
		//	//	, 400);

		//	//auto TextPanel = Subsystem.GUI->CreateWidget<Panel>(
		//	//	Subsystem,
		//	//	list<shared<Widget>>{Text},
		//	//	rectf{ {10.f, 10.f}, {10.f, 10.f} },
		//	//	Anchor::Center,
		//	//	Anchor::BottomCenter,
		//	//	vec2f{30,0}
		//	//);

		//	//// In a panel
		//	//auto TestPanel = Subsystem.GUI->CreateWidget<Panel>(
		//	//	Subsystem,
		//	//	list<shared<Widget>>{HealthBar, ManaBar, TextPanel},
		//	//	rectf{ {50.f, 50.f}, {50.f, 50.f} },
		//	//	Anchor::Center,
		//	//	Anchor::Center
		//	//);
		//	//auto Slid = Subsystem.GUI->CreateWidget<Slider>(Subsystem);

		//	//Btn1->OnClicked.Bind(this, &SettingsMenuUI::OnPlayClicked);
		//	//Btn3->OnClicked.Bind(this, &SettingsMenuUI::OnPlayClicked);
		//	//Btn2->OnClicked.Bind(this, &SettingsMenuUI::OnQuitClicked);
		//	//Btn4->OnClicked.Bind(this, &SettingsMenuUI::OnQuitClicked);

		//	//// Create a settings checkbox
		//	//auto SoundCheckbox = Subsystem.GUI->CreateWidget<Checkbox>(
		//	//	Subsystem,
		//	//	"Enable Sound Effects",
		//	//	true  // Initially checked
		//	//);

		//	//SoundCheckbox->SetAnchorPosition(
		//	//	Anchor::Center,
		//	//	Anchor::BottomLeft,
		//	//	{ 0.f, -20.f }
		//	//);

		//	//SoundCheckbox->SetChecked(On);
		//	//SoundCheckbox->OnToggled.Bind(this, &SettingsMenuUI::Test);
		//	//auto TestPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem,
		//	//	list<shared<Widget>>{Slid, SoundCheckbox, HealthBar, ManaBar}, rectf{ {50,50},{50,50} }, Anchor::Center, Anchor::Center, vec2f{-100.f,0.f});

	}

	void SettingsMenuUI::OnExitButtonClicked()
	{
		OnExitClicked.Broadcast();
		LOG("Exit Settings")
	}
}