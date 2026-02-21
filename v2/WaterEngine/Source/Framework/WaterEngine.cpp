// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <filesystem>

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
            .Window = *Subsystem.Window
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
        Subsystem.Camera = make_unique<CameraSubsystem>();
    }

    void WaterEngine::MountAssetDirectory()
    {
        // Mount .pak For Resources
        Subsystem.AssetLoader = make_unique<ResourceSubsystem>();
        auto PD = make_shared<PakDirectory>(EC.AssetDirectory);
        Subsystem.AssetLoader->SetAssetDirectory(PD);
        if (EC.DisableSFMLLogs) { sf::err().rdbuf(nullptr); }
    }

    void WaterEngine::TickGame()
    {
        float DeltaTime = Subsystem.Time->GetDeltaTime();

        // Update cursor (joystick control)
        Subsystem.Cursor->Update(DeltaTime);

        
        // Joystick axes polled on-demand via GetAxisValue(), no bulk polling needed

        // Update timers
        TimerManager::Get().Tick(DeltaTime);

        // Update GUI (for hover state and dragging)
        Subsystem.GUI->Update(DeltaTime);

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
            Subsystem.Cursor->Update(Subsystem.Time->GetUnscaledDeltaTime());
            Subsystem.Input->ProcessHeld();
            Subsystem.GUI->Update(Subsystem.Time->GetUnscaledDeltaTime());
        }
    }

    void WaterEngine::Render()
    {
        Subsystem.Audio->Update();
        Subsystem.Window->clear(color::Black);
        
        // 1. Begin frame - clear all targets, reset all views to default
        Subsystem.Render->BeginFrame();

        // 2. Set up world view from camera (if available)
        if (Subsystem.Camera->HasActiveCamera())
        {
            CameraView View;
            if (Subsystem.Camera->GetCurrentView(View))
            {
                Subsystem.Render->SetWorldView(View);
            }
        }
        // If no camera, world view stays at default (set by BeginFrame)

        // 3. Render world (uses camera view or default)
        WorldRender();

        // 4. Reset to default views for screen-space rendering
        Subsystem.Render->ResetToDefaultViews();

        // 5. Render UI (widgets route to ScreenUI or WorldUI based on their space)
        Subsystem.GUI->Render();

        // 6. Render cursor (screen space)
        Subsystem.Cursor->Render(*Subsystem.Render);

        // 7. Composite and present
        sprite Composite = Subsystem.Render->FinishRender();
        
        // Scale composite to fill the window without changing view
        // This keeps mouse coordinates accurate (1:1 with window pixels)
        vec2u WindowSize = Subsystem.Window->getSize();
        vec2f Scale(
            static_cast<float>(WindowSize.x) / EC.RenderResolution.x,
            static_cast<float>(WindowSize.y) / EC.RenderResolution.y
        );
        Composite.setScale(Scale);
        
        Subsystem.Window->draw(Composite);
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
            // Only update cursor from mouse if mouse actually moved
            vec2i CurrentMousePos = sf::Mouse::getPosition(*Subsystem.Window);
            if (CurrentMousePos != LastMousePosition)
            {
                LastMousePosition = CurrentMousePos;
                // Cursor uses window pixel coordinates directly
                Subsystem.Cursor->SetPosition(vec2f(CurrentMousePos));
            }
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
            Subsystem.Render->Draw(*RenderCmd.Drawable, ERenderLayer::World);
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