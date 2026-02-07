// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "GameStates.h"
#include "UI/MainMenuUI.h"
#include "Utility/Log.h"

namespace we
{
	MainMenu::MainMenu(EngineSubsystem& Subsystem)
		: World(Subsystem)
	{
		BgTexture = Asset().LoadTexture(EC.DefaultBackground);
		if (BgTexture)
		{
			Background.emplace(*BgTexture);
		}

		UI = make_unique<MainMenuUI>(Subsystem);
		UI->OnPlayButtonClicked.Bind(this, &MainMenu::OnPlayClicked);
		Subsystem.Audio->PlayMusic(EC.DefaultMusic);
		Subsystem.Audio->PlayAmbient(EC.DefaultAmbient);
	}

	MainMenu::~MainMenu()
	{
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