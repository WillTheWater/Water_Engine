// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"
#include "Tests/WidgetTests.h"
#include "UI/MainMenuUI.h"

namespace we
{
    MainMenu::MainMenu(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    void MainMenu::BeginPlay()
    {		
        BG = LoadAsset().LoadTexture("Assets/Textures/Default/defaultBackground.png");
        BGImage = SpawnActor<Actor>().lock();
        BGImage->SetSprite(BG);

		// Main Menu UI - initialize the member variable
        MenuUI = make_unique<MainMenuUI>();
		MenuUI->Initialize();
		MenuUI->Show();

        MenuUI->OnPlayButtonClicked.Bind(this, &MainMenu::Play);
        MenuUI->OnSettingsButtonClicked.Bind(this, &MainMenu::Settings);
        MenuUI->OnQuitButtonClicked.Bind(this, &MainMenu::Quit);
    }

    void MainMenu::Tick(float DeltaTime)
    {
      
    }

    void MainMenu::Play()
    {
    }

    void MainMenu::Settings()
    {
    }

    void MainMenu::Quit()
    {
        LOG("[MainMenu] Quit requested");
        Subsystem.Quit();
    }
}
