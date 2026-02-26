// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    struct EngineSubsystem;
    class World;
    class Actor;

    enum class EditorMode { Edit, Simulate, Play };

    // =============================================================================
    // EDITOR - Standalone editor class (not a subsystem)
    // =============================================================================

    class Editor
    {
    public:
        explicit Editor(EngineSubsystem& InSubsystem);
        ~Editor();

        void Initialize();
        void Shutdown();

        // Main UI - called every frame in Edit/Simulate mode
        void DrawUI();

        // Mode transitions
        void OnPlay();      // Edit/Simulate -> Play
        void OnPause();     // Play -> Simulate  
        void OnStop();      // Play/Simulate -> Edit

        // Input handling - returns true if consumed
        bool ProcessEvent(const sf::Event& e);

        // State queries
        bool IsEditing() const { return CurrentMode == EditorMode::Edit; }
        bool IsSimulating() const { return CurrentMode == EditorMode::Simulate; }
        bool IsPlaying() const { return CurrentMode == EditorMode::Play; }
        EditorMode GetMode() const { return CurrentMode; }

        // World state snapshot for play/stop
        void SaveWorldState();
        void RestoreWorldState();

    private:
        void DrawMainMenuBar();
        void DrawSceneViewport();   // Shows WorldRenderTarget texture
        void DrawHierarchy();       // Actor list
        void DrawInspector();       // Selected actor properties

        void HandleViewportInput(); // Camera pan, selection

    private:
        EngineSubsystem& Subsystem;
        EditorMode CurrentMode = EditorMode::Edit;

        // World state snapshot
        vector<uint8> WorldSnapshot;

        // Selection
        Actor* SelectedActor = nullptr;

        // Viewport state
        bool bViewportHovered = false;
        bool bViewportFocused = false;
    };
}
