// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Tests/TestCharacter.h"
#include "Tests/CollisionActor.h"
#include "Component/PhysicsComponent.h"
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

        // Spawn 3 obstacles with different physics body types for testing
        
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
        auto DynamicObstacle = SpawnActor<CollisionActor>().lock();
        DynamicObstacle->SetBodyType(b2_dynamicBody);
        DynamicObstacle->SetShapeType(we::PhysicsComponent::EShapeType::Circle);
        DynamicObstacle->SetPhysicsSize(42.0f);  
        DynamicObstacle->SetCollisionSize(64.0f);
        DynamicObstacle->SetPosition({640.0f, 680.0f});

        InputController().Bind(PAUSE_ACTION, Input::Keyboard{ sf::Keyboard::Scan::Escape });

        // Initialize pause menu
        PauseUI = make_unique<PauseMenuUI>();
        PauseUI->Initialize();

        // Bind delegates
        PauseUI->OnResumeClicked.Bind(this, &LevelOne::ResumeGame);
        PauseUI->OnSettingsClicked.Bind(this, &LevelOne::OnSettings);
        PauseUI->OnMainMenuClicked.Bind(this, &LevelOne::ReturnToMainMenu);
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