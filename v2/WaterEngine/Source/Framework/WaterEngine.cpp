// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EngineConfig.h"
#include "Framework/WaterEngine.h"
#include "AssetDirectory/PakDirectory.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Subsystem/AsyncResourceSubsystem.h"
#include "Framework/World/World.h"
#include "Utility/Timer.h"
#include "Utility/Log.h"

namespace we
{
    WaterEngine::WaterEngine()
    {
        PreConstruct();
        Configure();
    }

    WaterEngine::~WaterEngine()
    {

    }

    void WaterEngine::PreConstruct()
    {
        // Build All Subsystems

        Subsystem.Time = make_unique<TimerSubsystem>();
        Subsystem.Render = make_unique<RenderSubsystem>();
        Subsystem.SaveLoad = make_unique<SaveLoadSubsystem>();
        Subsystem.Audio = make_unique<AudioSubsystem>();

        WindowInit();

        Subsystem.Input = make_unique<InputSubsystem>(*Subsystem.Cursor);
        Subsystem.World = make_unique<WorldSubsystem>(Subsystem);
        Subsystem.GameState = make_unique<GameStateSubsystem>();
        
        Physics().Initialize();

    }

    void WaterEngine::Configure()
    {
        // Mount .pak For Resources
        auto PD = make_shared<PakDirectory>(EC.AssetDirectory);
        Asset().SetAssetDirectory(PD);
        AsyncAsset().SetAssetDirectory(PD);
        if (EC.DisableSFMLLogs) { sf::err().rdbuf(nullptr); }
    }

    void WaterEngine::TickGame()
    {
        if (!bHasBegunPlay)
        {
            bHasBegunPlay = true;
            BeginPlay();
        }

        float DeltaTime = Subsystem.Time->GetDeltaTime();

        Tick(DeltaTime);

        if (Subsystem.GameState->IsTransitionPending())
        {
            Subsystem.GameState->ApplyPendingState();
        }

        TimerManager::Get().Tick(DeltaTime);
        Subsystem.Input->ProcessHeld();
        Subsystem.Cursor->Update(DeltaTime);
        Subsystem.GUI->Update(DeltaTime);
        AsyncAsset().GarbageCycle(DeltaTime);

        if (auto World = Subsystem.World->GetCurrentWorld())
        {
            World->BeginPlayGlobal();
            World->TickGlobal(DeltaTime);
        }

        Physics().Step(DeltaTime);

        PostUpdate();
    }

    void WaterEngine::Run()
    {
        while (IsRunning())
        {
            ProcessEvents();

            Subsystem.Time->Tick(); // Always tick time

            AsyncAsset().PollCompletedRequests();

            if (!Subsystem.Time->IsPaused())
            {
                TickGame(); // World ticks only when not paused
            }
            else
            {
                // UI only update when paused
                Subsystem.Input->ProcessHeld();
                Subsystem.Cursor->Update(Subsystem.Time->GetUnscaledDeltaTime());
                Subsystem.GUI->Update(Subsystem.Time->GetUnscaledDeltaTime());
                PostUpdate();
            }

            Render();
        }
    }

    void WaterEngine::ProcessEvents()
    {
        while (const auto Event = Window->pollEvent())
        {
            Event->visit(GameWindowEventHandler{ *Window });
            Subsystem.Input->HandleEvent(*Event);
            Subsystem.GUI->ProcessEvent(*Event);
        }
    }
   
    void WaterEngine::Update()
    {
        Window->clear(color::Black);

        Subsystem.Render->StartRender();

        if (auto World = Subsystem.World->GetCurrentWorld())
        {
            World->Render();
        }

        Physics().DebugDraw(Subsystem.Render.get());

        Window->draw(sprite(Subsystem.Render->FinishRender()));
        
        Subsystem.GUI->Render();

        Subsystem.Cursor->Render();

        Window->display();
    }

    void WaterEngine::PostUpdate()
    {

    }

    bool WaterEngine::IsRunning() const
    {
        bExitRequested;
    }

    bool WaterEngine::HasFocus() const
    {
        return Window->hasFocus();
    }

    void WaterEngine::Shutdown()
    {
        bExitRequested = true;
    }

    void WaterEngine::BeginPlay()
    {

    }

    void WaterEngine::Tick(float DeltaTime)
    {
    }



    // NEEDS REFACTORED



    void WaterEngine::WindowInit()
    {
        Window = make_unique<GameWindow>();
        Window->OnResize.Bind(this, &WaterEngine::ConstrainRender);
        Subsystem.Cursor = make_unique<CursorSubsystem>(*Window);
        Subsystem.GUI = make_unique<GUISubsystem>(*Window, *Subsystem.Cursor);
    }

    void WaterEngine::ConstrainRender(vec2u NewSize)
    {
        Window->setView(Subsystem.Render->ConstrainView(NewSize));
    }

}