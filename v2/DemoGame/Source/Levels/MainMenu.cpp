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
#include "UI/SettingsUI.h"
#include "Component/PostProcessingComponent.h"
#include "PostProcess/Effects/PPEGrayscale.h"
#include "PostProcess/Effects/PPEWave.h"

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
        //BGImage->SetSprite(BG);
        PPC = make_shared<PostProcessingComponent>(BGImage.get());
        PPC->SetTexture(BG);
        //PPC->AddEffect(make_unique<PPEGrayscale>());
        PPC->AddEffect(make_unique<PPEWave>());
        PPC->BeginPlay();

		// Main Menu UI
        MenuUI = make_unique<MainMenuUI>();
		MenuUI->Initialize();
		MenuUI->Show();

        MenuUI->OnPlayButtonClicked.Bind(this, &MainMenu::Play);
        MenuUI->OnSettingsButtonClicked.Bind(this, &MainMenu::OpenSettings);
        MenuUI->OnQuitButtonClicked.Bind(this, &MainMenu::Quit);

		// Settings UI
		SettingsMenu = make_unique<SettingsUI>();
		SettingsMenu->Initialize(Subsystem.GetSave());
		SettingsMenu->OnBackClicked.Bind(this, &MainMenu::CloseSettings);

        // Play ambient background
        //PlayAudio().PlayAmbient("Assets/Audio/Default/defaultAmbient.ogg", 4);
    }

    void MainMenu::Tick(float DeltaTime)
    {
        PPC->Tick(DeltaTime);
    }

    void MainMenu::EndPlay()
    {
        if (SettingsMenu)
            SettingsMenu->ClearWidgets();
        MenuUI->ClearWidgets();
        PlayAudio().StopAmbient(3);
    }

    void MainMenu::Play()
    {
        Subsystem.LoadWorld<LevelOne>();
    }

    void MainMenu::OpenSettings()
    {
        MenuUI->Hide();
        SettingsMenu->Show();
    }

    void MainMenu::CloseSettings()
    {
        SettingsMenu->Hide();
        MenuUI->Show();
    }

    void MainMenu::Quit()
    {
        Subsystem.Quit();
    }
}