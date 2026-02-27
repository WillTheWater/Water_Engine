// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <functional>

namespace we
{
    struct EngineSubsystem;
    class WaterEngine;
    class World;
    class Actor;

    enum class EditorMode { Edit, Play };

    // =============================================================================
    // EDITOR - Standalone editor class (Debug builds only)
    // 
    // Panel Overview:
    // - Viewport: Shows the game at actual render resolution (1920x1080 etc)
    //             scaled to fit while maintaining aspect ratio
    // - World:    List of all actors in the current world (was "Hierarchy")
    // - Details:  Properties of the selected actor (was "Inspector")
    //
    // Docking: Full dockspace enabled - panels can be rearranged
    // =============================================================================

    class Editor
    {
    public:
        explicit Editor(EngineSubsystem& InSubsystem);
        ~Editor();

        void Initialize();
        void Shutdown();

        // Main UI - called every frame
        void DrawUI(bool bIsPlaying);

        // Mode request callback - set by WaterEngine
        std::function<void(bool bPlay)> OnRequestPlayStop;

        // Mode transitions - called by WaterEngine
        void OnPlay();      // Called when entering Play mode
        void OnStop();      // Called when returning to Edit mode

        // Input handling - returns true if consumed
        bool ProcessEvent(const sf::Event& e);

        // World state snapshot for play/stop
        void SaveWorldState();
        void RestoreWorldState();

    private:
        void DrawMainDockSpace();   // Root docking area
        void DrawMainMenuBar();     // File, Edit, View menus
        void DrawViewport();        // Game view at render resolution
        void DrawWorld();           // Actor list (was Hierarchy)
        void DrawDetails();         // Selected actor properties (was Inspector)

        void HandleViewportInput(); // Camera pan, selection

    private:
        EngineSubsystem& Subsystem;

        // World state snapshot
        vector<uint8> WorldSnapshot;

        // Selection
        Actor* SelectedActor = nullptr;

        // Viewport state
        bool bViewportHovered = false;
        bool bViewportFocused = false;
        bool bIsPanning = false;
        vec2f LastMousePos;
        vec2f MouseWorldPos;

        // UI settings
        float FontScale = 1.0f;
        float DetailsPanelWidth = 250.0f;
        float ViewportHeightRatio = 0.65f;
        bool bResizingDetails = false;
        bool bResizingViewport = false;
    };
}
