// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/EngineSubsystem.h"
#include "EngineConfig.h"
#include "GameConfig.h"
#include "UI/MainMenuUI.h"
#include "Utility/Log.h"

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
		// Load and setup background directly (no Actor overhead)
		BgTexture = LoadAsset().LoadTextureSync(GC.DefaultBackground);
		BgSprite.emplace(*BgTexture);

		// Center it
		auto bounds = BgSprite->getLocalBounds();
		BgSprite->setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
		BgSprite->setPosition(vec2f(EC.RenderResolution) / 2.0f);

		// Add to world rendering at back (depth -1000)
		AddRenderDepth(&*BgSprite, -1000.0f);

		// Create the UI
		MenuUI = make_unique<MainMenuUI>(Subsystem);
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
}
