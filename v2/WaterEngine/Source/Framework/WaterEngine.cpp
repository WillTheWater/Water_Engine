// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "AssetDirectory/PakDirectory.h"
#include "Framework/World/World.h"
#include "Utility/Log.h"
#include "Utility/DebugDraw.h"
#include "Utility/Timer.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/PhysicsSubsystem.h"

namespace we
{
    WaterEngine::WaterEngine()
    {
        PreConstruct();
        Construct();
    }

    WaterEngine::~WaterEngine()
    {
        // GameInstance is destroyed last (declared first in EngineSubsystem)
        // Shutdown is called automatically in GameInstance destructor
        if (Subsystem.GameInst)
        {
            Subsystem.GameInst->Shutdown();
        }
    }

    void WaterEngine::PreConstruct()
    {
        MountAssetDirectory();

        // Build All Subsystems with explicit configuration
        Subsystem.Window = make_unique<WindowSubsystem>(WindowConfig{
            .WindowName = EC.WindowName,
            .RenderResolution = EC.RenderResolution,
            .AspectRatio = EC.AspectRatio,
            .WindowMinimumSize = EC.WindowMinimumSize,
            .WindowIcon = EC.WindowIcon,
            .FullscreenMode = EC.FullscreenMode,
            .VsyncEnabled = EC.VsyncEnabled,
            .TargetFPS = EC.TargetFPS,
            .EnableKeyRepeat = EC.EnableKeyRepeat,
            .DisableSFMLLogs = EC.DisableSFMLLogs
        });

        Subsystem.Render = make_unique<RenderSubsystem>(RenderConfig{
            .RenderResolution = EC.RenderResolution,
            .SetRenderSmooth = EC.SetRenderSmooth
        });

        Subsystem.Time = make_unique<TimeSubsystem>();
        Subsystem.Input = make_unique<InputSubsystem>();
        Subsystem.SaveLoad = make_unique<SaveLoadSubsystem>();
        Subsystem.GameState = make_unique<GameStateSubsystem>();

        Subsystem.Cursor = make_unique<CursorSubsystem>(CursorConfig{
            .DefaultCursor = EC.DefaultCursor,
            .DefaultCursorSize = EC.DefaultCursorSize,
            .DefaultCursorSpeed = EC.DefaultCursorSpeed,
            .JoystickDeadzone = EC.JoystickDeadzone,
            .RenderResolution = EC.RenderResolution
        });

        Subsystem.Audio = make_unique<AudioSubsystem>(AudioConfig{
            .StartupGlobalVolume = EC.StartupGlobalVolume,
            .MaxSFXStack = EC.MaxSFXStack
        });

        Subsystem.Physics = make_unique<PhysicsSubsystem>(PhysicsConfig{
            .DefaultGravity = vec2f{EC.DefaultGravity.x, EC.DefaultGravity.y},
            .PhysicsScale = 0.01f,
            .FixedTimeStep = 1.0f / 60.0f,
            .VelocityIterations = 8,
            .PositionIterations = 3
        });

        // These subsystems need access to other subsystems
        Subsystem.World = make_unique<WorldSubsystem>(Subsystem);
        Subsystem.GUI = make_unique<GUISubsystem>(Subsystem);
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

        // Update cursor (joystick control)
        Subsystem.Cursor->Update(DeltaTime, *Subsystem.Window);

        // Poll gamepad axes (only if gamepad connected)
        Subsystem.Input->PollGamepadAxes();

        // Update timers
        TimerManager::Get().Tick(DeltaTime);

        Tick(DeltaTime);
        
        if (Subsystem.GameInst)
        {
            Subsystem.GameInst->Tick(DeltaTime);
        }

        if (Subsystem.GameState->IsTransitionPending())
        {
            Subsystem.GameState->ApplyPendingState();
        }

        Subsystem.Input->ProcessHeld();
        Subsystem.AssetLoader->GarbageCycle(DeltaTime);

        if (auto World = Subsystem.World->GetCurrentWorld())
        {
            World->BeginPlayGlobal();
            World->TickGlobal(DeltaTime);
        }

        Subsystem.Physics->Step(DeltaTime);
    }

    void WaterEngine::Initialize()
    {
        // Create GameInstance via game-specific factory
        Subsystem.GameInst = CreateGameInstance();
        if (Subsystem.GameInst)
        {
            Subsystem.GameInst->Init(Subsystem);
        }
        
        BeginPlay();
    }

    void WaterEngine::Tick()
    {
        Subsystem.Time->Tick();
        Subsystem.AssetLoader->PollCompletedRequests();

        if (!Subsystem.Time->IsPaused())
        {
            TickGame();
        }
        else
        {
            Subsystem.Cursor->Update(Subsystem.Time->GetUnscaledDeltaTime(), *Subsystem.Window);
            Subsystem.Input->ProcessHeld();
            Subsystem.GUI->Update(Subsystem.Time->GetUnscaledDeltaTime());
        }
    }

    void WaterEngine::Render()
    {
        Subsystem.Audio->Update();
        Subsystem.Window->clear(color::Black);
        Subsystem.Render->StartRender();

        WorldRender();
        Subsystem.GUI->Render();
        Subsystem.Cursor->Render(*Subsystem.Render);

        Subsystem.Window->setView(Subsystem.Window->GetConstrainedView());
        Subsystem.Window->draw(Subsystem.Render->FinishRender());
        Subsystem.Window->display();
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

        // Render debug primitives on top of game world (auto-clears after render)
        DebugDraw::Render(*Subsystem.Render);
    }

    void WaterEngine::PostUpdate()
    {

    }

    bool WaterEngine::IsRunning() const
    {
        return Subsystem.Window->isOpen() && !Subsystem.GameState->IsShutdownRequested();
    }

    bool WaterEngine::HasFocus() const
    {
        return Subsystem.Window->hasFocus();
    }
}