// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EngineConfig.h"
#include "EngineConfigData.h"
#include <nlohmann/json.hpp>

namespace we
{
    EngineConfig::EngineConfig()
    {
        // Parse the embedded JSON string (generated from Config/EngineConfig.json)
        json Config = json::parse(EMBEDDED_ENGINE_CONFIG);

        AssetDirectory = Config["AssetDirectory"];
        WindowSize = { Config["WindowSize"][0], Config["WindowSize"][1] };
        AspectRatio = { Config["AspectRatio"][0], Config["AspectRatio"][1] };
        WindowMinimumSize = { Config["WindowMinimumSize"][0], Config["WindowMinimumSize"][1] };
        TargetFPS = Config["TargetFPS"];
        VsyncEnabled = Config["VsyncEnabled"];
        FullscreenMode = Config["FullscreenMode"];
        SetRenderSmooth = Config["SetRenderSmooth"];
        WindowName = Config["WindowName"];
        DisableSFMLLogs = Config["DisableSFMLLogs"];
        MaxDeltaTime = sf::seconds(Config["MaxDeltaTime"]);
        WindowIcon = Config["WindowIcon"];
        DefaultBackground = Config["DefaultBackground"];
        DefaultCursor = Config["DefaultCursor"];
        StartupGlobalVolume = Config["StartupGlobalVolume"];
        EnableKeyRepeat = Config["EnableKeyRepeat"];
        DefaultCursorSize = { Config["DefaultCursorSize"][0], Config["DefaultCursorSize"][1] };
        DefaultCursorSpeed = Config["DefaultCursorSpeed"];
        JoystickDeadzone = Config["JoystickDeadzone"];
        DefaultTitleFont = Config["DefaultTitleFont"];
        DefaultFont = Config["DefaultFont"];
        DefaultPanel = Config["DefaultPanel"];
        DefaultButton = Config["DefaultButton"];
        DefaultMusic = Config["DefaultMusic"];
        DefaultAmbient = Config["DefaultAmbient"];
        DefaultButtonHoverSound = Config["DefaultButtonHoverSound"];
        DefaultButtonClickSound = Config["DefaultButtonClickSound"];
        MaxSFXStack = Config["MaxSFXStack"];
        LevelOneBackground = Config["LevelOneBackground"];
        CharacterSheetIdle = Config["CharacterSheetIdle"];
        CharacterSheetWalk = Config["CharacterSheetWalk"];
        Footstep1 = Config["Footstep1"];
        Footstep2 = Config["Footstep2"];
        Footstep3 = Config["Footstep3"];
        Footstep4 = Config["Footstep4"];
        DefaultGravity = { Config["DefaultGravity"][0], Config["DefaultGravity"][1] };
    }
}