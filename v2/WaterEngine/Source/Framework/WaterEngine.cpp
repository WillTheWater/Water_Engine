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
    // =========================================================================
    // Lifecycle
    // =========================================================================
    WaterEngine::WaterEngine()
    {
        PreConstruct();
        Construct();
    }

    WaterEngine::~WaterEngine()
    {
        // GameInstance is destroyed last
        if (Subsystem.GameInst)
        {
            Subsystem.GameInst->Shutdown();
        }
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

    // =========================================================================
    // Construction
    // =========================================================================
    void WaterEngine::PreConstruct()
    {
        MountAssetDirectory();
        CreateSubsystems();
    }

    void WaterEngine::MountAssetDirectory()
    {
        Subsystem.AssetLoader = make_unique<ResourceSubsystem>();
        auto PD = make_shared<PakDirectory>(EC.AssetDirectory);
        Subsystem.AssetLoader->SetAssetDirectory(PD);
        
        if (EC.DisableSFMLLogs) 
        { 
            sf::err().rdbuf(nullptr); 
        }
    }

    void WaterEngine::CreateSubsystems()
    {
        // Window and Render
        Subsystem.Window = make_unique<WindowSubsystem>(WindowConfig{
            .WindowName        = EC.WindowName,
            .RenderResolution  = EC.RenderResolution,
            .AspectRatio       = EC.AspectRatio,
            .WindowMinimumSize = EC.WindowMinimumSize,
            .WindowIcon        = EC.WindowIcon,
            .FullscreenMode    = EC.FullscreenMode,
            .VsyncEnabled      = EC.VsyncEnabled,
            .TargetFPS         = EC.TargetFPS,
            .EnableKeyRepeat   = EC.EnableKeyRepeat,
            .DisableSFMLLogs   = EC.DisableSFMLLogs
        });

        Subsystem.Render = make_unique<RenderSubsystem>(*Subsystem.Window, RenderConfig{
            .RenderResolution = EC.RenderResolution,
            .SetRenderSmooth  = EC.SetRenderSmooth
        });

        // Core subsystems
        Subsystem.Time      = make_unique<TimeSubsystem>();
        Subsystem.Input     = make_unique<InputSubsystem>();
        Subsystem.SaveLoad  = make_unique<SaveLoadSubsystem>();
        Subsystem.GameState = make_unique<GameStateSubsystem>();

        // Cursor
        Subsystem.Cursor = make_unique<CursorSubsystem>(CursorConfig{
            .DefaultCursor     = EC.DefaultCursor,
            .DefaultCursorSize = EC.DefaultCursorSize,
            .DefaultCursorSpeed= EC.DefaultCursorSpeed,
            .JoystickDeadzone  = EC.JoystickDeadzone,
            .Window            = *Subsystem.Window
        });

        // Audio
        Subsystem.Audio = make_unique<AudioSubsystem>(AudioConfig{
            .StartupGlobalVolume = EC.StartupGlobalVolume,
            .MaxSFXStack         = EC.MaxSFXStack
        });

        // Physics
        Subsystem.Physics = make_unique<PhysicsSubsystem>(PhysicsConfig{
            .DefaultGravity   = vec2f{EC.DefaultGravity.x, EC.DefaultGravity.y},
            .PhysicsScale     = 0.01f,
            .FixedTimeStep    = 1.0f / 60.0f,
            .VelocityIterations = 8,
            .PositionIterations = 3
        });

        // Subsystems that depend on other subsystems
        Subsystem.World  = make_unique<WorldSubsystem>(Subsystem);
        Subsystem.GUI    = make_unique<GUISubsystem>(Subsystem);
        Subsystem.Camera = make_unique<CameraSubsystem>();
    }

    // =========================================================================
    // Main Loop
    // =========================================================================
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
            TickPaused();
        }
    }

    void WaterEngine::TickGame()
    {
        float DeltaTime = Subsystem.Time->GetDeltaTime();

        // Update subsystems
        Subsystem.Cursor->Update(DeltaTime);
        Subsystem.GUI->Update(DeltaTime);
        TimerManager::Get().Tick(DeltaTime);

        // Game tick
        Tick(DeltaTime);
        
        if (Subsystem.GameInst)
        {
            Subsystem.GameInst->Tick(DeltaTime);
        }

        // State management
        if (Subsystem.GameState->IsTransitionPending())
        {
            Subsystem.GameState->ApplyPendingState();
        }

        // Input and resources
        Subsystem.Input->ProcessHeld();
        Subsystem.AssetLoader->GarbageCycle(DeltaTime);

        // World update
        if (auto World = Subsystem.World->GetCurrentWorld())
        {
            World->BeginPlayGlobal();
            World->TickGlobal(DeltaTime);
        }

        // Physics
        Subsystem.Physics->Step(DeltaTime);
    }

    void WaterEngine::TickPaused()
    {
        float UnscaledDeltaTime = Subsystem.Time->GetUnscaledDeltaTime();
        
        Subsystem.Cursor->Update(UnscaledDeltaTime);
        Subsystem.Input->ProcessHeld();
        Subsystem.GUI->Update(UnscaledDeltaTime);
    }

    void WaterEngine::Render()
    {
        Subsystem.Audio->Update();
        Subsystem.Window->clear(color::Black);

        // 1. Begin frame - clear targets, reset views
        Subsystem.Render->BeginFrame();

        // 2. Set up world view from active camera
        UpdateWorldViewFromCamera();

        // 3. Render game world
        WorldRender();

        // 4. Render UI
        Subsystem.Render->ResetToDefaultViews();
        Subsystem.GUI->Render();

        // 5. Render cursor to its target
        Subsystem.Cursor->Render(*Subsystem.Render);

        // 6. Composite game layers (world + UI) with letterboxing
        sprite Composite = Subsystem.Render->FinishComposite();

        // 7. Draw game to window
        Subsystem.Window->draw(Composite);

        // 8. Draw cursor directly to window (1:1, no scaling)
        Subsystem.Render->PresentCursor();

        // 9. Present frame
        Subsystem.Window->display();
    }

    void WaterEngine::PostUpdate()
    {
        // Reserved for post-frame cleanup if needed
    }

    void WaterEngine::UpdateWorldViewFromCamera()
    {
        if (Subsystem.Camera->HasActiveCamera())
        {
            CameraView View;
            if (Subsystem.Camera->GetCurrentView(View))
            {
                Subsystem.Render->SetWorldView(View);
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

        // Draw sorted world objects
        for (const auto& RenderCmd : WorldRenderDepths)
        {
            Subsystem.Render->Draw(*RenderCmd.Drawable, ERenderLayer::World);
        }

        // Debug primitives (auto-clears after render)
        DebugDraw::Render(*Subsystem.Render);
    }

    // =========================================================================
    // Event Processing
    // =========================================================================
    void WaterEngine::ProcessEvents()
    {
        while (const auto Event = Subsystem.Window->pollEvent())
        {
            Subsystem.Window->HandleEvent(*Event);

            if (!Subsystem.Window->hasFocus())
            {
                continue;
            }

            // Pass events to GUI and Input subsystems
            Subsystem.GUI->HandleEvent(*Event);
            Subsystem.Input->HandleEvent(*Event);

            // Update cursor position on mouse movement
            Subsystem.Cursor->UpdateFromMouse(vec2f(sf::Mouse::getPosition(*Subsystem.Window)));
        }
    }

    // =========================================================================
    // State Queries
    // =========================================================================
    bool WaterEngine::IsRunning() const
    {
        return Subsystem.Window->isOpen() && !Subsystem.GameState->IsShutdownRequested();
    }

    bool WaterEngine::HasFocus() const
    {
        return Subsystem.Window->hasFocus();
    }

} // namespace we
