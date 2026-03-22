// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Framework/World/Character.h"
#include "Tests/TestCharacter.h"
#include "Component/CameraComponent.h"
#include "PostProcess/Effects/PPEScroll.h"
#include "Tests/CollisionActor.h"
#include "Component/PhysicsComponent.h"
#include "UI/PauseMenuUI.h"
#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/SaveSubsystem.h"
#include "Component/PostProcessingComponent.h"
#include "PostProcess/Effects/PPEWave.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"
#include "Config/GameConfig.h"

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
        BG = LoadAsset().LoadTexture("Assets/Textures/Game/world.png");
        BGImage = SpawnActor<Actor>().lock();
        BGImage->SetSprite(BG);

        Water = LoadAsset().LoadTexture("Assets/Textures/Game/water.png");
        WaterImage = SpawnActor<Actor>().lock();
        WaterPPC = make_shared<PostProcessingComponent>(WaterImage.get());
        WaterPPC->SetTexture(Water);
        WaterPPC->AddEffect(make_unique<PPEScroll>());
        WaterPPC->AddEffect(make_unique<PPEWave>());
        WaterPPC->BeginPlay();

        // Load saved data and update session counter
        int TimesPlayed = Subsystem.GetSave().Get<int>(SAVE_TIMES_PLAYED, 0);
        TimesPlayed++;
        Subsystem.GetSave().Set(SAVE_TIMES_PLAYED, TimesPlayed);
        Subsystem.GetSave().Set(SAVE_LAST_LEVEL, "LevelOne");
        LOG("[LevelOne] Times played: {}", TimesPlayed);
        
        // Load positions
        float PlayerX = Subsystem.GetSave().Get<float>(SAVE_PLAYER_POS_X, 960.0f);
        float PlayerY = Subsystem.GetSave().Get<float>(SAVE_PLAYER_POS_Y, 540.0f);
        float ObstacleX = Subsystem.GetSave().Get<float>(SAVE_OBSTACLE_POS_X, 640.0f);
        float ObstacleY = Subsystem.GetSave().Get<float>(SAVE_OBSTACLE_POS_Y, 680.0f);
        
        // Test character
        Character = SpawnActor<TestCharacter>().lock();
        Character->SetPosition({PlayerX, PlayerY});

        // Static obstacle
        auto StaticObstacle = SpawnActor<CollisionActor>().lock();
        StaticObstacle->SetBodyType(b2_staticBody);
        StaticObstacle->SetShapeType(we::PhysicsComponent::EShapeType::Rectangle);
        StaticObstacle->SetPhysicsSize(42.0f); 
        StaticObstacle->SetCollisionSize(64.0f);
        StaticObstacle->SetPosition({400.0f, 540.0f});
        
        // Kinematic obstacle
        auto KinematicObstacle = SpawnActor<CollisionActor>().lock();
        KinematicObstacle->SetBodyType(b2_kinematicBody);
        KinematicObstacle->SetShapeType(we::PhysicsComponent::EShapeType::Circle);
        KinematicObstacle->SetPhysicsSize(42.0f); 
        KinematicObstacle->SetCollisionSize(64.0f);
        KinematicObstacle->SetPosition({640.0f, 400.0f});
        
        // Dynamic obstacle
        DynObstacle = SpawnActor<CollisionActor>().lock();
        DynObstacle->SetBodyType(b2_dynamicBody);
        DynObstacle->SetShapeType(we::PhysicsComponent::EShapeType::Circle);
        DynObstacle->SetPhysicsSize(42.0f);  
        DynObstacle->SetCollisionSize(64.0f);
        DynObstacle->SetPosition({ObstacleX, ObstacleY});

        InputController().Bind(PAUSE_ACTION, Input::Keyboard{ sf::Keyboard::Scan::Escape });

        // Initialize pause menu
        PauseUI = make_unique<PauseMenuUI>();
        PauseUI->Initialize();

        // Bind delegates
        PauseUI->OnResumeClicked.Bind(this, &LevelOne::ResumeGame);
        PauseUI->OnSettingsClicked.Bind(this, &LevelOne::OnSettings);
        PauseUI->OnMainMenuClicked.Bind(this, &LevelOne::ReturnToMainMenu);
        PauseUI->OnSaveAndQuitClicked.Bind(this, &LevelOne::SaveAndQuit);
    }

    void LevelOne::Tick(float DeltaTime)
    {
        if (WaterPPC)
        {
            WaterPPC->Tick(DeltaTime);
        }

        bool bIsPressed = InputController().Pressed(PAUSE_ACTION);
        if (bIsPressed && !bWasPausePressed)
        {
            TogglePauseMenu();
        }
        bWasPausePressed = bIsPressed;
    }

    void LevelOne::EndPlay()
    {
        // Save positions
        if (Character)
        {
            vec2f Pos = Character->GetPosition();
            Subsystem.GetSave().Set(SAVE_PLAYER_POS_X, Pos.x);
            Subsystem.GetSave().Set(SAVE_PLAYER_POS_Y, Pos.y);
            LOG("[LevelOne] Saved player position: {}, {}", Pos.x, Pos.y);
        }
        if (DynObstacle)
        {
            vec2f Pos = DynObstacle->GetPosition();
            Subsystem.GetSave().Set(SAVE_OBSTACLE_POS_X, Pos.x);
            Subsystem.GetSave().Set(SAVE_OBSTACLE_POS_Y, Pos.y);
            LOG("[LevelOne] Saved obstacle position: {}, {}", Pos.x, Pos.y);
        }
        Subsystem.GetSave().Save();
        
        if (PauseUI)
        {
            PauseUI->ClearWidgets();
            PauseUI.reset();
        }

        if (WaterPPC)
        {
            WaterPPC->EndPlay();
            WaterPPC.reset();
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

    void LevelOne::SaveAndQuit()
    {
        LOG("[LevelOne] Save & Quit requested");
        
        // Save positions first
        if (Character)
        {
            vec2f Pos = Character->GetPosition();
            Subsystem.GetSave().Set(SAVE_PLAYER_POS_X, Pos.x);
            Subsystem.GetSave().Set(SAVE_PLAYER_POS_Y, Pos.y);
            LOG("[LevelOne] Saved player position: {}, {}", Pos.x, Pos.y);
        }
        if (DynObstacle)
        {
            vec2f Pos = DynObstacle->GetPosition();
            Subsystem.GetSave().Set(SAVE_OBSTACLE_POS_X, Pos.x);
            Subsystem.GetSave().Set(SAVE_OBSTACLE_POS_Y, Pos.y);
            LOG("[LevelOne] Saved obstacle position: {}, {}", Pos.x, Pos.y);
        }
        
        // Flush to disk and quit
        Subsystem.GetSave().Save();
        Subsystem.Quit();
    }
}