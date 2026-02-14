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
		// * Mute			[X]
		// * Master Volume	[X]
		// * Music Volume	[X]
		// * Ambient Volume	[X]
		// * SFX Volume		[X]
		// * Controls		[ ]
		// * Full Screen	[ ]
		// * Exit Settings	[X]

		float Spacing = 300.f;

		auto VsyncText = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem, "Enable Vsync", Spacing);
		auto VsyncToggle = Subsystem.GUI->CreateWidget<Checkbox>(Subsystem, "", EC.VsyncEnabled, 30.f);
		auto HBox1 = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem, list<shared<Widget>>{VsyncText, VsyncToggle});

		auto MuteText = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem, "Mute Audio", Spacing);
		auto MuteToggle = Subsystem.GUI->CreateWidget<Checkbox>(Subsystem, "", false, 30.f);
		auto HBox2 = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem, list<shared<Widget>>{MuteText, MuteToggle});

		auto MVText = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem, "Master Volume", Spacing);
		auto MasterV = Subsystem.GUI->CreateWidget<Slider>(Subsystem, vec2f{ 200, 8 }, vec2f{ 20,30 });
		auto HBox3 = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem, list<shared<Widget>>{MVText, MasterV});

		auto MuVText = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem, "Music Volume", Spacing);
		auto MusicV = Subsystem.GUI->CreateWidget<Slider>(Subsystem, vec2f{ 200, 8 }, vec2f{ 20,30 });
		auto HBox4 = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem, list<shared<Widget>>{MuVText, MusicV});
		
		auto AVText = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem, "Ambient Volume", Spacing);
		auto AmbientV = Subsystem.GUI->CreateWidget<Slider>(Subsystem, vec2f{ 200, 8 }, vec2f{ 20,30 });
		auto HBox5 = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem, list<shared<Widget>>{AVText, AmbientV});
		
		auto SFXVText = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem, "SFX Volume", Spacing);
		auto SFXV = Subsystem.GUI->CreateWidget<Slider>(Subsystem, vec2f{ 200, 8 }, vec2f{ 20,30 });
		auto HBox6 = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem, list<shared<Widget>>{SFXVText, SFXV});

		auto FSText = Subsystem.GUI->CreateWidget<TextBlock>(Subsystem, "Toggle Fullscreen", Spacing);
		ToggleFullscreenButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "", vec2f{30,30});
		ToggleFullscreenButton->OnClicked.Bind(this, &SettingsMenuUI::ToggleFullscreen);
		auto HBox7 = Subsystem.GUI->CreateWidget<HorizontalBox>(Subsystem, list<shared<Widget>>{FSText, ToggleFullscreenButton});

		ExitButton = Subsystem.GUI->CreateWidget<Button>(Subsystem, "Exit", EC.DefaultButton);
		ExitButton->OnClicked.Bind(this, &SettingsMenuUI::OnExitButtonClicked);
		auto VBox1 = Subsystem.GUI->CreateWidget<VerticalBox>(Subsystem, 
			list<shared<Widget>>{ExitButton}, 0.f, Anchor::Center, Anchor::Center, vec2f{265,100});

		auto VBox = Subsystem.GUI->CreateWidget<VerticalBox>(Subsystem, list<shared<Widget>>
		{
			HBox1, 
			HBox2, 
			HBox3,
			HBox4,
			HBox5,
			HBox6,
			HBox7,
			VBox1,
		}, 30.f, Anchor::CenterLeft, Anchor::CenterLeft);

		SettingsPanel = Subsystem.GUI->CreateWidget<Panel>(Subsystem
			, list<shared<Widget>>{VBox}
			, rectf{ {10,10},{10,10} }
			, Anchor::CenterRight
			, Anchor::CenterRight
			, vec2f{-100, 0}
			);

	}

	void SettingsMenuUI::OnExitButtonClicked()
	{
		OnExitClicked.Broadcast();
	}

	void SettingsMenuUI::ToggleFullscreen()
	{
		OnToggleFSClicked.Broadcast();
		LOG("Toggle Fullscreen")
	}
}