// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Tests/TestCharacter.h"
#include "Tests/CollisionActor.h"
#include "UI/PauseMenuUI.h"
#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/WorldSubsystem.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"

namespace we
{
    LevelOne::LevelOne(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    LevelOne::~LevelOne() = default;

    void LevelOne::BeginPlay()
    {
        // Load background
        BG = LoadAsset().LoadTexture("Assets/Textures/Game/level1.png");
        BGImage = SpawnActor<Actor>().lock();
        BGImage->SetSprite(BG);

        // Spawn test character
        Character = SpawnActor<TestCharacter>().lock();
        Character->SetPosition({960.0f, 540.0f});

        auto Obstacle = SpawnActor<CollisionActor>().lock();
        Obstacle->SetPosition({640.0f, 540.0f});

        // Bind pause action to ESC key
        InputController().Bind(PAUSE_ACTION, Input::Keyboard{ sf::Keyboard::Scan::Escape });

        // Initialize pause menu
        PauseUI = make_unique<PauseMenuUI>();
        PauseUI->Initialize();

        // Bind delegates
        PauseUI->OnResumeClicked.Bind(this, &LevelOne::ResumeGame);
        PauseUI->OnSettingsClicked.Bind(this, &LevelOne::OnSettings);
        PauseUI->OnMainMenuClicked.Bind(this, &LevelOne::ReturnToMainMenu);

        LOG("[LevelOne] BeginPlay complete");
    }

    void LevelOne::Tick(float DeltaTime)
    {
        bool bIsPressed = InputController().Pressed(PAUSE_ACTION);
        if (bIsPressed && !bWasPausePressed)
        {
            TogglePauseMenu();
        }
        bWasPausePressed = bIsPressed;
    }

    void LevelOne::EndPlay()
    {
        if (PauseUI)
        {
            PauseUI->ClearWidgets();
            PauseUI.reset();
        }
    }

    void LevelOne::TogglePauseMenu()
    {
        if (PauseUI->IsVisible())
        {
            PauseUI->Hide();
        }
        else
            PauseUI->Show();
    }

    void LevelOne::PauseGame()
    {
        if (PauseUI)
        {
            PauseUI->Show();
        }
        Subsystem.Pause();
    }

    void LevelOne::ResumeGame()
    {
        if (PauseUI)
        {
            PauseUI->Hide();
        }
        Subsystem.Resume();
    }

    void LevelOne::ReturnToMainMenu()
    {
        if (PauseUI)
        {
            PauseUI->ClearWidgets();
            PauseUI.reset();
        }
        Subsystem.Resume();
        Subsystem.LoadWorld<MainMenu>();
    }

    void LevelOne::OnSettings()
    {
        // TODO: Implement settings UI
    }
}