// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"
#include "Framework/EngineSubsystem.h"

// Editor only exists in Debug builds
#ifndef WE_RELEASE
namespace we { class Editor; }
#endif

namespace we
{
    enum class EngineMode { Editor, Play };

    class WaterEngine
    {
    public:
        WaterEngine();
        ~WaterEngine();

        // Main loop functions
        void Tick();
        void Render();
        void ProcessEvents();
        bool IsRunning() const;

        // Mode control
        void SetMode(EngineMode NewMode);
        bool IsInEditor() const { return CurrentMode == EngineMode::Editor; }
        bool IsPlaying() const { return CurrentMode == EngineMode::Play; }

    protected:
        // Subsystems - accessible to derived Game classes
        EngineSubsystem Subsystem;
        
        // Override hooks for game
        virtual void Construct() {}

#ifndef WE_RELEASE
        unique<Editor> EditorInstance;
        EngineMode CurrentMode = EngineMode::Editor;
#else
        EngineMode CurrentMode = EngineMode::Play;  // Release always starts in Play mode
#endif

    public:
        void Initialize();
        void TickGame(float DeltaTime);
#ifndef WE_RELEASE
        void TickEditor(float DeltaTime);
#endif

        void PreConstruct();
        void CreateResourceSubsystem();  // Creates Resources and mounts pak (Release)
        void CreateSubsystems();         // Creates all other subsystems
        void LoadEngineConfig();
    };
}
