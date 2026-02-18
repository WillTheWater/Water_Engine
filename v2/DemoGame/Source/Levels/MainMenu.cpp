// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
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

		// Create the UI and bind all buttons
		MenuUI = make_unique<MainMenuUI>(Subsystem);
		MenuUI->OnPlayClicked.Bind(this, &MainMenu::OnPlayClicked);
		MenuUI->OnSettingsClicked.Bind(this, &MainMenu::OnSettingsClicked);
		MenuUI->OnQuitClicked.Bind(this, &MainMenu::OnQuitClicked);
		LOG("MainMenu: UI created");
	}

	void MainMenu::BeginPlay()
	{
		if (Subsystem.Audio)
		{
			Subsystem.Audio->PlayMusic(GC.DefaultMusic, true);
			Subsystem.Audio->PlayAmbient(GC.DefaultAmbient, true);
		}
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
		LOG("Settings clicked - placeholder for settings menu");
		// TODO: Show settings panel or transition to settings world
	}

	void MainMenu::OnQuitClicked()
	{
		LOG("Quit clicked - requesting shutdown");
		Subsystem.GameState->RequestShutdown();
	}
}
