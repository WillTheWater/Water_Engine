// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "GameInstance/DemoGameInstance.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/GameStateSubsystem.h"
#include "EngineConfig.h"
#include "GameConfig.h"
#include "UI/MainMenuUI.h"
#include "Utility/Log.h"
#include "GameStateTokens.h"

namespace we
{
	MainMenu::MainMenu(EngineSubsystem& Subsystem)
		: World(Subsystem)
	{
		Construct();
	}

	MainMenu::~MainMenu() = default;

	void MainMenu::Construct()
	{
		// Load and setup background
		BgTexture = LoadAsset().LoadTextureSync(GC.DefaultBackground);
		BgSprite.emplace(*BgTexture);

		// Center it
		auto bounds = BgSprite->getLocalBounds();
		BgSprite->setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
		BgSprite->setPosition(vec2f(EC.RenderResolution) / 2.0f);

		// Add to world rendering at back (depth -1000)
		AddRenderDepth(&*BgSprite, -1000.0f);

		// Create the main menu UI and bind all buttons
		MenuUI = make_unique<MainMenuUI>(Subsystem);
		MenuUI->OnPlayClicked.Bind(this, &MainMenu::OnPlayClicked);
		MenuUI->OnSettingsClicked.Bind(this, &MainMenu::OnSettingsClicked);
		MenuUI->OnQuitClicked.Bind(this, &MainMenu::OnQuitClicked);

		// Bind to GameInstance's settings closed event
		if (Subsystem.GameInst)
		{
			static_cast<DemoGameInstance*>(Subsystem.GameInst.get())->OnSettingsClosed.Bind(this, &MainMenu::OnSettingsBackClicked);
		}
	}

	void MainMenu::BeginPlay()
	{
		// Nothing needed here - MainMenuUI handles its own visibility
	}

	void MainMenu::Tick(float DeltaTime)
	{
	}

	void MainMenu::OnPlayClicked()
	{
		LOG("Transitioning to LevelOne");
		
		// Clear all GUI widgets - they'll be recreated in the new world if needed
		Subsystem.GUI->Clear();
		
		Subsystem.GameState->RequestStateChange(MakeState(EGameState::LevelOne));
	}

	void MainMenu::OnSettingsClicked()
	{
		LOG("Opening settings menu via GameInstance");
		
		// Hide main menu and show settings via GameInstance (persists across worlds)
		MenuUI->Hide();
		
		if (auto* GI = static_cast<DemoGameInstance*>(Subsystem.GameInst.get()))
		{
			GI->ShowSettings();
		}
	}

	void MainMenu::OnSettingsBackClicked()
	{
		LOG("Settings closed - returning to main menu");
		
		// Show main menu again
		MenuUI->Show();
	}

	void MainMenu::OnQuitClicked()
	{
		Subsystem.GameState->RequestShutdown();
	}
}
