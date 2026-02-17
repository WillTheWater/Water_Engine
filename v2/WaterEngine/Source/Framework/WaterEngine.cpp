// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "AssetDirectory/PakDirectory.h"
#include "Framework/World/World.h"
#include "Utility/Log.h"

//#include "Subsystem/PhysicsSubsystem.h"
//#include "Utility/Timer.h"

namespace we
{
    WaterEngine::WaterEngine()
    {
        PreConstruct();
        Construct();
    }

    WaterEngine::~WaterEngine()
    {

    }

    void WaterEngine::PreConstruct()
    {
        MountAssetDirectory();
        // Build All Subsystems
        Subsystem.Window     = make_unique<WindowSubsystem>();
        Subsystem.Render     = make_unique<RenderSubsystem>();
        Subsystem.Time       = make_unique<TimeSubsystem>();
        Subsystem.Cursor     = make_unique<CursorSubsystem>();
        Subsystem.Input      = make_unique<InputSubsystem>();
        Subsystem.SaveLoad   = make_unique<SaveLoadSubsystem>();
        Subsystem.GameState  = make_unique<GameStateSubsystem>();
        Subsystem.World      = make_unique<WorldSubsystem>(Subsystem);
        Subsystem.Audio      = make_unique<AudioSubsystem>();
        Subsystem.GUI        = make_unique<GUISubsystem>(Subsystem);

        //Physics().Initialize();
    }

    void WaterEngine::MountAssetDirectory()
    {
        // Mount .pak For Resources
        auto PD = make_shared<PakDirectory>(EC.AssetDirectory);
        Subsystem.AssetLoader = make_unique<ResourceSubsystem>();
        Subsystem.AssetLoader->SetAssetDirectory(PD);
        if (EC.DisableSFMLLogs) { sf::err().rdbuf(nullptr); }
    }

    void WaterEngine::TickGame()
    {
        float DeltaTime = Subsystem.Time->GetDeltaTime();

        Tick(DeltaTime);

        if (Subsystem.GameState->IsTransitionPending())
        {
            Subsystem.GameState->ApplyPendingState();
        }

        //TimerManager::Get().Tick(DeltaTime);
        
        Subsystem.Input->ProcessHeld();

        Subsystem.AssetLoader->GarbageCycle(DeltaTime);

        if (auto World = Subsystem.World->GetCurrentWorld())
        {
            World->BeginPlayGlobal();
            World->TickGlobal(DeltaTime);
        }

        //Physics().Step(DeltaTime);

        PostUpdate();
    }

    void WaterEngine::Run()
    {
        BeginPlay();

        while (IsRunning())
        {
            ProcessEvents();

            Subsystem.Time->Tick(); // Always tick time

            Subsystem.AssetLoader->PollCompletedRequests();

            if (!Subsystem.Time->IsPaused())
            {
                TickGame(); // World ticks only when not paused
            }
            else
            {
                Subsystem.Cursor->Update(Subsystem.Time->GetUnscaledDeltaTime());

                Subsystem.Input->ProcessHeld();

                Subsystem.GUI->Update(Subsystem.Time->GetUnscaledDeltaTime());

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
            Subsystem.GUI->HandleEvent(*Event);
            Subsystem.Input->HandleEvent(*Event);
        }

        if (Subsystem.Window->hasFocus())
        {
            // Set mapped position for game/world rendering
            Subsystem.Cursor->SetPosition(Subsystem.Window->GetMousePosition());
            // Set raw pixel position for UI hit testing
            vec2i PixelPos = sf::Mouse::getPosition(*Subsystem.Window);
            Subsystem.Cursor->SetPixelPosition(vec2f(static_cast<float>(PixelPos.x), static_cast<float>(PixelPos.y)));
        }
    }
   
    void WaterEngine::Update()
    {
        Subsystem.Audio->Update();
        
        Subsystem.Window->clear(color::Black);

        Subsystem.Render->StartRender();

        WorldRender();

        //Physics().DebugDraw(Subsystem.Render.get());

        Subsystem.GUI->Render();

        Subsystem.Cursor->Render(*Subsystem.Render);

        Subsystem.Window->setView(Subsystem.Window->GetConstrainedView());

        Subsystem.Window->draw(Subsystem.Render->FinishRender());

        Subsystem.Window->display();
    }

    void WaterEngine::WorldRender()
    {
        static vector<RenderDepth> WorldRenderDepths;
        WorldRenderDepths.clear();

        if (auto World = Subsystem.World->GetCurrentWorld())
        {
            World->CollectRenderDepths(WorldRenderDepths);
        }

        // Sort by depth (Y position) - back to front
        std::sort(WorldRenderDepths.begin(), WorldRenderDepths.end());

        // Draw sorted
        for (const auto& RenderCmd : WorldRenderDepths)
        {
            Subsystem.Render->Draw(*RenderCmd.Drawable, ERenderLayer::Game);
        }
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
}