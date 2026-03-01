// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "Framework/World/World.h"
#include "AssetDirectory/PakDirectory.h"
#include "AssetDirectory/FileDirectory.h"
#include "Utility/Log.h"
#include "Utility/Assert.h"
#include <exception>
#include <optional>

// Editor only exists in Debug builds
#ifndef WE_RELEASE
#include "Framework/Editor.h"
#endif

// ImGui-SFML
#include <imgui.h>
#include <imgui-SFML.h>

namespace we
{
    WaterEngine::WaterEngine()
    {
        PreConstruct();
    }

    WaterEngine::~WaterEngine() = default;

    void WaterEngine::PreConstruct()
    {
        // Create ResourceSubsystem first (needed for config loading)
        CreateResourceSubsystem();
        
        // Load config using ResourceSubsystem
        LoadEngineConfig();
        
        // Create remaining subsystems
        CreateSubsystems();
        
        // Create editor (Debug only)
#ifndef WE_RELEASE
        EditorInstance = make_unique<Editor>(Subsystem);
        EditorInstance->OnRequestPlayStop = [this](bool bPlay) {
            SetMode(bPlay ? EngineMode::Play : EngineMode::Editor);
        };
#endif
    }

    void WaterEngine::CreateResourceSubsystem()
    {
        Subsystem.Resources = make_unique<ResourceSubsystem>();

#ifdef WE_RELEASE
        // Release: Mount Content.pak
        try
        {
            auto pak = make_shared<PakDirectory>("Content.pak");
            Subsystem.Resources->SetAssetDirectory(pak);
            LOG("Mounted Content.pak");
        }
        catch (const std::exception& e)
        {
            LOG("Failed to mount Content.pak: {}", e.what());
        }
#else
        // Debug: Use loose files from Content directory
        auto fileDir = make_shared<FileDirectory>("Content");
        Subsystem.Resources->SetAssetDirectory(fileDir);
        LOG("Debug mode: Using loose files from Content/");
#endif
    }

    void WaterEngine::LoadEngineConfig()
    {
        VERIFY(Subsystem.Resources && "ResourceSubsystem must exist before loading config!");
        
        bool bLoaded = false;
        const char* ConfigPath = "Content/Config/EngineConfig.ini";

#ifdef WE_RELEASE
        // Release: Load from mounted pak via ResourceSubsystem
        if (auto assetDir = Subsystem.Resources->GetAssetDirectory())
        {
            bLoaded = EngineConfigManager::LoadFromAssetDirector(*assetDir, ConfigPath);
        }
#else
        // Debug: Load from loose file
        bLoaded = EngineConfigManager::LoadFromFile(ConfigPath);
#endif

        if (!bLoaded)
        {
            LOG("Failed to load EngineConfig.ini, using defaults");
        }
        else
        {
            LOG("EngineConfig loaded successfully");
        }
    }

    void WaterEngine::CreateSubsystems()
    {
        const EngineConfig& Config = EngineConfigManager::Get();
        
        // Create all subsystems with their config dependencies
        Subsystem.Window = make_unique<WindowSubsystem>(Config.Window);
        Subsystem.Camera = make_unique<CameraSubsystem>();
        Subsystem.World  = make_unique<WorldSubsystem>(Subsystem);
        Subsystem.Audio  = make_unique<AudioSubsystem>(Config.Audio, *Subsystem.Resources);
        Subsystem.Input  = make_unique<InputSubsystem>();
        Subsystem.Render = make_unique<RenderSubsystem>(Config.Render, *Subsystem.Window);
        Subsystem.Time   = make_unique<TimeSubsystem>();
        Subsystem.Physics = make_unique<PhysicsSubsystem>(Config.Physics);
    }

    void WaterEngine::Initialize()
    {
        VERIFY(Subsystem.Window && "Window not created!");
        VERIFY(Subsystem.Resources && "Resources not created!");
        
        // Initialize ImGui
        bool imguiInit = ImGui::SFML::Init(*Subsystem.Window);
        VERIFY(imguiInit && "ImGui::SFML::Init failed!");

#ifndef WE_RELEASE
        // Initialize Editor (Debug only)
        if (EditorInstance)
        {
            EditorInstance->Initialize();
        }
#endif

        LOG("WaterEngine Initialized");
        
        // Game-specific construction hook (loads world, calls PreConstruct)
        LOG("Calling Construct()...");
        Construct();
        LOG("Construct() completed");
        
#ifdef WE_RELEASE
        // Release: No editor, start Play mode immediately
        LOG("Release mode: Starting Play mode");
        if (Subsystem.World)
        {
            if (Subsystem.World->GetCurrentWorld())
            {
                Subsystem.World->StartPlay();
            }
            else
            {
                LOG("Release mode: CurrentWorld is null!");
            }
        }
        else
        {
            LOG("Release mode: World subsystem is null!");
        }
#endif
    }

    void WaterEngine::SetMode(EngineMode NewMode)
    {
        if (CurrentMode == NewMode) return;

#ifndef WE_RELEASE
        if (NewMode == EngineMode::Play)
        {
            // Start Play mode - BeginPlay is called on world
            if (Subsystem.World)
            {
                Subsystem.World->StartPlay();
            }
            EditorInstance->OnPlay();
        }
        else
        {
            // End Play mode - EndPlay is called on world
            if (Subsystem.World)
            {
                Subsystem.World->EndPlay();
            }
            EditorInstance->OnStop();
        }
#endif

        CurrentMode = NewMode;
    }

    void WaterEngine::Tick()
    {
        Subsystem.Time->Tick();
        auto DeltaTime = Subsystem.Time->GetDeltaTime();
        Subsystem.Resources->PollCompletedRequests();

        // Update Audio
        if (Subsystem.Audio)
        {
            Subsystem.Audio->Update(DeltaTime);
        }

        // Update Input (held callbacks)
        if (Subsystem.Input)
        {
            Subsystem.Input->ProcessHeld();
        }

        // Update ImGui
        ImGui::SFML::Update(*Subsystem.Window, sf::seconds(DeltaTime));

        if (CurrentMode == EngineMode::Play)
        {
            if (!Subsystem.Time->IsPaused())
            {
                TickGame(DeltaTime);
            }
        }
        else
        {
            // Edit or Simulate mode (Debug only)
#ifndef WE_RELEASE
            TickEditor(DeltaTime);
#endif
        }

        // Post-update Input (advance frame counter for IsJustPressed/IsJustReleased)
        if (Subsystem.Input)
        {
            Subsystem.Input->PostUpdate();
        }
    }

    void WaterEngine::TickGame(float DeltaTime)
    {
        // Step physics simulation
        if (Subsystem.Physics)
        {
            Subsystem.Physics->Step(DeltaTime);
        }

        // Tick current world if exists
        if (Subsystem.World)
        {
            Subsystem.World->Tick(DeltaTime);
        }
    }

#ifndef WE_RELEASE
    void WaterEngine::TickEditor(float DeltaTime)
    {
        // Edit mode: world is frozen, only editor UI updates
        // World only ticks when in Play mode via TickGame()
    }
#endif

    void WaterEngine::Render()
    {
        // Begin render frame
        Subsystem.Render->BeginFrame();

        // Get camera view if available
        std::optional<CameraView> CamView;
        if (Subsystem.Camera)
        {
            CamView = Subsystem.Camera->GetCurrentView();
        }

        // Set world view (uses camera if available, default if not)
        Subsystem.Render->SetWorldView(CamView);

        // Render current world
        if (Subsystem.World)
        {
            Subsystem.World->Render();
        }

#ifndef WE_RELEASE
        // Draw editor grid on top of everything when in editor mode
        if (CurrentMode != EngineMode::Play)
        {
            Subsystem.Render->DrawEditorGrid();
        }
#endif

        // End frame (display targets)
        Subsystem.Render->EndFrame();

        // Clear window
        Subsystem.Window->clear(color::Black);

        if (CurrentMode == EngineMode::Play)
        {
            // Full game rendering - get composited output with letterboxing
            sprite FinalFrame = Subsystem.Render->FinishComposite();
            Subsystem.Window->draw(FinalFrame);
            
            // Draw cursor on top (1:1 with window)
            Subsystem.Render->PresentCursor();
        }
#ifndef WE_RELEASE
        else
        {
            // Editor mode: Editor UI shows world texture via GetWorldTexture()
            EditorInstance->DrawUI(CurrentMode == EngineMode::Play);
        }
#endif

        // Render ImGui
        ImGui::SFML::Render(*Subsystem.Window);

        Subsystem.Window->display();
    }

    void WaterEngine::ProcessEvents()
    {
        while (const auto Event = Subsystem.Window->pollEvent())
        {
            // 1. Window Event Handling (always first)
            Subsystem.Window->HandleEvent(*Event);

#ifndef WE_RELEASE
            // 2. Editor Event Handling (Edit mode only)
            if (CurrentMode != EngineMode::Play)
            {
                EditorInstance->HandleEvent(*Event);
                
                // Editor consumed the event via ImGui
                if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
                {
                    continue;
                }
            }
            else
            {
                // 3. Play Mode: ESC to return to Editor
                if (const auto* keyPressed = Event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->code == sf::Keyboard::Key::Escape)
                    {
                        SetMode(EngineMode::Editor);
                        continue;
                    }
                }
            }
#endif

            // 4. Game Input Handling (Play mode only, window must have focus)
            if (Subsystem.Input && CurrentMode == EngineMode::Play && Subsystem.Window->hasFocus())
            {
                Subsystem.Input->HandleEvent(*Event);
            }
        }
    }

    bool WaterEngine::IsRunning() const
    {
        return Subsystem.Window->isOpen();
    }
}
