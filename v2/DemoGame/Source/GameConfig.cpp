// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "GameConfig.h"

namespace we
{
    GameConfig::GameConfig()
    {
        json Config = json::parse(EMBEDDED_GAME_CONFIG);

        StartupGlobalVolume = Config["StartupGlobalVolume"];

        DefaultBackground = Config["DefaultBackground"];

        LevelOneBackground = Config["LevelOneBackground"];
        GridBackground = Config["GridBackground"];

        CharacterSheetIdle = Config["CharacterSheetIdle"];
        CharacterSheetWalk = Config["CharacterSheetWalk"];

        NPCSheetIdle = Config["NPCSheetIdle"];
        OldNPCSheetIdle = Config["OldNPCSheetIdle"];
        OldNPCSheetWalk = Config["OldNPCSheetWalk"];

        DefaultMusic = Config["DefaultMusic"];
        DefaultAmbient = Config["DefaultAmbient"];
        DefaultButton = Config["DefaultButton"];
        DefaultButtonHoverSound = Config["DefaultButtonHoverSound"];
        DefaultButtonClickSound = Config["DefaultButtonClickSound"];
        Footstep1 = Config["Footstep1"];
        Footstep2 = Config["Footstep2"];
        Footstep3 = Config["Footstep3"];
        Footstep4 = Config["Footstep4"];        
    }
}