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
#include "Framework/World/Barrier.h"
#include "NPC/AoiMizukawa.h"
#include "NPC/Kiyoshi.h"
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
        BGImage->SetCustomRenderDepth(-20.f);

        Water = LoadAsset().LoadTexture("Assets/Textures/Game/water.png");
        WaterImage = SpawnActor<Actor>().lock();
        WaterImage->SetPosition({ 0, 3240 - 474 });
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

        auto Aoi = SpawnActor<AoiMizukawa>().lock();
        Aoi->SetPosition({ 4700, 2400 });

        auto Kiyo = SpawnActor<Kiyoshi>().lock();
        Kiyo->SetWaypoints({
            {1200.0f, 1530.0f}, 
            {1550.0f, 1800.0f}, 
            {3500.0f, 1350.0f}, 
            {4300.0f, 1100.0f}, 
            {5210.0f, 1130.0f}, 
            {5480.0f, 1300.0f}, 
            {4780.0f, 1830.0f}, 
            {1550.0f, 1800.0f}  
        });

        auto BottomBarrier = SpawnActor<Barrier>().lock();
        BottomBarrier->SetPoints({
            {0.0f, 2855.0f},
            {5760.0f, 2855.0f}
            });
        BottomBarrier->SetThickness(8.0f);

        auto ForestBarrier = SpawnActor<Barrier>().lock();
        ForestBarrier->SetPoints({
            { 0, 3400 },
            {0.0f, 980},
            {950, 400},
            {1600,220},
            {2200, 316},
            {2680, 290},
            {2950, 0},
            {3295, 0},
            {3510, 370},
            {4230, 460},
            {4970, 330},
            {5415, 480},
            {5760, 1080},
            {5760, 3400}
            });
        ForestBarrier->SetThickness(8.0f);
        ForestBarrier->SetClosed(true);

        Hut1 = SpawnActor<Actor>().lock();
        Hut1Shadow = SpawnActor<Actor>().lock();
        Hut1Tex = LoadAsset().LoadTexture("Assets/Textures/Game/hut1.png");
        Hut1TexShadow = LoadAsset().LoadTexture("Assets/Textures/Game/hut1shadow.png");
        Hut1->SetSprite(Hut1Tex);
        Hut1Shadow->SetSprite(Hut1TexShadow);
        Hut1->SetSpriteOrigin({ 515, 815 });
        Hut1Shadow->SetSpriteOrigin({ 515, 815 });
        Hut1->SetPosition({742, 1335});
        Hut1Shadow->SetPosition({742, 1335});
        Hut1Shadow->SetCustomRenderDepth(10000.f);

        auto Hut1Barrier = SpawnActor<Barrier>().lock();
        Hut1Barrier->SetPoints({
            {215, 1350},
            {865, 940},
            {1265, 1295},
            {915, 1600}
            });
        Hut1Barrier->SetThickness(8.0f);
        Hut1Barrier->SetClosed(true);

        Hut2 = SpawnActor<Actor>().lock();
        Hut2Shadow = SpawnActor<Actor>().lock();
        Hut2Tex = LoadAsset().LoadTexture("Assets/Textures/Game/hut2.png");
        Hut2TexShadow = LoadAsset().LoadTexture("Assets/Textures/Game/hut2shadow.png");
        Hut2->SetSprite(Hut2Tex);
        Hut2Shadow->SetSprite(Hut2TexShadow);
        Hut2->SetSpriteOrigin({ 475, 580 });
        Hut2Shadow->SetSpriteOrigin({ 475, 580 });
        Hut2->SetPosition({ 1420, 710 });
        Hut2Shadow->SetPosition({ 1420, 710 });
        Hut2Shadow->SetCustomRenderDepth(10000.f);

        auto Hut2Barrier = SpawnActor<Barrier>().lock();
        Hut2Barrier->SetPoints({
            {1314, 850},
            {958, 566},
            {1669, 400},
            {1945, 745}
            });
        Hut2Barrier->SetThickness(8.0f);
        Hut2Barrier->SetClosed(true);

        Hut3 = SpawnActor<Actor>().lock();
        Hut3Shadow = SpawnActor<Actor>().lock();
        Hut3Tex = LoadAsset().LoadTexture("Assets/Textures/Game/hut3.png");
        Hut3TexShadow = LoadAsset().LoadTexture("Assets/Textures/Game/hut3shadow.png");
        Hut3->SetSprite(Hut3Tex);
        Hut3Shadow->SetSprite(Hut3TexShadow);
        Hut3->SetSpriteOrigin({ 780, 760 });
        Hut3Shadow->SetSpriteOrigin({ 780, 760 });
        Hut3->SetPosition({ 4175, 870 });
        Hut3Shadow->SetPosition({ 4175, 870 });
        Hut3Shadow->SetCustomRenderDepth(10000.f);

        auto Hut3Barrier = SpawnActor<Barrier>().lock();
        Hut3Barrier->SetPoints({
            {3425, 735},
            {3700, 800},
            {3822, 989},
            {3709, 989},
            {3709, 1100},
            {4000, 1100},
            {4000, 980},
            {4380, 980},
            {4400, 1050},
            {4500, 1050},
            {4340, 500},
            });
        Hut3Barrier->SetThickness(8.0f);
        Hut3Barrier->SetClosed(true);

        InputController().Bind(PAUSE_ACTION, Input::Keyboard{ sf::Keyboard::Scan::Escape });

        // Initialize pause menu
        PauseUI = make_unique<PauseMenuUI>();
        PauseUI->Initialize(Subsystem.GetSave());

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
            Subsystem.Resume();
        }
        else
        {
            PauseUI->Show();
            Subsystem.Pause();
        }
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