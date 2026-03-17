// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    // =========================================================================
    // Display & Rendering Configuration
    // =========================================================================
    struct RenderConfig
    {
        // Render resolution (fixed internal resolution, letterboxed to window)
        static constexpr vec2u RenderResolution{1920, 1080};
        
        // Render target smooth settings
        static constexpr bool bWorldLayerSmooth = true;
        static constexpr bool bScreenUILayerSmooth = false;
        static constexpr bool bWorldUILayerSmooth = false;
        static constexpr bool bCursorLayerSmooth = false;
        static constexpr bool bCompositeSmooth = true;
        
        // Clear colors for each render target
        static constexpr color WorldClearColor = color::Green;
        static constexpr color ScreenUIClearColor = color::Transparent;
        static constexpr color WorldUIClearColor = color::Transparent;
        static constexpr color CursorClearColor = color::Transparent;
        static constexpr color CompositeClearColor = color::Transparent;
        
        // Final window clear color (before displaying composite)
        static constexpr color WindowClearColor = color::Black;
    };

    // =========================================================================
    // Window Configuration
    // =========================================================================
    struct WindowConfig
    {
        // Initial window settings
        static constexpr vec2u DefaultWindowSize{1920, 1080};
        static constexpr const char* WindowTitle = "Water Engine";
        static constexpr bool bVSyncEnabled = true;
        static constexpr bool bMouseCursorVisible = false;
        static constexpr bool bKeyRepeatEnabled = false;
        
        // Window icon texture path (empty = no icon)
        static constexpr const char* WindowIcon = "Assets/Icon/icon.png";
    };

    // =========================================================================
    // GUI Configuration
    // =========================================================================
    struct GUIConfig
    {
        // Default virtual resolution for coordinate transform
        static constexpr vec2u VirtualResolution{1920, 1080};
        
        // Initial window size reference
        static constexpr vec2u DefaultWindowSize{1920, 1080};
    };

    // =========================================================================
    // Cursor Configuration
    // =========================================================================
    struct CursorConfig
    {
        // Cursor size in pixels
        static constexpr vec2u CursorSize{42, 42};
        
        // Default cursor texture path
        static constexpr const char* DefaultCursorTexture = "Assets/Textures/Default/defaultCursor.png";
    };

    // =========================================================================
    // Resource Management Configuration
    // =========================================================================
    struct ResourceConfig
    {
        // Garbage collection intervals (seconds)
        static constexpr float TimerGCInterval = 3.0f;
        static constexpr float WorldGCInterval = 3.0f;
        
        // Asset paths (set by CMake in actual builds)
        static constexpr const char* AssetRootPath = ASSET_ROOT_PATH;
        
        #ifdef USE_PACKED_ASSETS
        static constexpr string_view AssetPackPath = ASSET_PACK_PATH;
        #endif
    };

    // =========================================================================
    // Save System Configuration
    // =========================================================================
    struct SaveConfig
    {
        // Company and game names for save location
        static constexpr const char* CompanyName = "WaterEngine";
        static constexpr const char* GameName = "WaterEngine";
        static constexpr const char* SaveFileName = "Save.json";
        
        // Platform-specific save paths
        #ifdef _WIN32
        static constexpr const char* WindowsSaveFolder = "%APPDATA%";
        #else
        static constexpr string_view UnixSaveFolder = "~/.config";
        #endif
    };

    // =========================================================================
    // Input Configuration
    // =========================================================================
    struct InputConfig
    {
        // Gamepad vendor IDs
        static constexpr int SonyVendorID = 0x054C;
        static constexpr int MicrosoftVendorID = 0x045E;
        
        // PlayStation button mapping (hardware button index for each GamepadButton)
        // Order: West, South, East, North, L1, R1, L2, R2, Select, Start, L3, R3, DPadUp, DPadDown, DPadLeft, DPadRight
        static constexpr std::array<int, 16> PlayStationMapping = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19};
        
        // Xbox button mapping (hardware button index for each GamepadButton)
        // -1 indicates unmapped buttons
        static constexpr std::array<int, 16> XboxMapping = {2, 0, 1, 3, 4, 5, -1, -1, 6, 7, 8, 9, 12, 13, 14, 15};
        
        // Hotkey bindings (scancodes)
        static constexpr sf::Keyboard::Scan FullscreenKey = sf::Keyboard::Scan::F11;
    };

    // =========================================================================
    // Engine Timing Configuration
    // =========================================================================
    struct TimingConfig
    {
        // Timer subsystem garbage collection interval (seconds)
        static constexpr float TimerGCInterval = 3.0f;
        
        // World subsystem garbage collection interval (seconds)
        static constexpr float WorldGCInterval = 3.0f;
    };

    // =========================================================================
    // Physics Configuration
    // =========================================================================
    struct PhysicsConfig
    {
        // Scale conversion: pixels to meters (100 pixels = 1 meter)
        static constexpr float PhysicsScale = 0.01f;
        
        // World gravity (meters per second squared)
        // Default (0, 0) for top-down games, use (0, 9.8f) for platformers
        static constexpr vec2f Gravity{0.0f, 0.0f};
        
        // Solver iterations (higher = more accurate, more expensive)
        static constexpr int VelocityIterations = 8;
        static constexpr int PositionIterations = 3;
    };

    // =========================================================================
    // Master Engine Configuration
    // =========================================================================
    struct EngineConfig
    {
        RenderConfig Render;
        WindowConfig Window;
        GUIConfig GUI;
        CursorConfig Cursor;
        ResourceConfig Resource;
        SaveConfig Save;
        InputConfig Input;
        TimingConfig Timing;
        PhysicsConfig Physics;
    };
}

extern we::EngineConfig WEConfig;