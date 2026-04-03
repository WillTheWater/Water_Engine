// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/LevelOne.h"
#include "Levels/MainMenu.h"
#include "Levels/Credits.h"
#include "Levels/LevelOneBarriers.h"
#include "Level/LevelObject.h"
#include "Framework/World/Actor.h"
#include "Framework/World/Character.h"
#include "Player/PlayerCharacter.h"
#include "Component/CameraComponent.h"
#include "PostProcess/Effects/PPEScroll.h"
#include "Component/PhysicsComponent.h"
#include "UI/PauseMenuUI.h"
#include "Subsystem/TimerSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/WorldSubsystem.h"
#include "Subsystem/SaveSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Component/PostProcessingComponent.h"
#include "Framework/World/Barrier.h"
#include "NPC/AoiMizukawa.h"
#include "NPC/Kiyoshi.h"
#include "Interaction/Compass.h"
#include "Interaction/ForestExit.h"
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
        WaterImage->SetPosition({ 0, 2900 });
        WaterImage->SetScale({ 1.1,1 });
        WaterPPC = we::make_shared<PostProcessingComponent>(WaterImage.get());
        WaterPPC->SetTexture(Water);
        WaterPPC->AddEffect(make_unique<PPEScroll>());
        WaterPPC->AddEffect(make_unique<PPEWave>());
        WaterPPC->BeginPlay();

        // Load saved data and update session counter
        int TimesPlayed = Subsystem.GetSave().Get<int>(SAVE_TIMES_PLAYED, 0);
        TimesPlayed++;
        Subsystem.GetSave().Set(SAVE_TIMES_PLAYED, TimesPlayed);
        Subsystem.GetSave().Set(SAVE_LAST_LEVEL, "LevelOne");
        
        // Load positions
        float PlayerX = Subsystem.GetSave().Get<float>(SAVE_PLAYER_POS_X, 5510.f);
        float PlayerY = Subsystem.GetSave().Get<float>(SAVE_PLAYER_POS_Y, 2600.f);
        
        // Test character
        Character = SpawnActor<PlayerCharacter>().lock();
        Character->SetPosition({PlayerX, PlayerY});

        auto Aoi = SpawnActor<AoiMizukawa>().lock();
        Aoi->SetPosition({ 4700, 2400 });

        auto Kiyo = SpawnActor<Kiyoshi>().lock();
        Kiyo->SetPosition({1280, 1020});
        Kiyo->SetWaypoints({
            {1280, 1020}, 
            {1626, 1175}, 
            {2630, 1360}, 
            {4200, 1180}, 
            {5381, 1111}, 
            {4770, 1681}, 
            {1352, 1750}
        });

        // Spawn quest item (compass)
        auto CompassItem = SpawnActor<Compass>().lock();
        CompassItem->SetPosition({ 5281, 544 });

        // Spawn forest exit (top of level)
        auto Exit = SpawnActor<ForestExit>().lock();
        Exit->SetPosition({ 3050.0f, 20.0f });
        Exit->OnExitTriggered.Bind(this, &LevelOne::OnExitGame);

        // Create all level barriers
        CreateLevelOneBarriers(*this);

        // Hut 1
        Hut1 = SpawnActor<Actor>().lock();
        Hut1Shadow = SpawnActor<Actor>().lock();
        Hut1Tex = LoadAsset().LoadTexture("Assets/Textures/Game/hut1.png");
        Hut1TexShadow = LoadAsset().LoadTexture("Assets/Textures/Game/hut1shadow.png");
        Hut1->SetSprite(Hut1Tex);
        Hut1Shadow->SetSprite(Hut1TexShadow);
        Hut1->SetSpriteOrigin({ 545, 747 });
        Hut1Shadow->SetSpriteOrigin({ 545, 747 });
        Hut1->SetPosition({777, 927});
        Hut1Shadow->SetPosition({777, 927});
        Hut1Shadow->SetCustomRenderDepth(10000.f);

        // Hut 2
        Hut2 = SpawnActor<Actor>().lock();
        Hut2Shadow = SpawnActor<Actor>().lock();
        Hut2Tex = LoadAsset().LoadTexture("Assets/Textures/Game/hut2.png");
        Hut2TexShadow = LoadAsset().LoadTexture("Assets/Textures/Game/hut2shadow.png");
        Hut2->SetSprite(Hut2Tex);
        Hut2Shadow->SetSprite(Hut2TexShadow);
        Hut2->SetSpriteOrigin({ 556, 674 });
        Hut2Shadow->SetSpriteOrigin({ 556, 674 });
        Hut2->SetPosition({ 2097, 707 });
        Hut2Shadow->SetPosition({ 2097, 707 });
        Hut2Shadow->SetCustomRenderDepth(10000.f);

        // Hut 3
        Hut3 = SpawnActor<Actor>().lock();
        Hut3Shadow = SpawnActor<Actor>().lock();
        Hut3Tex = LoadAsset().LoadTexture("Assets/Textures/Game/hut3.png");
        Hut3TexShadow = LoadAsset().LoadTexture("Assets/Textures/Game/hut3shadow.png");
        Hut3->SetSprite(Hut3Tex);
        Hut3Shadow->SetSprite(Hut3TexShadow);
        Hut3->SetSpriteOrigin({ 414, 606 });
        Hut3Shadow->SetSpriteOrigin({ 414, 606 });
        Hut3->SetPosition({ 3800, 606 });
        Hut3Shadow->SetPosition({ 3800, 606 });
        Hut3Shadow->SetCustomRenderDepth(10000.f);

        // Tree 1
        Tree1 = we::make_unique<LevelObject>(
            *this,
            "Assets/Textures/Game/tree1.png",
            "Assets/Textures/Game/tree1Shadow.png",
            vec2f{5358, 778},
            vec2f{384, 885},
            80.f,
            vec2f{0, 70}
        );
        Tree1->Init();

        // Tree 2
        Tree2 = we::make_unique<LevelObject>(
            *this,
            "Assets/Textures/Game/tree2.png",
            "Assets/Textures/Game/tree2Shadow.png",
            vec2f{ 2500, 1024 },
            vec2f{ 330, 580 },
            46.f,
            vec2f{ 10, 50 }
        );
        Tree2->Init();

        // Tree 3
        Tree3 = we::make_unique<LevelObject>(
            *this,
            "Assets/Textures/Game/tree3.png",
            "Assets/Textures/Game/tree3Shadow.png",
            vec2f{ 1398, 427 },
            vec2f{ 612, 903 },
            80.f,
            vec2f{ 0, 70 }
        );
        Tree3->Init();

        // Tree 4
        Tree4 = we::make_unique<LevelObject>(
            *this,
            "Assets/Textures/Game/tree4.png",
            "Assets/Textures/Game/tree4Shadow.png",
            vec2f{ 2496, 357 },
            vec2f{ 523, 813 },
            80.f,
            vec2f{ 0, 70 }
        );
        Tree4->Init();

        // Tree 5
        Tree5 = we::make_unique<LevelObject>(
            *this,
            "Assets/Textures/Game/palm1.png",
            "Assets/Textures/Game/palm1Shadow.png",
            vec2f{ 386, 1959 },
            vec2f{ 385, 803 },
            35.f,
            vec2f{ 0, 60 }
        );
        Tree5->Init();

        // Tree 6
        Tree6 = we::make_unique<LevelObject>(
            *this,
            "Assets/Textures/Game/palm1.png",
            "Assets/Textures/Game/palm1Shadow.png",
            vec2f{ 3907, 1407 },
            vec2f{ 385, 803 },
            35.f,
            vec2f{ 0, 60 }
        );
        Tree6->Init();

        // Tree 7
        Tree7 = we::make_unique<LevelObject>(
            *this,
            "Assets/Textures/Game/palm2.png",
            "Assets/Textures/Game/palm2Shadow.png",
            vec2f{ 5178, 2000 },
            vec2f{ 556, 900 },
            58.f,
            vec2f{ -10, 60 }
        );
        Tree7->Init();

        // Tree 8
        Tree8 = we::make_unique<LevelObject>(
            *this,
            "Assets/Textures/Game/palm2.png",
            "Assets/Textures/Game/palm2Shadow.png",
            vec2f{ 1966, 1490 },
            vec2f{ 556, 900 },
            58.f,
            vec2f{ -10, 60 }
        );
        Tree8->Init();

        InputController().Bind(PAUSE_ACTION, Input::Keyboard{ sf::Keyboard::Scan::Escape });
        PauseBinding = InputController().BindAction(PAUSE_ACTION, this, &LevelOne::TogglePauseMenu);

        // Initialize pause menu
        PauseUI = make_unique<PauseMenuUI>();
        PauseUI->Initialize(Subsystem.GetSave());

        // Bind delegates
        PauseUI->OnResumeClicked.Bind(this, &LevelOne::ResumeGame);
        PauseUI->OnMainMenuClicked.Bind(this, &LevelOne::ReturnToMainMenu);
        PauseUI->OnSaveAndQuitClicked.Bind(this, &LevelOne::SaveAndQuit);

        // Hide cursor during gameplay
        GetCursor().SetVisibility(false);
    }

    void LevelOne::Tick(float DeltaTime)
    {
        if (WaterPPC)
        {
            WaterPPC->Tick(DeltaTime);
        }
    }

    void LevelOne::EndPlay()
    {
        if (Character)
        {
            vec2f Pos = Character->GetPosition();
            Subsystem.GetSave().Set(SAVE_PLAYER_POS_X, Pos.x);
            Subsystem.GetSave().Set(SAVE_PLAYER_POS_Y, Pos.y);
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

    void LevelOne::SaveAndQuit()
    {
        if (Character)
        {
            vec2f Pos = Character->GetPosition();
            Subsystem.GetSave().Set(SAVE_PLAYER_POS_X, Pos.x);
            Subsystem.GetSave().Set(SAVE_PLAYER_POS_Y, Pos.y);
        }
        
        Subsystem.GetSave().Save();
        Subsystem.Quit();
    }

    void LevelOne::OnExitGame()
    {
        Subsystem.LoadWorld<Credits>();
    }
}