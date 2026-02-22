// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/GameInstance.h"

namespace we
{
    class WaterEngine
    {
    public:
        WaterEngine();
        virtual ~WaterEngine();

        // Main loop functions
        void Initialize();
        void Tick();
        void Render();
        void PostUpdate();
        void ProcessEvents();

        // State queries
        bool IsRunning() const;
        bool HasFocus() const;

    protected:
        // Override to create your game-specific GameInstance
        virtual unique<GameInstance> CreateGameInstance() { return make_unique<GameInstance>(); }
        
        // Override hooks for game-specific behavior
        virtual void Construct() {}
        virtual void BeginPlay() {}
        virtual void Tick(float DeltaTime) {}

    protected:
        EngineSubsystem Subsystem;

    private:
        // Construction
        void PreConstruct();
        void MountAssetDirectory();
        void CreateSubsystems();

        // Game loop helpers
        void TickGame();
        void TickPaused();
        void UpdateWorldViewFromCamera();
        void WorldRender();
        void UpdateCursorPosition();

    private:
        vec2i LastMousePosition{ -1, -1 };  // Track to detect actual mouse movement
    };

} // namespace we
