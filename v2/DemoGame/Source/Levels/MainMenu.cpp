// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/AsyncResourceSubsystem.h"
#include "GameStateTokens.h"
#include "UI/MainMenuUI.h"
#include "Utility/Log.h"

namespace we
{
	MainMenu::MainMenu(EngineSubsystem& Subsystem)
		: World(Subsystem)
	{
		UI = make_unique<MainMenuUI>(Subsystem);
		UI->OnPlayButtonClicked.Bind(this, &MainMenu::OnPlayClicked);
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::BeginPlay()
	{
		Subsystem.Audio->PlayMusic(EC.DefaultMusic);
		Subsystem.Audio->PlayAmbient(EC.DefaultAmbient);

		BgHandle = AsyncAsset().LoadTextureAsync(EC.DefaultBackground, AssetPriority::High);
	}

	void MainMenu::Tick(float DeltaTime)
	{
		if (BgHandle.IsReady() && !Background.has_value())
		{
			Background.emplace(*BgHandle);
		}
	}

	void MainMenu::Render()
	{
		if (Background)
		{
			Subsystem.Render->Draw(*Background);
		}

		RenderActors();
	}

	void MainMenu::OnPlayClicked()
	{
		Subsystem.GameState->RequestStateChange(MakeState(EGameState::LevelOne));
	}
}