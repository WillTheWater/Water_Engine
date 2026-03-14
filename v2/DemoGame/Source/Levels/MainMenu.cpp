// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Levels/LevelOne.h"
#include "Framework/World/Actor.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Utility/Log.h"
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

		// Main Menu UI
        MenuUI = make_unique<MainMenuUI>();
		MenuUI->Initialize();
		MenuUI->Show();

        MenuUI->OnPlayButtonClicked.Bind(this, &MainMenu::Play);
        MenuUI->OnSettingsButtonClicked.Bind(this, &MainMenu::Settings);
        MenuUI->OnQuitButtonClicked.Bind(this, &MainMenu::Quit);

        // Play ambient background
        //PlayAudio().PlayAmbient("Assets/Audio/Default/defaultAmbient.ogg", 4);
    }

    void MainMenu::Tick(float DeltaTime)
    {
      
    }

    void MainMenu::EndPlay()
    {
        MenuUI->ClearWidgets();
        PlayAudio().StopAmbient(3);
    }

    void MainMenu::Play()
    {
        Subsystem.LoadWorld<LevelOne>();
    }

    void MainMenu::Settings()
    {
    }

    void MainMenu::Quit()
    {
        Subsystem.Quit();
    }
}