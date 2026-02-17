// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/MainMenuUI.h"
#include "UI/Widget/Button.h"
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

	MainMenuUI::~MainMenuUI()
	{
	}

	void MainMenuUI::CreateUI()
	{
		ColorButton = Subsystem.GUI->CreateButton("Color Button", vec2f{ 200.f, 50.f });
		ColorButton->SetAnchorPosition(Anchor::Center, Anchor::Center, { 0.f, -40.f });
		ColorButton->SetRenderDepth(100.f);
		ColorButton->SetHoverSound(GC.DefaultButtonHoverSound);
		ColorButton->SetClickSound(GC.DefaultButtonClickSound);
		ColorButton->OnClicked.Bind(this, &MainMenuUI::OnColorButtonClicked);

		TextureButton = Subsystem.GUI->CreateTextureButton("Texture Button", GC.DefaultButton, { 200.f, 50.f });
		TextureButton->SetAnchorPosition(Anchor::Center, Anchor::Center, { 0.f, 40.f });
		TextureButton->SetRenderDepth(100.f);
		TextureButton->SetLocalScale({ 1.8f, 1.8f });
		TextureButton->SetHoverSound(GC.DefaultButtonHoverSound);
		TextureButton->SetClickSound(GC.DefaultButtonClickSound);
		TextureButton->OnClicked.Bind(this, &MainMenuUI::OnTextureButtonClicked);
	}

	void MainMenuUI::OnColorButtonClicked()
	{
		LOG("Color button clicked");
		OnPlayButtonClicked.Broadcast();
	}

	void MainMenuUI::OnTextureButtonClicked()
	{
		LOG("Texture button clicked");
		OnPlayButtonClicked.Broadcast();
	}
}
