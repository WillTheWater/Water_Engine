// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EngineConfig.h"
#include "Framework/WaterEngine.h"
#include "AssetDirectory/PakDirectory.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Framework/World/World.h"
#include "Utility/Log.h"

namespace we
{
    WaterEngine::WaterEngine()
    {
        Configure();
        Construct();
        WindowInit();
    }

    WaterEngine::~WaterEngine()
    {
        Subsystem.World->UnloadWorld();
        Subsystem.Render.reset();
        Asset().Shutdown();
    }

    void WaterEngine::Configure()
    {
        auto PD = make_shared<PakDirectory>(EC.AssetDirectory);
        Asset().SetAssetDirectory(PD);
        if (EC.DisableSFMLLogs) { sf::err().rdbuf(nullptr); }
    }

    void WaterEngine::Construct()
    {
        Subsystem.Time = make_unique<TimerSubsystem>();
        Subsystem.Render = make_unique<RenderSubsystem>();
        Subsystem.SaveLoad = make_unique<SaveLoadSubsystem>();
        Subsystem.Audio = make_unique<AudioSubsystem>();
        Subsystem.Input = make_unique<InputSubsystem>();
        Subsystem.World = make_unique<WorldSubsystem>(Subsystem);
        Subsystem.GameState = make_unique<GameStateSubsystem>();
    }

    void WaterEngine::WindowInit()
    {
        Window = make_unique<GameWindow>();
        Window->OnResize = [this](vec2u newSize)
            {
                const auto CorrectedView = Subsystem.Render->ConstrainView(Window->getSize());
                Window->setView(CorrectedView);
            };
        Subsystem.GUI = make_unique<GUISubsystem>(*Window);
        Subsystem.GameCursor = make_unique<Cursor>(*Window);
    }

    void WaterEngine::ProcessEvents()
    {
        while (const auto Event = Window->pollEvent())
        {
            Event->visit(GameWindowEventHandler{ *Window });
            Subsystem.Input->HandleEvent(*Event);
        }
    }

    void WaterEngine::PostUpdate()
    {
        Subsystem.Input->PostUpdate();
    }

    void WaterEngine::GlobalTick()
    {
        Subsystem.Time->Tick();
        Tick(Subsystem.Time->GetDeltaTime());
        if (Subsystem.GameState->IsTransitionPending()) { Subsystem.GameState->ApplyPendingState(); }
        Subsystem.Input->ProcessHeld();
        Subsystem.GameCursor->Update(Subsystem.Time->GetDeltaTime());
        if (auto World = Subsystem.World->GetCurrentWorld())
        {
            World->BeginPlayGlobal();
            World->TickGlobal(Subsystem.Time->GetDeltaTime());
        }
    }

    void WaterEngine::Tick(float DeltaTime)
    {
    }

    void WaterEngine::Render()
    {
        Window->clear();

        Subsystem.Render->StartRender();

        if (auto World = Subsystem.World->GetCurrentWorld())
        {
            World->Render();
        }

        Window->draw(sprite(Subsystem.Render->FinishRender()));

        Subsystem.GUI->Render();

        Subsystem.GameCursor->Render();

        Window->display();
    }

    bool WaterEngine::IsRunning() const
    {
        return Window->isOpen() && !bExitRequested;
    }

    bool we::WaterEngine::HasFocus() const
    {
        return Window->hasFocus();
    }

    void WaterEngine::ConstrainRender()
    {
        Subsystem.Render->ConstrainView(Window->getSize());
    }

    void WaterEngine::Quit()
    {
        LOG("Engine: Exit requested. Finishing current frame...");
        bExitRequested = true;
    }
}