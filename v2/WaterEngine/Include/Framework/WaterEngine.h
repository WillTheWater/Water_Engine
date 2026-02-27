// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/World/World.h"

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
        virtual ~WaterEngine();

        // Main loop functions
        virtual void Initialize();
        void Tick();
        void Render();
        void ProcessEvents();
        bool IsRunning() const;

        // Mode control
        void SetMode(EngineMode NewMode);
        bool IsInEditor() const { return CurrentMode == EngineMode::Editor; }
        bool IsPlaying() const { return CurrentMode == EngineMode::Play; }

        // World management - template factory
        template<typename WorldType>
        void LoadWorld()
        {
            static_assert(std::is_base_of_v<World, WorldType>, "WorldType must derive from World");
            Subsystem.CurrentWorld = make_unique<WorldType>(Subsystem);
            Subsystem.CurrentWorld->BeginPlayGlobal();
        }

    protected:
        EngineSubsystem Subsystem;

#ifndef WE_RELEASE
        unique<Editor> EditorInstance;
        EngineMode CurrentMode = EngineMode::Editor;
#else
        EngineMode CurrentMode = EngineMode::Play;  // Release always starts in Play mode
#endif

        virtual void TickGame(float DeltaTime);
#ifndef WE_RELEASE
        virtual void TickEditor(float DeltaTime);
#endif

    private:
        void PreConstruct();
        void MountAssetDirectory();
        void CreateSubsystems();
        void LoadEngineConfig();
    };
}
