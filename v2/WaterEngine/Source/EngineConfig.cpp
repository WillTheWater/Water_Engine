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

        VsyncEnabled = Config["VsyncEnabled"];
        TargetFPS = Config["TargetFPS"];
        MaxDeltaTime = sf::seconds(Config["MaxDeltaTime"]);
        FullscreenMode = Config["FullscreenMode"];
        SetRenderSmooth = Config["SetRenderSmooth"];
        DisableSFMLLogs = Config["DisableSFMLLogs"];
        MaxSFXStack = Config["MaxSFXStack"];

        JoystickDeadzone = Config["JoystickDeadzone"];
        DefaultCursorSpeed = Config["DefaultCursorSpeed"];
        
        DefaultGravity = { Config["DefaultGravity"][0], Config["DefaultGravity"][1] };
        
       /* DefaultBackground = Config["DefaultBackground"];
        StartupGlobalVolume = Config["StartupGlobalVolume"];
        DefaultTitleFont = Config["DefaultTitleFont"];
        DefaultPanel = Config["DefaultPanel"];
        DefaultButton = Config["DefaultButton"];
        DefaultMusic = Config["DefaultMusic"];
        DefaultAmbient = Config["DefaultAmbient"];
        DefaultButtonHoverSound = Config["DefaultButtonHoverSound"];
        DefaultButtonClickSound = Config["DefaultButtonClickSound"];
        LevelOneBackground = Config["LevelOneBackground"];
        CharacterSheetIdle = Config["CharacterSheetIdle"];
        CharacterSheetWalk = Config["CharacterSheetWalk"];
        Footstep1 = Config["Footstep1"];
        Footstep2 = Config["Footstep2"];
        Footstep3 = Config["Footstep3"];
        Footstep4 = Config["Footstep4"];
        */
    }
}