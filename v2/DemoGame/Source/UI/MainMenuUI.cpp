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
		// Create main menu panel
		MenuPanel = Subsystem.GUI->CreatePanel({ 400.f, 300.f }, color::White, color::Black, 4.f);
		MenuPanel->SetAnchorPosition(Anchor::Center, Anchor::Center);
		MenuPanel->SetRenderDepth(50.f);

		// Create Play button
		PlayButton = Subsystem.GUI->CreateButton("Play", vec2f{ 200.f, 50.f });
		PlayButton->SetRenderDepth(100.f);
		PlayButton->SetHoverSound(GC.DefaultButtonHoverSound);
		PlayButton->SetClickSound(GC.DefaultButtonClickSound);
		PlayButton->OnClicked.Bind(this, &MainMenuUI::OnPlayButtonClicked);
		MenuPanel->AddChild(PlayButton, Anchor::Center, Anchor::Center);
	}

	void MainMenuUI::OnPlayButtonClicked()
	{
		LOG("Play button clicked");
		OnPlayClicked.Broadcast();
	}
}
