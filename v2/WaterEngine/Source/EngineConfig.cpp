// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EngineConfig.h"
#include "EngineConfigData.h"
#include "Core/JsonTypes.h"

namespace we
{
    EngineConfig::EngineConfig()
    {
        // Parse the embedded JSON string (generated from Config/EngineConfig.json)
        json Config = json::parse(EMBEDDED_ENGINE_CONFIG);

        AssetDirectory = Config["AssetDirectory"];

        RenderResolution = { Config["RenderResolution"][0], Config["RenderResolution"][1] };
        AspectRatio = { Config["AspectRatio"][0], Config["AspectRatio"][1] };
        WindowMinimumSize = { Config["WindowMinimumSize"][0], Config["WindowMinimumSize"][1] };
        WindowIcon = Config["WindowIcon"];
        WindowName = Config["WindowName"];
        EnableKeyRepeat = Config["EnableKeyRepeat"];
        
        DefaultCursor = Config["DefaultCursor"];
        DefaultCursorSize = { Config["DefaultCursorSize"][0], Config["DefaultCursorSize"][1] };
        DefaultFont = Config["DefaultFont"];
        DefaultFont2 = Config["DefaultFont2"];

        VsyncEnabled = Config["VsyncEnabled"];
        TargetFPS = Config["TargetFPS"];
        MaxDeltaTime = sf::seconds(Config["MaxDeltaTime"]);
        FullscreenMode = Config["FullscreenMode"];
        SetRenderSmooth = Config["SetRenderSmooth"];
        DisableSFMLLogs = Config["DisableSFMLLogs"];
        StartupGlobalVolume = Config["StartupGlobalVolume"];
        MaxSFXStack = Config["MaxSFXStack"];

        JoystickDeadzone = Config["JoystickDeadzone"];
        DefaultCursorSpeed = Config["DefaultCursorSpeed"];
        
        DefaultGravity = { Config["DefaultGravity"][0], Config["DefaultGravity"][1] };
    }
}