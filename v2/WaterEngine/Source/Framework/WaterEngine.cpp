// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "AssetDirectory/PakDirectory.h"

//#include "EngineConfig.h"
//#include "Subsystem/ResourceSubsystem.h"
//#include "Subsystem/PhysicsSubsystem.h"
//#include "Subsystem/AsyncResourceSubsystem.h"
//#include "Framework/World/World.h"
//#include "Utility/Timer.h"
//#include "Utility/Log.h"

namespace we
{
    WaterEngine::WaterEngine()
    {
        PreConstruct();
    }

    WaterEngine::~WaterEngine()
    {

    }

    void WaterEngine::PreConstruct()
    {
        MountAssetDirectory();
        // Build All Subsystems
        Subsystem.Window   = make_unique<WindowSubsystem>();
        Subsystem.Render   = make_unique<RenderSubsystem>();
        Subsystem.Time     = make_unique<TimeSubsystem>();
        Subsystem.Cursor   = make_unique<CursorSubsystem>();
        Subsystem.Input    = make_unique<InputSubsystem>();

        /*Subsystem.SaveLoad = make_unique<SaveLoadSubsystem>();
        Subsystem.Audio = make_unique<AudioSubsystem>();

        WindowInit();

        Subsystem.World = make_unique<WorldSubsystem>(Subsystem);
        Subsystem.GameState = make_unique<GameStateSubsystem>();
        
        Physics().Initialize();*/

    }

    void WaterEngine::MountAssetDirectory()
    {
        // Mount .pak For Resources
        auto PD = make_shared<PakDirectory>(EC.AssetDirectory);
        //Asset().SetAssetDirectory(PD);
        //AsyncAsset().SetAssetDirectory(PD);
        if (EC.DisableSFMLLogs) { sf::err().rdbuf(nullptr); }
    }

    void WaterEngine::TickGame()
    {
        float DeltaTime = Subsystem.Time->GetDeltaTime();

        //if (Subsystem.GameState->IsTransitionPending())
        //{
        //    Subsystem.GameState->ApplyPendingState();
        //}

        //TimerManager::Get().Tick(DeltaTime);
        Subsystem.Input->ProcessHeld();
        //Subsystem.Cursor->Update(DeltaTime);
        //Subsystem.GUI->Update(DeltaTime);
        //AsyncAsset().GarbageCycle(DeltaTime);

        //if (auto World = Subsystem.World->GetCurrentWorld())
        //{
        //    World->BeginPlayGlobal();
        //    World->TickGlobal(DeltaTime);
        //}

        //Physics().Step(DeltaTime);

        PostUpdate();
    }

    void WaterEngine::Run()
    {
        while (IsRunning())
        {
            ProcessEvents();

            Subsystem.Time->Tick(); // Always tick time

            //AsyncAsset().PollCompletedRequests();

            if (!Subsystem.Time->IsPaused())
            {
                TickGame(); // World ticks only when not paused
            }
            else
            {
                Subsystem.Cursor->Update(Subsystem.Time->GetUnscaledDeltaTime());
                // UI only update when paused
                /*Subsystem.Input->ProcessHeld();

                Subsystem.GUI->Update(Subsystem.Time->GetUnscaledDeltaTime());*/
                PostUpdate();
            }

            Update();
        }
    }

    void WaterEngine::ProcessEvents()
    {
        while (const auto Event = Subsystem.Window->pollEvent())
        {
            Subsystem.Window->HandleEvent(*Event);
            Subsystem.Input->HandleEvent(*Event);
            //Subsystem.GUI->ProcessEvent(*Event);
        }

        if (Subsystem.Window->hasFocus())
        {
            Subsystem.Cursor->SetPosition(Subsystem.Window->GetMousePosition());
        }
    }
   
    void WaterEngine::Update()
    {
        Subsystem.Window->clear(color::Black);

        Subsystem.Render->StartRender();

       /* if (auto World = Subsystem.World->GetCurrentWorld())
        {
            World->Render();
        }*/

        //Physics().DebugDraw(Subsystem.Render.get());

        //Subsystem.Window->draw(sprite(Subsystem.Render->FinishRender()));
        
        // Subsystem.GUI->Render();

        Subsystem.Cursor->Render(*Subsystem.Render);

        Subsystem.Window->setView(Subsystem.Window->GetConstrainedView());

        Subsystem.Window->draw(Subsystem.Render->FinishRender());

        Subsystem.Window->display();
    }

    void WaterEngine::PostUpdate()
    {

    }

    bool WaterEngine::IsRunning() const
    {
        return Subsystem.Window->isOpen();
    }

    bool WaterEngine::HasFocus() const
    {
        return Subsystem.Window->hasFocus();
    }

    void WaterEngine::Shutdown()
    {
        //bExitRequested = true;
    }
}